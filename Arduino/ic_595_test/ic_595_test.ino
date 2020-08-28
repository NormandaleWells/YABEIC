// Sketch to test the IC_595 class.

#include <IC_595.h>

IC_595 sr;
unsigned long micros_time;

void setup()
{
  Serial.begin(9600);
  sr.int32_out(0);
  micros_time = micros();
  micros_time = micros() - micros_time;
  Serial.println(micros_time);
}

uint32_t data = 1;

void loop()
{
  unsigned long t = micros();
  sr.int32_out(data);
  t = micros() - t;
  Serial.println(t);
  data <<= 1;
  if (data == 0) data = 1;
  delay(1000);
}
