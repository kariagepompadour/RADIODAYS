// RADIO DAYS — types used in function signatures.
// The Arduino builder inserts function prototypes above the first function of
// the sketch, so these types must be declared in a header included first.
//
// SPDX-License-Identifier: MIT
#pragma once
#include <Arduino.h>

enum SaveResult : uint8_t { SAVE_OK, SAVE_UNCHANGED, SAVE_REFUSED_NEWER, SAVE_REFUSED_ERR, SAVE_NO_SPACE, SAVE_FAILED };

enum Screen : uint8_t { SC_START, SC_RADIO, SC_WIFI_LIST, SC_WIFI_PASS, SC_WIFI_JOIN, SC_WIFI_RESULT,
                        SC_LOCATION /* (3-1) */, SC_WIFI_SAVED /* (U4) */ };

// Results of one U135 poll, consumed by the current screen.
struct KnobInput {
    int detents = 0;         // all detents, + clockwise (lists in Wi-Fi setup)
    int tuneDetents = 0;     // detents turned with the knob released (Stage 2-2: tuning)
    int volumeDetents = 0;   // detents turned while the knob was held (Stage 2-2: volume)
    bool shortPress = false; // released within SHORT_PRESS_MAX_MS without being turned
};

// Key edges of one keyboard poll.
struct KeyInput {
    String chars;  // newly pressed printable characters
    bool enter = false;
    bool del = false;
    bool tab = false;
    bool fn = false;  // Fn held while the characters were pressed
};

// (3-3) Day or night at the chosen Location, from the Sun's altitude; unknown before NTP.
enum SkyState : uint8_t { SKY_UNKNOWN, SKY_DAY, SKY_NIGHT };

// (U2a2) Where the UFO is drawn in the globe canvas, and which lamp is lit.
// (U2a3) And the talk with the Earth: lamps all on / dim, wave fronts and their step.
// (U4) The same pose now places the satellite that replaced the UFO.
struct UfoPose {
    bool shown = false;
    int x = 0, y = 0;      // top-left in the globe canvas
    uint8_t lamp = 0;      // the chasing lamp (0..2)
    uint8_t lampMode = 0;  // 0: chase, 1: all on, 2: all dim
    uint8_t wave = 0;      // 0: none, 1: Earth -> UFO (cyan), 2: UFO -> Earth (yellow)
    uint8_t waveStep = 0;
    bool operator!=(const UfoPose& o) const
    {
        return shown != o.shown || x != o.x || y != o.y || lamp != o.lamp || lampMode != o.lampMode ||
               wave != o.wave || waveStep != o.waveStep;
    }
};

// (U4) Up to 3 saved Wi-Fi networks: NVS key "wifis" (the sketch reads and writes it).
static constexpr int WIFI_SAVED_MAX = 3;
struct WifiNet {
    char ssid[33];
    char pass[65];
};
struct WifiListRecord {
    uint32_t magic;
    uint8_t version;
    uint8_t count;  // 0..WIFI_SAVED_MAX
    uint8_t last;   // the entry that connected last (tried first)
    uint8_t reserved;
    WifiNet net[WIFI_SAVED_MAX];
    uint8_t pad[2];
    uint32_t crc;  // over every byte before this field
};

// (U4) A saved network found by the background scan.
struct WifiCandidate {
    int8_t index;  // saved entry
    int8_t channel;
    int8_t rssi;
    uint8_t bssid[6];
};
