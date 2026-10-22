#!/bin/sh
# RADIO DAYS U10: preview of the radio screen with U10's own dial, bevel, towers and waves
# (drawn by the sketch's functions on the Mac, step by step for the gliding case), and the
# rest of the screen as U9 / U10 draw it (tools/dial-preview: layout, colours, the device's
# 6 x 8 font). Not the Cardputer's pixels.
#   sh tools/u10-preview/make_u10_preview.sh [glcdfont.h] [output folder]
# SPDX-License-Identifier: MIT
set -e
HERE=$(cd "$(dirname "$0")" && pwd)
ROOT=$(cd "$HERE/../.." && pwd)
FONT=${1:-$HOME/Documents/Arduino/libraries/M5GFX/src/lgfx/Fonts/glcdfont.h}
OUT=${2:-$ROOT/docs/images}
WORK=$(mktemp -d)
python3 -I - "$ROOT/firmware/RadioDays_U10/RadioDays_U10.ino" "$WORK" <<'PY'
import sys
t, work = open(sys.argv[1]).read(), sys.argv[2]
i = t.index("static inline int floorMod(")
open(work + "/ring_part.inc", "w").write(t[i:t.index("\n", t.index("static inline int dialRing(")) + 1])
open(work + "/dial_part.inc", "w").write(t[t.index("// --- (U5) Analog dial"):t.index("// --- (2-5) clock, (3-3) sky window")])
i = t.index("    uint16_t waveL, waveR;\n")
j = t.index("drawTowerWaves(TOWER_R_X, waveR); }\n", i) + len("drawTowerWaves(TOWER_R_X, waveR); }\n")
open(work + "/wave_step.inc", "w").write(t[i:j])
PY
cp "$ROOT/firmware/RadioDays_U10/stations.h" "$WORK/"
clang++ -std=c++17 -O1 -w -I"$WORK" -o "$WORK/dump" "$HERE/u10_dump.cpp"
"$WORK/dump" > "$WORK/px.txt"
python3 -I "$HERE/compose_u10_preview.py" "$FONT" "$WORK/px.txt" "$OUT"
