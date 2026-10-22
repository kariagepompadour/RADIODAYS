#!/usr/bin/env python3
# RADIO DAYS: prints the freq values of a Location with N stations, as stations.h checks
# them (RD_DIAL_MIN + rdPos(i, N); rdRing(N) = 30 x ((2N + 1) / 3)). Nothing else is read
# or written. Python 3, standard library only.
#   python3 tools/freq-calc/freq_calc.py 6          one Location of 6 stations
#   python3 tools/freq-calc/freq_calc.py --table    1 to 12 stations
# SPDX-License-Identifier: MIT
import sys

RD_DIAL_MIN = 800
RD_PATTERN_STEPS = 30  # the ticks' common period: 60 px at 2 px a step


def ring(n):
    return RD_PATTERN_STEPS * ((2 * n + 1) // 3)


def positions(n):
    r = ring(n)
    return [RD_DIAL_MIN + (2 * i + 1) * r // (2 * n) for i in range(n)]


def main(argv):
    if len(argv) == 2 and argv[1] == "--table":
        for n in range(1, 13):
            print("%2d stations: ring %3d steps (%3d px): %s" % (n, ring(n), 2 * ring(n), ", ".join(map(str, positions(n)))))
        return 0
    if len(argv) != 2 or not argv[1].isdigit() or not 1 <= int(argv[1]) <= 99:
        print("use: python3 freq_calc.py <number of stations, 1..99> | --table", file=sys.stderr)
        return 2
    n = int(argv[1])
    print("%d stations: the dial is %d steps (%d px) round." % (n, ring(n), 2 * ring(n)))
    print("Give the stations of this Location these freq values, in dial order:")
    for i, f in enumerate(positions(n)):
        print("  station %2d (%d/%d): %d" % (i + 1, i + 1, n, f))
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))
