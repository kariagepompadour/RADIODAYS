#!/bin/sh
# RADIO DAYS: the U4 relay simulation (tools/usb-relay/sim4, unchanged) run against the
# Windows relay (RadioDaysRelay_U9_Windows.ps1) under PowerShell 7 on a Mac, with U10's
# link header. The simulated Cardputer runs U10's radiodays_link.h on a pseudo-terminal;
# local test stations stand in for the internet. It checks the protocol, credit, redirects
# and safety rules, not Windows itself (COM ports, Windows PowerShell 5.1).
# Author's environment only: tools/usb-relay/sim4 is not published. Needs pwsh (or $PWSH).
#   sh tools/usb-relay-windows-test/run_sim_windows.sh
# SPDX-License-Identifier: MIT
set -e
HERE=$(cd "$(dirname "$0")" && pwd)
ROOT=$(cd "$HERE/../.." && pwd)
SIM="$ROOT/tools/usb-relay/sim4"
PWSH=${PWSH:-$(command -v pwsh)}
[ -x "$PWSH" ] || { echo "FAIL  PowerShell 7 (pwsh) not found; set PWSH"; exit 1; }
[ -f "$SIM/device_sim.cpp" ] || { echo "FAIL  $SIM is missing (author's environment only)"; exit 1; }
WORK=$(mktemp -d)
P1=18783
P2=18784
SCRIPT=RadioDaysRelay_U9_Windows.ps1
printf '#include "%s"\n' "$ROOT/firmware/RadioDays_U10/radiodays_link.h" > "$WORK/link_select.h"
# The simulator starts the relay with /usr/bin/perl <script>: here a launcher starts pwsh.
sed -e 's|"/usr/bin/perl"|"./relay_launcher"|g' "$SIM/device_sim.cpp" > "$WORK/device_sim.cpp"
[ "$(grep -c '"./relay_launcher"' "$WORK/device_sim.cpp")" = 2 ] || { echo "FAIL  could not point the simulator at pwsh"; exit 1; }
printf '#!/bin/sh\nexec "%s" -NoProfile -NonInteractive -File "$1"\n' "$PWSH" > "$WORK/relay_launcher"
chmod +x "$WORK/relay_launcher"
cp "$ROOT/tools/usb-relay/$SCRIPT" "$WORK/"
printf 'localhost:%s\nredirect *.cdn.localhost:%s\n' $P1 $P1 > "$WORK/allowed_hosts.txt"
clang++ -std=c++17 -O1 -Wall -Wextra -pthread -I"$WORK" -o "$WORK/device_sim" "$WORK/device_sim.cpp"
python3 -I "$SIM/station_server.py" $P1 $P2 "$WORK/inject.txt" "$WORK/hits.log" > "$WORK/server.log" 2>&1 &
SERVER=$!
trap 'kill $SERVER 2>/dev/null' EXIT
sleep 1
cd "$WORK"
set +e
NO_COLOR=1 ./device_sim "$WORK/$SCRIPT" $P1 $P2 "$WORK/inject.txt" "$WORK/hits.log" "$@"
STATUS=$?
echo "logs: $WORK (device.log, relay.log, server.log)"
exit $STATUS
