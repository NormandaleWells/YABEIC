
// AB_register_test - sketch to test the A and B registers

// Setup:
//
// Each register is implemented using a '574 and a '245.  The '245 outputs and
// '574 inputs are tied to the bus, which is also attached to ports F (output)
// and K (input) of an Arduino Mega.

// There are 5 control lines, attached to the output lines of a series of
// '595 shift registers, controlled through an IC_595 object.  The IC_595
// is set up according the default ctor.
 
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

    pinMode(clock_pin, OUTPUT);
}

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
      delay(2000);
    }
    else
    {
      delay(100);
    }

    clock_high();
    delay(100);
    clock_low();

    control_out(AREG_OUT_ | BREG_IN_);

    data = input_port.read();
    if (data != i)
    {
      Serial.print("ERROR 2: invalid data on A out.  Read  ");
      Serial.print(data);
      Serial.print(" instead of ");
      Serial.println(i);
      delay(2000);
    }
    else
    {
      delay(100);
    }

    clock_high();
    delay(100);
    clock_low();

    control_out(BREG_OUT_);

    data = input_port.read();
    if (data != i)
    {
      Serial.print("ERROR 3: invalid data on B out.  Read  ");
      Serial.print(data);
      Serial.print(" instead of ");
      Serial.println(i);
      delay(2000);
    }
    else
    {
      delay(100);
    }

    clock_high();
    delay(100);
  }
}
