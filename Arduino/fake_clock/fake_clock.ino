
// Using pin 13 will also cause the built-in LED
// to flash at the same rate as our clock signal.
const int CLOCK_PIN = 13;

const int DELAY_TIME = 500;

void setup()
{
  Serial.begin(9600);
  Serial.println(__cplusplus);

  pinMode(CLOCK_PIN, OUTPUT);
}

void loop()
{
  digitalWrite(CLOCK_PIN, HIGH);
  delay(DELAY_TIME);
  digitalWrite(CLOCK_PIN, LOW);
  delay(DELAY_TIME);
}
