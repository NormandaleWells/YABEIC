# Build log

## Some time in August

Finished the clock module, based on Julian Ilett's design.  Has two minor flaws:

1. When the clock is running fast, it will not enter manual step mode.
2. Turning down the potentiometer will get to the point where the clock stops completely.

I can live with both of these.

I'm using a 1 MOhm potentiometer instead of Julian's 200 KOhm; it's what I had.

Switching the timing capacitor from 1 uF to 0.1 uF increases the clock rate from about 1.7 KHz to almost 12 KHz.
This was measured by feeding the clock into a series of 3 '161 timers and timing 25 cycles of 0..4095 counts.

The '161 timers were also used to verify that I don't get any spurious counts when switching from run mode to single-step mode,
and no spurious counts when single-stepping.

## 2020-08-28 (or so)

Created interface board with 4 '595 shift registers (for providing control signals),
and a '245 bus transceiver (for loading data from the Arduino onto the bus).

Wrote two Arduino libraries to support this:

* IC_595 - allows use of up to 4 cascaded '595s to supple 32 control signals.
Uses direct port accesses to (portably!) write all 32 bits in about 100 usecs.
* Port_8 - allows reading/writing all 8 bits of a port at a time

The latter can only be used in situations where all 8 bits of the port are mapped to adjacent Arduino pins.
The Arduino Mega makes ports F and K especially nice to use this way.  See the documentation in Port_8.h.

## 2020-08-30

A and B registers are complete and tested, using a program (AB_register_test.ino) that cycles from 0 to 255,
and with each step:
1. Loading the value into A
2. Loading the value from A to B
3. Reading the value from B
4. Verifying that the proper value was read.

This was complicated by 3 stoopid errors that took far longer to track down than they should have:

1. Forgot to hook up the clock!
2. Had the Arduino-bus '245 hooked to an Arduino port instead of the proper '595 output.
3. Had the A and B register output enable signals crossed.
