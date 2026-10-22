#!/bin/sh
# RADIO DAYS U10: host tests (no Cardputer).
#   sh tools/u10-test/run_u10_test.sh            offline tests
#   sh tools/u10-test/run_u10_test.sh --live     also the relay's live redirect checks
# 1. The sketch is U9's but for the U10 changes (U9 rebuilt from U10 by undoing them must be
#    U9's file, character for character); stations.h and the other headers are U9's.
# 2. The ivory face, the bevel, no residue after AUTO / MANUAL moves (every step drawn
#    equals the dial drawn whole), the same ink as U9, the towers' places, the waves against
#    the status row, and drawing only what changed (u10_test.cpp).
# 3. U9's tests on U10's code: stations, U8's AUTO glide test, n/N, freq_calc, U4's host
#    test, U9's relay (tools/u9-test/run_u9_test.sh without its U8 comparison).
# U9's own tests (tools/u9-test) are not changed; they still run on U9.
# SPDX-License-Identifier: MIT
set -e
HERE=$(cd "$(dirname "$0")" && pwd)
ROOT=$(cd "$HERE/../.." && pwd)
FW="$ROOT/firmware/RadioDays_U10"
U9="$ROOT/firmware/RadioDays_U9"
WORK=$(mktemp -d)
fails=0

echo "-- 1. U10's sketch is U9's but for the U10 changes"
python3 -I - "$FW/RadioDays_U10.ino" "$U9/RadioDays_U9.ino" <<'PY' || fails=$((fails + 1))
import sys
u10, u9 = open(sys.argv[1]).read(), open(sys.argv[2]).read()
t = u10
assert t.startswith("/*\n  RADIO DAYS — U10:")
i = t.index("  (U9 notes follow.)\n\n")
t = "/*\n" + t[i + len("  (U9 notes follow.)\n\n"):]
def cut(a, b, keep=""):
    """Remove from the line starting `a` up to (and including) the text `b`, once."""
    global t
    i = t.index(a)
    j = t.index(b, i) + len(b)
    assert t.count(a) == 1, a
    t = t[:i] + keep + t[j:]
def swap(a, b):
    global t
    assert t.count(a) == 1, a
    t = t.replace(a, b)
swap("Install only the app-only RadioDays_U10.ino.bin", "Install only the app-only RadioDays_U9.ino.bin")
swap('BUILD_TAG = "U10";', 'BUILD_TAG = "U9";')
cut("\n// (U10) The dial face (docs/dial-face-tower-proposal.md, plan B).", "BEVEL_IN_LIGHT = rgb565(150, 150, 150);\n", "")
swap("""                             DIAL_TICK_C);  // (U10) black on the ivory face""",
     """                             t == 2 ? lcd.color565(200, 190, 160) : lcd.color565(120, 120, 120));""")
cut("// (U10) A sunken window: two 1 px rectangles", "lcd.drawFastVLine(x1, y0 + 1, y1 - y0 - 1, light);\n}\n\n", "")
swap("""        lcd.fillRect(DIAL_X0, DIAL_Y0, DIAL_W, DIAL_Y1 - DIAL_Y0 + 1, DIAL_FACE);  // (U10) the face
        dialBevelRect(DIAL_FX0 - 1, DIAL_FY0 - 1, DIAL_FX0 + DIAL_FW, DIAL_FY0 + DIAL_FH, BEVEL_OUT_DARK, BEVEL_OUT_LIGHT);
        dialBevelRect(DIAL_FX0, DIAL_FY0, DIAL_FX0 + DIAL_FW - 1, DIAL_FY0 + DIAL_FH - 1, BEVEL_IN_DARK, BEVEL_IN_LIGHT);
        for (int r = 0; r < DIAL_ROWS; r++) lcd.drawFastHLine(DIAL_X0, dialBase(r), DIAL_W, DIAL_BASE_C);""",
     """        lcd.fillRect(DIAL_X0, DIAL_Y0, DIAL_W, DIAL_Y1 - DIAL_Y0 + 1, COL_BG);
        lcd.drawRect(DIAL_FX0, DIAL_FY0, DIAL_FW, DIAL_FH, lcd.color565(96, 96, 96));
        for (int r = 0; r < DIAL_ROWS; r++) lcd.drawFastHLine(DIAL_X0, dialBase(r), DIAL_W, lcd.color565(60, 60, 60));""")
swap("        dialMarks(dialView.pos, DIAL_FACE);  // (U10) cleared to the face", "        dialMarks(dialView.pos, COL_BG);")
swap("TICK_LONG - (t ? (t == 2 ? TICK_LONG : TICK_SHORT) : 0), DIAL_FACE);", "TICK_LONG - (t ? (t == 2 ? TICK_LONG : TICK_SHORT) : 0), COL_BG);")
swap("    dialMarks(pos, DIAL_MARK_C);\n", "    dialMarks(pos, COL_AMBER);\n")
cut("// --- (U10) Radio towers:", "    drawTowerPoints(cx, TOWER_WAVE_PX, sizeof(TOWER_WAVE_PX) / sizeof(TOWER_WAVE_PX[0]), c);\n}\n\n", "")
swap("    int32_t waveL = -1, waveR = -1;  // (U10) the towers' wave colours drawn (-1: none yet)\n", "")
swap("        drawTowers();     // (U10) the waves follow below, from the status\n", "")
cut("\n    // (U10) The towers' waves, from the status row's value", "drawTowerWaves(TOWER_R_X, waveR); }\n", "")
if t == u9:
    print("PASS  U10.ino = U9.ino + the U10 notes, .bin name, BUILD_TAG, face / bevel / mark colours, the 3 clears,")
    print("      dialBevelRect(), the towers section, 1 view field, drawTowers() and the wave step; nothing else differs")
else:
    import difflib
    print("FAIL  U10.ino differs from U9.ino beyond the U10 changes:")
    print("\n".join(list(difflib.unified_diff(u9.splitlines(), t.splitlines(), "U9", "U10 undone", lineterm="", n=1))[:40]))
    sys.exit(1)
PY
for f in stations.h AudioOutputM5Speaker.h globe_land.h radiodays_link.h radiodays_types.h; do
    if cmp -s "$FW/$f" "$U9/$f"; then echo "PASS  $f is U9's"; else echo "FAIL  $f differs from U9"; fails=$((fails + 1)); fi
done

echo "-- 2. the face, the bevel, no residue, the towers and waves"
python3 -I - "$FW/RadioDays_U10.ino" "$U9/RadioDays_U9.ino" "$WORK" <<'PY'
import sys
t, t9, work = open(sys.argv[1]).read(), open(sys.argv[2]).read(), sys.argv[3]
i = t.index("static inline int floorMod(")
open(work + "/ring_part.inc", "w").write(t[i:t.index("\n", t.index("static inline int dialRing(")) + 1])
open(work + "/dial_part.inc", "w").write(t[t.index("// --- (U5) Analog dial"):t.index("// --- (2-5) clock, (3-3) sky window")])
open(work + "/dial_u9_part.inc", "w").write(t9[t9.index("// --- (U5) Analog dial"):t9.index("// --- (2-5) clock, (3-3) sky window")])
i = t.index("    uint16_t waveL, waveR;\n")
j = t.index("drawTowerWaves(TOWER_R_X, waveR); }\n", i) + len("drawTowerWaves(TOWER_R_X, waveR); }\n")
open(work + "/wave_step.inc", "w").write(t[i:j])
PY
cp "$FW/stations.h" "$WORK/"
clang++ -std=c++17 -O1 -Wall -Wextra -Wno-unused-function -Wno-unused-variable -I"$WORK" -o "$WORK/u10_test" "$HERE/u10_test.cpp"
"$WORK/u10_test" || fails=$((fails + 1))

echo "-- 3. U9's tests on U10's code (without U9's comparison with U8)"
sed -e "s|^HERE=.*|HERE=$ROOT/tools/u9-test|" \
    -e "s|^FW=\"\$ROOT/firmware/RadioDays_U9\"|FW=\"$FW\"|" \
    -e 's|"\$FW/RadioDays_U9.ino"|"$FW/RadioDays_U10.ino"|g' \
    -e '/^echo "-- 1\. U9/,/^echo "-- 2\./{/^echo "-- 2\./!d;}' \
    "$ROOT/tools/u9-test/run_u9_test.sh" > "$WORK/run_u9_on_u10.sh"
grep -q "RadioDays_U10.ino" "$WORK/run_u9_on_u10.sh" && ! grep -q '^echo "-- 1\.' "$WORK/run_u9_on_u10.sh" \
    || { echo "FAIL  could not point U9's tests at U10"; fails=$((fails + 1)); }
# U4's host test is pointed at U9 inside run_u9_test.sh: point it at U10
sed -e 's#s|firmware/RadioDays_U4|firmware/RadioDays_U9|; s|RadioDays_U4.ino|RadioDays_U9.ino|#s|firmware/RadioDays_U4|firmware/RadioDays_U10|; s|RadioDays_U4.ino|RadioDays_U10.ino|#' \
    "$WORK/run_u9_on_u10.sh" > "$WORK/r.sh"
grep -q 's|RadioDays_U4.ino|RadioDays_U10.ino|' "$WORK/r.sh" || { echo "FAIL  could not point U4's host test at U10"; fails=$((fails + 1)); }
sh "$WORK/r.sh" $1 > "$WORK/u9.out" 2>&1 || fails=$((fails + 1))
grep -E "^FAIL|passed|failed|^-- " "$WORK/u9.out"

echo "-- U10 host tests: $fails group(s) failed"
[ "$fails" = 0 ]
