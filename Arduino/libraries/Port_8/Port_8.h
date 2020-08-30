
#include "Arduino.h"

// Port_8 - fast access to 8-bit ports which map to consecutive pins

// On the Arduino Mega (and perhaps others), some ATMega I/O ports
// are mapped to consecutive-numbered (and physcially adjacent) pins
// which allows us to access all 8 bits of that port faster (and
// perhaps more importantly, simultaneously).

// This may be useful, for example, when reading/writing an 8-bit
// bus.  One port could be set up for reading from the bus, and the
// other for writing to the bus (perhaps gated through a '245 bus
// transceiver, or similar chip).

// For the Arduino Mega, the following 5 ports are mapped to
// consecutive pins:

// Port    Bit 0 pin    Bit 7 pin    Notes
//
//   A        22           29
//   C        37           30        pin ordering is reversed
//   F        54           61        lined up at bottom of board
//   K        62           69        lined up at bottom of board
//   L        49           42        pin ordering is reversed

// Rather than assume we're working with the Mega, the ctor takes
// the pins for bits 0 and 7, and verifies that they all map to
// the same physical port.  Since the Arduino IDE does not support
// exceptions, there is an is_valid() function which can be used
// to verify that the requested port mapping is valid for the
// current hardware.  If it is not, the results of reading or
// writing the port are unpredictable.

// The ctor also sets up the port for either reading or writing.
// Any attempt to read a port set up for writing, or write to a
// port set up for reading, will give unpredictable results.

class Port_8
{
private:

   // Each port is 3 registers:
   //
   //	input     - used to read the port pins
   //	direction - used to set the direction of the port
   //	data      - used to write data to the port
   //
   // I'm sticking with ATMega documentation's terminology,
   // thus "data" instead  of "output".

   volatile uint8_t * input_reg = nullptr;
// volatile uint8_t * direction_reg = nullptr;
   volatile uint8_t * data_reg = nullptr;

public:

   Port_8(byte low_bit_pin, byte high_bit_pin, bool input);

   bool is_valid()
   {
      return data_reg != nullptr;
   }

   void write(uint8_t data)
   {
      *data_reg = data;
   }

   uint8_t read()
   {
      return *input_reg;
   }

   void debug()
   {
      Serial.print("input  : "); Serial.println((int)input_reg, HEX);
      Serial.print("output : "); Serial.println((int)data_reg,  HEX);
   }
};
