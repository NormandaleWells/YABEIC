
const byte CLOCK_PIN = 2;
const byte CLEAR_PIN = 3;
const byte ENABLE_PIN = 4;

void setup()
{
  Serial.begin(9600);

#if 0
  pinMode(49, INPUT);
  pinMode(48, INPUT);
  pinMode(47, INPUT);
  pinMode(46, INPUT);
  pinMode(45, INPUT);
  pinMode(44, INPUT);
  pinMode(43, INPUT);
  pinMode(42, INPUT);
#endif

  pinMode(CLOCK_PIN, OUTPUT);
  pinMode(CLEAR_PIN, OUTPUT);
  pinMode(ENABLE_PIN, OUTPUT);

  digitalWrite(ENABLE_PIN, LOW);
  digitalWrite(CLOCK_PIN, LOW);
  digitalWrite(CLEAR_PIN, HIGH);

  digitalWrite(CLEAR_PIN, LOW);
  digitalWrite(CLOCK_PIN, HIGH);
  digitalWrite(CLOCK_PIN, LOW);
  digitalWrite(CLEAR_PIN, HIGH);

  digitalWrite(ENABLE_PIN, HIGH);
}

void loop()
{
  digitalWrite(CLOCK_PIN, HIGH);
  delay(100);
  digitalWrite(CLOCK_PIN, LOW);
  delay(500);
}
