#!/usr/bin/env python3
# RADIO DAYS U10: puts the firmware's dial / tower pixels (from u10_dump) on the radio
# screen drawn as U9 / U10 draw the rest (U5's preview helpers). See make_u10_preview.sh.
# SPDX-License-Identifier: MIT
import importlib.util
import os
import sys

sys.dont_write_bytecode = True
HERE = os.path.dirname(os.path.abspath(__file__))
FONT, PX, OUT = sys.argv[1], sys.argv[2], sys.argv[3]
sys.argv = ["u5", FONT, __import__("tempfile").mkdtemp()]
spec = importlib.util.spec_from_file_location("u5prev", os.path.join(HERE, "..", "dial-preview", "make_dial_preview.py"))
u5 = importlib.util.module_from_spec(spec)
_out = sys.stdout
sys.stdout = open(os.devnull, "w")
spec.loader.exec_module(u5)
sys.stdout = _out

cases, cur = {}, None
for line in open(PX):
    if line.startswith("case "):
        cur = line.split()[1]
        cases[cur] = []
    else:
        x, y, c = line.split()
        cases[cur].append((int(x), int(y), u5.c565(int(c, 16))))


def q(rgb):
    r, g, b = rgb
    return u5.c565(((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3))


def screen(case, loc, n, i, name, status="PLAYING", status_c=None, vol="VOL  25%", level=5, mode="  AUTO"):
    s = u5.Screen()
    s.text(4, 2, 2, u5.AMBER, "RADIO DAYS")
    s.text(156, 2, 2, u5.OK if mode == "  AUTO" else u5.c565(0xFFE0), mode)
    for seg in range(8):
        y = 66 - (seg + 1) * 4 - seg * 2
        on = seg < level
        col = ((0, 230, 0) if on else (0, 48, 0)) if seg < 4 else (((240, 220, 0) if on else (56, 50, 0)) if seg < 6 else ((240, 0, 0) if on else (56, 0, 0)))
        s.rect(6, y, 22, 4, q(col))
    s.text(176, 20, 1, u5.WHITE, "2026.10.22")
    s.text(176, 29, 2, u5.AMBER, "09:41")
    u5.draw_sky_day(s)
    for x, y, c in cases[case]:
        s.set(x, y, c)
    if name:
        s.text((240 - 12 * len(name)) // 2, 96, 2, u5.WHITE, name)
    s.text(4, 114, 1, status_c or u5.OK, status)
    if name and not status.startswith(("WAITING", "TUNING")):
        label = "%d/%d" % (i + 1, n)
        s.text(max(154, 181 - 6 * len(label)), 114, 1, u5.DIM, label)
    s.text(186, 114, 1, (240, 60, 60) if vol.strip() == "MUTE" else u5.WHITE, vol)
    s.text(4, 126, 1, u5.AMBER, loc)
    s.text(50, 126, 1, u5.DIM, "WiFi -55dBm 142K")
    s.text(166, 126, 1, u5.DIM, "A:MODE L:LOC")
    return s


def crop(sc, x0, y0, w, h):
    c = u5.Screen(w, h)
    c.px = [row[x0:x0 + w] for row in sc.px[y0:y0 + h]]
    return c


Y = u5.c565(0xFFE0)
P = OUT + "/u10-%s.png"
u5.save([screen("uk3-left", "UK", 7, 2, "Radio X London"), screen("uk3-right", "UK", 7, 2, "Radio X London"),
         screen("uk3-buffering", "UK", 7, 2, "Radio X London", "BUFFERING 60%", Y)],
        P % "playing", 2, captions=["PLAYING: left lit", "PLAYING: 0.8 s later, right lit", "BUFFERING: both dim"])
u5.save([screen("world1-waiting", "WORLD", 6, 0, "Radio Paradise", "WAITING FOR WI-FI/USB", Y, level=0),
         screen("germany-gliding", "GERMANY", 6, 1, "", "TUNING...", u5.AMBER, level=0),
         screen("italy5-left", "ITALY", 5, 4, "Virgin Radio Italia", vol="    MUTE"),
         screen("france7-right", "FRANCE", 7, 6, "TSF Jazz", mode="MANUAL")],
        P % "states", 2, captions=["WAITING: dark (start, WORLD 1/6)", "AUTO gliding: no name, dark",
                                   "muted: as PLAYING; 19-char name", "MANUAL on a station"])
big = screen("uk3-left", "UK", 7, 2, "Radio X London")
u5.save([crop(big, 0, 14, 60, 86), crop(big, 90, 14, 90, 86), crop(big, 168, 40, 72, 60)], P % "zoom", 5,
        captions=["left: bevel, tower", "needle and marks", "right tower"])
