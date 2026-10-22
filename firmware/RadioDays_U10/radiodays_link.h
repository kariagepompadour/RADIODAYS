// RADIO DAYS U3 — the USB relay link (Cardputer side). See docs/usb-u2a-design.md,
// docs/usb-u2a2.md (changes from U2a) and docs/usb-u3-time-design.md (the time).
//
// Portable on purpose: the sketch and the Mac simulation (tools/usb-relay/sim2) compile
// this same file. Everything that touches the hardware goes through Io.
//
// Tasks:
//   service()          link task only (the sketch's loop, core 1). The only writer of
//                      requests to the Mac and the only reader of the receive queue
//                      outside L3.
//   post() / result()  decode task (core 0): ask for a station or a stop, read the outcome.
//   take()             decode task: MP3 bytes, only in L3 and only for its own request.
//
// Changes from U2a (docs/usb-u2a2.md):
//   - Credit is a running total: "@RD CREDIT <id> <total>". A lost or cut line no longer
//     leaves the Mac short of credit for good; the total is sent again every 500 ms
//     while bytes are outstanding.
//   - The plugged signal is only counted for the log. HELLO is always sent in L0, and
//     no state change depends on it (U2a stopped HELLO while it read "unplugged").
//   - A TUNE that could not be written whole is written again; an unanswered TUNE is
//     repeated (same id) every 3 s until the 12 s limit. The Mac ignores repeats.
//
// Added in U3 (docs/usb-u3-time-design.md): the time from the Mac. "@RD TIME <id>" is sent
// when the relay is found, and again (before a TUNE, or while idle) if it could not be
// written or has had no answer for 10 s, until
// "RDTIME <id> <UTC seconds>" comes back (at most 5 times per link-up; the U2a2 relay
// does not answer, which changes nothing else). It is sent and matched only outside L3,
// and the Mac answers only before RDSYNC, so no text ever enters the MP3 bytes. A reply
// to a TIME sent before a TUNE arrives before that TUNE's RDSYNC (the Mac handles lines
// in order), so going to L3 right after asking does not lose it.
//
// SPDX-License-Identifier: MIT
#pragma once
#include <atomic>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

namespace rdlink {

constexpr uint32_t RX_QUEUE_SIZE = 16384;  // set with Serial.setRxBufferSize() before begin()
constexpr uint32_t CREDIT_CHUNK = 2048;
constexpr uint32_t CREDIT_MARGIN = 1024;
constexpr uint32_t CREDIT_RESEND_MS = 500;   // the running total again while bytes are outstanding
constexpr uint32_t HELLO_MS = 2000;
constexpr uint32_t PING_MS = 2000;
constexpr uint32_t HOST_TIMEOUT_MS = 6000;   // L1: no RDHOST for this long -> L0
constexpr uint32_t TUNE_REPEAT_MS = 3000;    // L2: TUNE again (same id) while unanswered
constexpr uint32_t TUNE_TIMEOUT_MS = 12000;  // L2: no RDSYNC / RDNAK (the Mac answers within 10 s) -> L0
constexpr uint32_t SILENT_MS = 5000;         // L3: credit outstanding, nothing arrived -> STOP, L1
constexpr uint32_t TIME_RETRY_MS = 10000;   // (U3) TIME again while idle and unanswered
constexpr uint8_t TIME_MAX_TRIES = 5;       // (U3) per link-up (an older relay never answers)
constexpr uint32_t TIME_MIN_UTC = 1735689600u;  // (U3) 2025-01-01: anything earlier is refused
constexpr uint32_t TIME_MAX_UTC = 4102444800u;  // (U3) 2100-01-01
constexpr size_t LINE_LEN = 32;

enum State : uint8_t { L0_NO_RELAY, L1_IDLE, L2_TUNING, L3_STREAMING };
enum ReqKind : uint8_t { REQ_TUNE, REQ_STOP };
enum Result : uint8_t { RES_PENDING, RES_STREAMING, RES_NAK, RES_FAILED, RES_STOPPED };

// Reasons in "RDNAK <id> <reason>"; index 0 = not understood.
static const char* const NAK_REASONS[] = {"?", "URL", "HOST", "PORT", "REDIR", "HTTP", "CONN"};
constexpr uint8_t NAK_REASON_COUNT = sizeof(NAK_REASONS) / sizeof(NAK_REASONS[0]);
// A refusal that will not change by trying the same URL again.
inline bool nakIsPermanent(uint8_t r) { return r >= 1 && r <= 4; }

struct Io {
    uint32_t (*now)();
    bool (*plugged)();                          // a USB host is there (counted for the log only)
    int (*available)();                         // bytes in the receive queue
    size_t (*read)(uint8_t* buf, size_t len);   // non-blocking
    bool (*line)(const char* text);             // text + "\n" in ONE write; false if not all written
    void (*pause)();                            // a very short wait
};

// For the log and the diagnostic footer; reset with takeDiag().
struct Diag {
    uint32_t helloSent = 0;        // HELLO lines written
    uint32_t txFailed = 0;         // lines not written whole (HWCDC busy or full)
    uint32_t creditResent = 0;     // the running total sent again
    uint32_t unpluggedSeen = 0;    // the plugged signal went false (it may flap)
    uint32_t maxGapMs = 0;         // longest time between two service() calls
    uint32_t queueMax = 0;         // fullest receive queue seen in L3
    uint32_t toL0 = 0;             // times the link fell back to L0
    uint32_t timeAsked = 0;        // (U3) TIME lines written
    uint32_t timeGot = 0;          // (U3) RDTIME replies taken
};

class Link {
  public:
    explicit Link(const Io& io) : io_(io) {}

    void begin(uint32_t seed)
    {
        nextId_ = seed;
        hostId_ = prevHostId_ = newId();  // never 0: no fixed value is ever awaited
    }

    // ------------------------------------------------------------ decode task
    // A new request; the latest one wins. Returns its number for result() and take().
    uint32_t post(ReqKind kind, const char* url)
    {
        reqKind_.store(kind);
        reqUrl_.store(url);
        return reqSeq_.fetch_add(1) + 1;
    }
    Result result(uint32_t req) const
    {
        if (doneReq_.load() != req) return RES_PENDING;
        return (Result)doneResult_.load();
    }
    uint8_t nakReason() const { return nakReason_.load(); }
    bool ready() const { return state_.load() != L0_NO_RELAY; }  // the relay answered
    State state() const { return (State)state_.load(); }

    // MP3 bytes of request `req`; 0 when not streaming that request (design 3.3).
    size_t take(uint32_t req, uint8_t* buf, size_t len)
    {
        readerBusy_.store(true);
        if (state_.load() != L3_STREAMING || streamReq_.load() != req || len == 0) {
            readerBusy_.store(false);
            return 0;
        }
        size_t n = io_.read(buf, len);
        if (n) {
            rxTaken_.fetch_add((uint32_t)n);
            rxAll_.fetch_add((uint32_t)n);
            lastRxMs_.store(io_.now());
        }
        readerBusy_.store(false);
        return n;
    }

    // (U3) Link task only: the Mac's UTC seconds, once per reply (0: none new), and how
    // long ago (ms) it arrived. The caller sets the clock with it.
    uint32_t takeRelayTime(uint32_t& ageMs)
    {
        uint32_t t = relayTime_;
        relayTime_ = 0;
        ageMs = t ? io_.now() - relayTimeMs_ : 0;
        return t;
    }
    bool timeWanted() const { return timeWanted_; }

    // Counters for the screen and the log.
    uint32_t bytesReceived() const { return rxAll_.load(); }
    uint32_t bytesDiscarded() const { return discarded_.load(); }
    uint32_t inFlight() const { return inFlightShown_.load(); }
    // Link task only: the counters since the last call.
    Diag takeDiag()
    {
        Diag d = diag_;
        diag_ = Diag();
        return d;
    }

    // ------------------------------------------------------------ link task
    void service()
    {
        uint32_t now = io_.now();
        if (lastServiceMs_ && now - lastServiceMs_ > diag_.maxGapMs) diag_.maxGapMs = now - lastServiceMs_;
        lastServiceMs_ = now;
        bool plugged = io_.plugged();
        if (!plugged && wasPlugged_) diag_.unpluggedSeen++;
        wasPlugged_ = plugged;

        takeRequest(now);

        switch (state_.load()) {
        case L0_NO_RELAY:
            if (lastHelloMs_ == 0 || now - lastHelloMs_ >= HELLO_MS) {
                lastHelloMs_ = now ? now : 1;
                prevHostId_ = hostId_;
                hostId_ = newId();
                if (send("@RD HELLO U2a2 %08lx", hostId_)) diag_.helloSent++;
            }
            if (scan() == M_HOST) {
                state_.store(L1_IDLE);
                lastHostMs_ = now;
                lastPingMs_ = now;
                report("[LINK] relay found");
                timeWanted_ = true;  // (U3) every link-up asks again (it also corrects any drift)
                timeTries_ = 0;
                askTime(now);
            }
            break;
        case L1_IDLE:
            if (now - lastPingMs_ >= PING_MS) {
                lastPingMs_ = now;
                prevHostId_ = hostId_;
                hostId_ = newId();
                send("@RD PING %08lx", hostId_);
            }
            if (timeAskDue(now)) askTime(now);
            if (scan() == M_HOST) lastHostMs_ = now;
            if (now - lastHostMs_ >= HOST_TIMEOUT_MS) toNoRelay("no answer from the relay");
            break;
        case L2_TUNING: {
            if (tuneUnsent_ || now - tuneSentMs_ >= TUNE_REPEAT_MS) sendTune(now);
            Match m = scan();
            if (m == M_SYNC) {
                granted_ = 0;
                lastCreditMs_ = 0;
                rxTaken_.store(0);
                lastRxMs_.store(now);
                lastHostMs_ = now;
                state_.store(L3_STREAMING);  // from here only the decode task reads
                finish(streamReq_.load(), RES_STREAMING);
                reportId("[LINK] streaming %08lx", tuneId_);
            } else if (m == M_NAK) {
                state_.store(L1_IDLE);
                lastHostMs_ = now;
                lastPingMs_ = now;
                finish(streamReq_.load(), RES_NAK);
                reportId("[LINK] refused %08lx: %s", tuneId_);
            } else if (now - tuneStartMs_ >= TUNE_TIMEOUT_MS) {
                toNoRelay("no answer to TUNE");
            }
            break;
        }
        case L3_STREAMING:
            credit(now);
            break;
        }
    }

  private:
    enum Match : uint8_t { M_NONE, M_HOST, M_SYNC, M_NAK };

    const Io& io_;
    std::atomic<uint8_t> state_{L0_NO_RELAY};
    // decode task -> link task
    std::atomic<uint32_t> reqSeq_{0};
    std::atomic<uint8_t> reqKind_{REQ_STOP};
    std::atomic<const char*> reqUrl_{nullptr};
    // link task -> decode task
    std::atomic<uint32_t> streamReq_{0};
    std::atomic<uint32_t> doneReq_{0};
    std::atomic<uint8_t> doneResult_{RES_PENDING};
    std::atomic<uint8_t> nakReason_{0};
    // the receive queue (design 3.3, 4)
    std::atomic<bool> readerBusy_{false};
    std::atomic<uint32_t> rxTaken_{0};
    std::atomic<uint32_t> rxAll_{0};
    std::atomic<uint32_t> lastRxMs_{0};
    std::atomic<uint32_t> discarded_{0};
    std::atomic<uint32_t> inFlightShown_{0};
    // link task only
    uint32_t handledReq_ = 0;
    uint32_t nextId_ = 0;
    uint32_t hostId_ = 0, prevHostId_ = 0, tuneId_ = 0;
    uint32_t granted_ = 0;  // running total for this stream (wraps; the Mac works modulo 2^32)
    uint32_t lastHelloMs_ = 0, lastPingMs_ = 0, lastHostMs_ = 0, tuneStartMs_ = 0, tuneSentMs_ = 0;
    uint32_t lastCreditMs_ = 0, lastServiceMs_ = 0;
    bool tuneUnsent_ = false;
    // (U3) the time
    bool timeWanted_ = false;
    uint8_t timeTries_ = 0;
    uint32_t timeId_ = 0, lastTimeAskMs_ = 0;
    bool timeAskWritten_ = false;
    uint32_t relayTime_ = 0, relayTimeMs_ = 0;
    bool wasPlugged_ = true;
    const char* tuneUrl_ = "";  // the station table's string (static): sent again as it is
    char lineBuf_[LINE_LEN + 1] = {};
    size_t lineLen_ = 0;
    bool lineBad_ = false;
    Diag diag_;

    uint32_t newId() { return ++nextId_; }

    bool put(const char* text)
    {
        bool ok = io_.line(text);
        if (!ok) diag_.txFailed++;
        return ok;
    }
    bool send(const char* fmt, uint32_t id)
    {
        char t[48];
        snprintf(t, sizeof(t), fmt, (unsigned long)id);
        return put(t);
    }
    void report(const char* text) { put(text); }
    void reportId(const char* fmt, uint32_t id)
    {
        char t[64];
        snprintf(t, sizeof(t), fmt, (unsigned long)id, NAK_REASONS[nakReason_.load() % NAK_REASON_COUNT]);
        put(t);
    }

    void finish(uint32_t req, Result r)
    {
        doneResult_.store(r);  // before doneReq_: result() reads doneReq_ first
        doneReq_.store(req);
    }

    // Leaves L3 so that the link task may read the queue: the decode task's take()
    // either saw L3 and is reading (wait for it), or will see the new state.
    void leaveStreaming(State to)
    {
        bool wasStreaming = state_.load() == L3_STREAMING;
        state_.store(to);
        inFlightShown_.store(0);
        if (wasStreaming)
            while (readerBusy_.load()) io_.pause();
    }

    void toNoRelay(const char* why)
    {
        bool tuning = state_.load() == L2_TUNING;
        leaveStreaming(L0_NO_RELAY);
        if (tuning) finish(streamReq_.load(), RES_FAILED);
        lastHelloMs_ = 0;  // HELLO at once
        diag_.toL0++;
        char t[64];
        snprintf(t, sizeof(t), "[LINK] no relay: %s", why);
        put(t);
    }

    // (U3) Only called outside L3 (on link-up, in L1, and right before a TUNE).
    void askTime(uint32_t now)
    {
        timeId_ = newId();
        timeTries_++;
        lastTimeAskMs_ = now;
        timeAskWritten_ = send("@RD TIME %08lx", timeId_);
        if (timeAskWritten_) diag_.timeAsked++;
    }
    // (U3) Ask again only if the last request was not written whole, or is 10 s old with no
    // answer: a request just written is answered in order, before anything after it.
    bool timeAskDue(uint32_t now) const
    {
        return timeWanted_ && timeTries_ < TIME_MAX_TRIES && (!timeAskWritten_ || now - lastTimeAskMs_ >= TIME_RETRY_MS);
    }

    void sendTune(uint32_t now)
    {
        char t[400];  // a longer URL is cut here and refused by the Mac (stations.h has none)
        snprintf(t, sizeof(t), "@RD TUNE %08lx %s", (unsigned long)tuneId_, tuneUrl_);
        tuneUnsent_ = !put(t);
        tuneSentMs_ = now;
    }

    // Takes the decode task's latest request, if any.
    void takeRequest(uint32_t now)
    {
        uint32_t r = reqSeq_.load();
        if (r == handledReq_) return;
        ReqKind kind = (ReqKind)reqKind_.load();
        const char* url = reqUrl_.load();
        if (reqSeq_.load() != r) return;  // a newer request came in meanwhile: next time
        handledReq_ = r;
        State s = (State)state_.load();
        if (s == L0_NO_RELAY) {
            finish(r, RES_FAILED);
            return;
        }
        if (s == L2_TUNING) finish(streamReq_.load(), RES_FAILED);  // superseded
        if (kind == REQ_STOP || !url) {
            if (s == L2_TUNING || s == L3_STREAMING) send("@RD STOP %08lx", tuneId_);
            leaveStreaming(L1_IDLE);
            lastHostMs_ = now;
            lastPingMs_ = now;
            finish(r, RES_STOPPED);
            return;
        }
        leaveStreaming(L2_TUNING);
        if (timeAskDue(now)) askTime(now);  // (U3) its answer comes before this TUNE's RDSYNC
        tuneId_ = newId();
        tuneUrl_ = url;
        streamReq_.store(r);
        tuneStartMs_ = now;
        sendTune(now);
    }

    // L3: grant what fits (U1's rule, verified on hardware), and watch for silence.
    void credit(uint32_t now)
    {
        // Read the taken total first: a byte moving out of the queue meanwhile only
        // makes the estimate of bytes in flight larger (never an overflow).
        uint32_t taken = rxTaken_.load();
        int q = io_.available();
        uint32_t queued = q > 0 ? (uint32_t)q : 0;
        if (queued > diag_.queueMax) diag_.queueMax = queued;
        int32_t d = (int32_t)(granted_ - (taken + queued));
        uint32_t inFlight = d > 0 ? (uint32_t)d : 0;
        inFlightShown_.store(inFlight);
        uint32_t used = queued + inFlight;
        bool more = used + CREDIT_CHUNK + CREDIT_MARGIN <= RX_QUEUE_SIZE;
        if (more) granted_ += (RX_QUEUE_SIZE - CREDIT_MARGIN - used) / CREDIT_CHUNK * CREDIT_CHUNK;
        bool resend = !more && inFlight > 0 && now - lastCreditMs_ >= CREDIT_RESEND_MS;
        if (more || resend) {
            // The running total: whatever line was lost before, this one says it all.
            if (resend) diag_.creditResent++;
            char t[40];
            snprintf(t, sizeof(t), "@RD CREDIT %08lx %lu", (unsigned long)tuneId_, (unsigned long)granted_);
            put(t);
            lastCreditMs_ = now;
        }
        // Signed: the decode task may have stored a time later than `now` meanwhile.
        int32_t idle = (int32_t)(now - lastRxMs_.load());
        if (inFlight > 0 && idle >= (int32_t)SILENT_MS) {
            send("@RD STOP %08lx", tuneId_);
            leaveStreaming(L1_IDLE);
            lastHostMs_ = now;
            lastPingMs_ = now;
            report("[LINK] no data from the relay: stopped");
        }
    }

    // Outside L3: reads and discards everything, looking for the awaited reply line.
    // The line holder survives between calls, so a reply split over reads is found.
    Match scan()
    {
        uint8_t buf[64];
        for (;;) {
            int a = io_.available();
            if (a <= 0) return M_NONE;
            size_t n = io_.read(buf, (size_t)a < sizeof(buf) ? (size_t)a : sizeof(buf));
            if (n == 0) return M_NONE;
            for (size_t i = 0; i < n; i++) {
                Match m = feed(buf[i]);
                if (m != M_NONE) {
                    if (i + 1 < n) {
                        // Nothing may follow a reply before we ask (no credit yet): drop it.
                        discarded_.fetch_add((uint32_t)(n - i - 1));
                        report("[LINK] unexpected bytes after a reply (dropped)");
                    }
                    return m;
                }
            }
        }
    }

    Match feed(uint8_t c)
    {
        if (c != '\n') {
            if (lineLen_ < LINE_LEN) lineBuf_[lineLen_++] = (char)c;
            else lineBad_ = true;
            discarded_.fetch_add(1);
            return M_NONE;
        }
        discarded_.fetch_add(1);
        bool ok = !lineBad_;
        lineBuf_[lineLen_] = 0;
        lineLen_ = 0;
        lineBad_ = false;
        if (!ok) return M_NONE;
        return check(lineBuf_);
    }

    // (U3) "RDTIME <timeId_> <digits>": taken in L0..L2 (scan() never runs in L3), then
    // the scan goes on (it is not the reply the state is waiting for).
    bool checkTime(const char* s)
    {
        char want[LINE_LEN + 1];
        snprintf(want, sizeof(want), "RDTIME %08lx ", (unsigned long)timeId_);
        size_t w = strlen(want);
        if (!timeId_ || strncmp(s, want, w) != 0) return false;
        uint32_t v = 0;
        const char* p = s + w;
        int digits = 0;
        for (; *p >= '0' && *p <= '9' && digits < 10; p++, digits++) {
            uint64_t n = (uint64_t)v * 10u + (uint32_t)(*p - '0');
            if (n > 0xFFFFFFFFull) return true;  // ours, but not a time: ignored
            v = (uint32_t)n;
        }
        if (*p != 0 || digits == 0 || v < TIME_MIN_UTC || v > TIME_MAX_UTC) return true;  // ignored
        relayTime_ = v;
        relayTimeMs_ = io_.now();
        timeWanted_ = false;
        diag_.timeGot++;
        return true;
    }

    Match check(const char* s)
    {
        char want[LINE_LEN + 1];
        State st = (State)state_.load();
        if (checkTime(s)) return M_NONE;
        if (st == L0_NO_RELAY || st == L1_IDLE) {
            snprintf(want, sizeof(want), "RDHOST %08lx", (unsigned long)hostId_);
            if (strcmp(s, want) == 0) return M_HOST;
            snprintf(want, sizeof(want), "RDHOST %08lx", (unsigned long)prevHostId_);
            if (strcmp(s, want) == 0) return M_HOST;
            return M_NONE;
        }
        if (st == L2_TUNING) {
            snprintf(want, sizeof(want), "RDSYNC %08lx", (unsigned long)tuneId_);
            if (strcmp(s, want) == 0) return M_SYNC;
            snprintf(want, sizeof(want), "RDNAK %08lx ", (unsigned long)tuneId_);
            size_t w = strlen(want);
            if (strncmp(s, want, w) == 0) {
                uint8_t r = 0;
                for (uint8_t i = 1; i < NAK_REASON_COUNT; i++)
                    if (strcmp(s + w, NAK_REASONS[i]) == 0) r = i;
                nakReason_.store(r);
                return M_NAK;
            }
        }
        return M_NONE;
    }
};

}  // namespace rdlink
