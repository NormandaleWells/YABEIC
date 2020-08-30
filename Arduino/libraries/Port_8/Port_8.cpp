
#include "Port_8.h"

Port_8::Port_8(byte low_bit_pin, byte high_bit_pin, bool input)
{
   // Get the port and register for the data pin.
   auto port = digitalPinToPort(low_bit_pin);
   auto reg = portOutputRegister(port);

   auto mode = input ? INPUT : OUTPUT;

   // Make sure the other pins map to this same register.  If
   // not, we get out with all the registers set to nullprt.
   if (high_bit_pin - low_bit_pin == 7)
   {
      for (byte pin = low_bit_pin + 1; pin <= high_bit_pin; pin++)
      {
         if (portOutputRegister(digitalPinToPort(pin)) != reg)
            return;
         pinMode(pin, mode);
      }
      pinMode(low_bit_pin, mode);
   }
   else if (low_bit_pin - high_bit_pin == 7)
   {
      for (byte pin = low_bit_pin - 1; pin >= high_bit_pin; pin--)
      {
         if (portOutputRegister(digitalPinToPort(pin)) != reg)
            return;
         pinMode(pin, mode);
      }
      pinMode(low_bit_pin, mode);
   }

   // All checked out.  Get the other port registers.
   input_reg     = portInputRegister(port);
//    direction_reg = portModeRegister(port);
   data_reg      = reg;
}
