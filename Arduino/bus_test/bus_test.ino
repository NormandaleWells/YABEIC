
#include "Port_8.h"

// This is an initial test of reading and writing the databus
// from the Arduino.

// The output is using port F (pins 54 to 61), which is connected to the input
// through a '245 bus transceiver.  The output side of the '245 (the bus) is
// connected to port K.
// For now I'm just using port 53 to control the enable line on the '245, but
// later that will be driven by the full control mechanism through a series
// of cascaded '595 shift registers.

// The bus is pulled low by 1K resistors, so if there is no output through
// the '245, we should read 0.

const byte enable_pin = 53;

Port_8 output_port(54, 61, false);
Port_8  input_port(62, 69, true);

void setup()
{
  Serial.begin(9600);

  Serial.println("input:");
  input_port.debug();
  Serial.println("output:");
  output_port.debug();

  if (!output_port.is_valid())
    Serial.println("Output port is not valid!");
  if (!input_port.is_valid())
    Serial.println("Output port is not valid!");

  pinMode(enable_pin, OUTPUT);
  digitalWrite(enable_pin, HIGH);

  uint8_t bus_value = input_port.read();

  Serial.print("Initial port value: ");
  Serial.println(bus_value);
}

void loop()
{
  for (uint8_t i = 0; i < 256; i++)
  {
    output_port.write(i);

    uint8_t data = input_port.read();
    if (data != 0)
    {
      Serial.print("ERROR 1: enable high, but data is ");
      Serial.print(data);
      Serial.println(" instead of zero");
      delay(2000);
    }

    digitalWrite(enable_pin, LOW);
    data = input_port.read();
    if (data != i)
    {
      Serial.print("ERROR 2: enable is low, but data is ");
      Serial.print(data);
      Serial.print(" instead of ");
      Serial.println(i);
      delay(2000);
    }

    delay(50);

    digitalWrite(enable_pin, HIGH);
    data = input_port.read();
    if (data != 0)
    {
      Serial.print("ERROR 3: enable high, but data is ");
      Serial.print(data);
      Serial.println(" instead of zero");
      delay(2000);
    }

    delay(50);
  }
}
