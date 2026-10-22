// RADIO DAYS — stations by Location (Stage 3-1; U4: 14 more Locations by country; U5: dial positions by rule;
// U6: the dial is a ring; U7: each Location's time zone; U9: the standard lineup, 7 Locations
// and 42 stations chosen by the author — U8's 19 Locations and 85 stations stay in U8)
//
// Every URL below is a plain-HTTP MP3 stream (the only kind RADIO DAYS can play:
// no HTTPS, no HLS, no playlists). (U9) Each one was checked from a computer on
// 2026-10-21/22: HTTP 200, Content-Type audio/mpeg, valid MPEG Layer III frames, 192k or
// less (docs/u9-implementation-plan.md, docs/usb-u9.md). That is NOT a test on the
// Cardputer, and the stations' terms of use have not been reviewed. Only RNZ's addresses
// are published by the broadcaster itself; the others are NOT confirmed as official.
// Stations may change their addresses at any time.
//
// (U9) The order on each dial is the author's choice for listening (talk and music, and
// different kinds of music, take turns). It is not a popularity ranking; the radio screen
// shows the position as n/N (docs/station-lineup-survey-plan.md).
//
// (U4) Names are ASCII (the built-in font has no accents) and at most 19 characters;
// Location names at most 7 (footer field, saved record "loc"). New Locations go after
// WORLD only: the saved Location is found again by its name (savedLocationIndex()).
// Several stations redirect (the ARD stations to *.rndfnk.com, RTL, Antenne Bayern, Radio
// Swiss Jazz); with the USB relay they need RadioDaysRelay_U9.command (its host list and
// redirect rules are U9's), over Wi-Fi the player follows the redirect itself.
//
// To change stations (README, "Changing the stations"): edit a table below and give
// every station of that Location the freq printed by tools/freq-calc/freq_calc.py for the
// new number of stations. A wrong freq stops the build (the checks at the end).
//
// freq: the station's position on RADIO DAYS' own dial, in dial steps. It is not a
// broadcast frequency and is never shown. (U5) The dial has no numbers; positions follow
// one rule for every Location (docs/analog-dial-proposal.md, part 3): the dial starts at
// 800. (U6) The dial is a ring: after RD_DIAL_MAX comes 800 again. Its length is a whole
// number of the tick pattern's common period (60 px = 30 steps: the rows repeat every 10,
// 15, 20 and 30 px), so every lap shows exactly the same ticks. The number of periods
// follows the number of stations (about 20 steps a station): 3 -> 60 steps, 4 and 5 -> 90,
// 6 -> 120, 7 -> 150. Station i sits at ((2i + 1) x ring) / (2 x stations): evenly round the ring,
// half a gap from the seam, so the gaps differ by one step at most. Checked at build time.
//
// SPDX-License-Identifier: MIT
#pragma once

struct Station {
    const char* name;
    const char* city;
    const char* url;
    int freq;  // virtual, 0.1 MHz units
};

struct Location {
    const char* name;     // shown on screen, upper case
    const char* city;     // shown under the name on the Location screen
    float lat, lon;       // marker on the globe (Stage 3-2); WORLD has none
    bool hasMarker;
    int freqMin, freqMax; // dial ends, 0.1 MHz units
    const Station* stations;
    int count;
    const char* tz;       // (U7) POSIX time zone of the city, daylight saving included (the clock)
};

// ---- JAPAN
static constexpr Station STATIONS_JAPAN[] = {
    {"Shonan Beach FM",   "Zushi",    "http://shonanbeachfm.out.airtime.pro:8000/shonanbeachfm_a", 809},
    {"FM Setagaya",       "Setagaya", "http://fmsetagaya834.out.airtime.pro:8000/fmsetagaya834_a", 827},
    {"OTTAVA",            "Tokyo",    "http://ottava2.out.airtime.pro:8000/ottava2_a", 845},
    {"FM Kahoku",         "Kahoku",   "http://radio.kahoku.net:8000/;", 863},
    {"Otsuchi Seashore",  "Iwate",    "http://mp3s.nc.u-tokyo.ac.jp/OTSUCHI_CyberForest.mp3", 881},
};

// ---- USA
static constexpr Station STATIONS_USA[] = {
    {"WNYC",              "New York",    "http://fm939.wnyc.org/wnycfm", 810},
    {"WFUV",              "New York",    "http://wfuv-onair.streamguys.org/onair-hi", 830},
    {"WQXR",              "New York",    "http://stream.wqxr.org/wqxr", 850},
    {"WBGO",              "Newark",      "http://ais-sa8.cdnstream1.com/3629_128.mp3", 870},
    {"WFMU",              "Jersey City", "http://stream0.wfmu.org/freeform-128k", 890},
    {"KEXP",              "Seattle",     "http://kexp-mp3-128.streamguys1.com/kexp128.mp3", 910},
};

// ---- UK
static constexpr Station STATIONS_UK[] = {
    {"BBC World Service", "London",  "http://stream.live.vc.bbcmedia.co.uk/bbc_world_service", 810},
    {"Heart London",      "London",  "http://media-ice.musicradio.com/HeartLondonMP3", 832},
    {"Radio X London",    "London",  "http://media-ice.musicradio.com/RadioXLondonMP3", 853},
    {"LBC London",        "London",  "http://media-ice.musicradio.com/LBCLondonMP3", 875},
    {"Classic FM",        "London",  "http://media-ice.musicradio.com:80/ClassicFMMP3", 896},
    {"Capital London",    "London",  "http://media-ice.musicradio.com/CapitalMP3", 917},
    {"Radio Caroline",    "Essex",   "http://sc6.radiocaroline.net:8040/stream", 939},
};

// ---- FRANCE (RTL redirects to streaming-ice.audiomeans.fr)
static constexpr Station STATIONS_FRANCE[] = {
    {"France Inter",      "Paris",   "http://icecast.radiofrance.fr/franceinter-midfi.mp3", 810},
    {"FIP",               "Paris",   "http://icecast.radiofrance.fr/fip-midfi.mp3", 832},
    {"RTL",               "Paris",   "http://icecast.rtl.fr/rtl-1-44-128", 853},
    {"Radio Nova",        "Paris",   "http://novazz.ice.infomaniak.ch/novazz-128.mp3", 875},
    {"France Culture",    "Paris",   "http://icecast.radiofrance.fr/franceculture-midfi.mp3", 896},
    {"Skyrock",           "Paris",   "http://icecast.skyrock.net/s/natio_mp3_128k", 917},
    {"TSF Jazz",          "Paris",   "http://tsfjazz.ice.infomaniak.ch/tsfjazz-high.mp3", 939},
};

// ---- WORLD ((U9) Radio Paradise first: with no saved Location, RADIO DAYS starts here,
// on Radio Paradise, as it has since Stage 2-1. Radio Swiss Jazz redirects to
// livestreaming-node-<n>.srg-ssr.ch)
static constexpr Station STATIONS_WORLD[] = {
    {"Radio Paradise",    "California", "http://stream.radioparadise.com/mp3-128", 810},
    {"Radio Swiss Jazz",  "Bern",       "http://stream.srg-ssr.ch/m/rsj/mp3_128", 830},
    {"RNZ National",      "Wellington", "http://radionz-ice.streamguys.com/national.mp3", 850},
    {"Groove Salad",      "San Francisco", "http://ice1.somafm.com/groovesalad-128-mp3", 870},
    {"Secret Agent",      "San Francisco", "http://ice1.somafm.com/secretagent-128-mp3", 890},
    {"Nightwave Plaza",   "Internet",   "http://radio.plaza.one/mp3", 910},
};

// ---- (U4) Locations by country, after WORLD. City: the Location's capital (globe marker,
// day / night); each station's own city is in its entry.

// ---- GERMANY (the ARD stations redirect to *.rndfnk.com; Antenne Bayern to
// s<n>-webradio.antenne.de)
static constexpr Station STATIONS_GERMANY[] = {
    {"Deutschlandfunk",   "Cologne",     "http://st01.sslstream.dlf.de/dlf/01/128/mp3/stream.mp3", 810},
    {"SWR3",              "Baden-Baden", "http://dispatcher.rndfnk.com/swr/swr3/live/mp3/128/stream.mp3", 830},
    {"DLF Kultur",        "Berlin",      "http://st02.sslstream.dlf.de/dlf/02/128/mp3/stream.mp3", 850},
    {"radioeins",         "Berlin",      "http://dispatcher.rndfnk.com/rbb/radioeins/live/mp3/mid", 870},
    {"1LIVE",             "Cologne",     "http://wdr-1live-live.icecast.wdr.de/wdr/1live/live/mp3/128/stream.mp3", 890},
    {"Antenne Bayern",    "Munich",      "http://mp3channels.webradio.antenne.de/antenne", 910},
};

// ---- ITALY
static constexpr Station STATIONS_ITALY[] = {
    {"Radio 24",          "Milan",     "http://shoutcast2.radio24.it:8000/;", 809},
    {"Radio 105",         "Milan",     "http://icecast.unitedradio.it/Radio105.mp3", 827},
    {"Radio Radicale",    "Rome",      "http://live.radioradicale.it/live.mp3", 845},
    {"Radio Monte Carlo", "Milan",     "http://icecast.unitedradio.it/RMC.mp3", 863},
    {"Virgin Radio Italia", "Milan",   "http://icecast.unitedradio.it/Virgin.mp3", 881},
};

#define RD_COUNT(a) ((int)(sizeof(a) / sizeof((a)[0])))
// (U5) Dial positions: first station 810, then every 20 steps; the dial ends 10 after the last.
static constexpr int RD_DIAL_MIN = 800;
static constexpr int RD_PATTERN_STEPS = 30;  // (U6) the ticks' common period: 60 px at 2 px a step
static constexpr int rdRing(int n) { return RD_PATTERN_STEPS * ((2 * n + 1) / 3); }  // about 20 steps a station
static constexpr int rdPos(int i, int n) { return (2 * i + 1) * rdRing(n) / (2 * n); }
#define RD_DIAL_MAX(a) (RD_DIAL_MIN + rdRing(RD_COUNT(a)) - 1)  // (U6) the ring's last position

// (U7) Time zones (POSIX TZ: name, offset west of UTC, daylight-saving name and rules
// "Mm.w.d/hour"). The clock shows the Location city's local time; WORLD uses Japan time,
// as its sky uses Tokyo. Rules are built in: if a country changes them, this must change.
#define RD_TZ_JAPAN "JST-9"
#define RD_TZ_US_EAST "EST5EDT,M3.2.0,M11.1.0"        // New York
#define RD_TZ_UK "GMT0BST,M3.5.0/1,M10.5.0"           // London
#define RD_TZ_CET "CET-1CEST,M3.5.0,M10.5.0/3"        // Paris, Berlin, Rome

static constexpr Location LOCATIONS[] = {
    {"JAPAN",  "TOKYO",     35.68f, 139.69f, true, 800, RD_DIAL_MAX(STATIONS_JAPAN), STATIONS_JAPAN, RD_COUNT(STATIONS_JAPAN), RD_TZ_JAPAN},
    {"USA",    "NEW YORK",  40.71f, -74.01f, true, 800, RD_DIAL_MAX(STATIONS_USA), STATIONS_USA, RD_COUNT(STATIONS_USA), RD_TZ_US_EAST},
    {"UK",     "LONDON",    51.51f,  -0.13f, true, 800, RD_DIAL_MAX(STATIONS_UK), STATIONS_UK, RD_COUNT(STATIONS_UK), RD_TZ_UK},
    {"FRANCE", "PARIS",     48.86f,   2.35f, true, 800, RD_DIAL_MAX(STATIONS_FRANCE), STATIONS_FRANCE, RD_COUNT(STATIONS_FRANCE), RD_TZ_CET},
    {"WORLD",  "EVERYWHERE", 0.00f,   0.00f, false, 800, RD_DIAL_MAX(STATIONS_WORLD), STATIONS_WORLD, RD_COUNT(STATIONS_WORLD), RD_TZ_JAPAN},
    // (U4) after WORLD only (the saved Location is matched by name)
    {"GERMANY", "BERLIN",      52.52f,  13.40f, true, 800, RD_DIAL_MAX(STATIONS_GERMANY), STATIONS_GERMANY, RD_COUNT(STATIONS_GERMANY), RD_TZ_CET},
    {"ITALY",   "ROME",        41.90f,  12.50f, true, 800, RD_DIAL_MAX(STATIONS_ITALY), STATIONS_ITALY, RD_COUNT(STATIONS_ITALY), RD_TZ_CET},
};
static constexpr int LOCATION_COUNT = RD_COUNT(LOCATIONS);
static constexpr int LOCATION_DEFAULT = 4;  // (U9) WORLD: starts on Radio Paradise, as Stages 2-1 to U8 (U8: USA)

// Build-time checks: every Location has stations, rising unique frequencies inside its dial.
static constexpr bool stationsValid(const Station* s, int n, int lo, int hi, int i = 0)
{
    return i >= n || (s[i].freq >= lo && s[i].freq <= hi && (i == 0 || s[i].freq > s[i - 1].freq) &&
                      stationsValid(s, n, lo, hi, i + 1));
}
static constexpr bool locationsValid(int i = 0)
{
    return i >= LOCATION_COUNT ||
           (LOCATIONS[i].count > 0 && LOCATIONS[i].freqMin < LOCATIONS[i].freqMax &&
            stationsValid(LOCATIONS[i].stations, LOCATIONS[i].count, LOCATIONS[i].freqMin, LOCATIONS[i].freqMax) &&
            locationsValid(i + 1));
}
static_assert(locationsValid(), "each Location needs stations with unique, rising frequencies inside its dial");
static_assert(LOCATION_DEFAULT >= 0 && LOCATION_DEFAULT < LOCATION_COUNT, "default Location");

// (U4) Names: Location at most 7 characters (footer, saved record), station at most 19
// (radio screen), all unique Location names; the first five Locations keep their names.
static constexpr int rdLen(const char* s) { return *s ? 1 + rdLen(s + 1) : 0; }
static constexpr bool rdSame(const char* a, const char* b) { return *a == *b && (*a == 0 || rdSame(a + 1, b + 1)); }
static constexpr bool stationNamesValid(const Station* s, int n, int i = 0)
{
    return i >= n || (rdLen(s[i].name) <= 19 && stationNamesValid(s, n, i + 1));
}
static constexpr bool namesUnique(int i, int j)
{
    return j >= LOCATION_COUNT || (!rdSame(LOCATIONS[i].name, LOCATIONS[j].name) && namesUnique(i, j + 1));
}
static constexpr bool namesValid(int i = 0)
{
    return i >= LOCATION_COUNT || (rdLen(LOCATIONS[i].name) <= 7 && namesUnique(i, i + 1) &&
                                   stationNamesValid(LOCATIONS[i].stations, LOCATIONS[i].count) && namesValid(i + 1));
}
// (U5) Every station sits on the rule's position, and every dial ends where the rule says.
static constexpr bool positionsOk(const Station* s, int n, int i = 0)
{
    return i >= n || (s[i].freq == RD_DIAL_MIN + rdPos(i, n) && positionsOk(s, n, i + 1));
}
static constexpr bool dialsValid(int i = 0)
{
    return i >= LOCATION_COUNT ||
           (LOCATIONS[i].freqMin == RD_DIAL_MIN &&
            LOCATIONS[i].freqMax == RD_DIAL_MIN + rdRing(LOCATIONS[i].count) - 1 &&
            rdRing(LOCATIONS[i].count) % RD_PATTERN_STEPS == 0 &&
            positionsOk(LOCATIONS[i].stations, LOCATIONS[i].count) && dialsValid(i + 1));
}
static_assert(dialsValid(), "(U6) a ring of whole tick periods; stations evenly round it");
static_assert(namesValid(), "Location names <= 7 characters and unique, station names <= 19 characters");
static_assert(LOCATION_COUNT == 7 && rdSame(LOCATIONS[0].name, "JAPAN") && rdSame(LOCATIONS[1].name, "USA") &&
                  rdSame(LOCATIONS[2].name, "UK") && rdSame(LOCATIONS[3].name, "FRANCE") &&
                  rdSame(LOCATIONS[4].name, "WORLD"),
              "(U9) 7 Locations; the first five keep their names and places (saved records)");
static_assert(LOCATIONS[LOCATION_DEFAULT].stations[0].freq == RD_DIAL_MIN + rdPos(0, LOCATIONS[LOCATION_DEFAULT].count) &&
                  rdSame(LOCATIONS[LOCATION_DEFAULT].name, "WORLD") &&
                  rdSame(LOCATIONS[LOCATION_DEFAULT].stations[0].name, "Radio Paradise"),
              "(U9) with no saved Location RADIO DAYS starts on WORLD's first station, Radio Paradise");
