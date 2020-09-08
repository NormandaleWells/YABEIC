
// AB_register_test - sketch to test the A and B registers

// Setup:
//
// Each register is implemented using a '574 and a '245.  The '245 outputs and
// '574 inputs are tied to the bus, which is also attached to ports F (output)
// and K (input) of an Arduino Mega.  F is connected to the input side of
// the interface board's '245; K is connected to the output side of all the
// '245s (which constitutes the bus).

// There are 5 control lines, attached to the output lines of a series of
// '595 shift registers, controlled through an IC_595 object.  The IC_595
// is set up according the default ctor:
//    data  - Arduino pin 2 -> 595 pin 14
//    shift - Arduino pin 3 -> 595 pin 11
//    latch - Arduino pin 4 -> 595 pin 12

#include <IC_595.h>
#include <Port_8.h>

IC_595 control;

Port_8 output_port(54, 61, false);
Port_8  input_port(62, 69, true);

const byte clock_pin = 5;

// Signals whose names end with an underscore are active low.

const uint32_t AREG_IN_  = 0x00000002;
const uint32_t BREG_IN_  = 0x00000004;
const uint32_t AREG_OUT_ = 0x00020000;
const uint32_t BREG_OUT_ = 0x00040000;
const uint32_t MEM_OUT_  = 0x02000000;

const uint32_t active_low_signals = AREG_IN_ | BREG_IN_ | AREG_OUT_ | BREG_OUT_ | MEM_OUT_;

void control_out(uint32_t signals)
{
  control.int32_out(signals ^ active_low_signals);
}

void clock_high()
{
  digitalWrite(clock_pin, HIGH);
}

void clock_low()
{
  digitalWrite(clock_pin, LOW);
}

void setup()
{
  Serial.begin(9600);
  Serial.println("Make sure the Arduino out is driven by the 595s.");

  control_out(0);
  pinMode(clock_pin, OUTPUT);
}

// Each pass through the main loop executes the
// following sequence for each number 0..255
//
//    Mem out -> A in
//    A out   -> B in
//    B out   -> Mem in
//
// (Actually, for debugging purposes, the Arduino reads the
// data bus at each step the make sure the proper value is
// found there.)

const int error_delay = 20000;      // delay after invalid data found
const int pulse_width = 100;
const int view_delay = 1000;        // time to view results

void loop()
{
  for (byte i = 0; i <=255; i++)
  {
    clock_low();

    control_out(MEM_OUT_ | AREG_IN_);
    output_port.write(i);

    uint8_t data = input_port.read();
    if (data != i)
    {
      Serial.print("ERROR 1: invalid data on mem out.  Read  ");
      Serial.print(data);
      Serial.print(" instead of ");
      Serial.println(i);
      delay(error_delay);
    }

    delay(view_delay);

    clock_high();
    delay(pulse_width);
    clock_low();

    control_out(AREG_OUT_ | BREG_IN_);

    data = input_port.read();
    if (data != i)
    {
      Serial.print("ERROR 2: invalid data on A out.  Read  ");
      Serial.print(data);
      Serial.print(" instead of ");
      Serial.println(i);
      delay(error_delay);
    }

    delay(view_delay);

    clock_high();
    delay(pulse_width);
    clock_low();

    control_out(BREG_OUT_);

    data = input_port.read();
    if (data != i)
    {
      Serial.print("ERROR 3: invalid data on B out.  Read  ");
      Serial.print(data);
      Serial.print(" instead of ");
      Serial.println(i);
      delay(error_delay);
    }

    delay(view_delay);

    clock_high();
    delay(pulse_width);
  }
}
