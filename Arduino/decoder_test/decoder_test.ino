// This sketch plays with a 74HC138 decoder.

const int A_pin = 2;
const int B_pin = 3;
const int C_pin = 4;


void setup()
{
  Serial.begin(9600);
  Serial.println("Starting");

  pinMode(A_pin, OUTPUT);
  pinMode(B_pin, OUTPUT);
  pinMode(C_pin, OUTPUT);

  pinMode(49, INPUT);
  pinMode(48, INPUT);
  pinMode(47, INPUT);
  pinMode(46, INPUT);
  pinMode(45, INPUT);
  pinMode(44, INPUT);
  pinMode(43, INPUT);
  pinMode(42, INPUT);

  digitalWrite(A_pin, LOW);
  digitalWrite(B_pin, LOW);
  digitalWrite(C_pin, LOW);
}

byte counter = 0;

byte * const PORT_L = reinterpret_cast<byte *>(0x10B);
byte * const PIN_L = reinterpret_cast<byte *>(0x109);

void show_counter(byte counter)
{
  digitalWrite(A_pin, ((counter & 1) != 0) ? HIGH : LOW);
  digitalWrite(B_pin, ((counter & 2) != 0) ? HIGH : LOW);
  digitalWrite(C_pin, ((counter & 4) != 0) ? HIGH : LOW);
}

void loop()
{
  show_counter(counter++);

  byte value = *PIN_L;
  Serial.println(value, HEX);
  delay(1000);
}
