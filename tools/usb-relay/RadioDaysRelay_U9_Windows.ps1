# RADIO DAYS - U9 USB relay for Windows (Windows PowerShell 5.1, built into Windows 10 and 11).
# A port of RadioDaysRelay_U9.command (macOS) for RADIO DAYS U9 and U10: the same link
# protocol, built-in allowed hosts, redirect rules, allowed_hosts.txt, --check-url and
# --check-redirect. Nothing to install. Changes no network setting of the PC.
#
# Differences from the macOS script (docs/usb-relay-windows.md):
# - The station is received with .NET's TcpClient (HTTP/1.1, "Connection: close"; a chunked
#   reply is decoded, as curl does), not with curl. Same limits: http only, at most 3
#   redirects (checked with the same rules), no IP address, no user name, 8 s to connect.
#   "localhost" and "*.localhost" mean this PC, as in curl (used by the tests only).
# - The Cardputer's port is found by Espressif's USB vendor id (VID_303A) among the COM
#   ports; --port COM5 chooses one by hand. DTR is set and RTS is left clear (RTS alone
#   could reset the ESP32-S3).
# - Instead of caffeinate, SetThreadExecutionState keeps the PC from sleeping while the
#   relay runs (the display may still turn off).
#
# Use: double-click RadioDaysRelay_U9_Windows.cmd (next to this file), or in PowerShell:
#      powershell -NoProfile -ExecutionPolicy Bypass -File RadioDaysRelay_U9_Windows.ps1
#      Close the window or press Control-C to stop.
#      ... -File RadioDaysRelay_U9_Windows.ps1 --check-url <URL> ...       checks URLs only.
#      ... -File RadioDaysRelay_U9_Windows.ps1 --check-redirect <URL> ...  checks redirect targets only.
#      ... -File RadioDaysRelay_U9_Windows.ps1 --port COM5                 uses that port.
# Allowed hosts: the lists below, or only the lines of allowed_hosts.txt ("host:port",
# "redirect *.domain:port", "#" comments) when that file is next to this script.
#
# SPDX-License-Identifier: MIT

$ErrorActionPreference = 'Stop'

$UA = 'RADIODAYS-relay/U9 (Windows)'
$TWO32 = [int64]4294967296
$CONNECT_LIMIT = 10   # seconds: RDSYNC or RDNAK within this (the Cardputer waits 12)
$CONNECT_TIMEOUT = 8  # seconds to open the TCP connection (curl --connect-timeout 8)
$MAX_REDIRECTS = 3
$MAX_URL = 300
$LINE_MAX = 1024
$HEADER_MAX = 16384
$RETRY_WAIT = 2       # seconds before reconnecting a station that stopped

# The hosts and ports of U9's (and U10's) stations.h, as in RadioDaysRelay_U9.command.
$BUILTIN_ALLOWED = @(
    'ais-sa8.cdnstream1.com:80', 'dispatcher.rndfnk.com:80', 'fm939.wnyc.org:80',
    'fmsetagaya834.out.airtime.pro:8000', 'ice1.somafm.com:80', 'icecast.radiofrance.fr:80',
    'icecast.rtl.fr:80', 'icecast.skyrock.net:80', 'icecast.unitedradio.it:80', 'kexp-mp3-128.streamguys1.com:80',
    'live.radioradicale.it:80', 'media-ice.musicradio.com:80',
    'mp3channels.webradio.antenne.de:80', 'mp3s.nc.u-tokyo.ac.jp:80', 'novazz.ice.infomaniak.ch:80',
    'ottava2.out.airtime.pro:8000', 'radio.kahoku.net:8000', 'radio.plaza.one:80',
    'radionz-ice.streamguys.com:80', 'sc6.radiocaroline.net:8040', 'shonanbeachfm.out.airtime.pro:8000',
    'shoutcast2.radio24.it:8000', 'st01.sslstream.dlf.de:80', 'st02.sslstream.dlf.de:80',
    'stream.live.vc.bbcmedia.co.uk:80', 'stream.radioparadise.com:80', 'stream.srg-ssr.ch:80',
    'stream.wqxr.org:80', 'stream0.wfmu.org:80', 'streaming-ice.audiomeans.fr:80', 'tsfjazz.ice.infomaniak.ch:80',
    'wdr-1live-live.icecast.wdr.de:80', 'wfuv-onair.streamguys.org:80'
)
# Redirect targets only: a host under one of these domains, on this port.
$BUILTIN_REDIRECT = @('*.rndfnk.com:80', '*.antenne.de:80', '*.srg-ssr.ch:80')

$onWindows = ([Environment]::OSVersion.Platform -eq [PlatformID]::Win32NT)
$latin1 = [Text.Encoding]::GetEncoding('iso-8859-1')  # one char per byte, both ways
$clock = [Diagnostics.Stopwatch]::StartNew()
function Now { return $clock.Elapsed.TotalSeconds }

function Say([string]$msg) { [Console]::Out.WriteLine((Get-Date -Format 'HH:mm:ss') + '  ' + $msg) }
function Mod32([int64]$x) { $r = $x % $TWO32; if ($r -lt 0) { $r += $TWO32 }; return $r }
function Rx([string]$text, [string]$pattern) { return [regex]::Match($text, $pattern) }  # case-sensitive, as Perl

# ---------------------------------------------------------------- arguments
$checkMode = $null
$portArg = $null
$checkArgs = @()
$i = 0
while ($i -lt $args.Count) {
    $a = [string]$args[$i]
    if ($i -eq 0 -and ($a -ceq '--check-url' -or $a -ceq '--check-redirect')) {
        $checkMode = $a
    } elseif ($checkMode) {
        $checkArgs += $a
    } elseif ($a -ceq '--port' -and $i + 1 -lt $args.Count) {
        $i++
        $portArg = ([string]$args[$i]).ToUpperInvariant()
    } else {
        [Console]::Error.WriteLine("Unknown argument: $a")
        exit 2
    }
    $i++
}

# ---------------------------------------------------------------- allowed hosts and ports
$allowed = @{}
$allowedHost = @{}
$redirectRules = New-Object System.Collections.ArrayList
$allowedFrom = 'built-in list'

# "*.domain:port" -> @(domain, port), or $null. The domain needs two labels or more
# (no "*.com") and may not be an IP address.
function Parse-RedirectRule([string]$r) {
    $m = Rx $r '\A\*\.([A-Za-z0-9.-]+):([0-9]{1,5})\z'
    if (-not $m.Success) { return $null }
    $p = [int]$m.Groups[2].Value
    if ($p -lt 1 -or $p -gt 65535) { return $null }
    $d = $m.Groups[1].Value.ToLowerInvariant()
    if (-not (Rx $d '\A[a-z0-9](?:[a-z0-9-]*[a-z0-9])?(?:\.[a-z0-9](?:[a-z0-9-]*[a-z0-9])?)+\z').Success) { return $null }
    if ((Rx $d '\A[0-9.]+\z').Success) { return $null }
    return @{ domain = $d; port = $p }
}

$list = $BUILTIN_ALLOWED
$rules = $BUILTIN_REDIRECT
$hostsFile = Join-Path $PSScriptRoot 'allowed_hosts.txt'
if (Test-Path -LiteralPath $hostsFile -PathType Leaf) {
    $list = @()
    $rules = @()
    foreach ($l in [IO.File]::ReadAllLines($hostsFile)) {
        $l = $l -replace '#.*', ''
        $l = $l.Trim()
        if ($l.Length -eq 0) { continue }
        $m = Rx $l '(?i)\Aredirect\s+(\S+)\z'
        if ($m.Success) {
            if (Parse-RedirectRule $m.Groups[1].Value) { $rules += $m.Groups[1].Value }
            else { [Console]::Error.WriteLine("allowed_hosts.txt: not ""redirect *.domain:port"", ignored: $l") }
            continue
        }
        $m = Rx $l '\A([A-Za-z0-9.-]+):([0-9]{1,5})\z'
        if ($m.Success -and [int]$m.Groups[2].Value -ge 1 -and [int]$m.Groups[2].Value -le 65535) {
            $list += $m.Groups[1].Value.ToLowerInvariant() + ':' + [int]$m.Groups[2].Value
        } else {
            [Console]::Error.WriteLine("allowed_hosts.txt: not ""host:port"", ignored: $l")
        }
    }
    $allowedFrom = $hostsFile
}
foreach ($hp in $list) {
    $allowed[$hp] = $true
    $allowedHost[$hp.Split(':')[0]] = $true
}
foreach ($r in $rules) {
    $rule = Parse-RedirectRule $r
    if (-not $rule) { throw "Bad built-in redirect rule: $r" }
    [void]$redirectRules.Add($rule)
}

# A redirect target's host and port against the redirect rules: the host must end in
# "." + the domain (a label boundary) with at least one label before it, and the port
# must be the rule's. The host has already passed the host-name checks of Check-Url.
function Test-RedirectRule([string]$h, [int]$port) {
    foreach ($r in $redirectRules) {
        if ($port -ne $r.port) { continue }
        $d = $r.domain
        if ($h.Length -gt $d.Length + 1 -and $h.EndsWith('.' + $d, [StringComparison]::Ordinal)) { return $true }
    }
    return $false
}

# Returns @{norm; why}: the normalised URL, or $null and the reason. Redirect targets fail as REDIR.
function Check-Url([string]$u, [bool]$redirect) {
    $bad = 'URL'
    if ($redirect) { $bad = 'REDIR' }
    $no = @{ norm = $null; why = $bad }
    if ($null -eq $u -or $u.Length -gt $MAX_URL) { return $no }
    if (-not (Rx $u '\A[A-Za-z0-9\-._~/?=&%;:+,!*()$]+\z').Success) { return $no }  # no @ # space quotes
    $m = Rx $u '(?is)\Ahttp://([A-Za-z0-9.-]+)(?::([0-9]{1,5}))?(/.*)?\z'
    if (-not $m.Success) { return $no }
    $h = $m.Groups[1].Value.ToLowerInvariant()
    $port = 80
    if ($m.Groups[2].Success) { $port = [int]$m.Groups[2].Value }
    $path = '/'
    if ($m.Groups[3].Success) { $path = $m.Groups[3].Value }
    if ($port -lt 1 -or $port -gt 65535) { return $no }
    if (-not (Rx $h '\A[a-z0-9](?:[a-z0-9-]*[a-z0-9])?(?:\.[a-z0-9](?:[a-z0-9-]*[a-z0-9])?)*\z').Success) { return $no }
    if ((Rx $h '\A[0-9.]+\z').Success) { return $no }  # an IP address
    $hostPort = $h
    if ($port -ne 80) { $hostPort = "${h}:$port" }
    if (-not $allowed.ContainsKey("${h}:$port")) {
        if ($redirect -and (Test-RedirectRule $h $port)) { return @{ norm = "http://$hostPort$path"; why = $null } }
        if ($redirect) { return @{ norm = $null; why = 'REDIR' } }
        if ($allowedHost.ContainsKey($h)) { return @{ norm = $null; why = 'PORT' } }
        return @{ norm = $null; why = 'HOST' }
    }
    return @{ norm = "http://$hostPort$path"; why = $null }
}

# A redirect's Location against the current (normalised) URL: absolute or "/path" only.
function Resolve-Location($loc, [string]$base) {
    if ($null -eq $loc) { return $null }
    $loc = ([string]$loc) -replace '^\s+|\s+$', ''
    if ((Rx $loc '\A[A-Za-z][A-Za-z0-9+.-]*:').Success) { return $loc }
    if ((Rx $loc '\A/(?!/)').Success) {
        return (Rx $base '\A(http://[^/]+)').Groups[1].Value + $loc
    }
    return $null
}

if ($checkMode) {
    [Console]::Out.WriteLine("Allowed hosts: $allowedFrom")
    foreach ($u in $checkArgs) {
        $c = Check-Url $u ($checkMode -ceq '--check-redirect')
        if ($c.norm) { [Console]::Out.WriteLine('OK        ' + $c.norm) }
        else { [Console]::Out.WriteLine(('NO {0,-6} {1}' -f $c.why, $u)) }
    }
    exit 0
}

# ---------------------------------------------------------------- state
# $g: the port and counters. $st: the station; phase none | connecting (before RDSYNC) |
# sending | waiting (to reconnect) | reconnecting. Both are cleared in place, never replaced.
$g = @{
    sp = $null; portPath = $null; chosen = $null; inbuf = ''; skipLine = $false; searchSaid = $false
    linked = $false; lastTuneId = ''; versionSaid = $false; timeSaid = $false
    lastPortRx = (Now); silentSaid = $false; lastPortCheck = (Now)
    sent = [int64]0; sentAt = (Now); sentMark = [int64]0
    pend = (New-Object byte[] 65536); pendStart = 0; pendLen = 0
    rbuf = (New-Object byte[] 4096)
}
$dg = @{}
function Reset-Diag { foreach ($k in @('fromStation', 'toCard', 'waitCredit', 'waitStation', 'lines', 'hello', 'portBytes')) { $dg[$k] = 0 } }
Reset-Diag
$st = @{ phase = 'none' }

# ---------------------------------------------------------------- the Cardputer's port
function Find-Ports {
    if ($env:RD_PORT) {  # tests only
        if (Test-Path -LiteralPath $env:RD_PORT) { return ,@($env:RD_PORT) }
        return ,@()
    }
    $names = @([IO.Ports.SerialPort]::GetPortNames() | ForEach-Object { $_.ToUpperInvariant() } | Sort-Object -Unique)
    if ($portArg) {
        if ($names -contains $portArg) { return ,@($portArg) }
        return ,@()
    }
    $found = @()
    try {
        $devs = Get-CimInstance -ClassName Win32_PnPEntity -Filter "PNPClass = 'Ports'" -ErrorAction Stop
        foreach ($d in $devs) {
            if ([string]$d.PNPDeviceID -like 'USB\VID_303A*') {
                $m = Rx ([string]$d.Name) '\((COM[0-9]+)\)'
                if ($m.Success -and $names -contains $m.Groups[1].Value.ToUpperInvariant()) {
                    $found += $m.Groups[1].Value.ToUpperInvariant()
                }
            }
        }
    } catch {
        if (-not $g.cimSaid) { Say "Cannot list USB devices ($($_.Exception.Message)). Start with --port COMn."; $g.cimSaid = $true }
    }
    return ,@($found | Sort-Object -Unique)
}

function Open-Port([string]$name) {
    $p = $null
    try {
        $p = New-Object System.IO.Ports.SerialPort $name, 115200, ([IO.Ports.Parity]::None), 8, ([IO.Ports.StopBits]::One)
        $p.Handshake = [IO.Ports.Handshake]::None
        $p.ReadBufferSize = 65536
        $p.WriteBufferSize = 65536
        $p.WriteTimeout = [IO.Ports.SerialPort]::InfiniteTimeout  # USB holds us back when busy
        if ($onWindows) {
            $p.RtsEnable = $false  # RTS without DTR could reset the ESP32-S3
            $p.DtrEnable = $true
        }
        $p.Open()
        return $p
    } catch {
        if ($p) { try { $p.Dispose() } catch { } }
        if (-not $g.openSaid) { Say "Cannot open ${name}: $($_.Exception.Message) (is another program using it?)"; $g.openSaid = $true }
        return $null
    }
}

# Finds and opens the port; waits (2 s steps) until there is one. Never returns without one.
function Ensure-Port {
    while (-not $g.sp) {
        $ports = Find-Ports
        $pick = $null
        if ($g.chosen -and ($ports -contains $g.chosen)) {
            $pick = $g.chosen
        } elseif ($ports.Count -eq 1) {
            $pick = $ports[0]
        } elseif ($ports.Count -gt 1 -and -not $g.chosen) {
            [Console]::Out.WriteLine('Several Cardputer (Espressif) USB serial ports were found:')
            for ($k = 0; $k -lt $ports.Count; $k++) { [Console]::Out.WriteLine('  ' + ($k + 1) + ') ' + $ports[$k]) }
            $n = 0
            [void][int]::TryParse((Read-Host 'Type the number of the Cardputer and press Return'), [ref]$n)
            if ($n -ge 1 -and $n -le $ports.Count) { $pick = $ports[$n - 1] }
        }
        if ($pick) {
            $p = Open-Port $pick
            if ($p) {
                $g.sp = $p
                $g.portPath = $pick
                $g.chosen = $pick
                $g.inbuf = ''
                $g.skipLine = $false
                $g.searchSaid = $false
                $g.openSaid = $false
                Say "Port $pick opened. Waiting for RADIO DAYS (U2a2 or later) (""@RD HELLO"")..."
                $g.lastPortRx = Now
                $g.lastPortCheck = Now
                return
            }
        }
        if (-not $g.searchSaid) {
            Say 'Waiting for the Cardputer (connect it with a USB data cable and start RADIO DAYS)...'
            $g.searchSaid = $true
        }
        Start-Sleep -Seconds 2
    }
}

# ---------------------------------------------------------------- the station
function Clear-Pending { $g.pendStart = 0; $g.pendLen = 0 }

function Add-Pending([byte[]]$b, [int]$off, [int]$n) {
    if ($n -le 0) { return }
    if ($g.pendStart + $g.pendLen + $n -gt $g.pend.Length) {
        if ($g.pendLen -gt 0) { [Array]::Copy($g.pend, $g.pendStart, $g.pend, 0, $g.pendLen) }
        $g.pendStart = 0
        if ($g.pendLen + $n -gt $g.pend.Length) {
            $nb = New-Object byte[] ([Math]::Max($g.pend.Length * 2, $g.pendLen + $n))
            [Array]::Copy($g.pend, 0, $nb, 0, $g.pendLen)
            $g.pend = $nb
        }
    }
    [Array]::Copy($b, $off, $g.pend, $g.pendStart + $g.pendLen, $n)
    $g.pendLen += $n
}

function Stop-Fetch {
    if ($st.client) {
        try { $st.client.Close() } catch { }
    }
    $st.client = $null
    $st.task = $null
    $st.stream = $null
}

function Stop-Station {
    Stop-Fetch
    $st.Clear()
    $st.phase = 'none'
    Clear-Pending
}

function Port-Lost([string]$why) {
    Stop-Station
    if ($g.sp) { try { $g.sp.Close(); $g.sp.Dispose() } catch { } }
    $g.sp = $null
    $g.linked = $false
    Say "The Cardputer port was closed ($why). Searching again..."
}

function Write-Port([byte[]]$b, [int]$off, [int]$n) {
    if (-not $g.sp) { return $false }
    try {
        $g.sp.Write($b, $off, $n)
        return $true
    } catch {
        Port-Lost "write: $($_.Exception.Message)"
        return $false
    }
}

function Reply([string]$text) {
    $b = [Text.Encoding]::ASCII.GetBytes("`n$text`n")
    return (Write-Port $b 0 $b.Length)
}

function Start-Fetch([string]$url) {
    Stop-Fetch
    $m = Rx $url '\Ahttp://([^/:]+)(?::([0-9]+))?(/.*)?\z'  # a URL from Check-Url
    $h = $m.Groups[1].Value
    $port = 80
    if ($m.Groups[2].Success) { $port = [int]$m.Groups[2].Value }
    $path = '/'
    if ($m.Groups[3].Success) { $path = $m.Groups[3].Value }
    $hostHeader = $h
    if ($port -ne 80) { $hostHeader = "${h}:$port" }
    $connectTo = $h
    if ($h -ceq 'localhost' -or $h.EndsWith('.localhost', [StringComparison]::Ordinal)) { $connectTo = 'localhost' }  # as curl (RFC 6761)
    try {
        $c = New-Object System.Net.Sockets.TcpClient
        $c.NoDelay = $true
        $st.client = $c
        $st.task = $c.ConnectAsync($connectTo, $port)
    } catch {
        Say "Cannot start the connection: $($_.Exception.Message)"
        Stop-Fetch
        return $false
    }
    $st.url = $url
    $st.request = "GET $path HTTP/1.1`r`nHost: $hostHeader`r`nUser-Agent: $UA`r`nAccept: */*`r`nConnection: close`r`n`r`n"
    $st.connectBy = (Now) + $CONNECT_TIMEOUT
    $st.connected = $false
    $st.hdr = ''
    $st.headersDone = $false
    $st.chunked = $false
    return $true
}

# Before RDSYNC: refuse with a reason. After: wait and reconnect.
function Station-Failed([string]$why, [string]$detail) {
    if (-not $st.synced) {
        Say "Refused (${why}): $detail"
        $id = $st.id
        Stop-Station
        [void](Reply "RDNAK $id $why")
        return
    }
    Say "The station stopped ($detail). Reconnecting in $RETRY_WAIT s..."
    Stop-Fetch
    $st.phase = 'waiting'
    $st.retryAt = (Now) + $RETRY_WAIT
}

function Tune([string]$id, [string]$url) {
    Stop-Station  # the old station's bytes stop here: nothing old is written after this
    $c = Check-Url $url $false
    if (-not $c.norm) {
        Say "Refused ($($c.why)): $url"
        [void](Reply "RDNAK $id $($c.why)")
        return
    }
    $st.phase = 'connecting'
    $st.id = $id
    $st.first = $c.norm
    $st.credit = [int64]0
    $st.synced = $false
    $st.redirects = 0
    $st.deadline = (Now) + $CONNECT_LIMIT
    Say "Tuning $($c.norm)"
    if (-not (Start-Fetch $c.norm)) { Station-Failed 'CONN' 'the connection did not start' }
}

# Moves granted bytes from the station to the Cardputer.
function Pump {
    while ($g.sp -and $st.synced -and $st.credit -gt 0 -and $g.pendLen -gt 0) {
        $n = [Math]::Min([Math]::Min([int64]$g.pendLen, [int64]$st.credit), [int64]4096)
        if (-not (Write-Port $g.pend $g.pendStart ([int]$n))) { return }
        $g.pendStart += $n
        $g.pendLen -= $n
        if ($g.pendLen -eq 0) { $g.pendStart = 0 }
        $st.credit -= $n
        $st.sentTotal = Mod32 ($st.sentTotal + $n)
        $g.sent += $n
        $dg.toCard += $n
    }
}

# Body bytes of a 200 reply: decoded when chunked (as curl does), then queued.
function Add-Body([byte[]]$b, [int]$off, [int]$n) {
    if (-not $st.chunked) { Add-Pending $b $off $n; return $true }
    $k = $off
    $end = $off + $n
    while ($k -lt $end) {
        if ($st.ck -eq 'size') {
            $ch = $b[$k]; $k++
            if ($ch -eq 10) {
                $m = Rx $st.ckLine '\A\s*([0-9A-Fa-f]{1,8})\s*(?:;.*)?\r?\z'
                if (-not $m.Success) { Station-Failed 'HTTP' "bad chunked reply from $($st.url)"; return $false }
                $size = [Convert]::ToInt64($m.Groups[1].Value, 16)
                $st.ckLine = ''
                if ($size -eq 0) { $st.ck = 'end' } else { $st.ckLeft = $size; $st.ck = 'data' }
            } elseif ($st.ckLine.Length -lt 200) {
                $st.ckLine += [char]$ch
            }
        } elseif ($st.ck -eq 'data') {
            $take = [int][Math]::Min([int64]($end - $k), $st.ckLeft)
            Add-Pending $b $k $take
            $k += $take
            $st.ckLeft -= $take
            if ($st.ckLeft -eq 0) { $st.ck = 'crlf' }
        } elseif ($st.ck -eq 'crlf') {
            $ch = $b[$k]; $k++
            if ($ch -eq 10) { $st.ck = 'size' }
        } else {
            $k = $end  # after the last chunk: the server closes the connection
        }
    }
    return $true
}

function Headers-Received([string]$head, [string]$rest) {
    $lines = $head -split '\r?\n'
    $status = $lines[0]
    $m = Rx $status '\A(?:HTTP/[0-9](?:\.[0-9])?|ICY) ([0-9]{3})'
    if (-not $m.Success) {
        Station-Failed 'HTTP' "no HTTP status from $($st.url)"
        return
    }
    $code = $m.Groups[1].Value
    if ($code -ceq '200') {
        $st.headersDone = $true
        foreach ($l in $lines) { if ((Rx $l '(?i)\Atransfer-encoding:.*\bchunked\b').Success) { $st.chunked = $true } }
        $st.ck = 'size'
        $st.ckLine = ''
        $rb = $latin1.GetBytes($rest)
        if (-not (Add-Body $rb 0 $rb.Length)) { return }
        if (-not $st.synced) {
            # The reply is checked: from here only this station's bytes, only as granted.
            if (-not (Reply "RDSYNC $($st.id)")) { return }
            $st.synced = $true
            $st.credit = [int64]0
            $st.allowedTotal = [int64]0
            $st.sentTotal = [int64]0
            Say "Sending $($st.url)"
        } else {
            Say "Reconnected to $($st.url)"
        }
        $st.phase = 'sending'
        Pump
        return
    }
    if ((Rx $code '\A30[12378]\z').Success) {
        $loc = $null
        foreach ($l in $lines) {
            $lm = Rx $l '(?i)\Alocation:\s*(.*)\z'
            if ($lm.Success) { $loc = $lm.Groups[1].Value; break }
        }
        $target = Resolve-Location $loc $st.url
        $norm = $null
        if ($null -ne $target) { $norm = (Check-Url $target $true).norm }
        if (-not $norm) {
            $shown = '(none)'
            if ($null -ne $loc) { $shown = $loc }
            Station-Failed 'REDIR' "redirect to $shown"
            return
        }
        $st.redirects++
        if ($st.redirects -gt $MAX_REDIRECTS) {
            Station-Failed 'REDIR' "more than $MAX_REDIRECTS redirects"
            return
        }
        Say "Redirected to $norm"
        if (-not (Start-Fetch $norm)) { Station-Failed 'CONN' 'the connection did not start' }
        return
    }
    Station-Failed 'HTTP' "HTTP $code from $($st.url)"
}

# Connects, sends the request, reads the reply. True when something happened.
function Service-Fetch {
    if (-not $st.connected) {
        if (-not $st.task.IsCompleted) {
            if ((Now) -ge $st.connectBy) {
                Station-Failed 'CONN' "no reply from $($st.url)"
                return $true
            }
            return $false
        }
        if ($st.task.IsFaulted -or $st.task.IsCanceled -or -not $st.client.Connected) {
            Station-Failed 'CONN' "no reply from $($st.url)"
            return $true
        }
        try {
            $st.stream = $st.client.GetStream()
            $req = [Text.Encoding]::ASCII.GetBytes($st.request)
            $st.stream.Write($req, 0, $req.Length)
            $st.connected = $true
        } catch {
            Station-Failed 'CONN' "no reply from $($st.url)"
        }
        return $true
    }
    # As the macOS script: the reply is read only while the headers are incomplete, or when
    # the Cardputer has granted bytes and none are waiting.
    if ($st.headersDone -and -not ($st.credit -gt 0 -and $g.pendLen -eq 0)) { return $false }
    $ready = $false
    try { $ready = $st.client.Client.Poll(0, [Net.Sockets.SelectMode]::SelectRead) } catch { $ready = $true }
    if (-not $ready) { return $false }
    $want = 4096
    if ($st.headersDone -and $st.credit -lt 4096) { $want = [int]$st.credit }
    $n = 0
    try { $n = $st.stream.Read($g.rbuf, 0, $want) } catch { $n = 0 }
    if ($n -le 0) {
        if ($st.phase -eq 'connecting' -or -not $st.headersDone) { Station-Failed 'CONN' "no reply from $($st.url)" }
        else { Station-Failed 'CONN' 'end of stream' }
        return $true
    }
    $dg.fromStation += $n
    if ($st.headersDone) {
        if (Add-Body $g.rbuf 0 $n) { Pump }
        return $true
    }
    $st.hdr += $latin1.GetString($g.rbuf, 0, $n)
    $m = [regex]::Match($st.hdr, '\A(.*?)\r?\n\r?\n(.*)\z', [Text.RegularExpressions.RegexOptions]::Singleline)
    if ($m.Success) {
        $st.hdr = ''
        Headers-Received $m.Groups[1].Value $m.Groups[2].Value
    } elseif ($st.hdr.Length -gt $HEADER_MAX) {
        Station-Failed 'HTTP' "headers too long from $($st.url)"
    }
    return $true
}

# ---------------------------------------------------------------- lines from the Cardputer
function Handle-Line([string]$line) {
    $dg.lines++
    $m = Rx $line '\A@RD HELLO U2a2 ([0-9a-f]{8})\z'
    if ($m.Success) {
        # (Re)start: everything is reset.
        Stop-Station
        $dg.hello++
        if ($g.linked) { Say "HELLO again from the Cardputer ($($m.Groups[1].Value)): starting over." }
        else { Say "Linked with the Cardputer ($($m.Groups[1].Value))." }
        $g.linked = $true
        [void](Reply "RDHOST $($m.Groups[1].Value)")
        return
    }
    $m = Rx $line '\A@RD HELLO (\S+)'
    if ($m.Success) {
        if (-not $g.versionSaid) {
            Say "This Cardputer runs RADIO DAYS $($m.Groups[1].Value): this relay is for U2a2 or later (U9 and U10)."
            $g.versionSaid = $true
        }
        return
    }
    $m = Rx $line '\A@RD PING ([0-9a-f]{8})\z'
    if ($m.Success) {
        if ($st.phase -ne 'none') {
            Say 'The Cardputer is idle: stopped the station.'
            Stop-Station
        }
        $g.linked = $true
        [void](Reply "RDHOST $($m.Groups[1].Value)")
        return
    }
    $m = Rx $line '\A@RD TIME ([0-9a-f]{8})\z'
    if ($m.Success) {
        # Only before RDSYNC: once MP3 bytes flow, a reply would land in the audio.
        if ($st.synced) {
            Say 'Time request while sending audio: not answered.'
        } else {
            [void](Reply ("RDTIME $($m.Groups[1].Value) " + [DateTimeOffset]::UtcNow.ToUnixTimeSeconds()))  # whole UTC seconds
            if (-not $g.timeSaid) { Say 'Sent the time to the Cardputer (UTC).'; $g.timeSaid = $true }
        }
        return
    }
    $m = Rx $line '\A@RD TUNE ([0-9a-f]{8}) (\S+)\z'
    if ($m.Success) {
        if ($m.Groups[1].Value -ceq $g.lastTuneId) { return }  # a repeat of the TUNE being handled (or answered)
        $g.lastTuneId = $m.Groups[1].Value
        Tune $m.Groups[1].Value $m.Groups[2].Value
        return
    }
    $m = Rx $line '\A@RD STOP ([0-9a-f]{8})\z'
    if ($m.Success) {
        if ($st.phase -ne 'none') { Say 'Stopped.' }
        Stop-Station
        return
    }
    $m = Rx $line '\A@RD CREDIT ([0-9a-f]{8}) ([0-9]{1,10})\z'
    if ($m.Success) {
        # The running total since RDSYNC (modulo 2^32). Older ids, anything before RDSYNC,
        # and totals that are not newer are ignored, so lost lines cost nothing.
        $total = [int64]$m.Groups[2].Value
        if ($st.synced -and $st.id -ceq $m.Groups[1].Value -and $total -lt $TWO32) {
            $ahead = Mod32 ($total - $st.allowedTotal)
            if ($ahead -gt 0 -and $ahead -lt 1048576) {
                $st.allowedTotal = $total
                $st.credit = Mod32 ($st.allowedTotal - $st.sentTotal)
                Pump
            }
        }
        return
    }
    if ((Rx $line '\A@RD').Success) {
        $shown = $line
        if ($shown.Length -gt 80) { $shown = $shown.Substring(0, 80) }
        Say "Ignored (not understood): $shown"
    } elseif ($line.Length -gt 0) {
        Say "Cardputer> $line"
    }
}

# Reads what the Cardputer sent. True when something arrived.
function Read-Port {
    $n = 0
    try {
        if (-not $g.sp.IsOpen) { Port-Lost 'closed'; return $true }
        $n = $g.sp.BytesToRead
        if ($n -le 0) { return $false }
        if ($n -gt 4096) { $n = 4096 }
        $n = $g.sp.Read($g.rbuf, 0, $n)
    } catch {
        Port-Lost "read: $($_.Exception.Message)"
        return $true
    }
    if ($n -le 0) { return $false }
    $dg.portBytes += $n
    $g.lastPortRx = Now
    if ($g.silentSaid) {
        Say 'Bytes from the Cardputer again.'
        $g.silentSaid = $false
    }
    $g.inbuf += $latin1.GetString($g.rbuf, 0, $n)
    while ($g.sp) {
        $k = $g.inbuf.IndexOf("`n")
        if ($k -lt 0) {
            if ($g.inbuf.Length -gt $LINE_MAX) {
                $g.inbuf = ''
                $g.skipLine = $true
            }
            break
        }
        $line = $g.inbuf.Substring(0, $k)
        $g.inbuf = $g.inbuf.Substring($k + 1)
        if ($line.EndsWith("`r")) { $line = $line.Substring(0, $line.Length - 1) }
        if ($g.skipLine) {
            $g.skipLine = $false
            continue
        }
        if ($line.Length -le $LINE_MAX) { Handle-Line $line }
    }
    return $true
}

# ---------------------------------------------------------------- keeping the PC awake
function Set-KeepAwake([bool]$on) {
    if (-not $onWindows) { return }
    try {
        if (-not ('RadioDays.Power' -as [type])) {
            Add-Type -Namespace RadioDays -Name Power -MemberDefinition '[DllImport("kernel32.dll")] public static extern uint SetThreadExecutionState(uint esFlags);'
        }
        $flags = [uint32]2147483648  # ES_CONTINUOUS
        if ($on) { $flags = [uint32]2147483649 }  # ES_CONTINUOUS | ES_SYSTEM_REQUIRED
        [void][RadioDays.Power]::SetThreadExecutionState($flags)
    } catch {
        if ($on) { Say "Cannot keep the PC awake ($($_.Exception.Message)): it may sleep while you listen." }
    }
}

# ---------------------------------------------------------------- main
Say ("RADIO DAYS U9 relay for Windows (RADIO DAYS U9 and U10). Allowed hosts: $allowedFrom (" + $allowed.Count +
     ' host:port pairs, ' + $redirectRules.Count + ' redirect rules).')
Set-KeepAwake $true
$loopAt = Now
try {
    while ($true) {
        Ensure-Port
        $busy = $false
        if (Read-Port) { $busy = $true }
        if ($st.client) {
            if (Service-Fetch) { $busy = $true }
        }
        $now = Now
        # What held the stream back during this pass: the Cardputer's credit, or the station.
        if ($st.phase -eq 'sending') {
            if ($st.credit -le 0) { $dg.waitCredit += $now - $loopAt }
            elseif ($g.pendLen -eq 0) { $dg.waitStation += $now - $loopAt }
        }
        $loopAt = $now
        if ($g.sp -and $onWindows -and $now - $g.lastPortCheck -ge 2) {
            $g.lastPortCheck = $now
            if (-not ([IO.Ports.SerialPort]::GetPortNames() -contains $g.portPath)) { Port-Lost 'the port disappeared' }
        }
        if ($g.sp -and $now - $g.lastPortRx -ge 10 -and -not $g.silentSaid) {
            Say 'Nothing at all from the Cardputer for 10 s (no HELLO, CREDIT or log line). Is RADIO DAYS running?'
            $g.silentSaid = $true
        }
        if ($st.phase -eq 'connecting' -and $now -ge $st.deadline) {
            Station-Failed 'CONN' "no reply within $CONNECT_LIMIT s"
        } elseif ($st.phase -eq 'reconnecting' -and -not $st.headersDone -and $now -ge $st.deadline) {
            Station-Failed 'CONN' 'no reply when reconnecting'
        } elseif ($st.phase -eq 'waiting' -and $now -ge $st.retryAt) {
            $st.phase = 'reconnecting'
            $st.deadline = $now + $CONNECT_LIMIT
            $st.redirects = 0
            if (-not (Start-Fetch $st.first)) { Station-Failed 'CONN' 'the connection did not start' }
        }
        if ($now - $g.sentAt -ge 10) {
            $span = $now - $g.sentAt
            if ($st.phase -eq 'sending') {
                Say ('Sent {0:F1} MB in total, {1:F1} KB/s' -f ($g.sent / 1048576), (($g.sent - $g.sentMark) / $span / 1024))
            }
            if ($g.sp) {
                Say (('[relay] station {0:F1} KB/s, to Cardputer {1:F1} KB/s, waiting for credit {2}%, for the station {3}%, ' +
                      'lines from Cardputer {4} (HELLO {5}), bytes {6}') -f ($dg.fromStation / $span / 1024),
                     ($dg.toCard / $span / 1024), [int][Math]::Floor(100 * $dg.waitCredit / $span),
                     [int][Math]::Floor(100 * $dg.waitStation / $span), $dg.lines, $dg.hello, $dg.portBytes)
            }
            Reset-Diag
            $g.sentAt = $now
            $g.sentMark = $g.sent
        }
        if (-not $busy) { Start-Sleep -Milliseconds 5 }
    }
} finally {
    Stop-Station
    if ($g.sp) { try { $g.sp.Close() } catch { } }
    Set-KeepAwake $false
    Say 'Stopped.'
}
