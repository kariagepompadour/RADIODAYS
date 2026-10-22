#!/usr/bin/env python3
# RADIO DAYS: plays every station of U10's stations.h through the Windows relay
# (RadioDaysRelay_U9_Windows.ps1, under PowerShell 7 on a Mac) with a small fake Cardputer on
# a pseudo-terminal: HELLO, TUNE, CREDIT; checks RDSYNC and that the first bytes after it are
# MP3 frames. Uses the internet; this is not a Cardputer test.
#   python3 -I tools/usb-relay-windows-test/live_stations.py <pwsh>
# SPDX-License-Identifier: MIT
import os, re, select, subprocess, sys, time

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(os.path.dirname(HERE))
PWSH = sys.argv[1] if len(sys.argv) > 1 else "pwsh"
SCRIPT = os.path.join(ROOT, "tools", "usb-relay", "RadioDaysRelay_U9_Windows.ps1")
urls = re.findall(r'"(http://[^"]+)"', open(os.path.join(ROOT, "firmware", "RadioDays_U10", "stations.h")).read())

master, slave = os.openpty()
import tty
tty.setraw(slave)
env = dict(os.environ, RD_PORT=os.ttyname(slave), NO_COLOR="1")
log = open(os.path.join(os.environ.get("TMPDIR", "/tmp"), "rd_live_relay.log"), "w")
relay = subprocess.Popen([PWSH, "-NoProfile", "-NonInteractive", "-File", SCRIPT], env=env, stdout=log, stderr=log)
buf = b""

def read(t):
    global buf
    end = time.time() + t
    while time.time() < end:
        r, _, _ = select.select([master], [], [], 0.05)
        if r:
            buf += os.read(master, 65536)

def wait_for(pat, t):
    global buf
    end = time.time() + t
    while time.time() < end:
        m = re.search(pat, buf)
        if m:
            return m
        read(0.1)
    return None

def mp3_ok(data):  # two MPEG audio frame headers in a row
    for i in range(min(len(data) - 4, 8192)):
        if data[i] == 0xFF and (data[i + 1] & 0xE0) == 0xE0:
            b = data[i + 1]; c = data[i + 2]
            ver = (b >> 3) & 3; layer = (b >> 1) & 3; br = c >> 4; sr = (c >> 2) & 3; pad = (c >> 1) & 1
            if ver == 1 or layer != 1 or br in (0, 15) or sr == 3:
                continue
            rates = {3: [None, 32, 40, 48, 56, 64, 80, 96, 112, 128, 160, 192, 224, 256, 320],
                     2: [None, 8, 16, 24, 32, 40, 48, 56, 64, 80, 96, 112, 128, 144, 160]}
            srs = {3: [44100, 48000, 32000], 2: [22050, 24000, 16000], 0: [11025, 12000, 8000]}
            kbps = rates[3 if ver == 3 else 2][br]; hz = srs[ver][sr]
            size = (144000 * kbps // hz if ver == 3 else 72000 * kbps // hz) + pad
            j = i + size
            if j + 2 <= len(data) and data[j] == 0xFF and (data[j + 1] & 0xE0) == 0xE0:
                return True
    return False

os.write(master, b"\n@RD HELLO U2a2 00000001\n")
ok = wait_for(rb"RDHOST 00000001", 15)
print(("PASS" if ok else "FAIL") + "  HELLO -> RDHOST")
passed = failed = 0
for n, u in enumerate(urls, 1):
    tid = "%08x" % (0x100 + n)
    buf = b""
    os.write(master, ("\n@RD TUNE %s %s\n" % (tid, u)).encode())
    m = wait_for(("RD(SYNC|NAK) %s([^\n]*)\n" % tid).encode(), 13)
    if not m or m.group(1) != b"SYNC":
        failed += 1
        print("FAIL  %-60s %s" % (u[:60], m.group(0).decode().strip() if m else "no reply"))
        continue
    buf = buf[m.end():]
    os.write(master, ("\n@RD CREDIT %s 32768\n" % tid).encode())
    end = time.time() + 10
    while len(buf) < 32768 and time.time() < end:
        read(0.2)
    good = len(buf) <= 32768 and mp3_ok(buf)
    passed += good; failed += not good
    print("%s  %-60s %d bytes%s" % ("PASS" if good else "FAIL", u[:60], len(buf), "" if good else " (no MP3 frames or too many bytes)"))
    os.write(master, ("\n@RD STOP %s\n" % tid).encode())
    read(0.3)
relay.terminate(); relay.wait()
print("stations %d: pass %d, fail %d (relay log: %s)" % (len(urls), passed, failed, log.name))
