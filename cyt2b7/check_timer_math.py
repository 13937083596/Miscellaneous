#!/usr/bin/env python3
"""ponytail: sanity-check CYT2B7 timer divider math (no hardware needed)."""

PERI_CLK_HZ = 80_000_000
CNT_CLK_HZ = 2_000_000
PRESCALER = 128
TICK_HZ = CNT_CLK_HZ // PRESCALER
PERIOD = TICK_HZ - 1

assert PERI_CLK_HZ // CNT_CLK_HZ == 40
assert TICK_HZ == 15_625
assert (PERIOD + 1) / TICK_HZ == 1.0
print("ok: 1 s tick @ 80 MHz peri, div40 -> 2 MHz, prescaler/128 -> 15.625 kHz")
