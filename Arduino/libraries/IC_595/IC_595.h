
#include "Arduino.h"

// IC_595 provides an interface to the 595 shift register.
//
// The default constructor uses the following pin mappings:
//    data  - pin 2
//    shift - pin 3
//    latch - pin 4
// These mappings may be overridden using the 3-argument ctor.
//
// This handles up to 4 cascaded 595s, and provides the following
// functions to send the data:
//
//    int8_out()  - send 8 bits (a singl595)
//    int16_out() - send 16 bits (two cascaded 595s)
//    int32_out() - send 32 bits (four cascaded 595s)
//
// The data is always sent with the most significant bit first.
//
// As of this writing, the following functions have not been tested:
//
//    shift_data(int b)   - shift in 0 or 1, depending on b
//    shift_0()           - shift in a 0
//    shift_1()           - shift in a 1
//    latch()             - latch the shift reg to the storage reg
//
// As of this writing, this class does not support the output
// enable signal (it assumes that's tied low so the output is
// always enabled), but that would not be difficult to add.
//
// Rather than use digitalRead() and digitalWrite(), which are
// notoriously slow, this code gets the port address and bit mask
// number for each pin, and reads/writes the bits directly.
// This gives a nearly 6x improvement in speed, which was
// important to me when I wrote this.  (I'm aiming to call int32_out()
// about 10K times per second; there's still some work to be done
// to hit that target.  At last measurement, int32_out() was just
// under 100 usecs.)
//
// Most functions are defined in the header file to give the
// compiler maximum opportunities for inlining the code.
//
// At 5V, all the minimum timings for the 595 are under 60
// nsecs, which (at 16MHz) is the time for each instruction
// executed.  So even if the compiler uses a single instruction
// to set/reset bits in toggle_shift() and toggle_latch(),
// we're well within the minimum necessary signal pulse durations.

class IC_595
{
private:

  // Using the register and mask for each signal instead of the pin
  // numbers cuts the time of int32_out() from 660 usecs to 100 usecs.

  volatile uint8_t * data_reg;
  uint8_t  data_mask;
  uint8_t  data_mask_inv;
  volatile uint8_t * shift_reg;
  uint8_t  shift_mask;
  uint8_t  shift_mask_inv;
  volatile uint8_t * latch_reg;
  uint8_t  latch_mask;
  uint8_t  latch_mask_inv;

  void setup(int data_pin, int shift_pin, int latch_pin);

  void toggle_shift()
  {
    *shift_reg |= shift_mask;
    *shift_reg &= shift_mask_inv;
  }

  void toggle_latch()
  {
    *latch_reg |= latch_mask;
    *latch_reg &= latch_mask_inv;
  }

  void write_data(uint8_t b)
  {
    // Note that we treat the argument as a boolean value;
    // any non-zero value writes a 1, 0 writes a 0.
    if (b)
      *data_reg |= data_mask;
    else
      *data_reg &= data_mask_inv;
    toggle_shift();
  }

  void out_8(uint8_t data)
  {
    // Unrolling the loop increases the program size by
    // 48 bytes, and also increased the execution time
    // of int32_out() from 100 to 112!  No idea why that
    // would happen.
    uint8_t mask = 128;
    while (mask != 0)
    {
      write_data(data & mask);
      mask >>= 1;
    }
  }

public:

  IC_595();
  IC_595(int data_pin, int shift_pin, int latch_pin);

  // NOTE: All output functions are MSB first.

  void int8_out(uint8_t data)
  {
    out_8(data);
    toggle_latch();
  }

  void int16_out(uint16_t data)
  {
    out_8((data >>  8) & 0xff);
    out_8( data        & 0xff);
    toggle_latch();
  }

  void int32_out(uint32_t data)
  {
    out_8((data >> 24) & 0xff);
    out_8((data >> 16) & 0xff);
    out_8((data >>  8) & 0xff);
    out_8( data        & 0xff);
    toggle_latch();
  }

  void shift_data(int b)
  {
    write_data(b != 0);
  }

  void shift_0()
  {
      *data_reg &= data_mask_inv;
      toggle_shift();
  }

  void shift_1()
  {
      *data_reg |= data_mask;
      toggle_shift();
  }

  void latch()
  {
    toggle_latch();
  }
};
