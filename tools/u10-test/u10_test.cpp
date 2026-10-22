// RADIO DAYS U10: host test of the ivory dial face, its bevel and the radio towers (see
// run_u10_test.sh). The dial, bevel, tower and wave functions and drawRadio()'s wave step
// are the sketch's own (U10), and U9's drawDial() is drawn beside it for comparison; the
// display is a 240 x 135 pixel array that records every pixel written.
// SPDX-License-Identifier: MIT
#include <algorithm>
#include <atomic>
#include <cstdarg>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <set>
#include <utility>
#include <vector>
using std::max;
using std::min;

#include "stations.h"

static std::atomic<const Location*> gLoc{&LOCATIONS[LOCATION_DEFAULT]};
static inline int locFreqMin() { return gLoc.load()->freqMin; }
static inline int locFreqMax() { return gLoc.load()->freqMax; }
static constexpr uint16_t COL_BG = 0, COL_AMBER = 0xFD20;
static uint32_t nowMs = 0;
static uint32_t millis() { return nowMs; }
enum RadioState : uint8_t { RS_OFF, RS_WAIT_WIFI, RS_CONNECTING, RS_BUFFERING, RS_PLAYING, RS_SKIPPING,
                            RS_ALL_FAILED, RS_NO_MEMORY, RS_NO_SIGNAL, RS_NOISE };

struct Lcd {
    uint16_t fb[135][240];
    long writes = 0;
    std::set<std::pair<int, int>> touched;
    static constexpr uint16_t color565(uint8_t r, uint8_t g, uint8_t b)
    {
        return (uint16_t)(((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3));
    }
    void put(int x, int y, uint16_t c)
    {
        if (x < 0 || y < 0 || x >= 240 || y >= 135) return;
        fb[y][x] = c;
        writes++;
        touched.insert({x, y});
    }
    void fillRect(int x, int y, int w, int h, uint16_t c)
    {
        for (int j = y; j < y + h; j++)
            for (int i = x; i < x + w; i++) put(i, j, c);
    }
    void drawRect(int x, int y, int w, int h, uint16_t c)
    {
        for (int i = x; i < x + w; i++) { put(i, y, c); put(i, y + h - 1, c); }
        for (int j = y; j < y + h; j++) { put(x, j, c); put(x + w - 1, j, c); }
    }
    void drawFastVLine(int x, int y, int h, uint16_t c) { fillRect(x, y, 1, h, c); }
    void drawFastHLine(int x, int y, int w, uint16_t c) { fillRect(x, y, w, 1, c); }
    void drawPixel(int x, int y, uint16_t c) { put(x, y, c); }
    void clear() { memset(fb, 0, sizeof(fb)); touched.clear(); writes = 0; }
} lcd;

#include "ring_part.inc"     // floorMod, dialRing (U10)
#include "dial_part.inc"     // U10: dial, bevel, towers, waves
namespace u9 {
#include "dial_u9_part.inc"  // U9's dial, for comparison
}
struct { int32_t waveL = -1, waveR = -1; } view;
static void waveStep(uint8_t shown)
{
#include "wave_step.inc"     // drawRadio()'s wave step, as in the sketch
}

static int passes = 0, fails = 0;
static void check(bool ok, const char* name, const char* fmt = "", ...)
{
    char extra[200];
    va_list a;
    va_start(a, fmt);
    vsnprintf(extra, sizeof(extra), fmt, a);
    va_end(a);
    printf("%s  %-84s %s\n", ok ? "PASS" : "FAIL", name, extra);
    (ok ? passes : fails)++;
}

static uint16_t snap[135][240];
static void fresh(int f)  // the dial drawn whole at f, as after a full-screen drawing
{
    lcd.clear();
    dialView = DialView();
    drawDial(f);
}
static bool sameAsFresh(int f, int* diff = nullptr)
{
    memcpy(snap, lcd.fb, sizeof(snap));
    DialView keep = dialView;
    fresh(f);
    int d = 0;
    for (int y = 0; y < 135; y++)
        for (int x = 0; x < 240; x++) d += snap[y][x] != lcd.fb[y][x];
    memcpy(lcd.fb, snap, sizeof(snap));
    dialView = keep;
    if (diff) *diff = d;
    return d == 0;
}
static int ringOf(const Location& L) { return L.freqMax - L.freqMin + 1; }

int main()
{
    const uint16_t NEEDLE = Lcd::color565(235, 70, 40), SHADE = Lcd::color565(90, 25, 15);

    printf("-- the face, the bevel, the ticks (U10 drawDial, whole)\n");
    {
        gLoc = &LOCATIONS[2];  // UK
        fresh(LOCATIONS[2].stations[2].freq);
        bool inside = true, palette = true;
        for (int y = 20; y <= 91; y++)
            for (int x = 32; x <= 171; x++) {
                uint16_t c = lcd.fb[y][x];
                palette = palette && (c == DIAL_FACE || c == DIAL_TICK_C || c == DIAL_BASE_C || c == DIAL_MARK_C ||
                                      c == NEEDLE || c == SHADE);
            }
        for (auto& p : lcd.touched) inside = inside && p.first >= 30 && p.first <= 173 && p.second >= 18 && p.second <= 93;
        check(palette, "inside x 32..171, y 20..91: only face, black ticks, baselines, marks, needle");
        check(inside, "drawDial() writes nothing outside the bevel (x 30..173, y 18..93)");
        bool bevel = true;
        for (int x = 30; x <= 173; x++) bevel = bevel && lcd.fb[18][x] == BEVEL_OUT_DARK && lcd.fb[93][x] == BEVEL_OUT_LIGHT;
        for (int y = 19; y <= 92; y++) bevel = bevel && lcd.fb[y][30] == BEVEL_OUT_DARK && lcd.fb[y][173] == BEVEL_OUT_LIGHT;
        for (int x = 31; x <= 172; x++) bevel = bevel && lcd.fb[19][x] == BEVEL_IN_DARK && lcd.fb[92][x] == BEVEL_IN_LIGHT;
        for (int y = 20; y <= 91; y++) bevel = bevel && lcd.fb[y][31] == BEVEL_IN_DARK && lcd.fb[y][172] == BEVEL_IN_LIGHT;
        check(bevel, "bevel: outer x 30..173 y 18..93 (70 / 200), inner = U9's frame line (40 / 150); dark above-left");
        check(DIAL_FACE == Lcd::color565(240, 234, 214) && DIAL_BASE_C == Lcd::color565(150, 145, 132) &&
                  DIAL_MARK_C == Lcd::color565(200, 80, 0) && DIAL_TICK_C == 0,
              "colours: ivory (240,234,214), baselines (150,145,132), marks (200,80,0), ticks black");
    }

    printf("-- the same ticks, marks and needle as U9 (only the colours differ)\n");
    {
        int bad = 0, cases = 0;
        for (int li = 0; li < LOCATION_COUNT; li++) {
            const Location& L = LOCATIONS[li];
            gLoc = &L;
            for (int pos = 0; pos < ringOf(L); pos += 7) {
                cases++;
                fresh(L.freqMin + pos);
                static uint16_t a[135][240];
                memcpy(a, lcd.fb, sizeof(a));
                lcd.clear();
                u9::dialView = u9::DialView();
                u9::drawDial(L.freqMin + pos);
                for (int y = 20; y <= 91; y++)
                    for (int x = 32; x <= 171; x++) {
                        bool ink10 = a[y][x] != DIAL_FACE, ink9 = lcd.fb[y][x] != 0;
                        // U9's black ticks are not ink there, but U10's black ticks are: compare U9's
                        // ink (non-black) with U10's non-face pixels
                        if (ink10 != ink9) bad++;
                    }
            }
        }
        check(bad == 0, "every Location, every 7th position: U10's ink pixels are exactly U9's", "%d cases, %d pixels differ", cases, bad);
    }

    printf("-- no residue: the dial drawn step by step equals the dial drawn whole\n");
    {
        int worst = 0, steps = 0;
        bool ok = true;
        for (int li = 0; li < LOCATION_COUNT; li++) {
            const Location& L = LOCATIONS[li];
            gLoc = &L;
            int f = L.stations[0].freq;
            fresh(f);
            for (int k = 0; k < ringOf(L) + 5; k++) {  // AUTO glide: one step at a time, round the seam
                f = L.freqMin + floorMod(f - L.freqMin + 1, ringOf(L));
                drawDial(f);
                int d;
                ok = ok && sameAsFresh(f, &d);
                worst = max(worst, d);
                steps++;
            }
            for (int k = 0; k < ringOf(L) + 5; k++) {  // and back
                f = L.freqMin + floorMod(f - L.freqMin - 1, ringOf(L));
                drawDial(f);
                int d;
                ok = ok && sameAsFresh(f, &d);
                worst = max(worst, d);
                steps++;
            }
        }
        check(ok, "AUTO: every 1-step move both ways, every Location, across the seam: no stray pixel", "%d moves, worst %d px", steps, worst);
        srand(9);
        ok = true;
        int jumps = 0;
        for (int li = 0; li < LOCATION_COUNT; li++) {
            const Location& L = LOCATIONS[li];
            gLoc = &L;
            int f = L.stations[0].freq;
            fresh(f);
            for (int k = 0; k < 200; k++) {  // MANUAL turns, station jumps and long jumps
                int d = (k % 3 == 0) ? (rand() % 9 - 4) : (rand() % ringOf(L));
                f = L.freqMin + floorMod(f - L.freqMin + d, ringOf(L));
                drawDial(f);
                ok = ok && sameAsFresh(f);
                jumps++;
            }
            for (int i = 0; i < L.count; i++) {
                drawDial(L.stations[i].freq);
                ok = ok && sameAsFresh(L.stations[i].freq);
                jumps++;
            }
        }
        check(ok, "MANUAL and jumps (random, every station): the same pixels as drawn whole", "%d moves", jumps);
        // the needle and the bevel are never written after the whole drawing
        gLoc = &LOCATIONS[3];
        int f = LOCATIONS[3].stations[0].freq;
        fresh(f);
        lcd.touched.clear();
        for (int k = 0; k < 300; k++) { f = LOCATIONS[3].freqMin + floorMod(f - LOCATIONS[3].freqMin + 1, ringOf(LOCATIONS[3])); drawDial(f); }
        bool guarded = true;
        for (auto& p : lcd.touched) {
            int x = p.first, y = p.second;
            bool needle = (x == 101 || x == 102) && y >= 24 && y <= 91;
            bool rim = x <= 31 || x >= 172 || y <= 19 || y >= 92;
            guarded = guarded && !needle && !rim;
        }
        check(guarded, "moving never touches the needle or the bevel (no flicker there)", "%zu pixels written", lcd.touched.size());
    }

    printf("-- the towers and their waves\n");
    {
        lcd.clear();
        drawTowers();
        for (uint16_t c : {WAVE_LIT_C, WAVE_DIM_C}) { drawTowerWaves(TOWER_L_X, c); drawTowerWaves(TOWER_R_X, c); }
        int lx0 = 999, lx1 = -1, ly0 = 999, ly1 = -1, rx0 = 999, rx1 = -1, ry0 = 999, ry1 = -1;
        for (auto& p : lcd.touched) {
            int x = p.first, y = p.second;
            if (x < 120) { lx0 = min(lx0, x); lx1 = max(lx1, x); ly0 = min(ly0, y); ly1 = max(ly1, y); }
            else { rx0 = min(rx0, x); rx1 = max(rx1, x); ry0 = min(ry0, y); ry1 = max(ry1, y); }
        }
        check(lx0 == 6 && lx1 == 24 && ly0 == 71 && ly1 == 94, "left tower and waves: x 6..24, y 71..94", "x %d..%d y %d..%d", lx0, lx1, ly0, ly1);
        check(rx0 == 197 && rx1 == 215 && ry0 == 71 && ry1 == 94, "right tower and waves: x 197..215, y 71..94", "x %d..%d y %d..%d", rx0, rx1, ry0, ry1);
        struct Box { const char* name; int x0, y0, x1, y1; };
        const Box reserved[] = {
            {"title", 4, 2, 123, 17}, {"AUTO / MANUAL", 156, 2, 239, 17}, {"level meter", 6, 20, 27, 65},
            {"date", 176, 20, 237, 27}, {"clock", 176, 29, 237, 44}, {"sky window", 176, 45, 237, 69},
            {"dial and bevel", 30, 18, 173, 93}, {"station name", 0, 96, 239, 111}, {"status, n/N, volume", 0, 114, 239, 121},
            {"footer", 0, 126, 239, 133}};
        int hits = 0;
        for (auto& p : lcd.touched)
            for (auto& b : reserved)
                if (p.first >= b.x0 && p.first <= b.x1 && p.second >= b.y0 && p.second <= b.y1) {
                    hits++;
                    printf("      on %s: %d,%d\n", b.name, p.first, p.second);
                }
        check(hits == 0, "towers and waves touch nothing else the radio screen draws (incl. 19-char names)");
        std::set<std::pair<int, int>> body, wave;
        for (auto& t : TOWER_BODY_PX) body.insert({t[0], t[1]});
        for (auto& t : TOWER_BRACE_PX) body.insert({t[0], t[1]});
        for (auto& t : TOWER_WAVE_PX) wave.insert({t[0], t[1]});
        bool apart = true;
        for (auto& w : wave) apart = apart && !body.count(w);
        check(apart && wave.size() == 36 && body.size() == 65, "waves never draw over the tower (36 wave points, 65 tower points)");
    }

    printf("-- the waves follow the status row\n");
    {
        uint16_t l, r;
        struct S { uint8_t shown; const char* name; uint16_t wl, wr; } cases[] = {
            {RS_BUFFERING, "BUFFERING: both dim", WAVE_DIM_C, WAVE_DIM_C},
            {RS_CONNECTING, "CONNECTING: both dim", WAVE_DIM_C, WAVE_DIM_C},
            {RS_WAIT_WIFI, "WAITING FOR WI-FI/USB: dark", COL_BG, COL_BG},
            {RS_OFF, "STOPPED: dark", COL_BG, COL_BG},
            {RS_NOISE, "NO STATION (between stations): dark", COL_BG, COL_BG},
            {RS_NO_SIGNAL, "NO SIGNAL: dark", COL_BG, COL_BG},
            {RS_SKIPPING, "UNAVAILABLE - NEXT: dark", COL_BG, COL_BG},
            {RS_ALL_FAILED, "NO STATION - RETRY: dark", COL_BG, COL_BG},
            {RS_NO_MEMORY, "NOT ENOUGH MEMORY: dark", COL_BG, COL_BG},
            {0xFE, "TUNING... (gliding, waiting to open): dark", COL_BG, COL_BG},
            {0xFD, "USB RELAY REQUIRED: dark", COL_BG, COL_BG},
        };
        for (auto& c : cases) {
            bool ok = true;
            for (uint32_t t = 0; t < 4000; t += 100) {
                towerWaveColours(c.shown, t, l, r);
                ok = ok && l == c.wl && r == c.wr;
            }
            check(ok, c.name);
        }
        bool alt = true;
        for (uint32_t t = 0; t < 8000; t += 50) {
            towerWaveColours(RS_PLAYING, t, l, r);
            bool leftLit = (t / WAVE_BLINK_MS) % 2 == 0;
            alt = alt && l == (leftLit ? WAVE_LIT_C : WAVE_DIM_C) && r == (leftLit ? WAVE_DIM_C : WAVE_LIT_C);
        }
        towerWaveColours(RS_PLAYING, 799, l, r);
        uint16_t l2, r2;
        towerWaveColours(RS_PLAYING, 800, l2, r2);
        check(alt && l == WAVE_LIT_C && l2 == WAVE_DIM_C && r2 == WAVE_LIT_C && WAVE_BLINK_MS == 800,
              "PLAYING (muted too: the status stays PLAYING): one side lit, the other dim, swapping every 800 ms");

        // drawRadio()'s step: a side is drawn only when its colour changes
        lcd.clear();
        view.waveL = view.waveR = -1;
        nowMs = 0;
        waveStep(RS_CONNECTING);
        long first = lcd.writes;
        lcd.writes = 0;
        for (nowMs = 100; nowMs < 2000; nowMs += 100) waveStep(RS_BUFFERING);
        long still = lcd.writes;
        nowMs = 2000;
        waveStep(RS_PLAYING);  // 2000 / 800 = 2: left lit
        long toPlay = lcd.writes - still;
        lcd.writes = 0;
        for (nowMs = 2100; nowMs < 2400; nowMs += 100) waveStep(RS_PLAYING);
        long within = lcd.writes;
        nowMs = 2400;  // 3: right lit
        waveStep(RS_PLAYING);
        long swap = lcd.writes - within;
        bool colours = lcd.fb[75][15 + 9] == WAVE_DIM_C && lcd.fb[75][206 + 9] == WAVE_LIT_C;
        lcd.writes = 0;
        nowMs = 2500;
        waveStep(RS_WAIT_WIFI);
        long off = lcd.writes;
        bool dark = lcd.fb[75][15 + 9] == COL_BG && lcd.fb[75][206 + 9] == COL_BG && lcd.fb[75][15 - 5] == COL_BG;
        check(first == 72 && still == 0, "first step draws both sides (72 points); then nothing while unchanged", "%ld, %ld", first, still);
        check(toPlay == 36 && within == 0 && swap == 72 && colours,
              "BUFFERING -> PLAYING: one side changes (36); a swap redraws both (72); otherwise nothing", "%ld, %ld, %ld", toPlay, within, swap);
        check(off == 72 && dark, "-> WAITING: both sides cleared to the background", "%ld", off);
        lcd.clear();
        drawTowers();
        long tower = lcd.writes;
        view.waveL = view.waveR = -1;
        waveStep(RS_PLAYING);
        check(tower == 130, "the towers themselves: 130 points, with the whole screen only", "%ld", tower);
    }

    printf("%d passed, %d failed\n", passes, fails);
    return fails ? 1 : 0;
}
