// test_193
// This program was written to test whether I can use the '193 chips for the
// MAR in the 8080 implementation.  The problems I foresee with this are:
//
//    1.  The ~PL, CPD, and CPU signals all occur on the rising edge,
//        so I need to create pulses from the clk signals.
//    2.  The CPD and CPU signals need to remain high, and then taking
//        low and back high.

// Setup:
//
//    signal  Arduino 193   name
//    ------  ------- ---   ----
//    MR      pin 2   14    master reset
//    ~PL     pin 3   11    load
//    CPU     pin 4   5     clock up
//    CPD     pin 5   4     clock down

// The TCU pin (12) of each 193 is connected to the CPU of the next 193
// the TCD pin (13) of each 193 is connected to the CPD of the next 193
// All ~PL signals are connected.
// All MR signals are connected

const byte MR_pin  = 2;
const byte PL_pin  = 3;
const byte CPU_pin = 4;
const byte CPD_pin = 5;

void setup()
{
  Serial.begin(9600);

  pinMode(MR_pin,  OUTPUT);
  pinMode(PL_pin,  OUTPUT);
  pinMode(CPU_pin, OUTPUT);
  pinMode(CPD_pin, OUTPUT);

  digitalWrite(MR_pin,  LOW);
  digitalWrite(PL_pin,  HIGH);
  digitalWrite(CPU_pin, HIGH);
  digitalWrite(CPD_pin, HIGH);
}

void loop()
{
  digitalWrite(MR_pin,  HIGH);
  delay(100);
  digitalWrite(MR_pin,  LOW);
  for (long i = 0; i < 0x10000; i++)
  {
      digitalWrite(CPU_pin,  LOW);
      delay(1);
      digitalWrite(CPU_pin,  HIGH);
  }
  delay(10000);

  for (long i = 0; i < 0x10000; i++)
  {
      digitalWrite(CPD_pin,  LOW);
      delay(1);
      digitalWrite(CPD_pin,  HIGH);
  }
  delay(10000);

#if 0
  if (Serial.available() > 0) {
    byte b = Serial.read();
    
    if (b == 'r')
    {
      Serial.println("Reset");
      digitalWrite(MR_pin,  HIGH);
      delay(100);
      digitalWrite(MR_pin,  LOW);
    }
    else if (b == 'u')
    {
      Serial.println("Count up");
      digitalWrite(CPU_pin,  LOW);
      delay(100);
      digitalWrite(CPU_pin,  HIGH);
    }
    else if (b == 'd')
    {
      Serial.println("Count down");
      digitalWrite(CPD_pin,  LOW);
      delay(100);
      digitalWrite(CPD_pin,  HIGH);
    }
    else if (b == 'l')
    {
      Serial.println("Load");
      digitalWrite(PL_pin,  LOW);
      delay(100);
      digitalWrite(PL_pin,  HIGH);
    }
  }
#endif
}
