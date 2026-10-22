/*
  RADIO DAYS — U10: an ivory dial face in a sunken frame, and two small radio towers

  Not yet run on hardware. Based on U9 (kept unchanged). See docs/usb-u10.md and
  docs/dial-face-tower-proposal.md (plan B, chosen by the author).

  Changes from U9, on the radio screen only:
  - The dial: an ivory face (240, 234, 214), black ticks, warm grey baselines
    (150, 145, 132), station marks in dark orange (200, 80, 0; DIAL_MARK_C, U9's amber is
    one constant away), and a 2 px sunken bevel: U9's grey frame line becomes the inner
    line and one more line goes round it, darker above and left, lighter below and right.
    The rows, pitches, tick lengths, needle, marks' places and the changed-columns drawing
    are U9's; what U9 cleared to black (the inside, shortened ticks, old marks) is
    cleared to the face colour.
  - Two radio towers, 9 x 20 px with their waves 19 x 24 px: left under the level meter
    (x 6..24, y 71..94), right under the sky window (x 197..215, y 71..94). Drawn from
    small point tables with the whole screen only.
  - Their waves follow the status row's value: PLAYING (muted too) lights them left and
    right in turn every WAVE_BLINK_MS (800 ms), the other side dim; BUFFERING and
    CONNECTING dim on both; anything else dark. Only a side whose colour changes is
    drawn again (36 points); no delay, task, canvas or full-screen drawing.
  Everything else (stations, tuning, AUTO and MANUAL, U135, audio, network, USB relay,
  NVS, clock, sky, volume, n/N) is U9's code unchanged; the relay is U9's.

  (U9 notes follow.)

  RADIO DAYS — U9: the standard lineup (7 Locations, 42 stations) and the dial position n/N

  Not yet run on hardware. Based on U8 (kept unchanged). See docs/usb-u9.md and
  docs/u9-implementation-plan.md.

  Changes from U8:
  - stations.h: the author's standard lineup, JAPAN 5, USA 6, UK 7, FRANCE 7, WORLD 6,
    GERMANY 6, ITALY 5 (U8's 19 Locations and 85 stations stay in U8). The first five
    Locations keep their names and places, so a saved Location is found again; a saved
    Location U9 no longer has starts on the default.
  - With no saved Location RADIO DAYS starts on WORLD, whose first station is Radio
    Paradise (U8: USA, whose first station was Radio Paradise): the first sound is the same.
  - The radio screen's status row shows the dial position, e.g. 3/7, small and dim,
    between the status and the volume. It is the station's place in the Location's dial
    order, not a ranking. It is read from the same value as the station name (the station
    under the dial, none while gliding), so it is empty while the dial glides, between
    stations and while the relay hint is shown. Drawing it touches nothing in tuning.
  - The USB relay is RadioDaysRelay_U9.command (U9's hosts and redirect rules); the link
    protocol is U8's unchanged.
  Everything else (dial, AUTO glide, MANUAL and its 350 ms, Location switching, clock,
  sky, volume, mute, audio, USB and Wi-Fi) is U8's code unchanged.

  (U8 notes follow.)

  RADIO DAYS — U8: in AUTO the dial glides to the next station

  Not yet run on hardware. Based on U7 (kept unchanged). See docs/usb-u8.md and
  docs/auto-glide-proposal.md.

  Changes from U7, in AUTO only: a detent no longer jumps to the next station. The band
  glides there at a constant speed (one dial step, 2 px, every GLIDE_STEP_MS = 20 ms:
  100 px a second) while the receiver noise plays (MANUAL's noise: the station, and the
  USB relay, stop when the glide starts). On arrival the station is asked for at once (no
  350 ms wait); stations passed on the way are not. Each detent is one station; more
  detents the same way add stations, round the ring, up to GLIDE_MAX_STATIONS ahead
  (further detents are dropped). A detent the other way turns back from where the band
  is towards the nearest station that way, and drops the stations still ahead. While
  gliding the status says TUNING... and the name row is empty. Only the dial is drawn on
  each step (U6's changed-columns drawing); no task, wait, canvas or new timer. MANUAL,
  the clock, the dial's look, audio, USB and Wi-Fi are U7's.

  (U7 notes follow.)

  RADIO DAYS — U7: the clock shows the Location's local time

  Not yet run on hardware. Based on U6 (kept unchanged). See docs/usb-u7.md and
  docs/clock-local-time-proposal.md.

  Changes from U6: the date and clock show the local time of the Location's city
  (stations.h: a POSIX time zone for each Location, daylight saving included; WORLD keeps
  Japan time, as its sky uses Tokyo). The zone is set at start from the saved Location,
  again when a Location is confirmed, and kept when NTP starts (U6 set Japan time there).
  The system clock stays UTC, from NTP or the USB relay; the Sun and Moon still come from
  UTC and the city's latitude and longitude. The screen design is unchanged (no zone name
  or offset is shown). Dial, tuning, audio, USB, Wi-Fi and the Location screen are U6's.

  (U6 notes follow.)

  RADIO DAYS — U6: an endless, framed analog dial

  Not yet run on hardware. Based on U5 (hardware-checked by the author, kept unchanged).
  See docs/usb-u6.md.

  Changes from U5, on the radio screen and in tuning only:
  - Four rows of evenly spaced ticks, 2, 3, 4 and 6 px apart, every fifth one long (no
    log rows, no numbers). All four repeat together every 60 px. A 1 px grey frame round
    the dial; the needle inside it.
  - No end stops: the dial is a ring (stations.h) whose length is a whole number of those
    60 px, chosen by the number of stations (stations about 18-23 steps apart). Past the
    last station it comes to the first and back, in AUTO and in MANUAL; because the ring
    holds whole periods, the band looks the same on every lap and flows on across the
    seam without a jump. Noise between stations and the 350 ms start are U5's.
  Drawing is U5's way: only changed columns, in drawRadio() every 100 ms when the dial
  moved; the frame and needle only when the screen is drawn whole.

  (U5 notes follow.)

  RADIO DAYS — U5: an analog dial with a fixed needle

  Not yet run on hardware. Based on U4 (kept unchanged). See docs/usb-u5.md and
  docs/analog-dial-proposal.md (parts 2 and 3).

  Changes from U4, on the radio screen only:
  - The digital frequency, "MHz" and the numbered scale are replaced by five rows of
    ticks with no numbers, units or band names, moving behind a fixed needle (x 101).
    Each row has its own rule (every 2, 3 and 5 px, and two repeating log "decades");
    every fifth tick is long. Small amber wedges mark the stations and two light lines
    the ends of the dial. Only the columns that change are drawn again, in drawRadio()
    (every 100 ms, only when the dial moved); no task, wait, canvas or animation.
  - Station positions follow one rule (stations.h): first station 10 steps in, then every
    20 steps, the dial ends 10 steps after the last one; one step moves the band 2 px.
    So a Location with 3 stations has a 60-step dial, one with 7 a 140-step dial. Names,
    URLs and orders are unchanged; tuning code (AUTO / MANUAL, noise, 350 ms) is U4's.
  - Footer: the Wi-Fi / USB field starts at x 50 (a 7-letter Location touched it) and
    never reaches "A:MODE L:LOC".
  Relay, Wi-Fi, NVS, Locations, the globe, the satellite, clock and sky are U4's code; it
  pairs with RadioDaysRelay_U4.command.

  (U4 notes follow.)

  RADIO DAYS — U4: day sky, USB-wait hint, Locations by country, a satellite, 3 Wi-Fi networks

  Not yet run on hardware. Based on U3 (hardware-tested, kept unchanged). See docs/usb-u4.md
  and docs/u4-proposal.md.

  Changes from U3:
  - Day sky: #344F65 with smaller (about 55%), muted clouds. Sun, night sky, Moon,
    frequency and the amber text are unchanged.
  - When only the USB relay can play (U, or no saved Wi-Fi) and it has not answered, the
    radio screen says USB RELAY REQUIRED / "Start the relay on your computer." until it
    does. The device no longer says "Mac" anywhere (the relay script is still macOS only).
  - 14 Locations by country after WORLD (GERMANY ... ARGENT; 19 in all, 85 stations;
    stations.h). Saved Locations keep working: they are found again by name. The text
    list (when the globe cannot be made) shows the page of 5 that holds the cursor.
  - The Location screen's UFO is now a satellite: same size, drift, timing, wave fronts
    (Earth -> satellite, then satellite -> Earth) and frame rule as U2a3; only the picture.
  - Up to 3 saved Wi-Fi networks (NVS key "wifis"; "wifi" keeps the last one for U3).
    The background search tries the last network first, stops at "not found" / "refused"
    at once, scans once only with 2 or more saved, then tries the strongest saved ones by
    channel and BSSID: 12 s per attempt, 30 s per search, then the 5 -> 60 s pauses.
    Wi-Fi setup: * marks saved networks, S lists them (connect, delete), and a fourth
    network asks which one gives way. U (USB only) still never touches Wi-Fi.
  The USB link (radiodays_link.h) is U3's unchanged; the redirecting stations need
  RadioDaysRelay_U4.command for USB (Wi-Fi follows their redirects by itself).

  (U3 notes follow.)

  RADIO DAYS — U3: the clock, Sun and Moon also with the USB relay only

  Not yet run on hardware. Based on U2a4 (kept unchanged). See docs/usb-u3.md and
  docs/usb-u3-time-design.md.

  Changes from U2a4: when NTP (Wi-Fi) has not set the clock in this start, the time from
  the Mac relay (RadioDaysRelay_U3.command: "RDTIME <id> <UTC seconds>") sets it. Only UTC
  seconds travel; the clock still shows Japan time (the time zone is now set at start,
  not only when NTP starts), and the Sun and Moon still come from UTC and the Location's
  latitude and longitude, never from the Mac's local time. Once set, the clock runs on
  the internal timer when USB is gone; a restart forgets it until the relay answers
  again. NTP, when it syncs, takes precedence. The screen is unchanged. Playback,
  tuning, Wi-Fi, the U key and the UFO are U2a4's code; with the U2a2 relay it plays as
  U2a4 (no time).

  (U2a4 notes follow.)

  RADIO DAYS — U2a4: USB radio without Wi-Fi

  Not yet run on hardware. Based on U2a3 (hardware-tested 2026-10-21, kept unchanged).
  See docs/usb-u2a4.md.

  Changes from U2a3, on the START screen and in the Wi-Fi footer only: with no saved
  Wi-Fi, a key or the knob now opens the radio (it plays as soon as the Mac relay
  answers) instead of the Wi-Fi setup; W still opens the setup. U opens the radio
  without trying Wi-Fi for this start, even when a network is saved (nothing is erased;
  the next normal start uses it as before). The START screen shows whether the relay
  has answered. The radio footer says "WiFi unset" / "WiFi off" instead of "retry" in
  those cases. Relay protocol, playback, tuning, volume, Wi-Fi setup and the UFO are
  U2a3's code; it still pairs with RadioDaysRelay_U2a2.command.

  (U2a3 notes follow.)

  RADIO DAYS — U2a3: the UFO stays by the globe and now and then talks to the Earth

  Not yet run on hardware. Based on U2a2 (hardware-tested 2026-10-21, kept unchanged).
  See docs/usb-u2a3.md.

  Changes from U2a2, on the Location screen only: the UFO no longer goes round the
  globe (it was hidden behind it much of the time). It hovers at the globe's top right,
  left of the Location name, drifting a few pixels on slow, unequal waves that now and
  then ease off and pause. Every 15-25 s the Earth sends it a few cyan wave fronts, its
  lamps blink, and it answers with yellow ones. Pure decoration: it shows nothing about
  the real USB or Wi-Fi link. Everything else (relay, Wi-Fi, tuning, volume, screens)
  is U2a2's code unchanged; it still pairs with RadioDaysRelay_U2a2.command.

  (U2a2 notes follow.)

  RADIO DAYS — U2a2: U2a with the fixes from its first hardware test

  Not yet run on hardware. See docs/usb-u2a2.md.

  Changes from U2a: the link sends HELLO whatever the USB "plugged" signal says (U2a
  stopped HELLO while it read "unplugged", so the link could not come back); credit is
  a running total sent again while bytes are outstanding ("@RD CREDIT", needs
  RadioDaysRelay_U2a2.command), so a lost line cannot starve the stream; an unwritten or
  unanswered TUNE is sent again. While the relay is linked, no attempt is made to join
  Wi-Fi (attempts start again when the relay is gone). The log ([STAT]) and an optional
  footer (key I) show the link, the queue, underruns and lost lines. On the Location
  screen a smaller UFO (24 x 12) flies round the right side of the globe, slipping
  behind it at both ends; the globe moved 8 px left to make room.

  (U2a notes follow.)

  RADIO DAYS — U2a: choosing stations through the USB relay (based on Stage 3-3)

  Not yet run on hardware. See docs/usb-u2a-design.md (design) and docs/usb-u2a.md.

  Changes from 3-3: when the Mac relay (tools/usb-relay/RadioDaysRelay_U2.command) has
  answered on the USB serial port, every station is received by the Mac and sent over
  USB; otherwise Wi-Fi is used as in 3-3. The link (radiodays_link.h) numbers every
  request with an id, discards everything until "RDSYNC <id>" so the previous
  station's bytes never reach the new one, and grants credit so the receive queue
  cannot overflow (U1's rule). Only one task reads the receive queue at a time. A
  USB cable alone changes nothing. The UFO on the Location screen moves to the top
  right; "(now)" moves to the city line. Automatic Wi-Fi / USB switching while
  playing is U2b; the time from the Mac is U3.

  (Stage 3-3 notes follow.)

  RADIO DAYS — Stage 3-3: sun and moon with faces, day / night sky, UFO

  Based on Stage 3-2. Not yet run on hardware. See docs/stage-3-3.md.

  Changes from 3-2: under the clock, a small sky window: by day a blue sky with two
  clouds and a smiling sun, by night a dark sky with three stars and a moon with a
  face (real phase, as 2-5). Day or night follows the Sun's altitude at the chosen
  Location's city (-0.833 degrees), not a fixed hour; the clock stays in JST.
  Nothing is decided before NTP has synced. The Location screen gets a small floating
  UFO beside the globe. Faces and UFO after SOUND TERRARIUM (MIT, same author).
  Audio, tuning, noise, meter, Locations and the globe work as in 3-2.

  (Stage 3-2 notes follow.)

  RADIO DAYS — Stage 3-2: the globe on the Location screen

  Based on Stage 3-1. Not yet run on hardware. See docs/stage-3.md.

  Changes from 3-1: the Location screen shows a turning globe (blue sea, green
  land from Natural Earth, public domain) instead of the text list. Turning the
  knob turns the globe until the chosen place faces you; WORLD spins slowly. The
  globe exists only on that screen: its ~29 KB are allocated when the screen opens
  and freed when it closes; if memory is short, the 3-1 list is shown instead.
  Everything else is unchanged from 3-1.

  (Stage 3-1 notes follow.)

  RADIO DAYS — Stage 3-1: WORLD RADIO (Locations)

  Based on Stage 2-5. Not yet run on hardware. See docs/stage-3.md.

  Changes from 2-5: stations are grouped by Location (JAPAN, USA, UK, FRANCE, WORLD;
  stations.h). L opens the Location screen; turn the knob to choose, push or ok to
  listen, esc to go back. The choice is saved in NVS (namespace "radiodays", new key
  "loc"); the Wi-Fi record is untouched. AUTO / MANUAL, the noise, the meter, the
  clock and the knob work as before, on the chosen Location's stations. Every
  frequency is still virtual (decision pending, docs/stations.md).

  (Stage 2-5 notes follow.)

  RADIO DAYS — Stage 2-5: clock and real moon phase

  Based on Stage 2-4 (hardware-tested 2026-10-21). Stage 2-5 itself is not yet run
  on hardware. See docs/stage-2-5.md.

  Changes from 2-4: a date and 24-hour clock (NTP over Wi-Fi, JST) and a small moon
  showing the real phase, right of the frequency. Nothing is shown until NTP has
  synced in this session; nothing is stored in NVS. The clock's time zone is a
  constant, independent of the station Location (Stage 3). The frequency group
  (same size) now starts right after the level meter to make room. Audio, noise,
  tuning, meter, U135 and Wi-Fi are unchanged.

  (Stage 2-4 notes follow.)

  RADIO DAYS — Stage 2-4: 8-segment LED level meter

  Hardware: M5Stack Cardputer ADV + Unit Encoder U135 on the Grove port.
  Based on Stage 2-3. Stage 2-4 itself is not yet run on hardware.
  See docs/stage-2-4.md for install and test steps.

  Changes from 2-3: an 8-segment level meter (4 green, 2 yellow, 2 red) left of the
  frequency. It follows the RMS level of the decoded broadcast PCM, read through the
  output's monitor hook; fast rise, slower fall. It is dark for the receiver noise,
  while connecting / buffering / failing, while muted, and while tuning. Audio,
  noise, tuning, U135 handling and Wi-Fi setup are unchanged from 2-3.

  (Stage 2-3 notes follow.)

  RADIO DAYS — Stage 2-3: AUTO / MANUAL tuning, frequency display, receiver noise

  Changes from 2-2: every station has a virtual dial frequency (stations.h, 0.1 MHz
  integer units). AUTO tuning jumps from station to station (as 2-2); MANUAL moves
  0.1 MHz per detent and plays receiver noise between stations. A toggles the mode
  (every start is AUTO). The noise is generated in the decode task, the only task
  that feeds the speaker, so it never competes with the stream. Knob press/turn
  detection is unchanged from 2-2.

  (Stage 2-2 notes follow.)

  Changes from 2-1: turning the knob tunes (next / previous station, wrapping
  round); push + turn sets the volume; a short press still mutes. The station
  is opened only after the dial has rested for TUNE_SETTLE_MS, so turning
  quickly past several stations does not open each one. A station chosen with
  the dial stays chosen when it cannot be reached ("NO SIGNAL", retried).
  Wi-Fi setup and NVS storage are unchanged from 2-1.

  Arduino IDE settings (the first two are enforced by #error below):
    Board: M5Cardputer (m5stack:esp32 3.3.9)
    USB Mode: Hardware CDC and JTAG
    USB CDC On Boot: Enabled
    Flash Size: 8MB (64Mb)
    Partition Scheme: 8M with spiffs (3MB APP/1.5MB SPIFFS)  -- size check only;
      the Launcher creates the real partition when the app is installed.
  Install only the app-only RadioDays_U10.ino.bin through the Launcher.

  Screens:
    START   shown at power-on. Reads NVS only; Wi-Fi is not started and nothing
            is written until a key or the knob is pressed.
    RADIO   one station playing.
    Wi-Fi   network list -> password -> join. The network is saved only after
            a successful connection.

  Radio screen controls:
    turn knob ............ AUTO: next / previous station; MANUAL: 0.1 MHz per detent
    push + turn knob ..... volume (turning while muted unmutes); never mutes on release
    short knob press ..... mute on/off (decided on release, only if not turned)
    A .................... AUTO / MANUAL
    , / /  (left/right) .. one detent down / up (also P / N)
    ; / .  (up / down) ... volume
    M .................... mute on/off
    W .................... Wi-Fi setup

  NVS (shared with the Launcher and other apps): only namespace "radiodays",
  keys "wifi", "loc" and (U4) "wifis". Every write goes through nvsWriteWifi(),
  nvsWriteLocation() or nvsWriteWifiList(), which log it on the serial port. Nothing
  is erased. WiFi.persistent(false) keeps the Wi-Fi driver
  from storing its own copy of the network.

  SPDX-License-Identifier: MIT
*/
#include <atomic>
#include <math.h>
#include <time.h>
#include <sys/time.h>  // (U3) settimeofday()
#include <esp_sntp.h>
#include <new>
#include <M5Cardputer.h>
#include <Wire.h>
#include <WiFi.h>
#include <nvs.h>
#include <esp_heap_caps.h>
#include <esp_random.h>
#include <AudioFileSourceICYStream.h>
#include <AudioFileSourceBuffer.h>
#include <AudioGeneratorMP3.h>
#include "AudioOutputM5Speaker.h"
#include "stations.h"
#include "globe_land.h"  // (3-2) land mask, Natural Earth (public domain)
#include "radiodays_link.h"  // (U2a) the USB relay link
#include "radiodays_types.h"  // types used in function signatures (before the Arduino prototypes)

// (3-1) The current Location. All station tables are static and never freed, so a
// task that still holds the previous table for a moment reads valid memory; station
// indexes are range-checked where they are used.
static std::atomic<const Location*> gLoc{&LOCATIONS[LOCATION_DEFAULT]};
static int curLocation = LOCATION_DEFAULT;
static inline const Station* locStations() { return gLoc.load()->stations; }
static inline int locCount() { return gLoc.load()->count; }
static inline int locFreqMin() { return gLoc.load()->freqMin; }
static inline int locFreqMax() { return gLoc.load()->freqMax; }

#if !defined(ARDUINO_USB_CDC_ON_BOOT) || ARDUINO_USB_CDC_ON_BOOT != 1
// With CDC On Boot disabled, Serial becomes UART0 (TX = GPIO43), which the
// Cardputer ADV uses as the I2S word clock of the speaker.
#error "Tools > USB CDC On Boot must be 'Enabled'."
#endif
#if !defined(ARDUINO_USB_MODE) || ARDUINO_USB_MODE != 1
// RADIO DAYS does not use USB HID; TinyUSB would only add about 53 KB.
#error "Tools > USB Mode must be 'Hardware CDC and JTAG'."
#endif

#ifndef APP_CPU_NUM
#define APP_CPU_NUM 1
#endif
#ifndef PRO_CPU_NUM
#define PRO_CPU_NUM 0
#endif

static constexpr const char* BUILD_TAG = "U10";

// ---------------------------------------------------------------- settings
// U135 (same values as Cardputer Media Jog, measured there)
static constexpr uint8_t PORT_SDA = 2;
static constexpr uint8_t PORT_SCL = 1;
static constexpr uint32_t I2C_FREQ = 200000;
static constexpr uint8_t U135_ADDR = 0x40;
static constexpr uint8_t U135_REG_VALUE = 0x10;
static constexpr uint8_t U135_REG_BUTTON = 0x20;
static constexpr int16_t COUNTS_PER_DETENT = 2;
static constexpr uint8_t BUTTON_PRESSED_RAW = 0;
static constexpr uint32_t SHORT_PRESS_MAX_MS = 800;
static constexpr uint32_t U135_PROBE_INTERVAL_MS = 2000;
// Turning just after the knob is released still counts as volume, not tuning.
static constexpr uint32_t RELEASE_GRACE_MS = 200;
// The station is opened once the dial has rested this long.
static constexpr uint32_t TUNE_SETTLE_MS = 350;

// Volume (M5.Speaker master volume 0..255). Every start uses VOL_START.
static constexpr int VOL_START = 48;
static constexpr int VOL_MAX = 192;
static constexpr int VOL_STEP = 8;

// Stream (as the M5Unified WebRadio_with_ESP8266Audio example)
static constexpr int STREAM_BUFFER_SIZE = 32 * 1024;
static constexpr int STREAM_START_LEVEL = STREAM_BUFFER_SIZE * 3 / 4;
static constexpr int STREAM_LOW_LEVEL = STREAM_BUFFER_SIZE / 8;
static constexpr uint32_t BUFFER_WAIT_MS = 3000;
static constexpr int MP3_CODEC_SIZE = 29192;
static constexpr uint32_t STATION_OK_SECONDS = 2;
static constexpr uint32_t ALL_FAILED_WAIT_MS = 10000;
static constexpr uint32_t SELECTED_RETRY_MS = 10000;  // a tuned station that cannot be reached

// Wi-Fi
static constexpr uint32_t WIFI_CONNECT_TIMEOUT_MS = 20000;  // the setup's join screen (as U3)
static constexpr uint32_t WIFI_ATTEMPT_MS = 12000;   // (U4) one background attempt
static constexpr uint32_t WIFI_SEARCH_MS = 30000;    // (U4) one whole search (attempts + one scan)
static constexpr uint32_t WIFI_FAIL_GUARD_MS = 3000; // (U4) "not found" / "refused" trusted after this (as the join screen)
static constexpr uint32_t WIFI_RETRY_MIN_MS = 5000;
static constexpr uint32_t WIFI_RETRY_MAX_MS = 60000;
static constexpr int WIFI_LIST_MAX = 16;

// NVS
static constexpr const char* NVS_NS = "radiodays";
static constexpr const char* NVS_KEY_WIFI = "wifi";    // the last network that connected (U3 and earlier read only this)
static constexpr const char* NVS_KEY_WIFIS = "wifis";  // (U4) up to 3 networks
static constexpr uint32_t WIFI_LIST_MAGIC = 0x4C574452;  // "RDWL"
static constexpr uint8_t WIFI_LIST_VERSION = 1;
static constexpr uint32_t WIFI_REC_MAGIC = 0x46574452;  // "RDWF"
static constexpr uint8_t WIFI_REC_VERSION = 1;
static constexpr size_t NVS_MIN_FREE_ENTRIES = 32;  // as Cardputer Media Jog

// Colours
static constexpr uint16_t COL_BG = TFT_BLACK;
static constexpr uint16_t COL_AMBER = 0xFD20;
static constexpr uint16_t COL_DIM = 0x7BEF;
static constexpr uint16_t COL_TEXT = TFT_WHITE;
static constexpr uint16_t COL_OK = 0x07E0;
static constexpr uint16_t COL_WARN = 0xFFE0;
static constexpr uint16_t COL_ERR = 0xF800;

// ---------------------------------------------------------------- small helpers
static uint32_t crc32(const uint8_t* data, size_t len)
{
    uint32_t crc = 0xFFFFFFFFu;
    for (size_t i = 0; i < len; i++) {
        crc ^= data[i];
        for (int b = 0; b < 8; b++) crc = (crc >> 1) ^ (0xEDB88320u & (0u - (crc & 1u)));
    }
    return ~crc;
}

// Copies UTF-8 text into a printable ASCII buffer (the built-in font has no other glyphs).
static void toAscii(const char* in, char* out, size_t outSize)
{
    size_t o = 0;
    for (const unsigned char* p = (const unsigned char*)in; *p && o + 1 < outSize; p++) {
        unsigned char c = *p;
        if (c >= 0x20 && c < 0x7F) out[o++] = (char)c;
        else if (c >= 0xC0) out[o++] = '?';  // first byte of a multi-byte character
        // continuation bytes and control characters are dropped
    }
    out[o] = '\0';
}

// ---------------------------------------------------------------- NVS ("radiodays" only)
struct WifiRecord {
    uint32_t magic;
    uint8_t version;
    uint8_t reserved[3];
    char ssid[33];
    char pass[65];
    uint8_t pad[2];
    uint32_t crc;  // over every byte before this field
};

enum NvState : uint8_t { NV_NONE, NV_OK, NV_BAD, NV_NEWER, NV_ERR };
static const char* const NV_TEXT[] = {"none", "ok", "damaged", "newer version", "read error"};

static NvState nvState = NV_NONE;
static WifiRecord storedWifi = {};  // valid when nvState == NV_OK
static uint32_t nvsWriteCount = 0;

static void logNvsStats(const char* when)
{
    nvs_stats_t st = {};
    esp_err_t e = nvs_get_stats(NULL, &st);
    Serial.printf("[NVS] stats %s: err 0x%x used %u free %u total %u\n", when, (unsigned)e,
                  (unsigned)st.used_entries, (unsigned)st.free_entries, (unsigned)st.total_entries);
}

// Boot: reads only. A missing namespace is normal and creates nothing.
static void nvsLoadWifi()
{
    nvs_handle_t h;
    esp_err_t e = nvs_open(NVS_NS, NVS_READONLY, &h);
    if (e == ESP_ERR_NVS_NOT_FOUND) {
        nvState = NV_NONE;
    } else if (e != ESP_OK) {
        nvState = NV_ERR;
    } else {
        WifiRecord r = {};
        size_t len = sizeof(r);
        e = nvs_get_blob(h, NVS_KEY_WIFI, &r, &len);
        nvs_close(h);
        if (e == ESP_ERR_NVS_NOT_FOUND) {
            nvState = NV_NONE;
        } else if (e != ESP_OK && e != ESP_ERR_NVS_INVALID_LENGTH) {
            nvState = NV_ERR;
        } else if (e == ESP_ERR_NVS_INVALID_LENGTH || len != sizeof(r) || r.magic != WIFI_REC_MAGIC) {
            nvState = (r.magic == WIFI_REC_MAGIC && r.version > WIFI_REC_VERSION) ? NV_NEWER : NV_BAD;
        } else if (r.version > WIFI_REC_VERSION) {
            nvState = NV_NEWER;
        } else if (r.crc != crc32((const uint8_t*)&r, offsetof(WifiRecord, crc)) || r.ssid[0] == '\0') {
            nvState = NV_BAD;
        } else {
            r.ssid[sizeof(r.ssid) - 1] = '\0';
            r.pass[sizeof(r.pass) - 1] = '\0';
            storedWifi = r;
            nvState = NV_OK;
        }
    }
    Serial.printf("[NVS] load %s/%s: %s (err 0x%x)\n", NVS_NS, NVS_KEY_WIFI, NV_TEXT[nvState], (unsigned)e);
}


// The only function that writes NVS. allowOverwriteDamaged: the user confirmed it.
static SaveResult nvsWriteWifi(const String& ssid, const String& pass, bool allowOverwriteDamaged, const char* reason)
{
    if (nvState == NV_NEWER) return SAVE_REFUSED_NEWER;
    if (nvState == NV_ERR) return SAVE_REFUSED_ERR;
    if (nvState == NV_BAD && !allowOverwriteDamaged) return SAVE_REFUSED_ERR;

    WifiRecord r = {};
    r.magic = WIFI_REC_MAGIC;
    r.version = WIFI_REC_VERSION;
    strlcpy(r.ssid, ssid.c_str(), sizeof(r.ssid));
    strlcpy(r.pass, pass.c_str(), sizeof(r.pass));
    r.crc = crc32((const uint8_t*)&r, offsetof(WifiRecord, crc));
    if (nvState == NV_OK && memcmp(&r, &storedWifi, sizeof(r)) == 0) {
        Serial.printf("[NVS] write %s/%s skipped: unchanged (%s)\n", NVS_NS, NVS_KEY_WIFI, reason);
        return SAVE_UNCHANGED;
    }

    nvs_stats_t st = {};
    if (nvs_get_stats(NULL, &st) != ESP_OK || st.free_entries < NVS_MIN_FREE_ENTRIES) {
        Serial.printf("[NVS] write %s/%s refused: free entries %u < %u\n", NVS_NS, NVS_KEY_WIFI,
                      (unsigned)st.free_entries, (unsigned)NVS_MIN_FREE_ENTRIES);
        return SAVE_NO_SPACE;
    }

    logNvsStats("before write");
    nvs_handle_t h;
    esp_err_t e = nvs_open(NVS_NS, NVS_READWRITE, &h);
    if (e == ESP_OK) {
        e = nvs_set_blob(h, NVS_KEY_WIFI, &r, sizeof(r));
        if (e == ESP_OK) e = nvs_commit(h);
        nvs_close(h);
    }
    nvsWriteCount++;
    Serial.printf("[NVS] write #%lu %s/%s (%u bytes) reason=%s: err 0x%x\n", (unsigned long)nvsWriteCount,
                  NVS_NS, NVS_KEY_WIFI, (unsigned)sizeof(r), reason, (unsigned)e);
    logNvsStats("after write");
    if (e != ESP_OK) return SAVE_FAILED;
    storedWifi = r;
    nvState = NV_OK;
    return SAVE_OK;
}

// (U4) Up to 3 networks: key "wifis". The old key "wifi" stays and always holds the last
// network that connected, so U3 and earlier still start with it. When "wifis" is missing,
// a good "wifi" record becomes the first entry, in RAM only (nothing is written at boot).
// Written only when a network is added, replaced or deleted, or when a different saved
// network connects. Same safety rules as the Wi-Fi record (damaged: only after the user
// confirms; newer version or read error: never).
static NvState nvListState = NV_NONE;
static WifiListRecord savedList = {};  // what is used (from "wifis", or "wifi" moved over in RAM)
static WifiListRecord storedList = {}; // what "wifis" holds; valid when nvListState == NV_OK

static void nvsLoadWifiList()
{
    nvs_handle_t h;
    esp_err_t e = nvs_open(NVS_NS, NVS_READONLY, &h);
    if (e == ESP_ERR_NVS_NOT_FOUND) {
        nvListState = NV_NONE;
    } else if (e != ESP_OK) {
        nvListState = NV_ERR;
    } else {
        WifiListRecord r = {};
        size_t len = sizeof(r);
        e = nvs_get_blob(h, NVS_KEY_WIFIS, &r, &len);
        nvs_close(h);
        if (e == ESP_ERR_NVS_NOT_FOUND) nvListState = NV_NONE;
        else if (e != ESP_OK && e != ESP_ERR_NVS_INVALID_LENGTH) nvListState = NV_ERR;
        else if (r.magic == WIFI_LIST_MAGIC && r.version > WIFI_LIST_VERSION) nvListState = NV_NEWER;
        else if (e == ESP_ERR_NVS_INVALID_LENGTH || len != sizeof(r) || r.magic != WIFI_LIST_MAGIC ||
                 r.crc != crc32((const uint8_t*)&r, offsetof(WifiListRecord, crc)) || r.count > WIFI_SAVED_MAX ||
                 (r.count && r.last >= r.count))
            nvListState = NV_BAD;
        else {
            for (int i = 0; i < WIFI_SAVED_MAX; i++) {
                r.net[i].ssid[sizeof(r.net[i].ssid) - 1] = '\0';
                r.net[i].pass[sizeof(r.net[i].pass) - 1] = '\0';
            }
            storedList = r;
            nvListState = NV_OK;
        }
    }
    savedList = {};
    if (nvListState == NV_OK) {
        savedList = storedList;
    } else if (nvState == NV_OK) {  // U3 and earlier: the one network becomes entry 1
        savedList.count = 1;
        strlcpy(savedList.net[0].ssid, storedWifi.ssid, sizeof(savedList.net[0].ssid));
        strlcpy(savedList.net[0].pass, storedWifi.pass, sizeof(savedList.net[0].pass));
    }
    Serial.printf("[NVS] load %s/%s: %s (err 0x%x), %u network(s)%s\n", NVS_NS, NVS_KEY_WIFIS, NV_TEXT[nvListState],
                  (unsigned)e, (unsigned)savedList.count, nvListState != NV_OK && savedList.count ? " from \"wifi\"" : "");
}

// Index of a saved network by name, or -1.
static int savedIndexOf(const char* ssid)
{
    for (int i = 0; i < savedList.count; i++)
        if (strcmp(savedList.net[i].ssid, ssid) == 0) return i;
    return -1;
}

// The only function that writes "wifis"; afterwards "wifi" gets the last network (for
// U3 and earlier). Writes `next` and makes it the list in use.
static SaveResult nvsWriteWifiList(const WifiListRecord& next, bool allowOverwriteDamaged, const char* reason)
{
    if (nvListState == NV_NEWER) return SAVE_REFUSED_NEWER;
    if (nvListState == NV_ERR) return SAVE_REFUSED_ERR;
    if (nvListState == NV_BAD && !allowOverwriteDamaged) return SAVE_REFUSED_ERR;
    WifiListRecord r = {};
    r.magic = WIFI_LIST_MAGIC;
    r.version = WIFI_LIST_VERSION;
    r.count = next.count;
    r.last = next.count ? next.last : 0;
    for (int i = 0; i < next.count; i++) r.net[i] = next.net[i];
    r.crc = crc32((const uint8_t*)&r, offsetof(WifiListRecord, crc));
    if (nvListState == NV_OK && memcmp(&r, &storedList, sizeof(r)) == 0) {
        savedList = r;
        Serial.printf("[NVS] write %s/%s skipped: unchanged (%s)\n", NVS_NS, NVS_KEY_WIFIS, reason);
        return SAVE_UNCHANGED;
    }
    nvs_stats_t st = {};
    if (nvs_get_stats(NULL, &st) != ESP_OK || st.free_entries < NVS_MIN_FREE_ENTRIES) {
        Serial.printf("[NVS] write %s/%s refused: free entries %u < %u\n", NVS_NS, NVS_KEY_WIFIS,
                      (unsigned)st.free_entries, (unsigned)NVS_MIN_FREE_ENTRIES);
        return SAVE_NO_SPACE;
    }
    logNvsStats("before write");
    nvs_handle_t h;
    esp_err_t e = nvs_open(NVS_NS, NVS_READWRITE, &h);
    if (e == ESP_OK) {
        e = nvs_set_blob(h, NVS_KEY_WIFIS, &r, sizeof(r));
        if (e == ESP_OK) e = nvs_commit(h);
        nvs_close(h);
    }
    nvsWriteCount++;
    Serial.printf("[NVS] write #%lu %s/%s (%u bytes, %u networks) reason=%s: err 0x%x\n", (unsigned long)nvsWriteCount,
                  NVS_NS, NVS_KEY_WIFIS, (unsigned)sizeof(r), (unsigned)r.count, reason, (unsigned)e);
    logNvsStats("after write");
    if (e != ESP_OK) return SAVE_FAILED;
    storedList = r;
    savedList = r;
    nvListState = NV_OK;
    if (r.count) nvsWriteWifi(r.net[r.last].ssid, r.net[r.last].pass, allowOverwriteDamaged, reason);  // (for U3 and earlier)
    return SAVE_OK;
}

// (3-1) Location record: key "loc" in "radiodays". Read at boot; written only when the
// user confirms a different Location. Same safety rules as the Wi-Fi record.
struct LocRecord {
    uint32_t magic;
    uint8_t version;
    uint8_t index;
    char name[8];  // the Location name too, so a reordered table is detected
    uint8_t reserved[2];
    uint32_t crc;  // over every byte before this field
};
static constexpr const char* NVS_KEY_LOC = "loc";
static constexpr uint32_t LOC_REC_MAGIC = 0x434C4452;  // "RDLC"
static constexpr uint8_t LOC_REC_VERSION = 1;
static NvState nvLocState = NV_NONE;
static LocRecord storedLoc = {};

static void nvsLoadLocation()
{
    nvs_handle_t h;
    esp_err_t e = nvs_open(NVS_NS, NVS_READONLY, &h);
    if (e == ESP_ERR_NVS_NOT_FOUND) {
        nvLocState = NV_NONE;
    } else if (e != ESP_OK) {
        nvLocState = NV_ERR;
    } else {
        LocRecord r = {};
        size_t len = sizeof(r);
        e = nvs_get_blob(h, NVS_KEY_LOC, &r, &len);
        nvs_close(h);
        if (e == ESP_ERR_NVS_NOT_FOUND) nvLocState = NV_NONE;
        else if (e != ESP_OK && e != ESP_ERR_NVS_INVALID_LENGTH) nvLocState = NV_ERR;
        else if (r.magic == LOC_REC_MAGIC && r.version > LOC_REC_VERSION) nvLocState = NV_NEWER;
        else if (e == ESP_ERR_NVS_INVALID_LENGTH || len != sizeof(r) || r.magic != LOC_REC_MAGIC ||
                 r.crc != crc32((const uint8_t*)&r, offsetof(LocRecord, crc)))
            nvLocState = NV_BAD;
        else {
            r.name[sizeof(r.name) - 1] = '\0';
            storedLoc = r;
            nvLocState = NV_OK;
        }
    }
    Serial.printf("[NVS] load %s/%s: %s (err 0x%x)\n", NVS_NS, NVS_KEY_LOC, NV_TEXT[nvLocState], (unsigned)e);
}

// Index of the saved Location, or -1 if none / not usable (unknown name or index).
static int savedLocationIndex()
{
    if (nvLocState != NV_OK) return -1;
    if (storedLoc.index < LOCATION_COUNT && strcmp(LOCATIONS[storedLoc.index].name, storedLoc.name) == 0)
        return storedLoc.index;
    for (int i = 0; i < LOCATION_COUNT; i++)
        if (strcmp(LOCATIONS[i].name, storedLoc.name) == 0) return i;
    return -1;
}

// The only function that writes the Location record.
static SaveResult nvsWriteLocation(int index, const char* reason)
{
    if (nvLocState == NV_NEWER) return SAVE_REFUSED_NEWER;
    if (nvLocState == NV_ERR || nvLocState == NV_BAD) return SAVE_REFUSED_ERR;  // never overwrite silently
    LocRecord r = {};
    r.magic = LOC_REC_MAGIC;
    r.version = LOC_REC_VERSION;
    r.index = (uint8_t)index;
    strlcpy(r.name, LOCATIONS[index].name, sizeof(r.name));
    r.crc = crc32((const uint8_t*)&r, offsetof(LocRecord, crc));
    if (nvLocState == NV_OK && memcmp(&r, &storedLoc, sizeof(r)) == 0) {
        Serial.printf("[NVS] write %s/%s skipped: unchanged (%s)\n", NVS_NS, NVS_KEY_LOC, reason);
        return SAVE_UNCHANGED;
    }
    nvs_stats_t st = {};
    if (nvs_get_stats(NULL, &st) != ESP_OK || st.free_entries < NVS_MIN_FREE_ENTRIES) {
        Serial.printf("[NVS] write %s/%s refused: free entries %u < %u\n", NVS_NS, NVS_KEY_LOC,
                      (unsigned)st.free_entries, (unsigned)NVS_MIN_FREE_ENTRIES);
        return SAVE_NO_SPACE;
    }
    logNvsStats("before write");
    nvs_handle_t h;
    esp_err_t e = nvs_open(NVS_NS, NVS_READWRITE, &h);
    if (e == ESP_OK) {
        e = nvs_set_blob(h, NVS_KEY_LOC, &r, sizeof(r));
        if (e == ESP_OK) e = nvs_commit(h);
        nvs_close(h);
    }
    nvsWriteCount++;
    Serial.printf("[NVS] write #%lu %s/%s (%u bytes, %s) reason=%s: err 0x%x\n", (unsigned long)nvsWriteCount,
                  NVS_NS, NVS_KEY_LOC, (unsigned)sizeof(r), r.name, reason, (unsigned)e);
    logNvsStats("after write");
    if (e != ESP_OK) return SAVE_FAILED;
    storedLoc = r;
    nvLocState = NV_OK;
    return SAVE_OK;
}

// ---------------------------------------------------------------- radio engine (decode task, core 0)
enum RadioState : uint8_t { RS_OFF, RS_WAIT_WIFI, RS_CONNECTING, RS_BUFFERING, RS_PLAYING, RS_SKIPPING,
                            RS_ALL_FAILED, RS_NO_MEMORY, RS_NO_SIGNAL, RS_NOISE };

static std::atomic<uint8_t> radioState{RS_OFF};
static std::atomic<bool> radioEnabled{false};
// (2-3) Request to the decode task: a station index, REQ_NOISE, or REQ_NONE. The latest wins.
static constexpr int REQ_NONE = -1;
static constexpr int REQ_NOISE = -2;
static std::atomic<int> stationRequest{REQ_NONE};
static std::atomic<int> currentStation{0};  // -1 while playing noise
static std::atomic<uint32_t> decodeOffSeq{0};  // (3-1) bumped each time the task idles with the radio off
static std::atomic<uint32_t> autoMoveSeq{0};  // (2-2) bumped when the task moves on by itself
static std::atomic<uint8_t> bufferPercent{0};
static std::atomic<uint32_t> underruns{0};     // (U2a2) the stream buffer ran low while playing
static std::atomic<uint32_t> decodeGapMax{0};  // (U2a2) longest pause between decoder steps (ms)
static std::atomic<uint32_t> titleSeq{0};
static portMUX_TYPE titleMux = portMUX_INITIALIZER_UNLOCKED;
static char streamTitle[96] = "";

static AudioOutputM5Speaker out(&M5.Speaker, 0);
static AudioFileSourceICYStream* src = nullptr;
static AudioFileSourceBuffer* buff = nullptr;
static AudioGeneratorMP3* decoder = nullptr;
static void* streamBuffer = nullptr;
static void* codecBuffer = nullptr;

// ---------------------------------------------------------------- (U2a) USB relay link
static uint32_t ioNow() { return millis(); }
static bool ioPlugged() { return Serial.isPlugged(); }
static int ioAvailable() { return Serial.available(); }
static size_t ioRead(uint8_t* buf, size_t len) { return Serial.read(buf, len); }
// One write: lines never interleave. HWCDC may write nothing (busy) or part (full): false.
static bool ioLine(const char* text) { return Serial.printf("%s\n", text) == strlen(text) + 1; }
static void ioPause() { vTaskDelay(1); }
static const rdlink::Io linkIo = {ioNow, ioPlugged, ioAvailable, ioRead, ioLine, ioPause};
static rdlink::Link usbLink(linkIo);

static constexpr uint32_t USB_READ_WAIT_MS = 500;  // as U1: the decoder takes 0 bytes as the end

// MP3 source on the receive queue, for one request only (usbLink.take() checks it).
class UsbSource : public AudioFileSource {
  public:
    void attach(uint32_t req)
    {
        req_ = req;
        pos_ = 0;
    }
    uint32_t read(void* data, uint32_t len) override
    {
        uint32_t start = millis();
        uint32_t n;
        while ((n = take(data, len)) == 0 && millis() - start < USB_READ_WAIT_MS) {
            if (usbLink.state() != rdlink::L3_STREAMING) break;  // stopped: end at once
            vTaskDelay(pdMS_TO_TICKS(2));
        }
        return n;
    }
    uint32_t readNonBlock(void* data, uint32_t len) override { return take(data, len); }
    bool seek(int32_t, int) override { return false; }
    bool close() override { return true; }
    bool isOpen() override { return true; }
    uint32_t getSize() override { return 0; }
    uint32_t getPos() override { return pos_; }

  private:
    uint32_t req_ = 0;
    uint32_t pos_ = 0;
    uint32_t take(void* data, uint32_t len)
    {
        uint32_t n = (uint32_t)usbLink.take(req_, (uint8_t*)data, len);
        pos_ += n;
        return n;
    }
};
static UsbSource usbSource;
static bool usbOpen = false;  // decode task only: a request to the relay is open
static uint8_t usbNak = 0;    // decode task only: the relay's last refusal (rdlink::NAK_REASONS)

static void setTitle(const char* text)
{
    char tmp[sizeof(streamTitle)];
    toAscii(text, tmp, sizeof(tmp));
    portENTER_CRITICAL(&titleMux);
    memcpy(streamTitle, tmp, sizeof(tmp));
    portEXIT_CRITICAL(&titleMux);
    titleSeq++;
}

static void getTitle(char* outBuf, size_t size)
{
    portENTER_CRITICAL(&titleMux);
    strlcpy(outBuf, streamTitle, size);
    portEXIT_CRITICAL(&titleMux);
}

static void icyCallback(void*, const char* type, bool, const char* str)
{
    if (strcmp(type, "StreamTitle") == 0) setTitle(str);
}

// (2-4) Level meter input. The output calls meterMonitor() from the decode task with
// every buffer it hands to the speaker (stereo PCM, before the speaker volume). It
// keeps the loudest RMS since the screen last read it. Only broadcast audio counts:
// meterFromStream is true only while the MP3 decoder is running, so the receiver
// noise (same output) never reaches the meter.
static std::atomic<bool> meterFromStream{false};
static std::atomic<uint16_t> meterRmsMax{0};

static void meterMonitor(void*, const int16_t* stereo, size_t frames)
{
    if (!stereo || frames == 0 || !meterFromStream.load()) return;
    uint64_t sum = 0;
    for (size_t i = 0; i < frames * 2; i++) {
        int32_t s = stereo[i];
        sum += (uint64_t)(s * s);
    }
    uint16_t rms = (uint16_t)sqrtf((float)sum / (float)(frames * 2));
    if (rms > meterRmsMax.load()) meterRmsMax = rms;  // the screen takes it with exchange(0)
}

static void stopStream()
{
    meterFromStream = false;
    if (usbOpen) {  // (U2a) the relay stops sending; its bytes are discarded by the link
        usbLink.post(rdlink::REQ_STOP, nullptr);
        usbOpen = false;
    }
    if (decoder) {
        decoder->stop();
        delete decoder;
        decoder = nullptr;
    }
    if (buff) {
        buff->close();
        delete buff;
        buff = nullptr;
    }
    if (src) {
        src->close();
        delete src;
        src = nullptr;
    }
    out.stop();
    bufferPercent = 0;
}

// (U2a) A stream needs its own path: the relay for a USB stream, Wi-Fi for an ICY stream,
// either one before anything is open.
static bool pathUp()
{
    if (usbOpen) return usbLink.ready();
    if (src) return WiFi.status() == WL_CONNECTED;
    return usbLink.ready() || WiFi.status() == WL_CONNECTED;
}
static bool streamOpen() { return src || buff || decoder || usbOpen; }
static bool radioShouldRun() { return radioEnabled.load() && pathUp(); }
static bool radioInterrupted() { return !radioShouldRun() || stationRequest.load() != REQ_NONE; }

static void waitInterruptible(uint32_t ms)
{
    uint32_t start = millis();
    while (!radioInterrupted() && millis() - start < ms) vTaskDelay(pdMS_TO_TICKS(10));
}

static void updateBufferPercent()
{
    if (buff) bufferPercent = (uint8_t)(buff->getFillLevel() * 100u / STREAM_BUFFER_SIZE);
}

// Fills the stream buffer to `level`. Returns false if interrupted (station
// request, radio disabled, Wi-Fi lost); sets *timedOut if the stream was too slow.
static bool waitForBuffer(int level, bool* timedOut)
{
    *timedOut = false;
    uint32_t start = millis();
    while (buff->getFillLevel() < (uint32_t)level) {
        if (radioInterrupted()) return false;
        if (millis() - start >= BUFFER_WAIT_MS) {
            *timedOut = true;
            break;
        }
        buff->loop();
        updateBufferPercent();
        vTaskDelay(1);
    }
    updateBufferPercent();
    return true;
}

// (2-3) Receiver noise for MANUAL tuning between stations. It is generated here, in
// the decode task, into the same output object as the stream, so only one task ever
// feeds the speaker and no extra buffers are needed. The speaker's master volume
// (volume and mute) applies to it as to the stream. Quieter than a broadcast.
static constexpr int NOISE_RATE = 22050;
static constexpr int NOISE_FRAMES_PER_STEP = 256;
static constexpr int32_t NOISE_LEVEL = 8000;  // scale of the filtered noise, of 32767
static uint32_t noiseRng = 0x2545F491u;
static int32_t noiseLowpass = 0;

static void produceNoise(int frames)
{
    out.SetRate(NOISE_RATE);
    for (int i = 0; i < frames; i++) {
        noiseRng = noiseRng * 1664525u + 1013904223u;          // LCG
        int32_t x = (int32_t)(noiseRng >> 16) - 32768;         // white, -32768..32767
        noiseLowpass += (x - noiseLowpass) / 4;                // softer hiss
        int16_t s = (int16_t)(noiseLowpass * NOISE_LEVEL / 32768);
        int16_t frame[2] = {s, s};
        // When the output buffer is full, ConsumeSample() hands it to the speaker
        // (waiting for a free buffer, which paces this loop) and returns false.
        if (!out.ConsumeSample(frame)) out.ConsumeSample(frame);
    }
}

// (U2a) Asks the relay for a station and waits for its answer (design 5, steps 2-4).
// 1: streaming (usbSource attached), 0: refused or failed, -1: interrupted.
static int openUsb(const char* url)
{
    uint32_t req = usbLink.post(rdlink::REQ_TUNE, url);
    usbOpen = true;  // from here stopStream() also stops the relay
    usbNak = 0;
    for (;;) {
        rdlink::Result r = usbLink.result(req);
        if (r == rdlink::RES_STREAMING) {
            usbSource.attach(req);
            return 1;
        }
        if (r == rdlink::RES_NAK) {
            usbNak = usbLink.nakReason();
            return 0;
        }
        if (r != rdlink::RES_PENDING) return 0;
        if (radioInterrupted()) return -1;
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

static void decodeTask(void*)
{
    int index = 0;
    bool needOpen = true;
    bool userSelected = false;  // the station was chosen with the dial or keys
    bool noiseMode = false;     // (2-3) MANUAL dial between stations
    bool noiseActive = false;   // noise samples have gone to the output
    int failures = 0;
    bool starved = false;
    bool confirmed = false;
    uint32_t framesAtStart = 0;

    for (;;) {
        vTaskDelay(1);
        if (!radioEnabled.load()) {
            if (streamOpen() || noiseActive) {
                stopStream();
                noiseActive = false;
                Serial.println("[RADIO] stopped");
            }
            radioState = RS_OFF;
            needOpen = true;
            decodeOffSeq++;  // (3-1) nothing is open: the Location may be switched
            vTaskDelay(pdMS_TO_TICKS(50));
            continue;
        }
        // (2-3) Take the latest request: a station or the noise.
        int req = stationRequest.exchange(REQ_NONE);
        if (req == REQ_NOISE) {
            if (streamOpen()) stopStream();  // the broadcast (and the relay) stops before any noise
            noiseMode = true;
            meterFromStream = false;  // (2-4) noise never lights the meter
            userSelected = true;
            failures = 0;
            currentStation = -1;
            setTitle("");
            Serial.println("[RADIO] noise");
        } else if (req >= 0) {
            noiseMode = false;
            index = req % locCount();
            needOpen = true;
            userSelected = true;
            failures = 0;
        }
        if (noiseMode) {
            radioState = RS_NOISE;
            produceNoise(NOISE_FRAMES_PER_STEP);  // needs no network
            noiseActive = true;
            continue;
        }
        if (noiseActive) {
            out.stop();  // the queued noise plays out before the stream starts
            noiseActive = false;
        }
        if (!pathUp()) {  // (U2a) neither the relay nor Wi-Fi (or the open stream's path is gone)
            if (streamOpen()) {
                stopStream();
                Serial.println("[RADIO] stopped (no Wi-Fi / relay)");
            }
            radioState = RS_WAIT_WIFI;
            needOpen = true;
            vTaskDelay(pdMS_TO_TICKS(50));
            continue;
        }
        if (!streamBuffer || !codecBuffer) {
            radioState = RS_NO_MEMORY;
            vTaskDelay(pdMS_TO_TICKS(500));
            continue;
        }

        if (needOpen) {
            needOpen = false;
            if (index < 0 || index >= locCount()) index = 0;  // (3-1) the Location may have changed
            stopStream();
            currentStation = index;
            setTitle("");
            starved = false;
            confirmed = false;
            radioState = RS_CONNECTING;
            bool viaUsb = usbLink.ready();  // (U2a) the relay, once it has answered, comes first
            Serial.printf("[RADIO] open %d %s via %s (heap %u, largest %u)\n", index, locStations()[index].url,
                          viaUsb ? "USB" : "Wi-Fi", (unsigned)ESP.getFreeHeap(),
                          (unsigned)heap_caps_get_largest_free_block(MALLOC_CAP_8BIT));
            bool ok;
            if (viaUsb) {
                // (U2a) design 5: the old stream is closed (stopStream above); TUNE, wait for
                // "RDSYNC <id>", then a new, empty buffer object on the same 32 KB.
                int r = openUsb(locStations()[index].url);
                if (r < 0) {
                    needOpen = true;  // interrupted: re-evaluated at the top
                    continue;
                }
                ok = r > 0;
                if (ok) buff = new (std::nothrow) AudioFileSourceBuffer(&usbSource, streamBuffer, STREAM_BUFFER_SIZE);
            } else {
                // new (std::nothrow): out of memory is a failed station, not a crash.
                src = new (std::nothrow) AudioFileSourceICYStream(locStations()[index].url);
                ok = src && src->isOpen();
                if (ok) {
                    src->RegisterMetadataCB(icyCallback, nullptr);
                    buff = new (std::nothrow) AudioFileSourceBuffer(src, streamBuffer, STREAM_BUFFER_SIZE);
                }
            }
            if (ok && buff) {
                // The first read fills the buffer wholesale (M5Unified example).
                uint8_t dummy;
                buff->read(&dummy, 0);
                radioState = RS_BUFFERING;
                bool timedOut;
                if (!waitForBuffer(STREAM_START_LEVEL, &timedOut)) {
                    needOpen = true;  // re-evaluated at the top: request, disable or Wi-Fi loss
                    continue;
                }
                starved = timedOut;
                decoder = new (std::nothrow) AudioGeneratorMP3(codecBuffer, MP3_CODEC_SIZE);
                ok = decoder && decoder->begin(buff, &out);
            } else {
                ok = false;
            }
            if (ok) {
                framesAtStart = out.getFrames();
                meterFromStream = true;  // (2-4) from here the output carries the broadcast
                radioState = RS_PLAYING;
                Serial.println("[RADIO] playing");
                continue;
            }
        } else if (decoder && decoder->isRunning()) {
            if (!confirmed && out.getRate() != 0 &&
                out.getFrames() - framesAtStart >= out.getRate() * STATION_OK_SECONDS) {
                confirmed = true;
                failures = 0;
            }
            uint32_t level = buff->getFillLevel();
            updateBufferPercent();
            if (starved) {
                if (level >= (uint32_t)STREAM_START_LEVEL) starved = false;
            } else if (level < (uint32_t)STREAM_LOW_LEVEL) {
                underruns++;  // (U2a2) for the log: the stream did not keep up
                radioState = RS_BUFFERING;
                bool timedOut;
                if (!waitForBuffer(STREAM_START_LEVEL, &timedOut)) {
                    needOpen = true;
                    continue;
                }
                starved = timedOut;
                radioState = RS_PLAYING;
            }
            {
                // (U2a2) For the log: a long pause here means the decode task was held up.
                static uint32_t lastStepMs = 0;
                uint32_t nowMs = millis();
                if (lastStepMs && nowMs - lastStepMs < 60000 && nowMs - lastStepMs > decodeGapMax.load())
                    decodeGapMax = nowMs - lastStepMs;
                lastStepMs = nowMs;
            }
            if (decoder->loop()) continue;
            decoder->stop();
            if (confirmed) {
                // The stream ended or broke after playing: open the same station again.
                Serial.println("[RADIO] stream ended, reopening");
                stopStream();
                radioState = RS_CONNECTING;
                waitInterruptible(1000);
                needOpen = true;
                continue;
            }
        } else {
            needOpen = true;  // nothing open: start again
            continue;
        }

        // Could not open, broke while buffering, or stopped before STATION_OK_SECONDS.
        stopStream();
        Serial.printf("[RADIO] station %d failed%s%s\n", index, usbNak ? ", relay refused: " : "",
                      usbNak ? rdlink::NAK_REASONS[usbNak % rdlink::NAK_REASON_COUNT] : "");
        if (userSelected && rdlink::nakIsPermanent(usbNak)) {
            // (U2a) The relay will refuse this URL again: wait until the dial moves.
            usbNak = 0;
            radioState = RS_NO_SIGNAL;
            setTitle("(refused by the relay)");
            while (!radioInterrupted()) vTaskDelay(pdMS_TO_TICKS(50));
            needOpen = true;
            continue;
        }
        usbNak = 0;
        if (userSelected) {
            // Chosen with the dial: stay on it and try again later (the dial may move meanwhile).
            radioState = RS_NO_SIGNAL;
            setTitle("(no signal - retrying)");
            waitInterruptible(SELECTED_RETRY_MS);
            needOpen = true;
            continue;
        }
        // Start-up station: move on to the next one, as in 2-1.
        setTitle("(station unavailable)");
        if (++failures >= locCount()) {
            failures = 0;
            radioState = RS_ALL_FAILED;
            waitInterruptible(ALL_FAILED_WAIT_MS);
        } else {
            radioState = RS_SKIPPING;
            waitInterruptible(800);
        }
        index = (index + 1) % locCount();
        currentStation = index;
        autoMoveSeq++;  // the dial follows (only before the first tuning)
        needOpen = true;
    }
}

// ---------------------------------------------------------------- volume
static int volume = VOL_START;
static bool muted = false;

static void applyVolume()
{
    M5.Speaker.setVolume((uint8_t)(muted ? 0 : volume));
}

static void changeVolume(int detents)
{
    if (muted) muted = false;  // turning while muted unmutes
    volume = constrain(volume + detents * VOL_STEP, 0, VOL_MAX);
    applyVolume();
}

static void toggleMute()
{
    muted = !muted;
    applyVolume();
    Serial.printf("[VOL] %s\n", muted ? "mute" : "unmute");
}

// ---------------------------------------------------------------- U135 (checked reads, as Media Jog)
static bool u135Found = false;
static int16_t u135Last = 0;
static int16_t u135Accum = 0;
static bool knobDown = false;
static bool knobTurnedWhileDown = false;
static uint32_t knobDownMs = 0;
static uint32_t knobUpMs = 0;
static uint32_t u135ProbeMs = 0;
static uint8_t u135ReadErrors = 0;

static bool u135Read(uint8_t reg, uint8_t* buf, uint8_t len)
{
    Wire.beginTransmission(U135_ADDR);
    Wire.write(reg);
    if (Wire.endTransmission() != 0) return false;
    if (Wire.requestFrom((uint8_t)U135_ADDR, len) != len) return false;
    for (uint8_t i = 0; i < len; i++) {
        int b = Wire.read();
        if (b < 0) return false;
        buf[i] = (uint8_t)b;
    }
    return true;
}

static bool u135ReadState(int16_t& value, bool& down)
{
    uint8_t v[2];
    uint8_t b;
    if (!u135Read(U135_REG_VALUE, v, 2) || !u135Read(U135_REG_BUTTON, &b, 1) || b > 1) return false;
    value = (int16_t)(v[0] | (v[1] << 8));
    down = (b == BUTTON_PRESSED_RAW);
    return true;
}

static void u135Probe()
{
    u135ProbeMs = millis();
    Wire.beginTransmission(U135_ADDR);
    if (Wire.endTransmission() != 0) return;
    int16_t v;
    bool down;
    if (!u135ReadState(v, down)) return;
    u135Found = true;
    u135Last = v;
    u135Accum = 0;
    knobDown = down;  // a knob already held at detection does not count as a press
    knobTurnedWhileDown = true;
    u135ReadErrors = 0;
    Serial.printf("[U135] found, value %d\n", v);
}


static KnobInput u135Poll()
{
    KnobInput in;
    if (!u135Found) {
        if (millis() - u135ProbeMs >= U135_PROBE_INTERVAL_MS) u135Probe();
        return in;
    }
    int16_t v;
    bool down;
    if (!u135ReadState(v, down)) {
        if (++u135ReadErrors >= 10) {
            u135Found = false;
            u135ProbeMs = millis();
            Serial.println("[U135] lost");
        }
        return in;
    }
    u135ReadErrors = 0;
    u135Accum += (int16_t)(v - u135Last);
    u135Last = v;
    while (u135Accum >= COUNTS_PER_DETENT) {
        u135Accum -= COUNTS_PER_DETENT;
        in.detents++;
    }
    while (u135Accum <= -COUNTS_PER_DETENT) {
        u135Accum += COUNTS_PER_DETENT;
        in.detents--;
    }
    uint32_t now = millis();
    // (2-2) Turning counts as volume while the knob is held at this poll or the
    // previous one, and for RELEASE_GRACE_MS after release; otherwise it tunes.
    bool held = down || knobDown;
    bool justReleased = !held && now - knobUpMs < RELEASE_GRACE_MS;
    if (held || justReleased) in.volumeDetents = in.detents;
    else in.tuneDetents = in.detents;

    if (down && !knobDown) {
        knobDownMs = now;
        knobTurnedWhileDown = false;
    }
    if (held && in.detents != 0) knobTurnedWhileDown = true;
    if (!down && knobDown) {
        knobUpMs = now;
        // A press that turned the volume never toggles mute.
        if (!knobTurnedWhileDown && now - knobDownMs <= SHORT_PRESS_MAX_MS) in.shortPress = true;
    }
    knobDown = down;
    return in;
}

// ---------------------------------------------------------------- keyboard (edges from the key state)

static std::vector<char> prevWord;
static bool prevEnter = false, prevDel = false, prevTab = false;

static KeyInput keyboardPoll()
{
    KeyInput in;
    auto& ks = M5Cardputer.Keyboard.keysState();
    for (char c : ks.word) {
        bool wasDown = false;
        for (char p : prevWord) {
            if (p == c) {
                wasDown = true;
                break;
            }
        }
        if (!wasDown) in.chars += c;
    }
    in.enter = ks.enter && !prevEnter;
    in.del = ks.del && !prevDel;
    in.tab = ks.tab && !prevTab;
    in.fn = ks.fn;
    prevWord = ks.word;
    prevEnter = ks.enter;
    prevDel = ks.del;
    prevTab = ks.tab;
    return in;
}

static bool hasChar(const KeyInput& k, char c) { return k.chars.indexOf(c) >= 0; }
static bool hasCharI(const KeyInput& k, char c) { return hasChar(k, (char)tolower(c)) || hasChar(k, (char)toupper(c)); }
static bool anyKey(const KeyInput& k) { return k.chars.length() > 0 || k.enter || k.del || k.tab; }

// ---------------------------------------------------------------- Wi-Fi (active network)
// (U4) A search for a saved network, in the background, never waiting in loop():
//   1. the network that connected last, with WiFi.begin's own channel search; "not found"
//      or "refused" ends the attempt at once, else it ends after WIFI_ATTEMPT_MS;
//   2. only with 2 or more saved networks: one scan (no hidden networks), then the other
//      saved networks that were found, strongest first, by channel and BSSID;
//   3. the whole search ends after WIFI_SEARCH_MS; then a pause of 5 -> 60 s (as U3), and
//      the next search starts again at 1.
// Nothing runs while the USB relay is linked (U2a2) or after U (U2a4).
enum WifiState : uint8_t { WS_OFF, WS_CONNECTING, WS_CONNECTED, WS_WAIT_RETRY, WS_PAUSED /* (U2a2) */,
                           WS_SCANNING /* (U4) */ };
static WifiState wifiState = WS_OFF;
static uint32_t wifiStateMs = 0;  // the current attempt / scan / pause started
static uint32_t wifiRetryMs = WIFI_RETRY_MIN_MS;
static bool wifiInitDone = false;
static String activeSsid, activePass;  // the network that connected last (START screen)
static bool haveNetwork = false;       // (U4) at least one saved network
static bool usbOnly = false;  // (U2a4) started with U: no Wi-Fi attempt in this start (the saved network is kept)

// (U4) The search in progress.
static uint32_t searchStartMs = 0;
static uint8_t searchTried = 0;     // bit per saved entry
static bool searchScanned = false;
static int8_t searchIndex = -1;     // entry being tried
static WifiCandidate searchCand[WIFI_SAVED_MAX];
static int searchCandCount = 0, searchCandNext = 0;
static bool scanToDiscard = false;  // a background scan was left running when the search stopped

static void useSavedList()
{
    haveNetwork = savedList.count > 0;
    if (haveNetwork) {
        activeSsid = savedList.net[savedList.last].ssid;
        activePass = savedList.net[savedList.last].pass;
    } else {
        activeSsid = activePass = "";
    }
}

static void wifiInit()
{
    if (wifiInitDone) return;
    WiFi.persistent(false);       // the driver keeps the network in RAM only (no NVS write)
    WiFi.setAutoReconnect(false); // reconnection is done here, with a back-off
    WiFi.mode(WIFI_STA);
    WiFi.setSleep(false);         // modem sleep makes the stream arrive in bursts
    wifiInitDone = true;
    Serial.println("[WIFI] started (persistent off)");
}

static void wifiBegin(const String& ssid, const String& pass, int32_t channel = 0, const uint8_t* bssid = nullptr)
{
    wifiInit();
    WiFi.disconnect(false, false);  // never erase anything
    WiFi.begin(ssid.c_str(), pass.length() ? pass.c_str() : (const char*)nullptr, channel, bssid);
    if (bssid) Serial.printf("[WIFI] connecting to \"%s\" (channel %ld)\n", ssid.c_str(), (long)channel);
    else Serial.printf("[WIFI] connecting to \"%s\"\n", ssid.c_str());
}

static void wifiTry(int index, const WifiCandidate* c)
{
    searchIndex = (int8_t)index;
    searchTried |= (uint8_t)(1u << index);
    wifiBegin(savedList.net[index].ssid, savedList.net[index].pass, c ? c->channel : 0, c ? c->bssid : nullptr);
    wifiState = WS_CONNECTING;
    wifiStateMs = millis();
}

static void wifiGiveUp(const char* why)
{
    WiFi.disconnect(false, false);
    if (wifiState == WS_SCANNING) scanToDiscard = true;
    wifiState = WS_WAIT_RETRY;
    wifiStateMs = millis();
    Serial.printf("[WIFI] %s, retry in %lu s\n", why, (unsigned long)(wifiRetryMs / 1000));
}

// Starts a search (step 1). Nothing without a saved network.
static void wifiConnectActive()
{
    if (savedList.count == 0) return;
    searchStartMs = millis();
    searchTried = 0;
    searchScanned = false;
    searchCandCount = searchCandNext = 0;
    wifiTry(savedList.last, nullptr);
}

// After a failed attempt: the next step of the search.
static void wifiSearchNext()
{
    if (millis() - searchStartMs >= WIFI_SEARCH_MS) {
        wifiGiveUp("search time over");
        return;
    }
    if (!searchScanned && savedList.count >= 2 && !scanToDiscard) {
        searchScanned = true;
        WiFi.disconnect(false, false);
        if (WiFi.scanNetworks(true /* async */, false /* no hidden */) == WIFI_SCAN_RUNNING) {
            wifiState = WS_SCANNING;
            wifiStateMs = millis();
            Serial.println("[WIFI] scanning for the other saved networks");
            return;
        }
        Serial.println("[WIFI] scan failed to start");
    }
    while (searchCandNext < searchCandCount) {
        const WifiCandidate& c = searchCand[searchCandNext++];
        if (searchTried & (1u << c.index)) continue;
        wifiTry(c.index, &c);
        return;
    }
    wifiGiveUp("no saved network answered");
}

// The scan is done: the saved networks it found, strongest first (one per entry, its
// strongest access point).
static void wifiCollectCandidates(int n)
{
    searchCandCount = searchCandNext = 0;
    for (int e = 0; e < savedList.count; e++) {
        if (searchTried & (1u << e)) continue;
        int best = -1;
        for (int i = 0; i < n; i++)
            if (WiFi.SSID(i) == savedList.net[e].ssid && (best < 0 || WiFi.RSSI(i) > WiFi.RSSI(best))) best = i;
        if (best < 0) continue;
        WifiCandidate& c = searchCand[searchCandCount++];
        c.index = (int8_t)e;
        c.channel = (int8_t)WiFi.channel(best);
        c.rssi = (int8_t)WiFi.RSSI(best);
        memcpy(c.bssid, WiFi.BSSID(best), 6);
    }
    for (int i = 1; i < searchCandCount; i++)
        for (int j = i; j > 0 && searchCand[j].rssi > searchCand[j - 1].rssi; j--) std::swap(searchCand[j], searchCand[j - 1]);
    WiFi.scanDelete();
    Serial.printf("[WIFI] scan done: %d of the saved networks found\n", searchCandCount);
}

static void wifiConnected()
{
    wifiState = WS_CONNECTED;
    wifiRetryMs = WIFI_RETRY_MIN_MS;
    Serial.printf("[WIFI] connected to \"%s\" in %lu ms, IP %s, RSSI %d\n", savedList.net[searchIndex].ssid,
                  (unsigned long)(millis() - searchStartMs), WiFi.localIP().toString().c_str(), WiFi.RSSI());
    if (searchIndex != savedList.last) {  // a different saved network: it is tried first next time
        WifiListRecord next = savedList;
        next.last = (uint8_t)searchIndex;
        if (nvListState == NV_OK || nvListState == NV_NONE) nvsWriteWifiList(next, false, "wifi-last-changed");
        savedList.last = (uint8_t)searchIndex;  // also when it could not be written (used for now)
    }
    useSavedList();
}

static void wifiService()
{
    if (usbOnly) return;  // (U2a4) USB only in this start: Wi-Fi stays off until the setup (W) is opened

    if (scanToDiscard && WiFi.scanComplete() != WIFI_SCAN_RUNNING) {  // (U4) results nobody waits for
        WiFi.scanDelete();
        scanToDiscard = false;
    }
    // (U2a2) While the USB relay is linked, no attempt to join Wi-Fi. A connection already
    // made is kept. When the relay is gone, the attempts start again.
    if (usbLink.ready()) {
        if (wifiState == WS_CONNECTING || wifiState == WS_WAIT_RETRY || wifiState == WS_SCANNING) {
            if (wifiState == WS_SCANNING) scanToDiscard = true;
            WiFi.disconnect(false, false);
            wifiState = WS_PAUSED;
            Serial.println("[WIFI] paused: the USB relay is in use");
        }
        if (wifiState == WS_PAUSED) return;
    } else if (wifiState == WS_PAUSED) {
        Serial.println("[WIFI] resumed: no USB relay");
        wifiState = WS_OFF;
        wifiRetryMs = WIFI_RETRY_MIN_MS;
        wifiConnectActive();
        return;
    }
    uint32_t now = millis();
    switch (wifiState) {
    case WS_CONNECTING: {
        wl_status_t st = WiFi.status();
        if (st == WL_CONNECTED) {
            wifiConnected();
            break;
        }
        bool refused = (st == WL_NO_SSID_AVAIL || st == WL_CONNECT_FAILED) && now - wifiStateMs >= WIFI_FAIL_GUARD_MS;
        if (refused || now - wifiStateMs >= WIFI_ATTEMPT_MS || now - searchStartMs >= WIFI_SEARCH_MS) {
            Serial.printf("[WIFI] \"%s\": %s after %lu ms\n", savedList.net[searchIndex].ssid,
                          st == WL_NO_SSID_AVAIL ? "not found" : (st == WL_CONNECT_FAILED ? "refused" : "no answer"),
                          (unsigned long)(now - wifiStateMs));
            WiFi.disconnect(false, false);
            wifiSearchNext();
        }
        break;
    }
    case WS_SCANNING: {
        int n = WiFi.scanComplete();
        if (n == WIFI_SCAN_RUNNING) {
            if (now - searchStartMs >= WIFI_SEARCH_MS) wifiGiveUp("search time over (scanning)");
            break;
        }
        if (n > 0) wifiCollectCandidates(n);
        else WiFi.scanDelete();
        wifiState = WS_OFF;  // the scan is over (nothing left to discard)
        wifiSearchNext();
        break;
    }
    case WS_CONNECTED:
        if (WiFi.status() != WL_CONNECTED) {
            wifiState = WS_WAIT_RETRY;
            wifiStateMs = now;
            wifiRetryMs = WIFI_RETRY_MIN_MS;
            Serial.println("[WIFI] lost");
        }
        break;
    case WS_WAIT_RETRY:
        if (now - wifiStateMs >= wifiRetryMs) {
            wifiRetryMs = (wifiRetryMs * 2 > WIFI_RETRY_MAX_MS) ? WIFI_RETRY_MAX_MS : wifiRetryMs * 2;
            wifiConnectActive();
        }
        break;
    default:
        break;
    }
}

// ---------------------------------------------------------------- (2-5) clock (NTP) and moon phase
// The clock is shown only after SNTP has reported a sync in this session: a time
// kept in the RTC across a restart (for example set by another app) is not trusted.
// (U7) The time zone is the current Location's (Location::tz); U6 and earlier used Japan
// time whatever the Location. Nothing here writes NVS.
static constexpr const char* NTP_SERVER_1 = "ntp.nict.jp";
static constexpr const char* NTP_SERVER_2 = "pool.ntp.org";
static constexpr time_t CLOCK_MIN_VALID = 1735689600;  // 2025-01-01 UTC
static constexpr time_t CLOCK_MAX_VALID = 4102444800;  // (U3) 2100-01-01 UTC
static std::atomic<bool> timeSynced{false};  // the clock may be shown (NTP or, from U3, the Mac relay)
static std::atomic<bool> ntpSynced{false};   // (U3) NTP has set it: the relay's time is not used then
static bool ntpStarted = false;

static void onTimeSync(struct timeval*)
{
    ntpSynced = true;  // called by the SNTP task
    timeSynced = true;
}

// (U7) The display time zone of the current Location (localtime() follows it, daylight
// saving included). The Sun and Moon never use it: they work in UTC.
static void clockZone()
{
    setenv("TZ", gLoc.load()->tz, 1);
    tzset();
}

// (U3) The display time zone, set once at start (U2a4 set it only when NTP started, so a
// clock set any other way would have shown UTC). (U7) The saved Location's zone.
static void clockInit()
{
    clockZone();
    Serial.printf("[CLOCK] zone %s (%s)\n", gLoc.load()->tz, gLoc.load()->name);
}

// (U3) Sets the clock from the Mac relay's UTC seconds, received `ageMs` ago. Not used
// once NTP has synced, nor for a value outside 2025..2100. Returns true when set.
static bool applyRelayTime(time_t utc, uint32_t ageMs)
{
    if (ntpSynced.load() || utc < CLOCK_MIN_VALID || utc > CLOCK_MAX_VALID) return false;
    struct timeval tv;
    tv.tv_sec = utc + (time_t)(ageMs / 1000);
    tv.tv_usec = (suseconds_t)(ageMs % 1000) * 1000;
    settimeofday(&tv, nullptr);
    timeSynced = true;
    Serial.printf("[CLOCK] time from the USB relay (UTC %lu)\n", (unsigned long)utc);
    return true;
}

static void clockService()
{
    uint32_t ageMs;
    uint32_t relayUtc = usbLink.takeRelayTime(ageMs);  // (U3)
    if (relayUtc) applyRelayTime((time_t)relayUtc, ageMs);
    if (ntpStarted || WiFi.status() != WL_CONNECTED) return;
    sntp_set_time_sync_notification_cb(onTimeSync);
    configTzTime(gLoc.load()->tz, NTP_SERVER_1, NTP_SERVER_2);  // (U7) the Location's zone, not Japan time
    clockZone();  // as SOUND TERRARIUM: make localtime() use it in every case
    ntpStarted = true;
    Serial.printf("[CLOCK] NTP started (%s)\n", gLoc.load()->tz);
}

static bool clockValid()
{
    return timeSynced.load() && time(nullptr) > CLOCK_MIN_VALID;
}

// Moon phase 0..1 (0 = new, 0.5 = full) from the Moon-Sun elongation, using the
// largest periodic terms. Same model as SOUND TERRARIUM's currentLunarGeometry()
// (MIT, same author), enough for a 15-pixel moon.
static float moonPhase01(time_t utc)
{
    auto norm = [](float a) { a = fmodf(a, 360.f); return a < 0.f ? a + 360.f : a; };
    const float T = (float)(((2440587.5 + (double)utc / 86400.0) - 2451545.0) / 36525.0);
    const float D = norm(297.8501921f + 445267.1114034f * T);
    const float M = norm(357.5291092f + 35999.0502909f * T);
    const float Mp = norm(134.9633964f + 477198.8675055f * T);
    const float r = (float)M_PI / 180.f;
    float e = D + 6.289f * sinf(Mp * r) + 1.274f * sinf((2.f * D - Mp) * r) - 1.9146f * sinf(M * r);
    return norm(e) / 360.f;
}

// (3-3) Altitude of the Sun in degrees at (lat, lon) for a UTC time. Low-precision
// solar position (mean anomaly, ecliptic longitude, obliquity, sidereal time);
// about 0.1 degree, which is about a minute at sunrise. Arguments are the place, so
// a station's own city can be used later.
static float solarAltitudeDeg(time_t utc, float latDeg, float lonDeg)
{
    const double r = M_PI / 180.0;
    double d = (double)utc / 86400.0 + 2440587.5 - 2451545.0;  // days since J2000.0
    double g = fmod(357.529 + 0.98560028 * d, 360.0) * r;
    double q = fmod(280.459 + 0.98564736 * d, 360.0);
    double L = (q + 1.915 * sin(g) + 0.020 * sin(2.0 * g)) * r;  // ecliptic longitude
    double e = (23.439 - 0.00000036 * d) * r;                   // obliquity
    double ra = atan2(cos(e) * sin(L), cos(L));
    double dec = asin(sin(e) * sin(L));
    double gmst = fmod(18.697374558 + 24.06570982441908 * d, 24.0) * 15.0 * r;
    double ha = gmst + lonDeg * r - ra;
    double lat = latDeg * r;
    double alt = asin(sin(lat) * sin(dec) + cos(lat) * cos(dec) * cos(ha));
    return (float)(alt / r);
}

static constexpr float SUN_HORIZON_DEG = -0.833f;  // upper limb on the horizon, with refraction

// Where the sky is taken from: the Location's city; WORLD uses Tokyo for now.
static void skyPlace(int locIndex, float& lat, float& lon)
{
    if (LOCATIONS[locIndex].hasMarker) {
        lat = LOCATIONS[locIndex].lat;
        lon = LOCATIONS[locIndex].lon;
    } else {
        lat = 35.68f;
        lon = 139.69f;
    }
}

// ---------------------------------------------------------------- screens
static Screen screen = SC_START;
static bool screenDirty = true;
static auto& lcd = M5Cardputer.Display;

static void setScreen(Screen s)
{
    screen = s;
    screenDirty = true;
}

static void drawText(int x, int y, uint8_t size, uint16_t fg, const char* text, int clearWidth = 0)
{
    if (clearWidth > 0) lcd.fillRect(x, y, clearWidth, 8 * size, COL_BG);
    lcd.setFont(&fonts::Font0);  // the built-in font (U5: Font7 for the frequency is gone)
    lcd.setTextSize(size);
    lcd.setTextColor(fg, COL_BG);
    lcd.setCursor(x, y);
    lcd.print(text);
}

static void drawHeader(const char* right)
{
    drawText(4, 4, 2, COL_AMBER, "RADIO DAYS");
    drawText(240 - 6 * (int)strlen(right) - 4, 8, 1, COL_DIM, right);
    lcd.drawFastHLine(0, 24, 240, COL_AMBER);
}

// --- START
// (U2a4) START screen: has the USB relay answered? Redrawn by loop() when that changes.
static bool startUsbShown = false;
static void drawStartUsb()
{
    startUsbShown = usbLink.ready();
    drawText(4, 84, 1, startUsbShown ? COL_OK : COL_DIM,
             startUsbShown ? "USB   : relay found" : "USB   : waiting for the relay", 236);  // (U4) no "Mac"
}

static void drawStart()
{
    lcd.fillScreen(COL_BG);
    drawHeader(BUILD_TAG);
    drawText(4, 32, 1, COL_DIM, "INTERNET RADIO  test build");
    char line[64];
    if (haveNetwork) snprintf(line, sizeof(line), "Wi-Fi : %.30s", activeSsid.c_str());
    else snprintf(line, sizeof(line), "Wi-Fi : not set");
    drawText(4, 50, 1, COL_TEXT, line);
    // (U4) the networks in use, or what is wrong with the saved data
    NvState shownState = nvListState != NV_NONE ? nvListState : nvState;
    if (savedList.count) snprintf(line, sizeof(line), "Saved : %u network%s", (unsigned)savedList.count, savedList.count > 1 ? "s" : "");
    else snprintf(line, sizeof(line), "Saved : %s", NV_TEXT[shownState]);
    drawText(4, 62, 1, shownState == NV_OK || shownState == NV_NONE ? COL_TEXT : COL_WARN, line);
    snprintf(line, sizeof(line), "U135  : %s", u135Found ? "found" : "not found (keyboard works)");
    drawText(4, 74, 1, COL_TEXT, line);
    drawStartUsb();  // (U2a4)
    drawText(4, 96, 1, COL_AMBER, haveNetwork ? "Press a key or the knob to start" : "Press a key or the knob for USB radio");
    drawText(4, 110, 1, COL_DIM, haveNetwork ? "W: Wi-Fi setup   U: USB only" : "W: Wi-Fi setup");
}

// --- RADIO (Stage 3 will put the tuning dial between the header and the status)
// (2-3) Tuning. The dial is a virtual frequency in 0.1 MHz units (locStations()[].freq).
//   AUTO:   each detent jumps to the next / previous station frequency (wrapping round).
//   MANUAL: each detent moves 0.1 MHz between locFreqMin() and locFreqMax() (no wrap, like a dial end).
// The dial moves on screen at once. A station is asked for only after the dial has
// rested TUNE_SETTLE_MS on its frequency. In MANUAL, leaving a station's frequency
// asks for noise at once, so the broadcast never keeps playing off-station.
enum TuneMode : uint8_t { MODE_AUTO, MODE_MANUAL };
static TuneMode tuneMode = MODE_AUTO;  // every start is AUTO (not saved)
static int dialFreq = locStations()[0].freq;
// (U6) The dial is a ring: freqMin..freqMax, then freqMin again (stations.h).
static inline int floorMod(int a, int m) { int r = a % m; return r < 0 ? r + m : r; }
static inline int dialRing() { return locFreqMax() - locFreqMin() + 1; }
static bool tunePending = false;
static uint32_t tuneChangedMs = 0;
static uint32_t seenAutoMove = 0;
static int sentTarget = 0;  // last request sent: station index or REQ_NOISE (the task starts on station 0)

static int stationAtFreq(int f)
{
    for (int i = 0; i < locCount(); i++)
        if (locStations()[i].freq == f) return i;
    return -1;
}

// AUTO: the station frequency after (dir > 0) or before (dir < 0) f, wrapping round.
static int nextStationFreq(int f, int dir)
{
    if (dir > 0) {
        for (int i = 0; i < locCount(); i++)
            if (locStations()[i].freq > f) return locStations()[i].freq;
        return locStations()[0].freq;
    }
    for (int i = locCount() - 1; i >= 0; i--)
        if (locStations()[i].freq < f) return locStations()[i].freq;
    return locStations()[locCount() - 1].freq;
}

static int nearestStationFreq(int f)
{
    int best = locStations()[0].freq;
    for (int i = 1; i < locCount(); i++)
        if (abs(locStations()[i].freq - f) < abs(best - f)) best = locStations()[i].freq;
    return best;
}

static bool radioWorkingOn(int station)
{
    uint8_t rs = radioState.load();
    return currentStation.load() == station &&
           (rs == RS_CONNECTING || rs == RS_BUFFERING || rs == RS_PLAYING);
}

// Sends a request to the decode task unless it is already doing that.
static void sendTarget(int target)
{
    if (target == sentTarget && stationRequest.load() == REQ_NONE) {
        if (target == REQ_NOISE) return;
        if (radioWorkingOn(target)) {
            Serial.printf("[TUNE] back on %d, no change\n", target);
            return;
        }
    }
    sentTarget = target;
    stationRequest = target;  // the latest request wins
    if (target == REQ_NOISE) Serial.println("[TUNE] noise");
    else Serial.printf("[TUNE] request %d (%d.%d)\n", target, locStations()[target].freq / 10, locStations()[target].freq % 10);
}

// (U8) AUTO glide: the dial moves one step every GLIDE_STEP_MS towards glideTarget, the
// way glideDir, glideLeft steps still to go; glideStations counts the stations not
// yet reached (the target included). Stepped by glideService() in loop(), from the time, so the
// speed stays the same when loop() is late.
static constexpr uint32_t GLIDE_STEP_MS = 20;  // one step (2 px) every 20 ms: 100 px a second
static constexpr int GLIDE_MAX_STATIONS = 3;   // stations that can wait ahead; more detents are dropped
static bool glideActive = false;
static int glideDir = 0, glideTarget = 0, glideLeft = 0, glideStations = 0;
static uint32_t glideLastMs = 0;

static inline int ringSteps(int from, int to, int dir)
{
    return dir > 0 ? floorMod(to - from, dialRing()) : floorMod(from - to, dialRing());
}

// AUTO detents: one station each (see the notes at the top).
static void glideAdd(int detents)
{
    int dir = detents > 0 ? 1 : -1;
    int n = abs(detents);
    if (!glideActive) {
        glideActive = true;
        glideDir = dir;
        glideTarget = dialFreq;
        glideLeft = 0;
        glideStations = 0;
        glideLastMs = millis();
        sendTarget(REQ_NOISE);  // the station (and the relay) stop; MANUAL's noise plays
    } else if (dir != glideDir) {  // turn back from where the band is: the stations ahead are dropped
        glideDir = dir;
        glideTarget = dialFreq;
        glideLeft = 0;
        glideStations = 0;
    }
    for (; n > 0 && glideStations < GLIDE_MAX_STATIONS; n--, glideStations++) {
        int next = nextStationFreq(glideTarget, dir);
        glideLeft += ringSteps(glideTarget, next, dir);
        glideTarget = next;
    }
    tunePending = false;  // the glide asks for the station when it arrives
}

static void glideStop() { glideActive = false; }

// (U8) The status and name rows show "tuning" while the dial glides.
static inline bool dialGliding() { return glideActive; }

static void tuneBy(int detents)
{
    if (detents == 0) return;
    if (tuneMode == MODE_AUTO) {
        glideAdd(detents);  // (U8) glide there instead of jumping
        return;
    } else {
        dialFreq = locFreqMin() + floorMod(dialFreq - locFreqMin() + detents, dialRing());  // (U6) round the ring
        if (stationAtFreq(dialFreq) < 0) sendTarget(REQ_NOISE);  // off-station: stop the broadcast now
    }
    tunePending = true;
    tuneChangedMs = millis();
}

static void toggleTuneMode()
{
    if (glideActive) {  // (U8) the glide stops where the band is; MANUAL there (noise between stations)
        glideStop();
        if (stationAtFreq(dialFreq) >= 0) {
            tunePending = true;
            tuneChangedMs = millis();
        }
    }
    if (tuneMode == MODE_AUTO) {
        tuneMode = MODE_MANUAL;  // the dial stays where it is: the station keeps playing
    } else {
        tuneMode = MODE_AUTO;
        if (stationAtFreq(dialFreq) < 0) {  // between stations: settle on the nearest one
            dialFreq = nearestStationFreq(dialFreq);
            tunePending = true;
            tuneChangedMs = millis();
        }
    }
    Serial.printf("[TUNE] mode %s at %d.%d\n", tuneMode == MODE_AUTO ? "AUTO" : "MANUAL", dialFreq / 10, dialFreq % 10);
}

static void tuneService()
{
    if (glideActive) return;  // (U8) the glide asks for its station itself
    if (tunePending) {
        if (millis() - tuneChangedMs < TUNE_SETTLE_MS) return;
        tunePending = false;
        int st = stationAtFreq(dialFreq);
        sendTarget(st >= 0 ? st : REQ_NOISE);
        return;
    }
    // Before the first tuning, the task may move past a start-up station that cannot be
    // reached (as in 2-1); the dial follows it.
    uint32_t seq = autoMoveSeq.load();
    if (seq != seenAutoMove) {
        seenAutoMove = seq;
        int cs = currentStation.load();
        if (cs >= 0 && stationRequest.load() == REQ_NONE && tuneMode == MODE_AUTO) {
            dialFreq = locStations()[cs].freq;
            sentTarget = cs;
        }
    }
}

// --- RADIO screen layout (240 x 135). Stage 3 adds the BCL-style decoration.
static constexpr int Y_FREQ = 20;     // top of the dial window and of the clock column
static constexpr int Y_NAME = 96;     // station name, size 2
static constexpr int Y_STATUS = 114;  // status + volume, size 1
static constexpr int Y_FOOT = 126;    // Wi-Fi, memory, key hint, size 1
// (2-5) clock and moon, right of the frequency
static constexpr int CLOCK_X = 176;
// (3-3) sky window under the clock: x 176..237, y 45..69; the sun or moon in its middle
static constexpr int SKY_X = 176;
static constexpr int SKY_Y = 45;
static constexpr int SKY_W = 62;
static constexpr int SKY_H = 25;
static constexpr int BODY_CX = 206;  // centre of the date / time column
static constexpr int BODY_CY = 57;
static constexpr int BODY_R = 9;     // 19 px disc: the SOUND TERRARIUM face needs about this
// (2-4) level meter column, left of the frequency
static constexpr int METER_SEGMENTS = 8;
static constexpr int METER_X = 6;
static constexpr int METER_W = 22;
static constexpr int METER_H = 4;
static constexpr int METER_GAP = 2;
static constexpr int METER_BOTTOM = 66;  // Y_FREQ + 46: the column spans 20..65, beside the 48 px digits

struct RadioView {
    int freq = -1;
    uint8_t mode = 0xFF;
    int nameStation = -2;
    uint8_t status = 0xFF;
    uint8_t buffer = 0xFF;
    int volume = -1;
    bool muted = false;
    int rssi = 1;
    uint32_t heapKb = 0;
    bool u135 = false;
    uint8_t wifi = 0xFF;
    bool usb = false;  // (U2a) the relay has answered
    int8_t relayHint = -1;  // (U4) "Start the relay" shown instead of the station name
    int posStation = -2;    // (U9) dial position shown (-1: none)
    int posCount = -1;      // (U9) ... out of this many stations
    int32_t waveL = -1, waveR = -1;  // (U10) the towers' wave colours drawn (-1: none yet)
} view;
static bool diagFooter = false;   // (U2a2) key I
static rdlink::Diag diagTotal;    // (U2a2) link counters since start (txFailed, unpluggedSeen; maxGapMs = largest)

static const char* radioStatusText(uint8_t rs, uint16_t& colour)
{
    colour = COL_TEXT;
    switch (rs) {
    case RS_OFF:        colour = COL_DIM;  return "STOPPED";
    case RS_WAIT_WIFI:  colour = COL_WARN; return "WAITING FOR WI-FI/USB";
    case RS_CONNECTING: colour = COL_WARN; return "CONNECTING";
    case RS_BUFFERING:  colour = COL_WARN; return "BUFFERING";
    case RS_PLAYING:    colour = COL_OK;   return "PLAYING";
    case RS_SKIPPING:   colour = COL_ERR;  return "UNAVAILABLE - NEXT";
    case RS_ALL_FAILED: colour = COL_ERR;  return "NO STATION - RETRY";
    case RS_NO_MEMORY:  colour = COL_ERR;  return "NOT ENOUGH MEMORY";
    case RS_NO_SIGNAL:  colour = COL_ERR;  return "NO SIGNAL - RETRY";
    case RS_NOISE:      colour = COL_DIM;  return "NO STATION";
    }
    return "?";
}

// --- (U5) Analog dial: a fixed needle, and rows of ticks that move behind it. No numbers,
// units or band names. (U6) Four rows of evenly spaced ticks (2, 3, 4, 6 px; every fifth
// long; together they repeat every 60 px) in a 1 px grey frame, and no ends: the band is
// drawn from the ring position, and the ring holds whole 60 px periods, so it is the same
// on every lap and flows across the seam; the station marks repeat once a ring. Drawn in
// drawRadio() (every 100 ms, only when the dial moved): only the columns whose tick
// changed are drawn again; the frame and the needle only with the whole screen; no
// canvas, task or animation. docs/usb-u6.md.
static constexpr int DIAL_FX0 = 31, DIAL_FY0 = 19, DIAL_FW = 142, DIAL_FH = 74;  // frame x 31..172, y 19..92
static constexpr int DIAL_X0 = 32, DIAL_W = 140;   // inside the frame: x 32..171
static constexpr int DIAL_Y0 = 20, DIAL_Y1 = 91;   //                    y 20..91
static constexpr int DIAL_ROWS = 4, DIAL_ROW_H = 16; // baselines y 40, 56, 72, 88
static constexpr int DIAL_MARK_Y = 21;               // station wedges y 21..23
static constexpr int NEEDLE_X = 101;                 // needle x 101 (bright) and 102 (shade), y 24..91
static constexpr int NEEDLE_Y0 = 24;
static constexpr int DIAL_PX_PER_STEP = 2;
static constexpr int TICK_SHORT = 4, TICK_LONG = 9;

// (U10) The dial face (docs/dial-face-tower-proposal.md, plan B). RGB565 at build time.
static constexpr uint16_t rgb565(uint8_t r, uint8_t g, uint8_t b)
{
    return (uint16_t)(((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3));
}
static constexpr uint16_t DIAL_FACE = rgb565(240, 234, 214);   // ivory: the inside, and what is cleared to
static constexpr uint16_t DIAL_TICK_C = rgb565(0, 0, 0);       // short and long ticks
static constexpr uint16_t DIAL_BASE_C = rgb565(150, 145, 132); // the four baselines
static constexpr uint16_t DIAL_MARK_C = rgb565(200, 80, 0);    // station marks (U9: COL_AMBER)
static constexpr uint16_t BEVEL_OUT_DARK = rgb565(70, 70, 70), BEVEL_OUT_LIGHT = rgb565(200, 200, 200);
static constexpr uint16_t BEVEL_IN_DARK = rgb565(40, 40, 40), BEVEL_IN_LIGHT = rgb565(150, 150, 150);

struct DialView {
    bool drawn = false;
    int pos = 0;  // ring position drawn (steps from freqMin)
    const Location* loc = nullptr;
} dialView;

// Tick of row r at band coordinate u: 0 none, 1 short, 2 long. Rows: every 2, 3, 4, 6 px.
static int dialTick(int r, int u)
{
    int pitch = r == 3 ? 6 : r + 2;
    if (floorMod(u, pitch)) return 0;
    return floorMod(u, 5 * pitch) ? 1 : 2;
}

static inline bool dialNeedleCol(int x) { return x == NEEDLE_X || x == NEEDLE_X + 1; }
static inline int dialBase(int r) { return 40 + r * DIAL_ROW_H; }

static void dialDrawTick(int x, int r, int t)
{
    if (t) lcd.drawFastVLine(x, dialBase(r) - (t == 2 ? TICK_LONG : TICK_SHORT), t == 2 ? TICK_LONG : TICK_SHORT,
                             DIAL_TICK_C);  // (U10) black on the ivory face
}

// Station marks: 5 x 3 wedges pointing down at the needle's top, once a ring, clipped to
// the inside of the frame. `pos`: the ring position under the needle.
static void dialMarks(int pos, uint16_t colour)
{
    const Location* L = gLoc.load();
    int period = (L->freqMax - L->freqMin + 1) * DIAL_PX_PER_STEP;
    for (int i = 0; i < L->count; i++) {
        int x = NEEDLE_X + floorMod(L->stations[i].freq - L->freqMin - pos, L->freqMax - L->freqMin + 1) * DIAL_PX_PER_STEP;
        while (x - period >= DIAL_X0 - 2) x -= period;
        for (; x <= DIAL_X0 + DIAL_W + 1; x += period)
            for (int row = 0; row < 3; row++) {
                int x0 = max(x - 2 + row, DIAL_X0), x1 = min(x + 2 - row, DIAL_X0 + DIAL_W - 1);
                if (x0 <= x1) lcd.drawFastHLine(x0, DIAL_MARK_Y + row, x1 - x0 + 1, colour);
            }
    }
}

// (U10) A sunken window: two 1 px rectangles round the face, each darker above and left,
// lighter below and right. The inner one is U9's frame line; the outer one is x 30..173,
// y 18..93, where U9 draws nothing. Only round the outside: no line crosses the ticks.
static void dialBevelRect(int x0, int y0, int x1, int y1, uint16_t dark, uint16_t light)
{
    lcd.drawFastHLine(x0, y0, x1 - x0 + 1, dark);
    lcd.drawFastVLine(x0, y0 + 1, y1 - y0 - 1, dark);
    lcd.drawFastHLine(x0, y1, x1 - x0 + 1, light);
    lcd.drawFastVLine(x1, y0 + 1, y1 - y0 - 1, light);
}

static void drawDial(int f)
{
    const Location* L = gLoc.load();
    int pos = f - L->freqMin;
    int scroll = pos * DIAL_PX_PER_STEP, old = dialView.pos * DIAL_PX_PER_STEP;
    bool whole = !dialView.drawn || dialView.loc != L;
    if (whole) {  // the frame, the inside, the needle once; then every tick as new
        lcd.fillRect(DIAL_X0, DIAL_Y0, DIAL_W, DIAL_Y1 - DIAL_Y0 + 1, DIAL_FACE);  // (U10) the face
        dialBevelRect(DIAL_FX0 - 1, DIAL_FY0 - 1, DIAL_FX0 + DIAL_FW, DIAL_FY0 + DIAL_FH, BEVEL_OUT_DARK, BEVEL_OUT_LIGHT);
        dialBevelRect(DIAL_FX0, DIAL_FY0, DIAL_FX0 + DIAL_FW - 1, DIAL_FY0 + DIAL_FH - 1, BEVEL_IN_DARK, BEVEL_IN_LIGHT);
        for (int r = 0; r < DIAL_ROWS; r++) lcd.drawFastHLine(DIAL_X0, dialBase(r), DIAL_W, DIAL_BASE_C);
        lcd.drawFastVLine(NEEDLE_X, NEEDLE_Y0, DIAL_Y1 - NEEDLE_Y0 + 1, lcd.color565(235, 70, 40));
        lcd.drawFastVLine(NEEDLE_X + 1, NEEDLE_Y0 + 1, DIAL_Y1 - NEEDLE_Y0, lcd.color565(90, 25, 15));
        old = scroll;
    } else if (pos == dialView.pos) {
        return;
    } else {
        dialMarks(dialView.pos, DIAL_FACE);  // (U10) cleared to the face
    }
    for (int x = DIAL_X0; x < DIAL_X0 + DIAL_W; x++) {
        if (dialNeedleCol(x)) continue;
        for (int r = 0; r < DIAL_ROWS; r++) {
            int t = dialTick(r, scroll + (x - NEEDLE_X)), o = whole ? 0 : dialTick(r, old + (x - NEEDLE_X));
            if (t == o) continue;
            if (o > t) lcd.drawFastVLine(x, dialBase(r) - TICK_LONG, TICK_LONG - (t ? (t == 2 ? TICK_LONG : TICK_SHORT) : 0), DIAL_FACE);
            dialDrawTick(x, r, t);
        }
    }
    dialMarks(pos, DIAL_MARK_C);
    dialView.drawn = true;
    dialView.pos = pos;
    dialView.loc = L;
}

// --- (U10) Radio towers: two small lattice towers in the empty corners above the station
// name (left under the level meter, right under the sky window), drawn from point tables
// (docs/dial-face-tower-proposal.md). The waves either side of each tip follow the status
// row: towerWaveColours() picks a colour for each side, drawTowerWaves() draws a side only
// when its colour changed. The towers are drawn with the whole radio screen only.
static const int8_t TOWER_BODY_PX[][2] = {  // mast and legs: 36 points
    {-4, 17}, {-4, 18}, {-4, 19}, {-3, 12}, {-3, 13}, {-3, 14}, {-3, 15}, {-3, 16}, {-2, 7},
    {-2, 8}, {-2, 9}, {-2, 10}, {-2, 11}, {-1, 4}, {-1, 5}, {-1, 6}, {0, 0}, {0, 1},
    {0, 2}, {0, 3}, {1, 4}, {1, 5}, {1, 6}, {2, 7}, {2, 8}, {2, 9}, {2, 10},
    {2, 11}, {3, 12}, {3, 13}, {3, 14}, {3, 15}, {3, 16}, {4, 17}, {4, 18}, {4, 19},
};
static const int8_t TOWER_BRACE_PX[][2] = {  // struts and braces: 29 points
    {-3, 18}, {-3, 19}, {-2, 14}, {-2, 18}, {-2, 19}, {-1, 7}, {-1, 8}, {-1, 9}, {-1, 10},
    {-1, 14}, {-1, 17}, {-1, 19}, {0, 5}, {0, 6}, {0, 9}, {0, 11}, {0, 14}, {0, 16},
    {0, 19}, {1, 9}, {1, 12}, {1, 14}, {1, 15}, {1, 19}, {2, 13}, {2, 14}, {2, 15},
    {2, 19}, {3, 19},
};
static const int8_t TOWER_WAVE_PX[][2] = {  // two arcs either side of the tip (radius 5 and 9): 36 points
    {-9, -3}, {-9, -2}, {-9, -1}, {-9, 0}, {-9, 1}, {-9, 2}, {-9, 3}, {-8, -4}, {-8, -3},
    {-8, 3}, {-8, 4}, {-5, -2}, {-5, -1}, {-5, 0}, {-5, 1}, {-5, 2}, {-4, -2}, {-4, 2},
    {4, -2}, {4, 2}, {5, -2}, {5, -1}, {5, 0}, {5, 1}, {5, 2}, {8, -4}, {8, -3},
    {8, 3}, {8, 4}, {9, -3}, {9, -2}, {9, -1}, {9, 0}, {9, 1}, {9, 2}, {9, 3},
};
static constexpr int TOWER_L_X = 15, TOWER_R_X = 206, TOWER_TIP_Y = 75;  // the mast's tip
static constexpr uint16_t TOWER_BODY_C = rgb565(150, 150, 145), TOWER_BRACE_C = rgb565(95, 95, 92);
static constexpr uint16_t WAVE_LIT_C = rgb565(255, 150, 60), WAVE_DIM_C = rgb565(110, 62, 28);
static constexpr uint32_t WAVE_BLINK_MS = 800;  // PLAYING: each side lit this long in turn (1200: calmer)

static void drawTowerPoints(int cx, const int8_t (*pts)[2], int n, uint16_t c)
{
    for (int i = 0; i < n; i++) lcd.drawPixel(cx + pts[i][0], TOWER_TIP_Y + pts[i][1], c);
}

static void drawTowers()
{
    for (int cx : {TOWER_L_X, TOWER_R_X}) {
        drawTowerPoints(cx, TOWER_BODY_PX, sizeof(TOWER_BODY_PX) / sizeof(TOWER_BODY_PX[0]), TOWER_BODY_C);
        drawTowerPoints(cx, TOWER_BRACE_PX, sizeof(TOWER_BRACE_PX) / sizeof(TOWER_BRACE_PX[0]), TOWER_BRACE_C);
    }
}

// The waves' colours for the status row's value `shown` (as drawRadio() shows it): PLAYING,
// muted or not, lights one side and dims the other, in turn every WAVE_BLINK_MS; BUFFERING
// and CONNECTING dim both; everything else (waiting, tuning, no station, stopped, errors,
// the relay hint) leaves them dark.
static void towerWaveColours(uint8_t shown, uint32_t now, uint16_t& left, uint16_t& right)
{
    if (shown == RS_PLAYING) {
        bool l = (now / WAVE_BLINK_MS) % 2 == 0;
        left = l ? WAVE_LIT_C : WAVE_DIM_C;
        right = l ? WAVE_DIM_C : WAVE_LIT_C;
    } else if (shown == RS_BUFFERING || shown == RS_CONNECTING) {
        left = right = WAVE_DIM_C;
    } else {
        left = right = COL_BG;
    }
}

static void drawTowerWaves(int cx, uint16_t c)
{
    drawTowerPoints(cx, TOWER_WAVE_PX, sizeof(TOWER_WAVE_PX) / sizeof(TOWER_WAVE_PX[0]), c);
}

// --- (2-5) clock, (3-3) sky window with the sun or the moon
// Face after SOUND TERRARIUM's drawClassicSun / drawClassicMoon (MIT, same author),
// drawn at the same 9-10 px radius it was designed for.
static void drawFace(int cx, int cy, uint16_t ink)
{
    lcd.drawLine(cx - 6, cy - 4, cx - 4, cy - 5, ink); lcd.drawLine(cx - 4, cy - 5, cx - 2, cy - 4, ink);  // brows
    lcd.drawLine(cx + 2, cy - 4, cx + 4, cy - 5, ink); lcd.drawLine(cx + 4, cy - 5, cx + 6, cy - 4, ink);
    lcd.drawLine(cx - 6, cy - 2, cx - 4, cy - 3, ink); lcd.drawLine(cx - 4, cy - 3, cx - 2, cy - 2, ink);  // eyes
    lcd.drawPixel(cx - 4, cy - 2, ink);
    lcd.drawLine(cx + 2, cy - 2, cx + 4, cy - 3, ink); lcd.drawLine(cx + 4, cy - 3, cx + 6, cy - 2, ink);
    lcd.drawPixel(cx + 4, cy - 2, ink);
    lcd.drawLine(cx, cy - 1, cx - 1, cy + 2, ink); lcd.drawLine(cx - 1, cy + 2, cx, cy + 3, ink);          // nose
    lcd.drawLine(cx, cy + 3, cx + 2, cy + 2, ink);
    lcd.drawPixel(cx, cy + 4, ink);                                                                       // philtrum
    lcd.drawLine(cx - 2, cy + 5, cx, cy + 4, ink); lcd.drawLine(cx, cy + 4, cx + 2, cy + 5, ink);          // lips
    lcd.drawLine(cx - 2, cy + 6, cx, cy + 7, ink); lcd.drawLine(cx, cy + 7, cx + 2, cy + 6, ink);
}

static void drawSun(int cx, int cy)
{
    const uint16_t ink = 0x4204, face = 0xF6AD, ray = lcd.color565(255, 210, 60);
    for (int i = 0; i < 16; i++) {  // rays shortened to fit the 25 px window
        float a = i * (2.f * (float)M_PI / 16.f);
        int r2 = (i % 2 == 0) ? 12 : 11;
        lcd.drawLine(cx + (int)lroundf(cosf(a) * 10), cy + (int)lroundf(sinf(a) * 10),
                     cx + (int)lroundf(cosf(a) * r2), cy + (int)lroundf(sinf(a) * r2), ray);
    }
    lcd.fillCircle(cx, cy, BODY_R, face);
    lcd.drawCircle(cx, cy, BODY_R, ink);
    drawFace(cx, cy, ink);
}

// Real phase as in 2-5 (northern-hemisphere view: waxing = lit on the right).
static void drawFacedMoon(int cx, int cy, float phase)
{
    const uint16_t lit = 0xF689, dark = 0x3186, ink = 0x4204;
    const float sx = sinf(2.f * (float)M_PI * phase), sz = -cosf(2.f * (float)M_PI * phase);
    for (int y = -BODY_R; y <= BODY_R; y++) {
        for (int x = -BODY_R; x <= BODY_R; x++) {
            int d2 = x * x + y * y;
            if (d2 > BODY_R * BODY_R) continue;
            float z = sqrtf((float)(BODY_R * BODY_R - d2));
            lcd.drawPixel(cx + x, cy + y, ((float)x * sx + z * sz > 0.f) ? lit : dark);
        }
    }
    lcd.drawCircle(cx, cy, BODY_R, ink);
    drawFace(cx, cy, ink);
}

// (U4) About 55% of the 3-3 cloud, in a muted white and blue grey.
static void drawCloudSmall(int x, int y)
{
    const uint16_t body = lcd.color565(200, 206, 212), edge = lcd.color565(130, 148, 165);
    lcd.fillCircle(x + 1, y + 1, 1, edge); lcd.fillCircle(x + 3, y, 2, edge); lcd.fillCircle(x + 5, y + 1, 1, edge);
    lcd.fillCircle(x, y, 1, body); lcd.fillCircle(x + 2, y - 1, 2, body); lcd.fillCircle(x + 4, y, 1, body);
    lcd.fillRect(x, y, 4, 2, body);
}


static void drawSkyWindow(SkyState sky, float moonPhase)
{
    if (sky == SKY_UNKNOWN) {  // before NTP: nothing is decided, nothing is drawn
        lcd.fillRect(SKY_X, SKY_Y, SKY_W, SKY_H, COL_BG);
        return;
    }
    if (sky == SKY_DAY) {
        lcd.fillRoundRect(SKY_X, SKY_Y, SKY_W, SKY_H, 3, lcd.color565(0x34, 0x4F, 0x65));  // (U4) #344F65
        drawCloudSmall(SKY_X + 5, SKY_Y + 7);
        drawCloudSmall(SKY_X + 50, SKY_Y + 16);
        drawSun(BODY_CX, BODY_CY);
    } else {
        lcd.fillRoundRect(SKY_X, SKY_Y, SKY_W, SKY_H, 3, lcd.color565(8, 12, 40));
        const uint16_t star = lcd.color565(255, 255, 200);
        lcd.drawPixel(SKY_X + 7, SKY_Y + 5, star);
        lcd.drawPixel(SKY_X + 52, SKY_Y + 8, star);
        lcd.drawPixel(SKY_X + 12, SKY_Y + 18, star);
        lcd.drawFastHLine(SKY_X + 51, SKY_Y + 18, 3, star);  // one small twinkle
        lcd.drawFastVLine(SKY_X + 52, SKY_Y + 17, 3, star);
        drawFacedMoon(BODY_CX, BODY_CY, moonPhase);
    }
}

struct ClockView {
    int minuteKey = -1;  // yyyymmddhhmm folded, or -2 for "not synced"
    int colon = -1;
    int skyKey = -1;     // (3-3) day/night and moon phase as drawn
} clockView;

static SkyState skyNow = SKY_UNKNOWN;
static uint32_t skyCheckedMs = 0;
static int skyLocation = -1;
static constexpr uint32_t SKY_CHECK_MS = 60000;  // the Sun's altitude is checked once a minute

static void updateSky(time_t now)
{
    uint32_t ms = millis();
    if (skyLocation == curLocation && skyNow != SKY_UNKNOWN && ms - skyCheckedMs < SKY_CHECK_MS) return;
    skyCheckedMs = ms;
    skyLocation = curLocation;
    float lat, lon;
    skyPlace(curLocation, lat, lon);
    float alt = solarAltitudeDeg(now, lat, lon);
    SkyState s = alt > SUN_HORIZON_DEG ? SKY_DAY : SKY_NIGHT;
    if (s != skyNow) Serial.printf("[SKY] %s: sun %.1f deg -> %s\n", LOCATIONS[curLocation].name, alt, s == SKY_DAY ? "day" : "night");
    skyNow = s;
}

static void drawClock(bool full)
{
    if (full) clockView = ClockView();
    char line[40];
    if (!clockValid()) {
        if (clockView.minuteKey != -2) {
            clockView.minuteKey = -2;
            drawText(CLOCK_X, Y_FREQ, 1, COL_DIM, "----.--.--", 62);
            drawText(CLOCK_X, Y_FREQ + 9, 2, COL_DIM, "--:--", 62);
            skyNow = SKY_UNKNOWN;
            drawSkyWindow(SKY_UNKNOWN, 0.f);
            clockView.skyKey = -1;
        }
        return;
    }
    time_t now = time(nullptr);
    struct tm lt;
    localtime_r(&now, &lt);  // (U7) the Location's local time
    int key = (((lt.tm_year * 13 + lt.tm_mon) * 32 + lt.tm_mday) * 24 + lt.tm_hour) * 60 + lt.tm_min;
    if (key != clockView.minuteKey) {
        clockView.minuteKey = key;
        snprintf(line, sizeof(line), "%04d.%02d.%02d", lt.tm_year + 1900, lt.tm_mon + 1, lt.tm_mday);
        drawText(CLOCK_X, Y_FREQ, 1, COL_TEXT, line, 62);
        snprintf(line, sizeof(line), "%02d %02d", lt.tm_hour, lt.tm_min);
        drawText(CLOCK_X, Y_FREQ + 9, 2, COL_AMBER, line, 62);
        clockView.colon = -1;
    }
    int colon = lt.tm_sec & 1;  // the colon blinks once a second
    if (colon != clockView.colon) {
        clockView.colon = colon;
        drawText(CLOCK_X + 24, Y_FREQ + 9, 2, colon ? COL_AMBER : COL_BG, ":");
    }
    // (3-3) The window is redrawn only when day/night changes or the moon moves by 1/64.
    updateSky(now);
    float phase = moonPhase01(now);
    int skyKey = skyNow == SKY_DAY ? 1000 : ((int)(phase * 64.f) & 63);
    if (skyKey != clockView.skyKey) {
        clockView.skyKey = skyKey;
        drawSkyWindow(skyNow, phase);
    }
}

// --- (2-4) 8-segment level meter, left of the frequency
// Bottom to top: 4 green, 2 yellow, 2 red. Each segment is one solid rectangle; a dark
// version keeps its shape when off. Thresholds are the RMS of the decoded broadcast in
// dBFS (before the speaker volume, like a line-level meter on a tuner).
static constexpr float METER_DB[METER_SEGMENTS] = {-36.f, -30.f, -25.f, -21.f, -17.f, -14.f, -11.f, -8.f};
static constexpr uint32_t METER_TICK_MS = 33;           // about 30 updates a second
static constexpr float METER_RELEASE_PER_TICK = 0.25f;  // falls 8 segments in about 1 s; rises at once

static uint16_t meterThreshold[METER_SEGMENTS];  // linear RMS, from METER_DB at start
static float meterShown = 0.f;
static int meterLit = -1;  // segments drawn lit; -1 = nothing drawn yet
static uint32_t meterLastMs = 0;

static void meterInit()
{
    for (int i = 0; i < METER_SEGMENTS; i++)
        meterThreshold[i] = (uint16_t)lroundf(32768.f * powf(10.f, METER_DB[i] / 20.f));
}

static int meterSegmentsFor(uint16_t rms)
{
    int n = 0;
    while (n < METER_SEGMENTS && rms >= meterThreshold[n]) n++;
    return n;
}

static uint16_t meterColour(int seg, bool on)
{
    if (seg < 4) return on ? lcd.color565(0, 230, 0) : lcd.color565(0, 48, 0);       // green
    if (seg < 6) return on ? lcd.color565(240, 220, 0) : lcd.color565(56, 50, 0);    // yellow
    return on ? lcd.color565(240, 0, 0) : lcd.color565(56, 0, 0);                    // red
}

static void drawMeterSegment(int seg, bool on)
{
    int y = METER_BOTTOM - (seg + 1) * METER_H - seg * METER_GAP;
    lcd.fillRect(METER_X, y, METER_W, METER_H, meterColour(seg, on));
}

static void drawMeter(int lit)
{
    for (int i = 0; i < METER_SEGMENTS; i++) {
        bool on = i < lit;
        bool wasOn = i < meterLit;
        if (meterLit < 0 || on != wasOn) drawMeterSegment(i, on);  // only what changed
    }
    meterLit = lit;
}

// Lit only for broadcast audio that is actually playing on the station under the dial.
static bool meterActive()
{
    int st = stationAtFreq(dialFreq);
    return screen == SC_RADIO && radioState.load() == RS_PLAYING && meterFromStream.load() && !muted &&
           st >= 0 && currentStation.load() == st && !tunePending && stationRequest.load() == REQ_NONE;
}

static void meterService()
{
    uint32_t now = millis();
    if (now - meterLastMs < METER_TICK_MS) return;
    meterLastMs = now;
    uint16_t rms = meterRmsMax.exchange(0);
    if (!meterActive()) {
        meterShown = 0.f;  // dark at once (noise, connecting, buffering, failing, muted, tuning)
    } else {
        float target = (float)meterSegmentsFor(rms);
        if (target >= meterShown) meterShown = target;  // fast rise
        else meterShown = fmaxf(target, meterShown - METER_RELEASE_PER_TICK);  // slower fall
    }
    int lit = (int)meterShown;
    if (lit != meterLit) drawMeter(lit);
}

// (U2a4) The footer's Wi-Fi word: "unset" with no saved network, "off" when started with U
// (U2a3 showed "retry" in both cases), else as before.
static void wifiLabel(char* out, size_t size, int rssi)
{
    if (!haveNetwork) strlcpy(out, "unset", size);
    else if (usbOnly) strlcpy(out, "off", size);
    else if (wifiState == WS_CONNECTED) snprintf(out, size, "%ddBm", rssi);
    else strlcpy(out, wifiState == WS_CONNECTING || wifiState == WS_SCANNING ? "joining" : "retry", size);
}

// (U4) Only the USB relay can play (started with U, or no saved Wi-Fi) and it has not
// answered yet: the status says USB RELAY REQUIRED and the name row how to start it.
// It goes by itself when the relay answers.
static bool relayHintNow(uint8_t rs)
{
    return !usbLink.ready() && (usbOnly || !haveNetwork) && (rs == RS_WAIT_WIFI || rs == RS_OFF);
}

// (U5) The footer's middle field: x 50..163 (U4: 46..163, so a 7-letter Location such as
// HUNGARY touched "WiFi"), at most 19 characters so it never reaches "A:MODE L:LOC" at
// x 166. Without the U135 its warning takes the place of the free memory.
static constexpr int FOOT_MID_X = 50, FOOT_MID_W = 114, FOOT_MID_CHARS = FOOT_MID_W / 6;
static void footerMiddle(char* out, size_t size, bool usb, const char* wifi, uint32_t heapKb, bool u135)
{
    char tail[12];
    if (u135) snprintf(tail, sizeof(tail), "%luK", (unsigned long)heapKb);
    else strlcpy(tail, "U135--", sizeof(tail));
    if (usb) snprintf(out, size, "USB relay %s", tail);
    else snprintf(out, size, "WiFi %s %s", wifi, tail);
    if (size > (size_t)FOOT_MID_CHARS) out[FOOT_MID_CHARS] = '\0';
}

// (U9) The dial position "n/N" for the station under the dial (st: the name row's value,
// -1 for none), or "" for none. Not a ranking: the place in the Location's dial order.
// Up to 99/99 fits the 32 px between the status (cleared x 4..153) and the volume (x 186..).
static void dialPositionText(char* out, size_t size, int st, int count)
{
    if (st >= 0 && st < count && count <= 99) snprintf(out, size, "%d/%d", st + 1, count);
    else if (size > 0) out[0] = '\0';
}
static constexpr int POS_X = 154, POS_W = 32;  // (U9) the gap in the status row (cleared for n/N)
static constexpr int POS_END = 181;  // (U9) n/N ends here, 5 px before the volume; 5 characters start at POS_X
static inline int dialPositionX(int len) { return POS_END - 6 * len < POS_X ? POS_X : POS_END - 6 * len; }

static void drawRadio(bool full)
{
    if (full) {
        lcd.fillScreen(COL_BG);
        drawText(4, 2, 2, COL_AMBER, "RADIO DAYS");
        view = RadioView();
        dialView = DialView();  // (U5) the dial and its needle are drawn whole
        meterLit = -1;  // (2-4) draw every segment again
        meterShown = 0.f;
        drawMeter(0);
        drawClock(true);  // (2-5)
        drawTowers();     // (U10) the waves follow below, from the status
    }
    char line[64];

    if (tuneMode != view.mode) {
        view.mode = tuneMode;
        bool manual = tuneMode == MODE_MANUAL;
        drawText(156, 2, 2, manual ? COL_WARN : COL_OK, manual ? "MANUAL" : "  AUTO", 84);
    }

    drawClock(false);  // (2-5) only what changed: the colon each second, digits each minute

    if (dialFreq != view.freq) {
        view.freq = dialFreq;
        drawDial(dialFreq);  // (U5) the analog dial
    }

    uint8_t rs = radioState.load();
    bool relayHint = relayHintNow(rs);  // (U4)

    // Station name only when the dial is on a station's frequency.
    int st = dialGliding() ? -1 : stationAtFreq(dialFreq);  // (U8) no name while gliding
    if (st != view.nameStation || (int8_t)relayHint != view.relayHint) {
        view.nameStation = st;
        view.relayHint = relayHint;
        lcd.fillRect(0, Y_NAME, 240, 16, COL_BG);
        if (relayHint) {
            static const char HINT[] = "Start the relay on your computer.";
            drawText((240 - 6 * (int)(sizeof(HINT) - 1)) / 2, Y_NAME + 4, 1, COL_TEXT, HINT);
        } else if (st >= 0) {
            snprintf(line, sizeof(line), "%.19s", locStations()[st].name);
            drawText((240 - 12 * (int)strlen(line)) / 2, Y_NAME, 2, COL_TEXT, line);
        }
    }

    // (U9) Dial position n/N, from the same station as the name row (none while gliding,
    // between stations or with the relay hint). Read only: nothing here changes tuning.
    int pos = relayHint ? -1 : st;
    if (pos != view.posStation || locCount() != view.posCount) {
        view.posStation = pos;
        view.posCount = locCount();
        dialPositionText(line, sizeof(line), pos, locCount());
        lcd.fillRect(POS_X, Y_STATUS, POS_W, 8, COL_BG);
        if (line[0]) drawText(dialPositionX((int)strlen(line)), Y_STATUS, 1, COL_DIM, line);
    }

    // Status: off-station -> NO STATION; dial moving or request not yet taken -> TUNING...
    uint8_t shown;
    if (relayHint) {
        shown = 0xFD;  // (U4) USB RELAY REQUIRED
    } else if (dialGliding()) {
        shown = 0xFE;  // (U8) TUNING... while gliding
    } else if (st < 0) {
        shown = RS_NOISE;
    } else if (tunePending || stationRequest.load() != REQ_NONE ||
               (currentStation.load() != st && rs != RS_WAIT_WIFI && rs != RS_OFF)) {
        shown = 0xFE;  // tuning
    } else {
        shown = rs;
    }
    uint8_t bp = bufferPercent.load() / 10 * 10;
    if (shown != view.status || (shown == RS_BUFFERING && bp != view.buffer)) {
        view.status = shown;
        view.buffer = bp;
        uint16_t colour;
        if (shown == 0xFE) {
            colour = COL_AMBER;
            strlcpy(line, "TUNING...", sizeof(line));
        } else if (shown == 0xFD) {
            colour = COL_WARN;
            strlcpy(line, "USB RELAY REQUIRED", sizeof(line));
        } else {
            const char* text = radioStatusText(shown, colour);
            if (shown == RS_BUFFERING) snprintf(line, sizeof(line), "%s %u%%", text, bp);
            else strlcpy(line, text, sizeof(line));
        }
        drawText(4, Y_STATUS, 1, colour, line, 150);
    }

    // (U10) The towers' waves, from the status row's value (read only); a side is drawn
    // again only when its colour changes.
    uint16_t waveL, waveR;
    towerWaveColours(shown, millis(), waveL, waveR);
    if (waveL != view.waveL) { view.waveL = waveL; drawTowerWaves(TOWER_L_X, waveL); }
    if (waveR != view.waveR) { view.waveR = waveR; drawTowerWaves(TOWER_R_X, waveR); }

    if (volume != view.volume || muted != view.muted) {
        view.volume = volume;
        view.muted = muted;
        if (muted) snprintf(line, sizeof(line), "    MUTE");
        else snprintf(line, sizeof(line), "VOL %3d%%", volume * 100 / VOL_MAX);
        drawText(186, Y_STATUS, 1, muted ? COL_ERR : COL_TEXT, line, 54);
    }

    int rssi = (WiFi.status() == WL_CONNECTED) ? WiFi.RSSI() : 0;
    uint32_t heapKb = ESP.getFreeHeap() / 1024;
    bool usb = usbLink.ready();
    if (diagFooter) {
        // (U2a2) link state, receive queue, bytes in flight, underruns, lines not written,
        // plugged-signal drops, longest loop pause (totals since start).
        static uint32_t lastDiagMs = 0;
        if (millis() - lastDiagMs >= 1000 || view.wifi == 0xFF) {
            lastDiagMs = millis();
            view.wifi = 0xFE;
            int q = Serial.available();
            snprintf(line, sizeof(line), "L%u Q%dK F%luK UR%lu TX%lu PL%lu G%lu", (unsigned)usbLink.state(),
                     q > 0 ? q / 1024 : 0, (unsigned long)(usbLink.inFlight() / 1024), (unsigned long)underruns.load(),
                     (unsigned long)diagTotal.txFailed, (unsigned long)diagTotal.unpluggedSeen,
                     (unsigned long)diagTotal.maxGapMs);
            drawText(4, Y_FOOT, 1, COL_WARN, line, 236);
        }
    } else if (wifiState != view.wifi || rssi / 5 != view.rssi / 5 || heapKb != view.heapKb || u135Found != view.u135 ||
        usb != view.usb) {
        view.usb = usb;
        view.wifi = wifiState;
        view.rssi = rssi;
        view.heapKb = heapKb;
        view.u135 = u135Found;
        char wifi[16];
        wifiLabel(wifi, sizeof(wifi), rssi);
        drawText(4, Y_FOOT, 1, COL_AMBER, gLoc.load()->name, 42);  // (3-1) Location
        footerMiddle(line, sizeof(line), usb, wifi, heapKb, u135Found);
        drawText(FOOT_MID_X, Y_FOOT, 1, COL_DIM, line, FOOT_MID_W);
        drawText(166, Y_FOOT, 1, COL_DIM, "A:MODE L:LOC");
    }
}

// --- (3-1) Location screen
// Turning the knob (or , / keys) chooses a Location; push / ok switches to it; esc
// goes back without a change. The radio keeps playing until a different Location
// is confirmed. Push + turn still sets the volume.
static int locSel = 0;              // Location under the cursor
static bool locSwitching = false;   // waiting for the decode task to stop
static int locSwitchTarget = 0;
static uint32_t locSwitchSeq = 0;
static uint32_t locSwitchStartMs = 0;
static constexpr uint32_t LOC_SWITCH_TIMEOUT_MS = 10000;

// --- (3-2) Globe. Orthographic view from GLOBE_TILT_DEG north. For every pixel the
// latitude (map row) and the longitude relative to the centre meridian (map column)
// are computed once when the screen opens; turning the globe is then a table
// look-up of (relative column + centre column) per pixel. Only longitude turns.
static constexpr int GLOBE_R = 42;
static constexpr int GLOBE_D = 2 * GLOBE_R + 1;     // 85 x 85 globe
static constexpr int GLOBE_CW = 112;                 // (U2a2) canvas: the globe and the UFO's (U4: satellite's) path (x 4..115)
static constexpr int GLOBE_X = 4;                    // (U2a2) canvas top-left on screen (3-3: 12)
static constexpr int GLOBE_Y = 27;
static constexpr float GLOBE_TILT_DEG = 25.f;
static constexpr uint32_t GLOBE_FRAME_MS = 33;       // at most ~30 frames a second, only while turning
static constexpr float WORLD_SPIN_DEG = 1.0f;        // per frame on WORLD (~30 degrees a second)

static M5Canvas* globeSprite = nullptr;
static uint8_t* globeRow = nullptr;  // per pixel: map row 0..127, 255 = outside the globe
static uint8_t* globeCol = nullptr;  // per pixel: column relative to the centre meridian (0..255)
static uint8_t* globeDepth = nullptr;  // per pixel: 0..2, shading towards the rim
static bool globeReady = false;
static float globeCenterLon = 0.f;
static float globeTargetLon = 0.f;
static bool globeDirty = true;
static uint32_t globeLastMs = 0;

// --- (U4) Satellite by the globe. It replaces the U2a3 UFO and keeps all of its code:
// the same 24 x 12 dot picture size, home point, drift, speed swell and pauses, the same
// talk (cyan wave fronts Earth -> satellite, a short blink of its lamps, yellow ones
// satellite -> Earth: one direction at a time) and the same frame rule (a new globe
// frame only when the pose changes). Only the picture and its colours are new: a small
// gold body with a dish towards the Earth, two blue solar panels on struts, an antenna
// lamp, and three lamps on the body that chase as the UFO's rim lamps did. The UFO_ /
// ufo names are kept so the code stays U2a3's. Decoration only (not the real link).
// Only drawn when the globe is (the text-list fallback has none).
static constexpr int UFO_W = 24;
static constexpr int UFO_H = 12;
static constexpr int UFO_LAMP_ROW = 5;  // the chasing lamps (the UFO's rim was row 6)
static const char* const UFO_ART[UFO_H] = {
    "...........y............",
    "...........k............",
    "kkkkkkk..kkkkkk..kkkkkkk",
    "kbbkbbk..kssssk..kbbkbbk",
    "kbbkbbkggkwssskggkbbkbbk",
    "kbbkbbk..kyrysk..kbbkbbk",
    "kkkkkkk..kssssk..kkkkkkk",
    ".........kkkkkk.........",
    "...........k............",
    ".........kdddk..........",
    "..........kkk...........",
    "........................",
};
static constexpr int UFO_HOME_X = 93;            // centre in the globe canvas (screen x 97, y 37);
static constexpr int UFO_HOME_Y = 10;            // all of it stays clear of the disc within the drift
static constexpr uint32_t UFO_LAMP_MS = 400;     // the rim lamps chase slowly
static constexpr uint32_t TALK_FIRST_MS = 8000;  // after the screen opens
static constexpr uint32_t TALK_EVERY_MS = 15000; // then every 15 s + 0..10 s
static constexpr uint32_t TALK_EXTRA_MS = 10000;
static constexpr uint32_t TALK_DOWN_MS = 1300;   // Earth -> UFO, cyan
static constexpr uint32_t TALK_BLINK_MS = 600;   // the lamps blink
static constexpr uint32_t TALK_UP_MS = 1300;     // UFO -> Earth, yellow
static constexpr uint32_t TALK_FRAME_MS = 40;    // wave fronts move in steps of this

static float ufoPhase = 0.f;      // drift time (ms), advanced at a varying speed
static uint32_t ufoLastMs = 0;
static uint32_t ufoTalkAt = 0;     // next talk starts
static uint32_t ufoTalkStart = 0;
static bool ufoTalking = false;
static uint32_t ufoRng = 0x1234567u;
static UfoPose ufoDrawn;

static void ufoReset(uint32_t now)
{
    ufoLastMs = now;
    ufoTalking = false;
    ufoTalkAt = now + TALK_FIRST_MS;
    ufoDrawn = UfoPose();
}

static UfoPose ufoPoseAt(uint32_t now)
{
    const float TAU = 2.f * (float)M_PI;
    uint32_t dt = now - ufoLastMs;
    ufoLastMs = now;
    if (dt > 100) dt = 100;
    // Speed: a slow swell (19 s), and a gentle stop for a moment every 23 s.
    float rate = 0.35f + 0.65f * (0.5f + 0.5f * sinf(TAU * (float)(now % 19000) / 19000.f));
    uint32_t pm = now % 23000;
    if (pm < 3000) rate *= 0.5f + 0.5f * cosf(TAU * (float)pm / 3000.f);  // 1 -> 0 -> 1
    ufoPhase += (float)dt * rate;
    if (ufoPhase > 1.0e7f) ufoPhase -= 1.0e7f;  // keeps float precision; any jump is far apart

    UfoPose p;
    p.shown = true;
    float dx = 4.2f * sinf(TAU * ufoPhase / 6100.f) + 1.2f * sinf(TAU * ufoPhase / 2300.f + 0.7f);
    float dy = 2.0f * sinf(TAU * ufoPhase / 3700.f + 1.3f) + 0.7f * sinf(TAU * ufoPhase / 1900.f);
    p.x = (int)lroundf(UFO_HOME_X + dx) - UFO_W / 2;  // |dx| <= 5.4, |dy| <= 2.7
    p.y = (int)lroundf(UFO_HOME_Y + dy) - UFO_H / 2;
    p.lamp = (uint8_t)((now / UFO_LAMP_MS) % 3);

    if (!ufoTalking && (int32_t)(now - ufoTalkAt) >= 0) {
        ufoTalking = true;
        ufoTalkStart = now;
    }
    if (ufoTalking) {
        uint32_t t = now - ufoTalkStart;
        if (t < TALK_DOWN_MS) {
            p.wave = 1;
            p.waveStep = (uint8_t)(t / TALK_FRAME_MS);
        } else if (t < TALK_DOWN_MS + TALK_BLINK_MS) {
            p.lampMode = ((t - TALK_DOWN_MS) / 100) % 2 ? 2 : 1;  // all on / all dim
        } else if (t < TALK_DOWN_MS + TALK_BLINK_MS + TALK_UP_MS) {
            p.wave = 2;
            p.waveStep = (uint8_t)((t - TALK_DOWN_MS - TALK_BLINK_MS) / TALK_FRAME_MS);
        } else {
            ufoTalking = false;
            ufoRng = ufoRng * 1664525u + 1013904223u;
            ufoTalkAt = now + TALK_EVERY_MS + (ufoRng >> 8) % TALK_EXTRA_MS;
        }
    }
    return p;
}

static inline bool overDisc(int x, int y)
{
    int dx = x - GLOBE_R, dy = y - GLOBE_R;
    return dx * dx + dy * dy <= (GLOBE_R + 1) * (GLOBE_R + 1);  // the disc and its thin atmosphere ring
}

// Wave fronts: up to three arcs of circles round the sender, moving towards the receiver.
// `kind` 1: from the Earth's edge to the satellite (cyan); 2: from it to the Earth (yellow).
static void talkDraw(uint8_t* buf, int w, int h, const UfoPose& p)
{
    if (!p.wave) return;
    float ux = p.x + UFO_W / 2.f, uy = p.y + UFO_H / 2.f;
    float cx = ux - GLOBE_R, cy = uy - GLOBE_R;
    float d = sqrtf(cx * cx + cy * cy);
    // The Earth's waves spread from a point a little inside the disc (flat ripples; the parts
    // over the disc are not drawn); the UFO's from its centre.
    float ex = GLOBE_R + cx / d * (GLOBE_R - 10), ey = GLOBE_R + cy / d * (GLOBE_R - 10);
    float gap = sqrtf((ux - ex) * (ux - ex) + (uy - ey) * (uy - ey));
    float sx = p.wave == 1 ? ex : ux, sy = p.wave == 1 ? ey : uy;  // sender
    float base = p.wave == 1 ? atan2f(uy - ey, ux - ex) : atan2f(ey - uy, ex - ux);
    float r0 = p.wave == 1 ? 12.f : 7.f;         // the Earth's edge / the UFO's own half size
    float r1 = p.wave == 1 ? gap - 6.f : gap - 12.f;  // stop short of the receiver
    uint32_t dur = p.wave == 1 ? TALK_DOWN_MS : TALK_UP_MS;
    float progress = (float)p.waveStep * TALK_FRAME_MS / dur;
    for (int k = 0; k < 3; k++) {
        float r = r0 + progress * (r1 - r0 + 10.f) - 5.f * k;
        if (r < r0 || r > r1) continue;
        uint8_t col = p.wave == 1 ? (k == 0 ? lgfx::color332(90, 230, 255) : lgfx::color332(40, 150, 190))
                                  : (k == 0 ? lgfx::color332(255, 225, 60) : lgfx::color332(170, 140, 30));
        float half = p.wave == 1 ? 0.32f : 0.5f;  // +-18 / +-29 degrees
        int steps = (int)(r * 2.f * half) + 2;
        for (int i = 0; i <= steps; i++) {
            float a = base - half + 2.f * half * i / steps;
            int x = (int)lroundf(sx + r * cosf(a)), y = (int)lroundf(sy + r * sinf(a));
            if (x < 0 || y < 0 || x >= w || y >= h || overDisc(x, y)) continue;
            buf[y * w + x] = col;
        }
    }
}

// Into the canvas (8-bit), skipping pixels over the globe's disc and off the canvas.
static void ufoDraw(uint8_t* buf, int w, int h, const UfoPose& p)
{
    if (!p.shown) return;
    talkDraw(buf, w, h, p);  // under the UFO
    int lampIndex = 0;
    for (int ry = 0; ry < UFO_H; ry++) {
        for (int rx = 0; rx < UFO_W; rx++) {
            char c = UFO_ART[ry][rx];
            if (c == '.') continue;
            uint8_t col;
            switch (c) {
            case 'k': col = lgfx::color332(60, 60, 80); break;      // outline, panel grid
            case 'b': col = lgfx::color332(50, 90, 210); break;     // (U4) solar cells
            case 'd': col = lgfx::color332(200, 200, 210); break;   // (U4) dish
            case 'w': col = lgfx::color332(255, 255, 255); break;
            case 's': col = lgfx::color332(220, 180, 70); break;    // (U4) gold foil body
            case 'g': col = lgfx::color332(150, 150, 165); break;   // struts
            default: {  // 'y' / 'r' lamps; on the body (UFO_LAMP_ROW) they chase, or all blink while receiving
                bool lit;
                if (ry != UFO_LAMP_ROW) lit = true;
                else if (p.lampMode) lit = p.lampMode == 1;
                else lit = (lampIndex % 3) == p.lamp;
                if (ry == UFO_LAMP_ROW) lampIndex++;
                if (c == 'y') col = lit ? lgfx::color332(255, 230, 0) : lgfx::color332(110, 90, 0);
                else col = lit ? lgfx::color332(255, 60, 60) : lgfx::color332(110, 30, 30);
            }
            }
            int x = p.x + rx, y = p.y + ry;
            if (x < 0 || y < 0 || x >= w || y >= h || overDisc(x, y)) continue;
            buf[y * w + x] = col;
        }
    }
}

static void globeEnd()
{
    if (globeSprite) {
        globeSprite->deleteSprite();
        delete globeSprite;
        globeSprite = nullptr;
    }
    free(globeRow);
    free(globeCol);
    free(globeDepth);
    globeRow = globeCol = globeDepth = nullptr;
    globeReady = false;
}

static bool globeBegin()
{
    globeEnd();
    globeRow = (uint8_t*)malloc(GLOBE_D * GLOBE_D);
    globeCol = (uint8_t*)malloc(GLOBE_D * GLOBE_D);
    globeDepth = (uint8_t*)malloc(GLOBE_D * GLOBE_D);
    globeSprite = new (std::nothrow) M5Canvas(&lcd);
    if (!globeRow || !globeCol || !globeDepth || !globeSprite) {
        globeEnd();
        return false;
    }
    globeSprite->setColorDepth(8);
    if (!globeSprite->createSprite(GLOBE_CW, GLOBE_D)) {  // (U2a2) wider: room for the UFO
        globeEnd();
        return false;
    }
    const float tr = GLOBE_TILT_DEG * (float)M_PI / 180.f;
    const float ct = cosf(tr), st = sinf(tr);
    for (int py = 0; py < GLOBE_D; py++) {
        for (int px = 0; px < GLOBE_D; px++) {
            int i = py * GLOBE_D + px;
            float x = (float)(px - GLOBE_R) / GLOBE_R, y = (float)(GLOBE_R - py) / GLOBE_R;
            float d2 = x * x + y * y;
            if (d2 > 1.f) {
                globeRow[i] = 255;
                continue;
            }
            float z = sqrtf(1.f - d2);
            float yg = y * ct + z * st;   // view -> globe: turn about the x axis by the tilt
            float zg = -y * st + z * ct;
            float lat = asinf(fmaxf(-1.f, fminf(1.f, yg))) * 180.f / (float)M_PI;
            float lonRel = atan2f(x, zg) * 180.f / (float)M_PI;
            int row = (int)((90.f - lat) * GLOBE_MAP_H / 180.f);
            globeRow[i] = (uint8_t)constrain(row, 0, GLOBE_MAP_H - 1);
            globeCol[i] = (uint8_t)((int)lroundf(lonRel * GLOBE_MAP_W / 360.f) & (GLOBE_MAP_W - 1));
            globeDepth[i] = z > 0.55f ? 0 : (z > 0.25f ? 1 : 2);
        }
    }
    globeReady = true;
    globeDirty = true;
    Serial.printf("[GLOBE] ready, heap %u, largest %u\n", (unsigned)ESP.getFreeHeap(),
                  (unsigned)heap_caps_get_largest_free_block(MALLOC_CAP_8BIT));
    return true;
}

// Screen position of a place, or false when it is on the far side.
static bool globeProject(float lat, float lon, int& sx, int& sy)
{
    const float r = (float)M_PI / 180.f;
    const float tr = GLOBE_TILT_DEG * r;
    float dl = (lon - globeCenterLon) * r, la = lat * r;
    float xg = cosf(la) * sinf(dl), yg = sinf(la), zg = cosf(la) * cosf(dl);
    float y = yg * cosf(tr) - zg * sinf(tr);  // globe -> view
    float z = yg * sinf(tr) + zg * cosf(tr);
    if (z <= 0.05f) return false;
    sx = GLOBE_R + (int)lroundf(xg * GLOBE_R);
    sy = GLOBE_R - (int)lroundf(y * GLOBE_R);
    return true;
}

static void globeRender(int selected)
{
    static const uint8_t SEA[3] = {
        (uint8_t)lgfx::color332(30, 110, 230), (uint8_t)lgfx::color332(20, 70, 180), (uint8_t)lgfx::color332(10, 40, 110)};
    static const uint8_t LAND[3] = {
        (uint8_t)lgfx::color332(60, 200, 60), (uint8_t)lgfx::color332(40, 150, 40), (uint8_t)lgfx::color332(20, 90, 20)};
    uint8_t* buf = (uint8_t*)globeSprite->getBuffer();
    int centerCol = (int)lroundf((globeCenterLon + 180.f) * GLOBE_MAP_W / 360.f);
    memset(buf, 0, GLOBE_CW * GLOBE_D);  // (U2a2) the canvas is wider than the globe
    for (int i = 0; i < GLOBE_D * GLOBE_D; i++) {
        uint8_t row = globeRow[i];
        if (row == 255) continue;
        int col = (centerCol + globeCol[i]) & (GLOBE_MAP_W - 1);
        bool land = (GLOBE_LAND[row][col >> 3] >> (7 - (col & 7))) & 1;
        buf[(i / GLOBE_D) * GLOBE_CW + i % GLOBE_D] = land ? LAND[globeDepth[i]] : SEA[globeDepth[i]];
    }
    globeSprite->drawCircle(GLOBE_R, GLOBE_R, GLOBE_R, lgfx::color332(90, 140, 220));  // thin atmosphere
    for (int i = 0; i < LOCATION_COUNT; i++) {
        if (!LOCATIONS[i].hasMarker) continue;
        int sx, sy;
        if (!globeProject(LOCATIONS[i].lat, LOCATIONS[i].lon, sx, sy)) continue;
        if (i == selected) {
            globeSprite->fillCircle(sx, sy, 3, lgfx::color332(255, 255, 255));
            globeSprite->fillCircle(sx, sy, 2, lgfx::color332(255, 0, 0));
        } else {
            globeSprite->fillRect(sx - 1, sy - 1, 2, 2, lgfx::color332(255, 220, 0));
        }
    }
    ufoDraw(buf, GLOBE_CW, GLOBE_D, ufoDrawn);  // (U2a2) last: over nothing but space
    globeSprite->pushSprite(&lcd, GLOBE_X, GLOBE_Y);
}

static void globeAim(int index, bool immediately)
{
    const Location& L = LOCATIONS[index];
    if (L.hasMarker) globeTargetLon = L.lon;
    if (immediately && L.hasMarker) globeCenterLon = L.lon;
    globeDirty = true;
}

// Turns the globe towards the target (shortest way, easing), or spins on WORLD.
static void globeService(int selected)
{
    if (!globeReady) return;
    uint32_t now = millis();
    if (now - globeLastMs < GLOBE_FRAME_MS) return;
    globeLastMs = now;
    bool moved = false;
    if (!LOCATIONS[selected].hasMarker) {
        globeCenterLon += WORLD_SPIN_DEG;
        moved = true;
    } else {
        float diff = fmodf(globeTargetLon - globeCenterLon + 540.f, 360.f) - 180.f;  // -180..180
        if (fabsf(diff) > 0.3f) {
            float step = diff * 0.2f;
            if (fabsf(step) < 0.6f) step = diff > 0 ? fminf(0.6f, diff) : fmaxf(-0.6f, diff);
            globeCenterLon += step;
            moved = true;
        } else if (globeCenterLon != globeTargetLon) {
            globeCenterLon = globeTargetLon;
            moved = true;
        }
    }
    if (globeCenterLon > 180.f) globeCenterLon -= 360.f;
    if (globeCenterLon < -180.f) globeCenterLon += 360.f;
    UfoPose ufo = ufoPoseAt(now);  // (U2a2) a new frame also when only the UFO moved
    if (moved || globeDirty || ufo != ufoDrawn) {
        globeDirty = false;
        ufoDrawn = ufo;
        globeRender(selected);
    }
}

static void openLocationScreen()
{
    locSel = curLocation;
    globeBegin();  // (3-2) false: the text list is shown instead; (U2a2) the UFO is in the globe
    ufoReset(millis());  // (U2a3) first talk a few seconds after opening
    globeAim(locSel, true);
    setScreen(SC_LOCATION);
}

static void drawLocationInfo()
{
    // Right panel: the Location under the cursor.
    const Location& L = LOCATIONS[locSel];
    // (U2a2) The panel starts right of the globe canvas (x 4..115); "(now)" is back after the name.
    lcd.fillRect(116, 28, 124, 66, COL_BG);
    drawText(116, 30, 2, COL_AMBER, L.name);
    if (locSel == curLocation) drawText(116 + 12 * (int)strlen(L.name) + 4, 34, 1, COL_DIM, "(now)");
    drawText(116, 50, 1, COL_TEXT, L.city);
    char line[40];
    snprintf(line, sizeof(line), "%d stations", L.count);
    drawText(116, 62, 1, COL_DIM, line);
    snprintf(line, sizeof(line), "%.20s", L.stations[0].name);
    drawText(116, 74, 1, COL_DIM, line);
    if (L.count > 1) {
        snprintf(line, sizeof(line), "%.17s ...", L.stations[1].name);
        drawText(116, 84, 1, COL_DIM, line);
    }
}

// (U4) Five rows a page; the page holding the cursor is shown, so it is always visible.
static constexpr int LOC_LIST_ROWS = 5;
static void drawLocationList()
{
    // Left panel (Stage 3-2 shows the globe here).
    lcd.fillRect(0, 28, 116, 80, COL_BG);
    int first = locSel / LOC_LIST_ROWS * LOC_LIST_ROWS;
    for (int i = first; i < first + LOC_LIST_ROWS && i < LOCATION_COUNT; i++) {
        char line[24];
        snprintf(line, sizeof(line), "%c%s%s", i == locSel ? '>' : ' ', LOCATIONS[i].name, i == curLocation ? "*" : "");
        drawText(4, 30 + (i - first) * 14, i == locSel ? 2 : 1, i == locSel ? COL_AMBER : COL_TEXT, line);
    }
}

static void drawLocation(bool full)
{
    if (full) {
        lcd.fillScreen(COL_BG);
        drawHeader("LOCATION");
        drawText(4, 124, 1, COL_DIM, "turn:choose  push/ok:listen  esc:back");
        globeDirty = true;
    }
    if (locSwitching) {
        lcd.fillRect(0, 28, 240, 80, COL_BG);
        char line[40];
        snprintf(line, sizeof(line), "TUNING TO %s ...", LOCATIONS[locSwitchTarget].name);
        drawText(8, 60, 1, COL_WARN, line);
        return;
    }
    if (globeReady) globeService(locSel);  // (3-2) the globe draws itself; dirty -> drawn at once
    else drawLocationList();
    drawLocationInfo();
}

// Called once the decode task has stopped (or after the timeout; the tables are static).
static void applyLocation(int index)
{
    gLoc = &LOCATIONS[index];
    curLocation = index;
    dialFreq = locStations()[0].freq;  // the first station of the Location
    glideStop();  // (U8)
    tunePending = false;
    sentTarget = 0;
    stationRequest = 0;
    seenAutoMove = autoMoveSeq.load();
    radioEnabled = true;
    SaveResult r = nvsWriteLocation(index, "location-confirmed");  // the result goes to the serial log
    Serial.printf("[LOC] now %s (%d stations), save result %d\n", LOCATIONS[index].name, locCount(), (int)r);
    clockZone();  // (U7) the clock and date in the new city's time; drawn whole with the radio screen
    Serial.printf("[CLOCK] zone %s\n", LOCATIONS[index].tz);
}

static void locationService()
{
    if (!locSwitching) return;
    bool idle = decodeOffSeq.load() != locSwitchSeq && radioState.load() == RS_OFF;
    bool timeout = millis() - locSwitchStartMs >= LOC_SWITCH_TIMEOUT_MS;
    if (!idle && !timeout) return;
    if (!idle) Serial.println("[LOC] decode task still busy; switching anyway (tables are static)");
    locSwitching = false;
    applyLocation(locSwitchTarget);
    setScreen(SC_RADIO);
}

static void confirmLocation()
{
    if (locSel == curLocation) {  // same Location: nothing to switch
        setScreen(SC_RADIO);
        return;
    }
    locSwitchTarget = locSel;
    radioEnabled = false;  // the decode task closes the stream / stops the noise
    locSwitchSeq = decodeOffSeq.load();
    locSwitchStartMs = millis();
    locSwitching = true;
    screenDirty = true;
    Serial.printf("[LOC] switching to %s\n", LOCATIONS[locSel].name);
}

static void handleLocation(const KeyInput& k, const KnobInput& kn)
{
    if (kn.volumeDetents) changeVolume(kn.volumeDetents);
    if (locSwitching) return;
    if (hasChar(k, '`')) {
        setScreen(SC_RADIO);
        return;
    }
    int move = kn.tuneDetents + (hasChar(k, '/') ? 1 : 0) - (hasChar(k, ',') ? 1 : 0) +
               (hasChar(k, '.') ? 1 : 0) - (hasChar(k, ';') ? 1 : 0);
    if (move) {
        locSel = ((locSel + move) % LOCATION_COUNT + LOCATION_COUNT) % LOCATION_COUNT;
        if (globeReady) globeAim(locSel, false);  // (3-2) the globe turns there
        else drawLocationList();
        drawLocationInfo();
    }
    if (k.enter || kn.shortPress) confirmLocation();
}

// --- Wi-Fi setup
struct NetEntry {
    String ssid;
    int rssi;
    bool open;
};
static NetEntry nets[WIFI_LIST_MAX];
static int netCount = 0;
static int netSel = 0;
static bool scanning = false;
static String joinSsid, joinPass;
static bool showPass = false;
static uint32_t joinStartMs = 0;
static String resultText;
static bool resultOk = false;
static bool askOverwrite = false;
static bool askReplace = false;  // (U4) 3 networks saved: which one gives way
static int replaceSel = 0;
static int savedSel = 0;         // (U4) saved-networks screen
static bool askDelete = false;

static void startScan()
{
    usbOnly = false;       // (U2a4) the setup was asked for: Wi-Fi may be used again
    radioEnabled = false;  // the decode task stops the stream
    // (U4) a background search stops here (a scan it left running is picked up below)
    if (wifiState == WS_CONNECTING || wifiState == WS_SCANNING || wifiState == WS_WAIT_RETRY) wifiState = WS_OFF;
    wifiInit();
    netCount = 0;
    netSel = 0;
    scanning = WiFi.scanNetworks(true /* async */, false /* no hidden */) == WIFI_SCAN_RUNNING;
    Serial.printf("[WIFI] scan %s\n", scanning ? "started" : "failed to start");
    setScreen(SC_WIFI_LIST);
}

static void collectScan()
{
    int n = WiFi.scanComplete();
    if (n == WIFI_SCAN_RUNNING) return;
    scanning = false;
    netCount = 0;
    for (int i = 0; i < n && netCount < WIFI_LIST_MAX; i++) {
        String s = WiFi.SSID(i);
        if (s.length() == 0) continue;
        bool dup = false;
        for (int j = 0; j < netCount; j++) {
            if (nets[j].ssid == s) {
                dup = true;
                if (WiFi.RSSI(i) > nets[j].rssi) nets[j].rssi = WiFi.RSSI(i);
            }
        }
        if (dup) continue;
        nets[netCount++] = {s, WiFi.RSSI(i), WiFi.encryptionType(i) == WIFI_AUTH_OPEN};
    }
    WiFi.scanDelete();
    // strongest first
    for (int i = 1; i < netCount; i++)
        for (int j = i; j > 0 && nets[j].rssi > nets[j - 1].rssi; j--) std::swap(nets[j], nets[j - 1]);
    Serial.printf("[WIFI] scan done: %d networks\n", netCount);
    screenDirty = true;
}

static void leaveSetup()
{
    // Back to the radio with the network that was active before (if any).
    if (haveNetwork) {
        if (WiFi.status() != WL_CONNECTED || WiFi.SSID() != activeSsid) wifiConnectActive();
        radioEnabled = true;
        setScreen(SC_RADIO);
    } else {
        setScreen(SC_START);
    }
}

static void startJoin()
{
    wifiBegin(joinSsid, joinPass);
    joinStartMs = millis();
    setScreen(SC_WIFI_JOIN);
}

// (U4) Saves the joined network: its own entry if saved (new password), a free entry, or
// the entry `replace` (0..2) when all 3 are used; -1: not saved. It becomes "last".
static SaveResult saveJoined(int replace, bool allowOverwriteDamaged)
{
    WifiListRecord next = savedList;
    int i = savedIndexOf(joinSsid.c_str());
    if (i < 0) i = next.count < WIFI_SAVED_MAX ? next.count++ : replace;
    if (i < 0) return SAVE_UNCHANGED;
    strlcpy(next.net[i].ssid, joinSsid.c_str(), sizeof(next.net[i].ssid));
    memset(next.net[i].pass, 0, sizeof(next.net[i].pass));
    strlcpy(next.net[i].pass, joinPass.c_str(), sizeof(next.net[i].pass));
    next.last = (uint8_t)i;
    return nvsWriteWifiList(next, allowOverwriteDamaged, "wifi-setup-connected");
}

// replace: -1 ask when 3 are saved, 0..2 the entry that gives way, -2 do not save.
static bool joinAllowOverwrite = false;  // (U4) kept while asking which entry gives way
static void finishJoinSuccess(bool allowOverwriteDamaged, int replace = -1)
{
    bool full = savedIndexOf(joinSsid.c_str()) < 0 && savedList.count >= WIFI_SAVED_MAX;
    if (full && replace == -1) {  // (U4) ask which saved network gives way
        joinAllowOverwrite = allowOverwriteDamaged;
        askReplace = true;
        replaceSel = 0;
        resultOk = true;
        setScreen(SC_WIFI_RESULT);
        return;
    }
    askReplace = false;
    SaveResult r = (full && replace < 0) ? SAVE_UNCHANGED : saveJoined(replace, allowOverwriteDamaged);
    switch (r) {
    case SAVE_OK:            resultText = "Connected and saved."; break;
    case SAVE_UNCHANGED:     resultText = "Connected (already saved)."; break;
    case SAVE_REFUSED_NEWER: resultText = "Connected. Not saved: newer saved data."; break;
    case SAVE_REFUSED_ERR:   resultText = "Connected. Not saved (saved data unreadable)."; break;
    case SAVE_NO_SPACE:      resultText = "Connected. Not saved: NVS almost full."; break;
    default:                 resultText = "Connected. Saving failed."; break;
    }
    if (full && replace < 0) resultText = "Connected. Not saved (3 kept).";
    useSavedList();  // (U4) the saved list decides what the background search uses
    activeSsid = joinSsid;  // connected now (also when not saved)
    activePass = joinPass;
    haveNetwork = true;
    wifiState = WS_CONNECTED;
    wifiRetryMs = WIFI_RETRY_MIN_MS;
    resultOk = true;
    setScreen(SC_WIFI_RESULT);
}

static void drawWifiList()
{
    lcd.fillScreen(COL_BG);
    drawHeader("WI-FI SETUP");
    if (scanning) {
        drawText(4, 40, 1, COL_WARN, "Scanning...");
        return;
    }
    if (netCount == 0) {
        drawText(4, 40, 1, COL_WARN, "No networks found.");
        drawText(4, 110, 1, COL_DIM, "R: rescan  S: saved  esc(`): back");  // (U4) S
        return;
    }
    int first = max(0, min(netSel - 3, netCount - 7));
    for (int row = 0; row < 7 && first + row < netCount; row++) {
        int i = first + row;
        char line[48];
        snprintf(line, sizeof(line), "%c%c%-26.26s %4d%s", i == netSel ? '>' : ' ',
                 savedIndexOf(nets[i].ssid.c_str()) >= 0 ? '*' : ' ', nets[i].ssid.c_str(), nets[i].rssi,
                 nets[i].open ? " open" : "");  // (U4) * = saved
        drawText(4, 30 + row * 11, 1, i == netSel ? COL_AMBER : COL_TEXT, line);
    }
    drawText(4, 112, 1, COL_DIM, "knob/;/. move  ok/push: select");
    drawText(4, 123, 1, COL_DIM, "R: rescan  S: saved  esc(`): back");  // (U4) S
}

static void drawWifiPass()
{
    lcd.fillScreen(COL_BG);
    drawHeader("WI-FI SETUP");
    char line[64];
    snprintf(line, sizeof(line), "Network: %.30s", joinSsid.c_str());
    drawText(4, 32, 1, COL_TEXT, line);
    drawText(4, 48, 1, COL_DIM, "Password:");
    String shown = showPass ? joinPass : String();
    if (!showPass) for (size_t i = 0; i < joinPass.length(); i++) shown += '*';
    // last 38 characters
    if (shown.length() > 38) shown = shown.substring(shown.length() - 38);
    drawText(4, 60, 1, COL_AMBER, (shown + "_").c_str());
    snprintf(line, sizeof(line), "%u characters", (unsigned)joinPass.length());
    drawText(4, 74, 1, COL_DIM, line);
    drawText(4, 101, 1, COL_DIM, "ok: connect   del: delete");
    drawText(4, 112, 1, COL_DIM, "tab: show/hide password");
    drawText(4, 123, 1, COL_DIM, "Fn+esc(`): back to list");
}

static void drawWifiJoin()
{
    lcd.fillScreen(COL_BG);
    drawHeader("WI-FI SETUP");
    char line[64];
    snprintf(line, sizeof(line), "Connecting to %.24s", joinSsid.c_str());
    drawText(4, 40, 1, COL_WARN, line);
}

// (U4) The saved networks (at most 3): connect, or delete after Y / N.
static String savedMsg;
static void drawWifiSaved()
{
    lcd.fillScreen(COL_BG);
    drawHeader("WI-FI SETUP");
    drawText(4, 30, 1, COL_DIM, "Saved networks (* = tried first)");
    char line[48];
    if (savedList.count == 0) drawText(4, 46, 1, COL_WARN, "No saved networks.");
    for (int i = 0; i < savedList.count; i++) {
        snprintf(line, sizeof(line), "%c%c%.32s", i == savedSel ? '>' : ' ', i == savedList.last ? '*' : ' ', savedList.net[i].ssid);
        drawText(4, 46 + i * 12, 1, i == savedSel ? COL_AMBER : COL_TEXT, line);
    }
    if (askDelete) {
        snprintf(line, sizeof(line), "Delete \"%.24s\"? Y / N", savedList.net[savedSel].ssid);
        drawText(4, 86, 1, COL_WARN, line);
    } else if (savedMsg.length()) {
        drawText(4, 86, 1, COL_DIM, savedMsg.c_str());
    }
    drawText(4, 112, 1, COL_DIM, savedList.count ? "ok/push: connect   del: delete" : "");
    drawText(4, 123, 1, COL_DIM, "esc(`): back to the list");
}

static void drawWifiResult()
{
    lcd.fillScreen(COL_BG);
    drawHeader("WI-FI SETUP");
    if (askReplace) {  // (U4)
        drawText(4, 30, 1, COL_OK, "Connected. 3 networks are saved:");
        drawText(4, 42, 1, COL_TEXT, "which one gives way?");
        for (int i = 0; i < savedList.count; i++) {
            char line[48];
            snprintf(line, sizeof(line), "%c %.32s", i == replaceSel ? '>' : ' ', savedList.net[i].ssid);
            drawText(4, 58 + i * 12, 1, i == replaceSel ? COL_AMBER : COL_TEXT, line);
        }
        drawText(4, 101, 1, COL_DIM, "knob/;/. move  ok/push: replace");
        drawText(4, 112, 1, COL_DIM, "esc(`): use without saving");
        return;
    }
    if (askOverwrite) {
        drawText(4, 36, 1, COL_WARN, "Connected. The saved Wi-Fi data");
        drawText(4, 48, 1, COL_WARN, "is damaged. Replace it?");
        drawText(4, 110, 1, COL_DIM, "Y: replace   N: use without saving");
        return;
    }
    drawText(4, 40, 1, resultOk ? COL_OK : COL_ERR, resultText.c_str());
    drawText(4, 110, 1, COL_DIM, resultOk ? "Press a key or the knob" : "ok: try again   esc(`): list");
}

// ---------------------------------------------------------------- screen input handling
static void startRadio()
{
    radioEnabled = true;
    if (wifiState == WS_OFF || WiFi.status() != WL_CONNECTED) wifiConnectActive();
    setScreen(SC_RADIO);
}

// (U2a4) USB only for this start: the radio without any Wi-Fi attempt. The saved network
// is not touched; the next normal start uses it. Opening the setup (W) ends this.
static void startUsbOnly()
{
    usbOnly = true;
    radioEnabled = true;
    Serial.println("[START] USB only: no Wi-Fi in this start");
    setScreen(SC_RADIO);
}

static void handleStart(const KeyInput& k, const KnobInput& kn)
{
    if (hasCharI(k, 'w')) {
        startScan();
        return;
    }
    if (hasCharI(k, 'u')) {  // (U2a4)
        startUsbOnly();
        return;
    }
    if (anyKey(k) || kn.shortPress) {
        // (U2a4) With no saved network the radio still opens: it plays through the Mac
        // relay as soon as it answers (startRadio() tries Wi-Fi only with a saved network).
        startRadio();
    }
}

static void handleRadio(const KeyInput& k, const KnobInput& kn)
{
    if (kn.tuneDetents) tuneBy(kn.tuneDetents);
    if (kn.volumeDetents) changeVolume(kn.volumeDetents);
    if (kn.shortPress) toggleMute();
    if (hasChar(k, ';')) changeVolume(+1);
    if (hasChar(k, '.')) changeVolume(-1);
    if (hasChar(k, '/') || hasCharI(k, 'n')) tuneBy(+1);
    if (hasChar(k, ',') || hasCharI(k, 'p')) tuneBy(-1);
    if (hasCharI(k, 'a')) toggleTuneMode();
    if (hasCharI(k, 'm')) toggleMute();
    if (hasCharI(k, 'w')) startScan();
    if (hasCharI(k, 'l')) openLocationScreen();  // (3-1)
    if (hasCharI(k, 'i')) {                      // (U2a2) diagnostic footer on / off
        diagFooter = !diagFooter;
        view.wifi = 0xFF;  // redraw the footer
    }
}

static void handleWifiList(const KeyInput& k, const KnobInput& kn)
{
    if (scanning) {
        collectScan();
        if (hasChar(k, '`')) leaveSetup();
        return;
    }
    if (hasChar(k, '`')) {
        leaveSetup();
        return;
    }
    if (hasCharI(k, 'r')) {
        startScan();
        return;
    }
    if (hasCharI(k, 's')) {  // (U4) the saved networks
        savedSel = savedList.count ? savedList.last : 0;
        askDelete = false;
        savedMsg = "";
        setScreen(SC_WIFI_SAVED);
        return;
    }
    if (netCount == 0) return;
    int move = kn.detents + (hasChar(k, '.') ? 1 : 0) - (hasChar(k, ';') ? 1 : 0);
    if (move) {
        netSel = constrain(netSel + move, 0, netCount - 1);
        screenDirty = true;
    }
    if (k.enter || kn.shortPress) {
        joinSsid = nets[netSel].ssid;
        joinPass = "";
        showPass = false;
        if (nets[netSel].open) startJoin();
        else setScreen(SC_WIFI_PASS);
    }
}

static void handleWifiPass(const KeyInput& k, const KnobInput&)
{
    if (k.fn && hasChar(k, '`')) {
        setScreen(SC_WIFI_LIST);
        return;
    }
    bool changed = false;
    if (k.del && joinPass.length()) {
        joinPass.remove(joinPass.length() - 1);
        changed = true;
    }
    if (k.tab) {
        showPass = !showPass;
        changed = true;
    }
    if (!k.fn) {
        for (size_t i = 0; i < k.chars.length() && joinPass.length() < 63; i++) {
            char c = k.chars[i];
            if (c >= 0x20 && c < 0x7F) {
                joinPass += c;
                changed = true;
            }
        }
    }
    if (k.enter) {
        startJoin();
        return;
    }
    if (changed) screenDirty = true;
}

static void handleWifiJoin(const KeyInput& k, const KnobInput&)
{
    wl_status_t st = WiFi.status();
    if (st == WL_CONNECTED) {
        Serial.printf("[WIFI] joined \"%s\", IP %s\n", joinSsid.c_str(), WiFi.localIP().toString().c_str());
        if (nvState == NV_BAD || nvListState == NV_BAD) {  // (U4) either record
            askOverwrite = true;
            resultOk = true;
            setScreen(SC_WIFI_RESULT);
        } else {
            finishJoinSuccess(false);
        }
        return;
    }
    bool failed = (st == WL_CONNECT_FAILED || st == WL_NO_SSID_AVAIL) && millis() - joinStartMs > 3000;
    if (failed || millis() - joinStartMs >= WIFI_CONNECT_TIMEOUT_MS || hasChar(k, '`')) {
        WiFi.disconnect(false, false);
        resultOk = false;
        resultText = st == WL_NO_SSID_AVAIL ? "Network not found." : "Could not connect. Check the password.";
        Serial.printf("[WIFI] join failed (status %d); nothing saved\n", (int)st);
        setScreen(SC_WIFI_RESULT);
        return;
    }
    // progress dots
    static uint32_t lastDot = 0;
    if (millis() - lastDot > 500) {
        lastDot = millis();
        int dots = (int)((millis() - joinStartMs) / 500) % 30;
        lcd.fillRect(4 + dots * 6, 56, 6, 8, COL_WARN);
    }
}

// (U4) Deletes saved entry `i` (the order of the others is kept).
static void deleteSaved(int i)
{
    WifiListRecord next = savedList;
    String name = next.net[i].ssid;
    for (int j = i; j + 1 < next.count; j++) next.net[j] = next.net[j + 1];
    next.count--;
    memset(&next.net[next.count], 0, sizeof(next.net[0]));
    if (next.last > i) next.last--;
    else if (next.last == i || next.count == 0) next.last = 0;  // the first one is tried first now
    SaveResult r = nvsWriteWifiList(next, false, "wifi-deleted");  // savedList changes only when written
    if (r == SAVE_OK || r == SAVE_UNCHANGED) savedMsg = "Deleted: " + name.substring(0, 28);
    else savedMsg = r == SAVE_NO_SPACE ? "Not deleted: NVS almost full." : "Not deleted (saved data unreadable).";
    useSavedList();
    if (savedSel >= savedList.count) savedSel = savedList.count ? savedList.count - 1 : 0;
}

static void handleWifiSaved(const KeyInput& k, const KnobInput& kn)
{
    if (askDelete) {
        if (hasCharI(k, 'y')) {
            askDelete = false;
            deleteSaved(savedSel);
            screenDirty = true;
        } else if (hasCharI(k, 'n') || hasChar(k, '`')) {
            askDelete = false;
            screenDirty = true;
        }
        return;
    }
    if (hasChar(k, '`')) {
        setScreen(SC_WIFI_LIST);
        return;
    }
    if (savedList.count == 0) return;
    int move = kn.detents + (hasChar(k, '.') ? 1 : 0) - (hasChar(k, ';') ? 1 : 0);
    if (move) {
        savedSel = constrain(savedSel + move, 0, savedList.count - 1);
        savedMsg = "";
        screenDirty = true;
    }
    if (k.del) {
        askDelete = true;
        screenDirty = true;
    } else if (k.enter || kn.shortPress) {
        joinSsid = savedList.net[savedSel].ssid;
        joinPass = savedList.net[savedSel].pass;
        showPass = false;
        startJoin();
    }
}

static void handleWifiResult(const KeyInput& k, const KnobInput& kn)
{
    if (askReplace) {  // (U4)
        int move = kn.detents + (hasChar(k, '.') ? 1 : 0) - (hasChar(k, ';') ? 1 : 0);
        if (move) {
            replaceSel = constrain(replaceSel + move, 0, savedList.count - 1);
            screenDirty = true;
        }
        if (k.enter || kn.shortPress) finishJoinSuccess(joinAllowOverwrite, replaceSel);
        else if (hasChar(k, '`')) finishJoinSuccess(joinAllowOverwrite, -2);
        return;
    }
    if (askOverwrite) {
        if (hasCharI(k, 'y')) {
            askOverwrite = false;
            finishJoinSuccess(true);
        } else if (hasCharI(k, 'n')) {
            askOverwrite = false;
            finishJoinSuccess(false);  // refused inside: "not saved"
        }
        return;
    }
    if (resultOk) {
        if (anyKey(k) || kn.shortPress) {
            radioEnabled = true;
            setScreen(SC_RADIO);
        }
        return;
    }
    if (hasChar(k, '`')) {
        if (haveNetwork) wifiConnectActive();  // back to the saved network in the background
        setScreen(SC_WIFI_LIST);
    } else if (k.enter || kn.shortPress) {
        setScreen(SC_WIFI_PASS);
    }
}

// (U8) Steps the AUTO glide by the time passed (at most a few steps a call, so a late
// loop() costs a small jump, not a run), draws only the dial on the radio screen, and
// asks for the station on arrival (no 350 ms wait).
static void glideService()
{
    if (!glideActive) return;
    uint32_t now = millis();
    int moved = 0;
    while (glideLeft > 0 && now - glideLastMs >= GLIDE_STEP_MS && moved < 4) {
        glideLastMs += GLIDE_STEP_MS;
        dialFreq = locFreqMin() + floorMod(dialFreq - locFreqMin() + glideDir, dialRing());
        glideLeft--;
        moved++;
        if (glideLeft > 0 && stationAtFreq(dialFreq) >= 0) glideStations--;  // passed one: room for another detent
    }
    if (now - glideLastMs >= GLIDE_STEP_MS) glideLastMs = now;  // far behind: go on from now
    if (moved && screen == SC_RADIO && !screenDirty && dialView.drawn) drawDial(dialFreq);
    if (glideLeft > 0) return;
    glideActive = false;
    int st = stationAtFreq(dialFreq);
    Serial.printf("[TUNE] glide arrived at %d\n", st);
    if (st >= 0) sendTarget(st);
}

// ---------------------------------------------------------------- setup / loop
static uint32_t lastDrawMs = 0;
static uint32_t lastLogMs = 0;

void setup()
{
    // (U2a) The queues are made before begin(): the HWCDC driver drops what does not fit.
    Serial.setRxBufferSize(rdlink::RX_QUEUE_SIZE);
    Serial.setTxBufferSize(1024);  // room for a few lines at once (MORE, TUNE, the log)
    Serial.begin(115200);
    Serial.setTxTimeoutMs(0);  // never stall when no one reads the USB serial port
    delay(300);
    Serial.printf("[BOOT] RADIO DAYS %s\n", BUILD_TAG);

    // Same display-first start as Cardputer Media Jog. Only what RADIO DAYS uses is on.
    M5Cardputer.Display.begin();
    auto cfg = M5.config();
    cfg.clear_display = false;
    cfg.internal_imu = false;
    cfg.internal_rtc = false;
    cfg.internal_spk = true;
    cfg.internal_mic = false;
    cfg.external_imu = false;  // Port A (Grove) belongs to the U135 through Wire
    cfg.external_rtc = false;
    cfg.external_display_value = 0;
    cfg.external_speaker_value = 0;
    cfg.serial_baudrate = 0;   // Serial is already started
    M5Cardputer.begin(cfg, true);

    lcd.setRotation(1);
    lcd.setBrightness(128);
    lcd.setTextFont(1);
    lcd.fillScreen(COL_BG);

    logNvsStats("boot");
    nvsLoadWifi();  // reads only
    nvsLoadWifiList();  // (U4) reads only; without "wifis", the "wifi" network is entry 1 in RAM
    nvsLoadLocation();  // (3-1) reads only
    {
        int saved = savedLocationIndex();
        int idx = saved >= 0 ? saved : LOCATION_DEFAULT;
        gLoc = &LOCATIONS[idx];
        curLocation = idx;
        dialFreq = locStations()[0].freq;
        Serial.printf("[LOC] start in %s (%s)\n", LOCATIONS[idx].name, saved >= 0 ? "saved" : "default");
    }
    useSavedList();  // (U4)

    // Memory for the stream, taken before Wi-Fi so the blocks are contiguous.
    streamBuffer = malloc(STREAM_BUFFER_SIZE);
    codecBuffer = malloc(MP3_CODEC_SIZE);
    Serial.printf("[MEM] stream %s, codec %s, free %u, largest %u\n", streamBuffer ? "ok" : "FAILED",
                  codecBuffer ? "ok" : "FAILED", (unsigned)ESP.getFreeHeap(),
                  (unsigned)heap_caps_get_largest_free_block(MALLOC_CAP_8BIT));

    {
        auto spk = M5.Speaker.config();
        spk.sample_rate = 48000;           // as the M5Unified WebRadio example
        spk.task_pinned_core = APP_CPU_NUM;
        M5.Speaker.config(spk);
    }
    M5.Speaker.begin();
    applyVolume();  // VOL_START on every start
    out.setup();

    out.setMonitor(meterMonitor, nullptr);  // (2-4) level meter
    meterInit();

    Wire.begin(PORT_SDA, PORT_SCL, I2C_FREQ);
    u135Probe();

    clockInit();                  // (U3) the clock's time zone, whatever set it; (U7) the saved Location's
    usbLink.begin(esp_random());  // (U2a) ids start at a random value
    xTaskCreatePinnedToCore(decodeTask, "decode", 8192, nullptr, 1, nullptr, PRO_CPU_NUM);
    setScreen(SC_START);
}

static uint32_t loopGapMax = 0;  // (U2a2) longest pause between two loop() runs (ms)

void loop()
{
    {
        static uint32_t lastLoopMs = 0;
        uint32_t t = millis();
        if (lastLoopMs && t - lastLoopMs > loopGapMax) loopGapMax = t - lastLoopMs;
        lastLoopMs = t;
    }
    M5Cardputer.update();
    KeyInput k = keyboardPoll();
    KnobInput kn = u135Poll();

    if (screen != SC_WIFI_LIST && screen != SC_WIFI_PASS && screen != SC_WIFI_JOIN && screen != SC_WIFI_SAVED) wifiService();
    usbLink.service();  // (U2a) HELLO / PING / TUNE / credit; the only reader outside L3
    tuneService();
    glideService();  // (U8) the AUTO glide: a dial step every 20 ms while it runs
    clockService();  // (2-5) starts NTP once Wi-Fi is connected

    switch (screen) {
    case SC_START:       handleStart(k, kn); break;
    case SC_RADIO:       handleRadio(k, kn); break;
    case SC_WIFI_LIST:   handleWifiList(k, kn); break;
    case SC_WIFI_PASS:   handleWifiPass(k, kn); break;
    case SC_WIFI_JOIN:   handleWifiJoin(k, kn); break;
    case SC_WIFI_RESULT: handleWifiResult(k, kn); break;
    case SC_WIFI_SAVED:  handleWifiSaved(k, kn); break;  // (U4)
    case SC_LOCATION:    handleLocation(k, kn); break;
    }
    locationService();  // (3-1)
    if (screen == SC_LOCATION && !locSwitching) {
        globeService(locSel);  // (3-2)
    } else if (screen != SC_LOCATION) {
        if (globeReady) globeEnd();  // (3-2) free the globe
    }

    uint32_t now = millis();
    if (screenDirty) {
        screenDirty = false;
        switch (screen) {
        case SC_START:       drawStart(); break;
        case SC_RADIO:       drawRadio(true); break;
        case SC_WIFI_LIST:   drawWifiList(); break;
        case SC_WIFI_PASS:   drawWifiPass(); break;
        case SC_WIFI_JOIN:   drawWifiJoin(); break;
        case SC_WIFI_RESULT: drawWifiResult(); break;
        case SC_WIFI_SAVED:  drawWifiSaved(); break;  // (U4)
        case SC_LOCATION:    drawLocation(true); break;
        }
        lastDrawMs = now;
    } else if (screen == SC_START && usbLink.ready() != startUsbShown) {
        drawStartUsb();  // (U2a4) the relay answered (or went away)
    } else if (screen == SC_RADIO && now - lastDrawMs >= 100) {
        drawRadio(false);
        lastDrawMs = now;
    } else if (screen == SC_WIFI_LIST && scanning && now - lastDrawMs >= 500) {
        lastDrawMs = now;  // waiting for the scan; collectScan() marks the screen dirty
    }
    if (screen == SC_RADIO) meterService();  // (2-4) about 30 times a second; only changed segments are drawn

    // (2-3) The stream title has no place on the 2-3 screen (Stage 3); it goes to the serial log.
    static uint32_t loggedTitleSeq = 0;
    if (titleSeq.load() != loggedTitleSeq) {
        loggedTitleSeq = titleSeq.load();
        char title[96];
        getTitle(title, sizeof(title));
        if (title[0]) Serial.printf("[ICY] %s\n", title);
    }

    if (now - lastLogMs >= 10000) {
        lastLogMs = now;
        // (U2a2) What the link and the decoder saw in the last 10 s (docs/usb-u2a2.md, 4).
        rdlink::Diag d = usbLink.takeDiag();
        diagTotal.txFailed += d.txFailed;
        diagTotal.unpluggedSeen += d.unpluggedSeen;
        if (d.maxGapMs > diagTotal.maxGapMs) diagTotal.maxGapMs = d.maxGapMs;
        Serial.printf("[DIAG] link L%u q-max %lu in-flight %lu hello %lu tx-fail %lu credit-resent %lu unplugged %lu "
                      "to-L0 %lu gap-link %lu gap-loop %lu gap-decode %lu underruns %lu time-asked %lu time-got %lu\n",
                      (unsigned)usbLink.state(), (unsigned long)d.queueMax, (unsigned long)usbLink.inFlight(),
                      (unsigned long)d.helloSent, (unsigned long)d.txFailed, (unsigned long)d.creditResent,
                      (unsigned long)d.unpluggedSeen, (unsigned long)d.toL0, (unsigned long)d.maxGapMs,
                      (unsigned long)loopGapMax, (unsigned long)decodeGapMax.exchange(0), (unsigned long)underruns.load(),
                      (unsigned long)d.timeAsked, (unsigned long)d.timeGot);
        loopGapMax = 0;
        Serial.printf("[STAT] radio %u link L%u rx %lu disc %lu wifi %u rssi %d buf %u%% vol %d%s heap %u min %u largest %u nvs-writes %lu\n",
                      (unsigned)radioState.load(), (unsigned)usbLink.state(), (unsigned long)usbLink.bytesReceived(),
                      (unsigned long)usbLink.bytesDiscarded(), (unsigned)wifiState,
                      WiFi.status() == WL_CONNECTED ? WiFi.RSSI() : 0, (unsigned)bufferPercent.load(), volume,
                      muted ? " muted" : "", (unsigned)ESP.getFreeHeap(), (unsigned)ESP.getMinFreeHeap(),
                      (unsigned)heap_caps_get_largest_free_block(MALLOC_CAP_8BIT), (unsigned long)nvsWriteCount);
    }
    delay(10);
}
