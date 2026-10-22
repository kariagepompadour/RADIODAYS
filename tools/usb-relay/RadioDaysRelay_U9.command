#!/usr/bin/perl
# RADIO DAYS — U9 USB relay for macOS (for RADIO DAYS U9 only: its allowed hosts are U9's
# 42 stations. For U4 to U8 use RadioDaysRelay_U4.command, which is unchanged).
#
# Changes from the U4 relay (docs/usb-u9.md): only the built-in allowed hosts and redirect
# rules (U9's stations; U8's other hosts are no longer allowed), this text and the
# user agent. The link protocol, http only, at most 3 redirects, no IP address, no user
# name, allowed_hosts.txt, --check-url and --check-redirect are the U4 relay's.
#
# (U4 relay notes follow.)
# RADIO DAYS — U4 USB relay for macOS (for RADIO DAYS U2a2, U2a3, U2a4, U3 and U4;
# U2a needs RadioDaysRelay_U2.command). macOS only: Windows and Linux are not supported
# and have not been tried.
#
# Added in U4 (docs/usb-u4.md): the hosts of U4's stations, and "redirect rules" for the
# few stations that redirect to a host that changes every time (Deutschlandfunk and WDR
# to *.rndfnk.com, Radio ZET to *.cdn.eurozet.pl). A redirect rule ("redirect
# *.example.com:80") is used ONLY for a redirect target, never for the URL the Cardputer
# asks for (that one must still match a host:port exactly). It matches a host that ends
# in ".example.com" after at least one more label, on that exact port: "a.example.com"
# matches; "example.com", "badexample.com" and "example.com.other.org" do not. Redirects
# stay http only, at most 3, with no IP address and no user name. Everything else is
# the U3 relay unchanged (time, credit, TUNE, RDSYNC).
#
# Added in U3 (docs/usb-u3-time-design.md): "@RD TIME <id>" is answered with
# "RDTIME <id> <UTC seconds>" (Perl's time: UTC whatever the Mac's time zone; no local
# time, date text or time zone is sent). It is answered only before RDSYNC, never while
# MP3 bytes are being sent, so no text can enter the audio. Everything else is the U2a2
# relay unchanged; older Cardputers never ask, so they work as before.
#
# Changes from the U2a relay (docs/usb-u2a2.md): credit is a running total
# ("@RD CREDIT <id> <total>"), so a lost line from the Cardputer costs nothing; a
# repeated TUNE (same id) is ignored; every 10 s a [relay] line shows what held the
# stream back (the station or the Cardputer's credit), and the script warns when
# nothing at all comes from the Cardputer.
#
# The Cardputer asks for a station ("@RD TUNE <id> <URL>"). This script checks the URL
# against the allowed hosts and ports, receives it with the Mac's own curl, checks the
# reply (redirects are checked with the same rules), then writes "RDSYNC <id>" and
# only as many MP3 bytes as the Cardputer asks for ("@RD MORE <id> <n>").
# See docs/usb-u2a-design.md. Uses only programs that come with macOS: perl, curl,
# caffeinate. Changes no network setting of the Mac.
#
# Use: double-click this file (Terminal opens), or run it in Terminal with perl.
#      Close the Terminal window or press Control-C to stop.
#      perl RadioDaysRelay_U9.command --check-url <URL> ...        checks URLs only.
#      perl RadioDaysRelay_U9.command --check-redirect <URL> ...   checks redirect targets only.
# Allowed hosts: the lists below, or only the lines of allowed_hosts.txt ("host:port",
# "redirect *.domain:port", "#" comments) when that file is next to this script.
#
# SPDX-License-Identifier: MIT
use strict;
use warnings;
use POSIX qw(:termios_h);
use IO::Select;
use Fcntl;
use File::Basename qw(dirname);
use Time::HiRes qw(time sleep);

my $UA = 'RADIODAYS-relay/U9';
my $TWO32 = 4294967296;
my $CONNECT_LIMIT = 10;  # seconds: RDSYNC or RDNAK within this (the Cardputer waits 12)
my $MAX_REDIRECTS = 3;
my $MAX_URL = 300;
my $LINE_MAX = 1024;
my $HEADER_MAX = 16384;
my $RETRY_WAIT = 2;      # seconds before reconnecting a station that stopped

# (U9) The hosts and ports of firmware/RadioDays_U9/stations.h: every station's first URL,
# and RTL's one fixed redirect target (streaming-ice.audiomeans.fr:80; an exact host:port,
# not a rule: the U4 relay's checks are kept as they are).
my @BUILTIN_ALLOWED = qw(
    ais-sa8.cdnstream1.com:80 dispatcher.rndfnk.com:80 fm939.wnyc.org:80
    fmsetagaya834.out.airtime.pro:8000 ice1.somafm.com:80 icecast.radiofrance.fr:80
    icecast.rtl.fr:80 icecast.skyrock.net:80 icecast.unitedradio.it:80 kexp-mp3-128.streamguys1.com:80
    live.radioradicale.it:80 media-ice.musicradio.com:80
    mp3channels.webradio.antenne.de:80 mp3s.nc.u-tokyo.ac.jp:80 novazz.ice.infomaniak.ch:80
    ottava2.out.airtime.pro:8000 radio.kahoku.net:8000 radio.plaza.one:80
    radionz-ice.streamguys.com:80 sc6.radiocaroline.net:8040 shonanbeachfm.out.airtime.pro:8000
    shoutcast2.radio24.it:8000 st01.sslstream.dlf.de:80 st02.sslstream.dlf.de:80
    stream.live.vc.bbcmedia.co.uk:80 stream.radioparadise.com:80 stream.srg-ssr.ch:80
    stream.wqxr.org:80 stream0.wfmu.org:80 streaming-ice.audiomeans.fr:80 tsfjazz.ice.infomaniak.ch:80
    wdr-1live-live.icecast.wdr.de:80 wfuv-onair.streamguys.org:80
);
# (U4) Redirect targets only: a host under one of these domains, on this port.
# (U9) *.rndfnk.com: the ARD stations (d111/f121... change each time); *.antenne.de: Antenne
# Bayern (s1-webradio, s2-webradio...); *.srg-ssr.ch: Radio Swiss Jazz
# (livestreaming-node-2, -3...). U4's *.cdn.eurozet.pl (Radio ZET) is not needed by U9.
my @BUILTIN_REDIRECT = qw(
    *.rndfnk.com:80 *.antenne.de:80 *.srg-ssr.ch:80
);

$| = 1;
$SIG{PIPE} = 'IGNORE';

sub say_ { print scalar(localtime) =~ s/^\w+ \w+ +\d+ //r, "  ", @_, "\n"; }

# Diagnostics, every 10 s (docs/usb-u2a2.md, 4).
my %dg = (fromStation => 0, toCard => 0, waitCredit => 0, waitStation => 0, lines => 0, hello => 0, portBytes => 0);
my $lastPortRx = time;
my $silentSaid = 0;
my $versionSaid = 0;
my $lastTuneId = '';
my $timeSaid = 0;

# ---------------------------------------------------------------- allowed hosts and ports
my (%allowed, %allowedHost, @redirectRules);
my $allowedFrom = 'built-in list';

# (U4) "*.domain:port" -> [domain, port], or undef. The domain needs two labels or more
# (no "*.com") and may not be an IP address.
sub parse_redirect_rule {
    my ($r) = @_;
    return undef unless $r =~ /\A\*\.([A-Za-z0-9.-]+):(\d{1,5})\z/ && $2 >= 1 && $2 <= 65535;
    my ($d, $p) = (lc $1, $2 + 0);
    return undef unless $d =~ /\A[a-z0-9](?:[a-z0-9-]*[a-z0-9])?(?:\.[a-z0-9](?:[a-z0-9-]*[a-z0-9])?)+\z/;
    return undef if $d =~ /\A[0-9.]+\z/;
    return [$d, $p];
}
{
    my @list = @BUILTIN_ALLOWED;
    my @rules = @BUILTIN_REDIRECT;
    my $file = dirname($0) . '/allowed_hosts.txt';
    if (-f $file) {
        open(my $af, '<', $file) or die "Cannot read $file: $!\n";
        @list = ();
        @rules = ();
        while (my $l = <$af>) {
            $l =~ s/#.*//;
            $l =~ s/^\s+|\s+$//g;
            next unless length $l;
            if ($l =~ /\Aredirect\s+(\S+)\z/i) {
                if (parse_redirect_rule($1)) { push @rules, $1; }
                else { warn "allowed_hosts.txt: not \"redirect *.domain:port\", ignored: $l\n"; }
            } elsif ($l =~ /\A([A-Za-z0-9.-]+):(\d{1,5})\z/ && $2 >= 1 && $2 <= 65535) {
                push @list, lc($1) . ":" . ($2 + 0);
            } else {
                warn "allowed_hosts.txt: not \"host:port\", ignored: $l\n";
            }
        }
        close $af;
        $allowedFrom = $file;
    }
    for my $hp (@list) {
        $allowed{$hp} = 1;
        $allowedHost{(split /:/, $hp)[0]} = 1;
    }
    for my $r (@rules) {
        my $rule = parse_redirect_rule($r);
        die "Bad built-in redirect rule: $r\n" unless $rule;
        push @redirectRules, $rule;
    }
}

# (U4) A redirect target's host and port against the redirect rules: the host must end in
# "." + the domain (a label boundary) with at least one label before it, and the port
# must be the rule's. The host has already passed the host-name checks of check_url.
sub redirect_rule_ok {
    my ($host, $port) = @_;
    for my $r (@redirectRules) {
        my ($d, $p) = @$r;
        next unless $port == $p;
        return 1 if length($host) > length($d) + 1 && substr($host, -(length($d) + 1)) eq ".$d";
    }
    return 0;
}

# Returns (normalised URL, undef) or (undef, reason). Redirect targets fail as REDIR.
sub check_url {
    my ($u, $redirect) = @_;
    my $bad = $redirect ? 'REDIR' : 'URL';
    return (undef, $bad) if !defined $u || length($u) > $MAX_URL;
    return (undef, $bad) unless $u =~ m{\A[A-Za-z0-9\-._~/?=&%;:+,!*()\$]+\z};  # no @ # space quotes
    return (undef, $bad) unless $u =~ m{\Ahttp://([A-Za-z0-9.-]+)(?::(\d{1,5}))?(/.*)?\z}is;
    my ($host, $port, $path) = (lc $1, defined $2 ? $2 + 0 : 80, defined $3 ? $3 : '/');
    return (undef, $bad) if $port < 1 || $port > 65535;
    return (undef, $bad) unless $host =~ /\A[a-z0-9](?:[a-z0-9-]*[a-z0-9])?(?:\.[a-z0-9](?:[a-z0-9-]*[a-z0-9])?)*\z/;
    return (undef, $bad) if $host =~ /\A[0-9.]+\z/;  # an IP address
    if (!$allowed{"$host:$port"}) {
        return ("http://" . ($port == 80 ? $host : "$host:$port") . $path, undef)
            if $redirect && redirect_rule_ok($host, $port);  # (U4) redirect targets only
        return (undef, 'REDIR') if $redirect;
        return (undef, $allowedHost{$host} ? 'PORT' : 'HOST');
    }
    return ("http://" . ($port == 80 ? $host : "$host:$port") . $path, undef);
}

# A redirect's Location against the current (normalised) URL: absolute or "/path" only.
sub resolve_location {
    my ($loc, $base) = @_;
    return undef unless defined $loc;
    $loc =~ s/^\s+|\s+$//g;
    return $loc if $loc =~ m{\A[A-Za-z][A-Za-z0-9+.-]*:};
    if ($loc =~ m{\A/(?!/)}) {
        my ($origin) = $base =~ m{\A(http://[^/]+)};
        return $origin . $loc;
    }
    return undef;
}

if (@ARGV && ($ARGV[0] eq '--check-url' || $ARGV[0] eq '--check-redirect')) {
    my $asRedirect = shift(@ARGV) eq '--check-redirect' ? 1 : 0;  # (U4)
    print "Allowed hosts: $allowedFrom\n";
    for my $u (@ARGV) {
        my ($norm, $why) = check_url($u, $asRedirect);
        print $norm ? "OK        $norm\n" : sprintf("NO %-6s %s\n", $why, $u);
    }
    exit 0;
}

# ---------------------------------------------------------------- the Cardputer's port
my ($fh, $portPath, $chosen);
my $inbuf = '';
my $skipLine = 0;      # the rest of an over-long line is dropped
my $searchSaid = 0;

sub open_port {
    my ($path) = @_;
    sysopen(my $h, $path, O_RDWR | O_NOCTTY | O_NONBLOCK) or return undef;
    binmode $h;
    my $t = POSIX::Termios->new;
    $t->getattr(fileno $h) or return undef;
    $t->setiflag($t->getiflag & ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON | IXOFF));
    $t->setoflag($t->getoflag & ~(OPOST));
    $t->setlflag($t->getlflag & ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN));
    $t->setcflag(($t->getcflag & ~(CSIZE | PARENB | HUPCL)) | CS8 | CLOCAL | CREAD);
    $t->setcc(VMIN, 1);
    $t->setcc(VTIME, 0);
    $t->setattr(fileno $h, TCSANOW) or return undef;
    my $flags = fcntl($h, F_GETFL, 0);
    fcntl($h, F_SETFL, $flags & ~O_NONBLOCK);  # blocking writes: USB holds us back when busy
    return $h;
}

# Finds and opens the port; waits (2 s steps) until there is one. Never exits.
sub ensure_port {
    while (!$fh) {
        my @ports = $ENV{RD_PORT} ? grep { -e } ($ENV{RD_PORT}) : glob('/dev/cu.usbmodem*');  # RD_PORT only for tests
        my $pick;
        if ($chosen && grep { $_ eq $chosen } @ports) {
            $pick = $chosen;
        } elsif (@ports == 1) {
            $pick = $ports[0];
        } elsif (@ports > 1 && !$chosen) {
            print "Several USB serial ports were found:\n";
            for my $i (0 .. $#ports) { print "  ", $i + 1, ") $ports[$i]\n"; }
            print "Type the number of the Cardputer and press Return: ";
            my $n = <STDIN>;
            $n = defined $n ? int($n) : 0;
            $pick = $ports[$n - 1] if $n >= 1 && $n <= @ports;
        }
        if ($pick && ($fh = open_port($pick))) {
            ($portPath, $chosen) = ($pick, $pick);
            $inbuf = '';
            $skipLine = 0;
            $searchSaid = 0;
            say_("Port $portPath opened (raw). Waiting for RADIO DAYS (U2a2 or later) (\"\@RD HELLO\")...");
            $lastPortRx = time;
            return;
        }
        if (!$searchSaid) {
            say_("Waiting for the Cardputer (connect it with a USB data cable and start RADIO DAYS U2a2)...");
            $searchSaid = 1;
        }
        sleep 2;
    }
}

# ---------------------------------------------------------------- the station
# phase: none | connecting (before RDSYNC) | sending | waiting (to reconnect) | reconnecting
my %st = (phase => 'none');
my ($sent, $sentAt, $sentMark) = (0, time, 0);
my $linked = 0;

sub stop_curl {
    if ($st{curl}) {
        kill 'TERM', $st{pid} if $st{pid};
        close $st{curl};
    }
    delete @st{qw(curl pid)};
}

sub stop_station {
    stop_curl();
    %st = (phase => 'none');
}

sub port_lost {
    my ($why) = @_;
    stop_station();
    close $fh if $fh;
    undef $fh;
    $linked = 0;
    say_("The Cardputer port was closed ($why). Searching again...");
}

sub write_port {
    my ($data) = @_;
    my $off = 0;
    while ($fh && $off < length $data) {
        my $n = syswrite($fh, $data, length($data) - $off, $off);
        if (!defined $n) {
            port_lost("write: $!");
            return 0;
        }
        $off += $n;
    }
    return $fh ? 1 : 0;
}

sub reply { write_port("\n$_[0]\n"); }

sub start_fetch {
    my ($url) = @_;
    stop_curl();
    # List form: no shell; the URL is one argument and was checked (it starts with http://).
    my $pid = open(my $c, '-|', '/usr/bin/curl', '-sS', '-i', '--http0.9', '--max-redirs', '0',
                   '--proto', '=http', '--connect-timeout', '8', '--no-buffer', '-A', $UA, $url);
    if (!$pid) {
        say_("Cannot start curl: $!");
        return 0;
    }
    binmode $c;
    @st{qw(curl pid url hdr)} = ($c, $pid, $url, '');
    $st{headersDone} = 0;
    return 1;
}

# Before RDSYNC: refuse with a reason. After: wait and reconnect.
sub station_failed {
    my ($why, $detail) = @_;
    if (!$st{synced}) {
        say_("Refused ($why): $detail");
        my $id = $st{id};
        stop_station();
        reply("RDNAK $id $why");
        return;
    }
    say_("The station stopped ($detail). Reconnecting in $RETRY_WAIT s...");
    stop_curl();
    $st{phase} = 'waiting';
    $st{retryAt} = time + $RETRY_WAIT;
}

sub tune {
    my ($id, $url) = @_;
    stop_station();  # the old station's bytes stop here: nothing old is written after this
    my ($norm, $why) = check_url($url, 0);
    if (!$norm) {
        say_("Refused ($why): $url");
        reply("RDNAK $id $why");
        return;
    }
    %st = (phase => 'connecting', id => $id, first => $norm, credit => 0, pending => '', synced => 0, redirects => 0,
           deadline => time + $CONNECT_LIMIT);
    say_("Tuning $norm");
    station_failed('CONN', 'curl did not start') unless start_fetch($norm);
}

# Moves granted bytes from the station to the Cardputer.
sub pump {
    while ($fh && $st{synced} && $st{credit} > 0 && length $st{pending}) {
        my $n = length $st{pending};
        $n = $st{credit} if $n > $st{credit};
        $n = 4096 if $n > 4096;
        return unless write_port(substr($st{pending}, 0, $n, ''));
        $st{credit} -= $n;
        $st{sentTotal} = ($st{sentTotal} + $n) % $TWO32;
        $sent += $n;
        $dg{toCard} += $n;
    }
}

sub headers_received {
    my ($head, $rest) = @_;
    my @lines = split /\r?\n/, $head;
    my $status = shift(@lines) // '';
    my ($code) = $status =~ m{\A(?:HTTP/\d(?:\.\d)?|ICY) (\d{3})};
    if (!defined $code) {
        station_failed('HTTP', "no HTTP status from $st{url}");
        return;
    }
    if ($code == 200) {
        $st{headersDone} = 1;
        $st{pending} .= $rest;
        if (!$st{synced}) {
            # The reply is checked: from here only this station's bytes, only as granted.
            return unless reply("RDSYNC $st{id}");
            $st{synced} = 1;
            @st{qw(credit allowedTotal sentTotal)} = (0, 0, 0);
            say_("Sending $st{url}");
        } else {
            say_("Reconnected to $st{url}");
        }
        $st{phase} = 'sending';
        pump();
        return;
    }
    if ($code =~ /\A30[12378]\z/) {
        my ($loc) = map { /\Alocation:\s*(.*)\z/i ? $1 : () } @lines;
        my $target = resolve_location($loc, $st{url});
        my ($norm) = defined $target ? check_url($target, 1) : (undef);
        if (!$norm) {
            station_failed('REDIR', "redirect to " . ($loc // '(none)'));
            return;
        }
        if (++$st{redirects} > $MAX_REDIRECTS) {
            station_failed('REDIR', "more than $MAX_REDIRECTS redirects");
            return;
        }
        say_("Redirected to $norm");
        station_failed('CONN', 'curl did not start') unless start_fetch($norm);
        return;
    }
    station_failed('HTTP', "HTTP $code from $st{url}");
}

sub read_curl {
    my $want = $st{headersDone} ? ($st{credit} < 4096 ? $st{credit} : 4096) : 4096;
    my $n = sysread($st{curl}, my $buf, $want);
    if (!$n) {
        if ($st{phase} eq 'connecting' || !$st{headersDone}) {
            station_failed('CONN', "no reply from $st{url}");
        } else {
            station_failed('CONN', 'end of stream');
        }
        return;
    }
    $dg{fromStation} += $n;
    if ($st{headersDone}) {
        $st{pending} .= $buf;
        pump();
        return;
    }
    $st{hdr} .= $buf;
    if ($st{hdr} =~ /\A(.*?)\r?\n\r?\n(.*)\z/s) {
        my ($head, $rest) = ($1, $2);
        $st{hdr} = '';
        headers_received($head, $rest);
    } elsif (length $st{hdr} > $HEADER_MAX) {
        station_failed('HTTP', "headers too long from $st{url}");
    }
}

sub handle_line {
    my ($line) = @_;
    $dg{lines}++;
    if ($line =~ /\A\@RD HELLO U2a2 ([0-9a-f]{8})\z/) {
        # (Re)start: everything is reset.
        stop_station();
        $dg{hello}++;
        say_($linked ? "HELLO again from the Cardputer ($1): starting over." : "Linked with the Cardputer ($1).");
        $linked = 1;
        reply("RDHOST $1");
    } elsif ($line =~ /\A\@RD HELLO (\S+)/) {
        say_("This Cardputer runs RADIO DAYS $1: use the relay script made for it (U2a: RadioDaysRelay_U2.command).")
            unless $versionSaid++;
    } elsif ($line =~ /\A\@RD PING ([0-9a-f]{8})\z/) {
        if ($st{phase} ne 'none') {
            say_("The Cardputer is idle: stopped the station.");
            stop_station();
        }
        $linked = 1;
        reply("RDHOST $1");
    } elsif ($line =~ /\A\@RD TIME ([0-9a-f]{8})\z/) {
        # (U3) Only before RDSYNC: once MP3 bytes flow, a reply would land in the audio.
        if ($st{synced}) {
            say_("Time request while sending audio: not answered.");
        } else {
            reply("RDTIME $1 " . int(time));  # whole UTC seconds (time is Time::HiRes's: fractional)
            say_("Sent the time to the Cardputer (UTC).") unless $timeSaid++;
        }
    } elsif ($line =~ /\A\@RD TUNE ([0-9a-f]{8}) (\S+)\z/) {
        return if $1 eq $lastTuneId;  # a repeat of the TUNE being handled (or answered)
        $lastTuneId = $1;
        tune($1, $2);
    } elsif ($line =~ /\A\@RD STOP ([0-9a-f]{8})\z/) {
        say_("Stopped.") if $st{phase} ne 'none';
        stop_station();
    } elsif ($line =~ /\A\@RD CREDIT ([0-9a-f]{8}) (\d{1,10})\z/) {
        # The running total since RDSYNC (modulo 2^32). Older ids, anything before RDSYNC,
        # and totals that are not newer are ignored, so lost lines cost nothing.
        if ($st{synced} && $st{id} eq $1 && $2 < $TWO32) {
            my $ahead = ($2 - $st{allowedTotal}) % $TWO32;
            if ($ahead > 0 && $ahead < 1048576) {
                $st{allowedTotal} = $2 + 0;
                $st{credit} = ($st{allowedTotal} - $st{sentTotal}) % $TWO32;
                pump();
            }
        }
    } elsif ($line =~ /\A\@RD/) {
        say_("Ignored (not understood): " . substr($line, 0, 80));
    } elsif (length $line) {
        say_("Cardputer> $line");
    }
}

sub read_port {
    my $n = sysread($fh, my $chunk, 4096);
    if (!defined $n || $n == 0) {
        port_lost(defined $n ? 'end of file' : "read: $!");
        return;
    }
    $dg{portBytes} += $n;
    $lastPortRx = time;
    if ($silentSaid) {
        say_("Bytes from the Cardputer again.");
        $silentSaid = 0;
    }
    $inbuf .= $chunk;
    while ($fh) {
        my $i = index($inbuf, "\n");
        if ($i < 0) {
            if (length $inbuf > $LINE_MAX) {
                $inbuf = '';
                $skipLine = 1;
            }
            last;
        }
        my $line = substr($inbuf, 0, $i + 1, '');
        chomp $line;
        $line =~ s/\r\z//;
        if ($skipLine) {
            $skipLine = 0;
            next;
        }
        handle_line($line) if length($line) <= $LINE_MAX;
    }
}

# ---------------------------------------------------------------- main
say_("RADIO DAYS U9 relay. Allowed hosts: $allowedFrom (" . scalar(keys %allowed) . " host:port pairs, "
     . scalar(@redirectRules) . " redirect rules).");
my $caff = fork();
if (defined $caff && $caff == 0) { exec('/usr/bin/caffeinate', '-i', '-w', getppid()) or exit 0; }
$SIG{INT} = $SIG{TERM} = sub { stop_station(); say_("Stopped."); exit 0; };

my $loopAt = time;
while (1) {
    ensure_port();
    my $sel = IO::Select->new($fh);
    if ($st{curl} && (!$st{headersDone} || ($st{credit} > 0 && !length $st{pending}))) {
        $sel->add($st{curl});
    }
    my @ready = $sel->can_read(0.25);
    for my $h (@ready) {
        if ($fh && $h == $fh) {
            read_port();
        } elsif ($st{curl} && $h == $st{curl}) {
            read_curl();
        }
    }
    my $now = time;
    # What held the stream back during this pass: the Cardputer's credit, or the station.
    if ($st{phase} eq 'sending') {
        if ($st{credit} <= 0) { $dg{waitCredit} += $now - $loopAt; }
        elsif (!length $st{pending}) { $dg{waitStation} += $now - $loopAt; }
    }
    $loopAt = $now;
    if ($fh && $now - $lastPortRx >= 10 && !$silentSaid) {
        say_("Nothing at all from the Cardputer for 10 s (no HELLO, CREDIT or log line). Is RADIO DAYS U2a2 running?");
        $silentSaid = 1;
    }
    if ($st{phase} eq 'connecting' && $now >= $st{deadline}) {
        station_failed('CONN', "no reply within $CONNECT_LIMIT s");
    } elsif ($st{phase} eq 'reconnecting' && !$st{headersDone} && $now >= $st{deadline}) {
        station_failed('CONN', 'no reply when reconnecting');
    } elsif ($st{phase} eq 'waiting' && $now >= $st{retryAt}) {
        $st{phase} = 'reconnecting';
        $st{deadline} = $now + $CONNECT_LIMIT;
        $st{redirects} = 0;
        station_failed('CONN', 'curl did not start') unless start_fetch($st{first});
    }
    if ($now - $sentAt >= 10) {
        my $span = $now - $sentAt;
        say_(sprintf("Sent %.1f MB in total, %.1f KB/s", $sent / 1048576, ($sent - $sentMark) / $span / 1024))
            if $st{phase} eq 'sending';
        say_(sprintf("[relay] station %.1f KB/s, to Cardputer %.1f KB/s, waiting for credit %d%%, for the station %d%%, "
                     . "lines from Cardputer %d (HELLO %d), bytes %d",
                     $dg{fromStation} / $span / 1024, $dg{toCard} / $span / 1024, 100 * $dg{waitCredit} / $span,
                     100 * $dg{waitStation} / $span, $dg{lines}, $dg{hello}, $dg{portBytes}))
            if $fh;
        $dg{$_} = 0 for keys %dg;
        ($sentAt, $sentMark) = ($now, $sent);
    }
}
