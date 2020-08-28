
#include "IC_595.h"

IC_595::IC_595()
{
  setup(2, 3, 4);
}

IC_595::IC_595(int data_pin, int shift_pin, int latch_pin)
{
  setup(data_pin, shift_pin, latch_pin);
}

void IC_595::setup(int data_pin, int shift_pin, int latch_pin)
{
  data_reg   = reinterpret_cast<volatile uint8_t *>(portOutputRegister(digitalPinToPort(data_pin)));
  data_mask  = digitalPinToBitMask(data_pin);
  data_mask_inv = ~data_mask;
  shift_reg  = reinterpret_cast<volatile uint8_t *>(portOutputRegister(digitalPinToPort(shift_pin)));
  shift_mask = digitalPinToBitMask(shift_pin);
  shift_mask_inv = ~shift_mask;
  latch_reg  = reinterpret_cast<volatile uint8_t *>(portOutputRegister(digitalPinToPort(latch_pin)));
  latch_mask = digitalPinToBitMask(latch_pin);
  latch_mask_inv = ~latch_mask;

  pinMode(data_pin,  OUTPUT);
  pinMode(shift_pin, OUTPUT);
  pinMode(latch_pin, OUTPUT);

  digitalWrite(data_pin,  LOW);
  digitalWrite(shift_pin, LOW);
  digitalWrite(latch_pin, LOW);
}
