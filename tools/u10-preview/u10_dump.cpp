// RADIO DAYS U10: draws U10's own dial, bevel, towers and waves (the sketch's functions,
// cut out by make_u10_preview.sh) for the preview, and prints every pixel they wrote as
// "x y rgb565", one case after another ("case <name>").
// SPDX-License-Identifier: MIT
#include <algorithm>
#include <atomic>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <map>
#include <utility>
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
    std::map<std::pair<int, int>, uint16_t> px;
    static constexpr uint16_t color565(uint8_t r, uint8_t g, uint8_t b)
    {
        return (uint16_t)(((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3));
    }
    void put(int x, int y, uint16_t c) { if (x >= 0 && y >= 0 && x < 240 && y < 135) px[{x, y}] = c; }
    void fillRect(int x, int y, int w, int h, uint16_t c) { for (int j = y; j < y + h; j++) for (int i = x; i < x + w; i++) put(i, j, c); }
    void drawRect(int x, int y, int w, int h, uint16_t c) { fillRect(x, y, w, 1, c); fillRect(x, y + h - 1, w, 1, c); fillRect(x, y, 1, h, c); fillRect(x + w - 1, y, 1, h, c); }
    void drawFastVLine(int x, int y, int h, uint16_t c) { fillRect(x, y, 1, h, c); }
    void drawFastHLine(int x, int y, int w, uint16_t c) { fillRect(x, y, w, 1, c); }
    void drawPixel(int x, int y, uint16_t c) { put(x, y, c); }
} lcd;
#include "ring_part.inc"
#include "dial_part.inc"
struct { int32_t waveL = -1, waveR = -1; } view;
static void waveStep(uint8_t shown)
{
#include "wave_step.inc"
}
static void shot(const char* name, int loc, int station, int glide, uint8_t shown, uint32_t t)
{
    lcd.px.clear();
    gLoc = &LOCATIONS[loc];
    dialView = DialView();
    int f = LOCATIONS[loc].stations[station].freq;
    drawDial(f);
    for (int k = 0; k < glide; k++) {  // AUTO: step by step from the station, as the glide draws
        f = locFreqMin() + floorMod(f - locFreqMin() + 1, locFreqMax() - locFreqMin() + 1);
        drawDial(f);
    }
    drawTowers();
    view.waveL = view.waveR = -1;
    nowMs = t;
    waveStep(shown);
    printf("case %s\n", name);
    for (auto& p : lcd.px) printf("%d %d %04x\n", p.first.first, p.first.second, p.second);
}
int main()
{
    shot("uk3-left", 2, 2, 0, RS_PLAYING, 0);
    shot("uk3-right", 2, 2, 0, RS_PLAYING, 800);
    shot("uk3-buffering", 2, 2, 0, RS_BUFFERING, 0);
    shot("world1-waiting", 4, 0, 0, RS_WAIT_WIFI, 0);
    shot("italy5-left", 6, 4, 0, RS_PLAYING, 0);
    shot("germany-gliding", 5, 1, 9, 0xFE, 0);  // 9 steps after SWR3, drawn step by step
    shot("france7-right", 3, 6, 0, RS_PLAYING, 800);
    return 0;
}
