#!/bin/sh
# RADIO DAYS: the Windows relay (RadioDaysRelay_U9_Windows.ps1) must sort URLs exactly as the
# macOS relay (RadioDaysRelay_U9.command) does. Runs both with --check-url and
# --check-redirect on the same URLs (U10's stations, the rule tests' URLs and edge cases),
# with the built-in lists and with a test allowed_hosts.txt, and compares every line.
# Mac only; needs PowerShell 7 (pwsh) on the PATH or in $PWSH. No Cardputer, no network.
#   sh tools/usb-relay-windows-test/run_compare_rules.sh
# SPDX-License-Identifier: MIT
HERE=$(cd "$(dirname "$0")" && pwd)
ROOT=$(cd "$HERE/../.." && pwd)
PWSH=${PWSH:-pwsh}
WORK=$(mktemp -d)
pass=0; fail=0
ok() { pass=$((pass + 1)); printf 'PASS  %s\n' "$1"; }
ng() { fail=$((fail + 1)); printf 'FAIL  %s\n' "$1"; }

# The URLs: U10's stations, every http URL in the relay rule tests, and edge cases.
{
    grep -o '"http://[^"]*"' "$ROOT/firmware/RadioDays_U10/stations.h" | tr -d '"'
    cat "$ROOT"/tools/relay-rule-test/*.sh "$ROOT"/tools/u9-test/run_relay_rule_test_u9.sh 2>/dev/null |
        grep -o '"[hH][tT][tT][pP][sS]*://[^"]*"' | tr -d '"'
    cat <<'URLS'
http://stream.radioparadise.com
http://stream.radioparadise.com/
http://stream.radioparadise.com:80/mp3-192
http://stream.radioparadise.com:080/mp3-192
http://stream.radioparadise.com:0/x
http://stream.radioparadise.com:65536/x
http://stream.radioparadise.com:99999/x
http://STREAM.RadioParadise.COM/MP3-192
HTTP://stream.radioparadise.com/x
https://stream.radioparadise.com/x
ftp://stream.radioparadise.com/x
http:/stream.radioparadise.com/x
http://stream.radioparadise.com./x
http://.stream.radioparadise.com/x
http://-a.rndfnk.com/x
http://a-.rndfnk.com/x
http://a..rndfnk.com/x
http://a.rndfnk.com/x?b=1&c=2;d=%41+e,f!g*h(i)$j~k
http://a.rndfnk.com/x#frag
http://a.rndfnk.com/x y
http://a.rndfnk.com/x"y
http://a.rndfnk.com/x'y
http://a.rndfnk.com/x<y>
http://user:pw@stream.radioparadise.com/x
http://127.0.0.1/x
http://10.0.0.1:8000/x
http://1.2.3/x
http://[::1]/x
http://stream.radioparadise.com\x
http://a.b.c.d.e.f.g.h.antenne.de/x
http://rndfnk.com/x
http://xrndfnk.com/x
http://x.rndfnk.com:81/x
http://x.rndfnk.com.evil.org/x
http://localhost/x
http://x.localhost/x
URLS
    # long URLs around the 300-character limit
    for n in 270 271 272 273; do
        printf 'http://stream.radioparadise.com/%s\n' "$(head -c "$n" /dev/zero | tr '\0' a)"
    done
} | sort -u > "$WORK/urls.txt"
COUNT=$(wc -l < "$WORK/urls.txt" | tr -d ' ')

compare() {  # compare <folder> <label>
    for mode in --check-url --check-redirect; do
        # one call each; the URLs are arguments (no shell is involved)
        tr '\n' '\0' < "$WORK/urls.txt" | xargs -0 perl "$1/RadioDaysRelay_U9.command" "$mode" > "$WORK/perl.out" 2>"$WORK/perl.err"
        tr '\n' '\0' < "$WORK/urls.txt" | xargs -0 "$PWSH" -NoProfile -NonInteractive -File "$1/RadioDaysRelay_U9_Windows.ps1" "$mode" > "$WORK/ps.out" 2>"$WORK/ps.err"
        sed -e 's|Allowed hosts: .*/allowed_hosts.txt|Allowed hosts: allowed_hosts.txt|' "$WORK/perl.out" > "$WORK/a"
        sed -e 's|Allowed hosts: .*/allowed_hosts.txt|Allowed hosts: allowed_hosts.txt|' "$WORK/ps.out" > "$WORK/b"
        lines=$(($(wc -l < "$WORK/a") - 1))
        oks=$(grep -c '^OK' "$WORK/a")
        if cmp -s "$WORK/a" "$WORK/b" && [ "$lines" = "$COUNT" ]; then
            ok "$2 $mode: $lines URLs, identical ($oks OK)"
        else
            ng "$2 $mode: outputs differ"; diff "$WORK/a" "$WORK/b" | head -20
        fi
        if cmp -s "$WORK/perl.err" "$WORK/ps.err"; then ok "$2 $mode: identical warnings ($(wc -l < "$WORK/perl.err" | tr -d ' '))"
        else ng "$2 $mode: warnings differ"; diff "$WORK/perl.err" "$WORK/ps.err" | head; fi
    done
}

# 1. The built-in lists (a copy of both scripts, so that an allowed_hosts.txt next to the
#    real scripts cannot change the result).
mkdir -p "$WORK/builtin" "$WORK/file"
cp "$ROOT/tools/usb-relay/RadioDaysRelay_U9.command" "$ROOT/tools/usb-relay/RadioDaysRelay_U9_Windows.ps1" "$WORK/builtin/"
compare "$WORK/builtin" "1 built-in"

# 2. The built-in host lists are the same.
P=$(perl -ne 'if (/my \@BUILTIN_ALLOWED = qw\(/../\);/) { print }' "$WORK/builtin/RadioDaysRelay_U9.command" | grep -v 'qw(\|);' | tr -s ' \n' '\n' | grep . | sort)
W=$(sed -n '/^\$BUILTIN_ALLOWED = @(/,/^)/p' "$WORK/builtin/RadioDaysRelay_U9_Windows.ps1" | grep -o "'[^']*'" | tr -d "'" | sort)
[ "$P" = "$W" ] && ok "2 the same $(echo "$P" | wc -l | tr -d ' ') built-in host:port pairs" || { ng "2 the built-in host lists differ"; }
PR=$(perl -ne 'if (/my \@BUILTIN_REDIRECT = qw\(/../\);/) { print }' "$WORK/builtin/RadioDaysRelay_U9.command" | grep -v 'qw(\|);' | tr -s ' \n' '\n' | grep . | sort)
WR=$(grep '^\$BUILTIN_REDIRECT' "$WORK/builtin/RadioDaysRelay_U9_Windows.ps1" | grep -o "'[^']*'" | tr -d "'" | sort)
[ "$PR" = "$WR" ] && ok "2 the same redirect rules: $(echo $PR)" || ng "2 the redirect rules differ"

# 3. An allowed_hosts.txt with good, bad and odd lines (both scripts read the same file).
cp "$WORK/builtin/"* "$WORK/file/"
cat > "$WORK/file/allowed_hosts.txt" <<'HOSTS'
# a comment
stream.radioparadise.com:80
ICE1.SOMAFM.COM:80   # upper case, a comment after
  x.example.org:8000
localhost:18783
redirect *.rndfnk.com:80
REDIRECT *.Antenne.de:80
redirect *.com:80
redirect *.1.2.3:80
redirect rndfnk.com:80
redirect *.srg-ssr.ch:0
noport.example.org
bad host:80
x.example.org:65536
x.example.org:099
HOSTS
compare "$WORK/file" "3 allowed_hosts.txt"

rm -rf "$WORK"
echo "pass $pass, fail $fail"
[ "$fail" = 0 ]
