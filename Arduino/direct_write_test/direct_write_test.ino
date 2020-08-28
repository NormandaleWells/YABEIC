//
//  This only works on an Arduino Mega2560, since it assumes the following:
//
//  Port L is address 0x10B
//  The port L pins map to these Arduino pins:
//    0 : 49
//    1 : 48
//    2 : 47
//    3 : 46
//    4 : 45
//    5 : 44
//    6 : 43
//    7 : 42
//
//  Apparently, the Arduino designers wished the world were big-endian (:-).
//
//  I chose port L because its pins have no other uses.  Other possibilities:
//
//  Port A - bits 0 to 7 are pins 22 to 29 respectively
//  Port C - bits 0 to 7 are pins 37 to 30 respectively (backwards)
//  Port F - bits 0 to 7 are pins 54 to 61 respectively
//  Port K - bits 0 to 7 are pins 62 to 69 respectively
//
//  F and K have the advantage of being nicely lined up at the bottom of
//  the board.
//  Ports A, C, F, and K also have other uses, such as analog I/O,
//  communication, and the external memory interface.


byte * const PORT_L = reinterpret_cast<byte *>(0x10B);

void setup()
{
  Serial.begin(9600);

  pinMode(49, OUTPUT);
  pinMode(48, OUTPUT);
  pinMode(47, OUTPUT);
  pinMode(46, OUTPUT);
  pinMode(45, OUTPUT);
  pinMode(44, OUTPUT);
  pinMode(43, OUTPUT);
  pinMode(42, OUTPUT);
}

byte led = 0;

void show_value(byte value)
{
  for (int i = 0; i < 8; i++)
  {
    byte mask = 1 << i;
    digitalWrite(49-i, value & mask ? HIGH : LOW);
  }
}

void show_value_fast(byte value)
{
  *PORT_L = value;
}

void loop()
{
//  Serial.print("LED = ");
//  Serial.println(led);
  show_value_fast(led++);
  delay(50);
}
