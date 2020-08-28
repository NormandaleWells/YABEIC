// We're playing with the 74HC373 here.

// NOTE: There is an omission from the 373 datasheet - the counter
// is incremented on the high-low transition of the clock signal.
// This allows 1QD to be connected to 2CLK to create a single
// 8-bit counter.

const int CLK = 2;    // 1CLK input
const int CLR = 3;    // 1CLR / 2CLR input

void setup()
{
  Serial.begin(9600);
  Serial.println("Starting");

  pinMode(CLK, OUTPUT);
  pinMode(CLR, OUTPUT);

  pinMode(49, INPUT);
  pinMode(48, INPUT);
  pinMode(47, INPUT);
  pinMode(46, INPUT);
  pinMode(45, INPUT);
  pinMode(44, INPUT);
  pinMode(43, INPUT);
  pinMode(42, INPUT);

  digitalWrite(CLR, HIGH);
  digitalWrite(CLR, LOW);
}

byte * const PORT_L = reinterpret_cast<byte *>(0x10B);
byte * const PIN_L = reinterpret_cast<byte *>(0x109);

void loop()
{
  // Pulse the clock.  Note that the CLK signal is active
  // on the high-low transition.
  digitalWrite(CLK, LOW);
  digitalWrite(CLK, HIGH);

  byte value = *PIN_L;
  Serial.println(value, HEX);
  delay(100);
}
