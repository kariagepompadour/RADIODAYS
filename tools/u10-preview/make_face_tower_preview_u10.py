#!/usr/bin/env python3
# RADIO DAYS (design, after U9): the radio screen at 240 x 135 with
#   - plan A: the dial on an ivory face, black ticks (flat)
#   - plan B: the same with a 2 px grey bevel round the outside (sunken window)
#   - two small radio towers in the empty corners above the station name, with their
#     waves lit, dimmed or dark
# drawn on the Mac. The dial is drawn as RadioDays_U9.ino's drawDial() draws it (the same
# rows, pitches, tick lengths, frame, marks and needle; only the colours change), the rest
# of the screen as U9 draws it, with the device's 6 x 8 font (M5GFX glcdfont.h) and
# colours quantised to RGB565. Not the firmware: nothing here is in U10.
#   python3 tools/u10-preview/make_face_tower_preview_u10.py <glcdfont.h> <output folder>
# Also checks that the new pixels (bevel, towers, waves) touch nothing U9 draws.
# SPDX-License-Identifier: MIT
import importlib.util
import math
import os
import sys

sys.dont_write_bytecode = True  # no __pycache__ in the U5 preview folder
HERE = os.path.dirname(os.path.abspath(__file__))
FONT, OUT = sys.argv[1], sys.argv[2]
sys.argv = ["u5", FONT, __import__("tempfile").mkdtemp()]
spec = importlib.util.spec_from_file_location("u5prev", os.path.join(HERE, "..", "dial-preview", "make_dial_preview.py"))
u5 = importlib.util.module_from_spec(spec)
_out = sys.stdout
sys.stdout = open(os.devnull, "w")
spec.loader.exec_module(u5)  # (it writes U5's images into a temporary folder)
sys.stdout = _out


def q(rgb):  # as lcd.color565(): what the panel can show
    r, g, b = rgb
    return u5.c565(((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3))


# ---- dial base (RadioDays_U9.ino; U10 uses the same dial code)
DIAL_FX0, DIAL_FY0, DIAL_FW, DIAL_FH = 31, 19, 142, 74
DIAL_X0, DIAL_W, DIAL_Y0, DIAL_Y1 = 32, 140, 20, 91
ROW_H, MARK_Y, NEEDLE_X, NEEDLE_Y0 = 16, 21, 101, 24
TICK_SHORT, TICK_LONG = 4, 9
U9 = dict(face=(0, 0, 0), frame=(96, 96, 96), base=(60, 60, 60), short=(120, 120, 120), long=(200, 190, 160),
          mark=u5.AMBER, bevel=None)


def ring_steps(n):
    return 30 * ((2 * n + 1) // 3)


def station_pos(i, n):
    return (2 * i + 1) * ring_steps(n) // (2 * n)


def dial_tick(r, u):
    pitch = 6 if r == 3 else r + 2
    if u % pitch:
        return 0
    return 1 if u % (5 * pitch) else 2


def draw_dial(s, n, pos, st):
    """As drawDial(f) with the whole dial drawn; `st` the colour set."""
    face = q(st["face"])
    s.rect(DIAL_X0, DIAL_Y0, DIAL_W, DIAL_Y1 - DIAL_Y0 + 1, face)
    if st["bevel"]:  # plan B: two lines round the face, dark above / left, light below / right
        (o_dark, o_light), (i_dark, i_light) = st["bevel"]
        x0, y0, x1, y1 = DIAL_FX0 - 1, DIAL_FY0 - 1, DIAL_FX0 + DIAL_FW, DIAL_FY0 + DIAL_FH
        for x in range(x0, x1 + 1):
            s.set(x, y0, q(o_dark)); s.set(x, y1, q(o_light))
        for y in range(y0 + 1, y1):
            s.set(x0, y, q(o_dark)); s.set(x1, y, q(o_light))
        x0, y0, x1, y1 = DIAL_FX0, DIAL_FY0, DIAL_FX0 + DIAL_FW - 1, DIAL_FY0 + DIAL_FH - 1
        for x in range(x0, x1 + 1):
            s.set(x, y0, q(i_dark)); s.set(x, y1, q(i_light))
        for y in range(y0 + 1, y1):
            s.set(x0, y, q(i_dark)); s.set(x1, y, q(i_light))
    else:
        c = q(st["frame"])
        for x in range(DIAL_FX0, DIAL_FX0 + DIAL_FW):
            s.set(x, DIAL_FY0, c); s.set(x, DIAL_FY0 + DIAL_FH - 1, c)
        for y in range(DIAL_FY0, DIAL_FY0 + DIAL_FH):
            s.set(DIAL_FX0, y, c); s.set(DIAL_FX0 + DIAL_FW - 1, y, c)
    for r in range(4):
        for x in range(DIAL_X0, DIAL_X0 + DIAL_W):
            s.set(x, 40 + r * ROW_H, q(st["base"]))
    s.vline(NEEDLE_X, NEEDLE_Y0, DIAL_Y1 - NEEDLE_Y0 + 1, q((235, 70, 40)))
    s.vline(NEEDLE_X + 1, NEEDLE_Y0 + 1, DIAL_Y1 - NEEDLE_Y0, q((90, 25, 15)))
    scroll = pos * 2
    for x in range(DIAL_X0, DIAL_X0 + DIAL_W):
        if x in (NEEDLE_X, NEEDLE_X + 1):
            continue
        for r in range(4):
            t = dial_tick(r, scroll + (x - NEEDLE_X))
            if t:
                ln = TICK_LONG if t == 2 else TICK_SHORT
                s.vline(x, 40 + r * ROW_H - ln, ln, q(st["long"] if t == 2 else st["short"]))
    ring = ring_steps(n)
    period = ring * 2
    for i in range(n):
        x = NEEDLE_X + ((station_pos(i, n) - pos) % ring) * 2
        while x - period >= DIAL_X0 - 2:
            x -= period
        while x <= DIAL_X0 + DIAL_W + 1:
            for row in range(3):
                for xx in range(max(x - 2 + row, DIAL_X0), min(x + 2 - row, DIAL_X0 + DIAL_W - 1) + 1):
                    s.set(xx, MARK_Y + row, st["mark"] if isinstance(st["mark"], tuple) and len(st["mark"]) == 3 and st["mark"] == u5.AMBER else q(st["mark"]))
            x += period


# ---- the towers (design)
TOWER_GREY, TOWER_DARK = (150, 150, 145), (95, 95, 92)
WAVE_LIT, WAVE_DIM = (255, 150, 60), (110, 62, 28)
LEFT_TOWER = (15, 75)    # (x of the mast, y of the tip): left corner x 0..29, y 66..95
RIGHT_TOWER = (206, 75)  # right corner x 174..239, y 70..95, under the sky window (same height as the left)
TOWER_BASE_Y = 94


def line_px(x0, y0, x1, y1):
    pts = []
    dx, dy = abs(x1 - x0), -abs(y1 - y0)
    sx, sy = (1 if x0 < x1 else -1), (1 if y0 < y1 else -1)
    err = dx + dy
    while True:
        pts.append((x0, y0))
        if x0 == x1 and y0 == y1:
            return pts
        e2 = 2 * err
        if e2 >= dy:
            err += dy; x0 += sx
        if e2 <= dx:
            err += dx; y0 += sy


def tower_pixels(cx, top):
    """A slim lattice tower: a mast with its tip at `top`, two straight legs from 3 px to
    9 px wide at TOWER_BASE_Y, struts every few rows and an X between them. Lines only."""
    px = {}
    for y in range(top, top + 4):
        px[(cx, y)] = TOWER_GREY                                  # mast
    y0, y1 = top + 4, TOWER_BASE_Y
    leg_l, leg_r = line_px(cx - 1, y0, cx - 4, y1), line_px(cx + 1, y0, cx + 4, y1)
    for p in leg_l + leg_r:
        px[p] = TOWER_GREY
    half = lambda y: 1 + round(3 * (y - y0) / (y1 - y0))
    levels = [y0, y0 + (y1 - y0) // 3, y0 + 2 * (y1 - y0) // 3, y1]
    for y in levels[1:]:
        for x in range(cx - half(y) + 1, cx + half(y)):
            px[(x, y)] = TOWER_DARK                               # strut
    for k, (a, b) in enumerate(zip(levels, levels[1:])):          # one zigzag brace a section
        sgn = 1 if k % 2 == 0 else -1
        for p in line_px(cx - sgn * half(b), b, cx + sgn * half(a), a):
            px.setdefault(p, TOWER_DARK)
    return px


def wave_pixels(cx, top, radii=(5, 9)):
    """Thin arcs either side of the tip, 30 degrees each way from level: ( ( | ) )."""
    px = set()
    for r in radii:
        for a10 in range(-300, 301, 10):
            a = math.radians(a10 / 10)
            dx, dy = round(r * math.cos(a)), round(r * math.sin(a))
            px.add((cx + dx, top + dy))
            px.add((cx - dx, top + dy))
    return px


def draw_tower(s, where, waves):
    """waves: None (dark), 'lit', 'dim', or a set of radii lit (ripple)."""
    cx, top = where
    for (x, y), c in tower_pixels(cx, top).items():
        s.set(x, y, q(c))
    if waves is None:
        return
    if waves in ("lit", "dim"):
        for x, y in wave_pixels(cx, top):
            s.set(x, y, q(WAVE_LIT if waves == "lit" else WAVE_DIM))
    else:
        for r in (5, 9):
            for x, y in wave_pixels(cx, top, (r,)):
                s.set(x, y, q(WAVE_LIT if r in waves else WAVE_DIM))


# ---- the radio screen (U9 / U10 base)
def screen(st, loc="UK", n=7, i=2, name="Radio X London", status="PLAYING", status_c=None,
           vol="VOL  25%", towers=None, level=5):
    s = u5.Screen()
    s.text(4, 2, 2, u5.AMBER, "RADIO DAYS")
    s.text(156, 2, 2, u5.OK, "  AUTO")
    for seg in range(8):
        y = 66 - (seg + 1) * 4 - seg * 2
        on = seg < level
        col = ((0, 230, 0) if on else (0, 48, 0)) if seg < 4 else (((240, 220, 0) if on else (56, 50, 0)) if seg < 6 else ((240, 0, 0) if on else (56, 0, 0)))
        s.rect(6, y, 22, 4, q(col))
    s.text(176, 20, 1, u5.WHITE, "2026.10.22")
    s.text(176, 29, 2, u5.AMBER, "09:41")
    u5.draw_sky_day(s)
    draw_dial(s, n, station_pos(i, n), st)
    s.text((240 - 12 * len(name)) // 2, 96, 2, u5.WHITE, name)
    s.text(4, 114, 1, status_c or u5.OK, status)
    if status in ("PLAYING", "BUFFERING 60%"):
        label = "%d/%d" % (i + 1, n)
        s.text(max(154, 181 - 6 * len(label)), 114, 1, u5.DIM, label)
    s.text(186, 114, 1, u5.WHITE, vol)
    s.text(4, 126, 1, u5.AMBER, loc)
    s.text(50, 126, 1, u5.DIM, "WiFi -55dBm 142K")
    s.text(166, 126, 1, u5.DIM, "A:MODE L:LOC")
    if towers:
        draw_tower(s, LEFT_TOWER, towers[0])
        draw_tower(s, RIGHT_TOWER, towers[1])
    return s


IVORY, WHITE_FACE = (240, 234, 214), (250, 250, 246)
FACE = dict(face=IVORY, frame=(96, 96, 96), base=(150, 145, 132), short=(0, 0, 0), long=(0, 0, 0), mark=u5.AMBER, bevel=None)
PLAN_A = dict(FACE)
PLAN_B = dict(FACE, bevel=(((70, 70, 70), (200, 200, 200)), ((40, 40, 40), (150, 150, 150))))
PLAN_B_WHITE = dict(PLAN_B, face=WHITE_FACE)
PLAN_B_MARK = dict(PLAN_B, mark=(200, 80, 0))


def crop(sc, x0, y0, w, h):
    c = u5.Screen(w, h)
    c.px = [row[x0:x0 + w] for row in sc.px[y0:y0 + h]]
    return c


# ---- the check: the new pixels touch nothing U9 draws (and stay clear of what can change)
def new_pixels(plan_b=True):
    px = set()
    if plan_b:
        x0, y0, x1, y1 = DIAL_FX0 - 1, DIAL_FY0 - 1, DIAL_FX0 + DIAL_FW, DIAL_FY0 + DIAL_FH
        px |= {(x, y0) for x in range(x0, x1 + 1)} | {(x, y1) for x in range(x0, x1 + 1)}
        px |= {(x0, y) for y in range(y0, y1 + 1)} | {(x1, y) for y in range(y0, y1 + 1)}
    for cx, top in (LEFT_TOWER, RIGHT_TOWER):
        px |= set(tower_pixels(cx, top)) | wave_pixels(cx, top)
    return px


RESERVED = {  # everything U9 may draw on the radio screen (whole boxes, the widest texts)
    "title RADIO DAYS": (4, 2, 123, 17), "AUTO / MANUAL": (156, 2, 239, 17),
    "level meter": (6, 20, 27, 65), "date": (176, 20, 237, 27), "clock": (176, 29, 237, 44),
    "sky window": (176, 45, 237, 69), "dial frame (U9)": (31, 19, 172, 92),
    "station name (19 chars)": (0, 96, 239, 111), "status / n/N / volume": (0, 114, 239, 121),
    "footer": (0, 126, 239, 133),
}


def check():
    ok = True
    t = new_pixels()
    bevel = new_pixels(True) - new_pixels(False)
    towers = new_pixels(False)
    for name, (x0, y0, x1, y1) in RESERVED.items():
        hit = [p for p in towers if x0 <= p[0] <= x1 and y0 <= p[1] <= y1]
        if name != "dial frame (U9)":
            hit += [p for p in bevel if x0 <= p[0] <= x1 and y0 <= p[1] <= y1]
        if hit:
            ok = False
            print("OVERLAP  %s: %d pixels, e.g. %s" % (name, len(hit), sorted(hit)[:3]))
    for name, pts in (("left tower + waves", {p for p in towers if p[0] < 120}), ("right tower + waves", {p for p in towers if p[0] >= 120})):
        xs, ys = [p[0] for p in pts], [p[1] for p in pts]
        print("%-20s x %d..%d, y %d..%d (%d x %d px)" % (name, min(xs), max(xs), min(ys), max(ys), max(xs) - min(xs) + 1, max(ys) - min(ys) + 1))
    xs, ys = [p[0] for p in bevel], [p[1] for p in bevel]
    print("%-20s x %d..%d, y %d..%d (outside U9's frame; U9's frame line becomes the inner bevel line)" % ("plan B outer line", min(xs), max(xs), min(ys), max(ys)))
    print("no overlap with anything U9 draws" if ok else "OVERLAPS FOUND")
    return ok


os.makedirs(OUT, exist_ok=True)
ok = check()
P = "%s/face-tower-%s.png"
u5.save([screen(U9), screen(PLAN_A, towers=("lit", "dim")), screen(PLAN_B, towers=("lit", "dim"))],
        P % (OUT, "1-plans"), 2, captions=["U9 base", "plan A: ivory face, flat", "plan B: ivory face, grey bevel"])
u5.save([screen(PLAN_B, towers=("lit", "dim")), screen(PLAN_B, towers=("dim", "lit")),
         screen(PLAN_B, status="BUFFERING 60%", status_c=u5.c565(0xFFE0), towers=("dim", "dim")),
         screen(PLAN_B, status="WAITING FOR WI-FI/USB", status_c=u5.c565(0xFFE0), towers=(None, None), level=0)],
        P % (OUT, "2-waves"), 2, vertical=False,
        captions=["PLAYING: left lit", "PLAYING: right lit (0.8 s later)", "BUFFERING: both dim", "WAITING: dark"])
u5.save([screen(PLAN_B, loc="ITALY", n=5, i=4, name="Virgin Radio Italia", towers=("lit", "dim")),
         screen(PLAN_B, loc="WORLD", n=6, i=0, name="Radio Paradise", status="TUNING...", status_c=u5.AMBER, towers=(None, None), level=0),
         screen(PLAN_B_WHITE, towers=("lit", "dim"))],
        P % (OUT, "3-checks"), 2,
        captions=["the longest name (19 chars)", "AUTO gliding: no name, dark", "plan B on white (250,250,246)"])
big = screen(PLAN_B, towers=("lit", "dim"))
flat = screen(PLAN_A, towers=("lit", "dim"))
u5.save([crop(big, 0, 60, 60, 40), crop(big, 168, 60, 72, 40), crop(big, 20, 10, 40, 30), crop(flat, 20, 10, 40, 30),
         crop(big, 140, 70, 40, 30)],
        P % (OUT, "4-zoom"), 6,
        captions=["left tower", "right tower", "B corner", "A corner", "B lower right"])
u5.save([screen(PLAN_B, towers=({5}, {5})), screen(PLAN_B, towers=({9}, {9})), screen(PLAN_B, towers=(set(), set()))],
        P % (OUT, "5-ripple"), 2,
        captions=["option: ripple 1 (inner arc)", "ripple 2 (outer arc)", "ripple 3 (rest, both dim)"])
u5.save([crop(screen(PLAN_B), 30, 17, 145, 78), crop(screen(PLAN_B_MARK), 30, 17, 145, 78)],
        P % (OUT, "6-marks"), 4, captions=["station marks: amber (U9's)", "darker orange (200, 80, 0)"])
sys.exit(0 if ok else 1)
