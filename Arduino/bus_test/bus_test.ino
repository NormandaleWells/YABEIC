
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

volatile uint8_t * port_F_data_reg   = reinterpret_cast<volatile uint8_t *>(0x31);
volatile uint8_t * port_F_dir_reg    = reinterpret_cast<volatile uint8_t *>(0x30);
volatile uint8_t * port_F_input_reg  = reinterpret_cast<volatile uint8_t *>(0x2F);

volatile uint8_t * port_K_data_reg   = reinterpret_cast<volatile uint8_t *>(0x108);
volatile uint8_t * port_K_dir_reg    = reinterpret_cast<volatile uint8_t *>(0x107);
volatile uint8_t * port_K_input_reg  = reinterpret_cast<volatile uint8_t *>(0x106);

void setup()
{
  Serial.begin(9600);
  pinMode(enable_pin, OUTPUT);
  digitalWrite(enable_pin, HIGH);

  // Configure port F
  pinMode(54, OUTPUT);
  pinMode(55, OUTPUT);
  pinMode(56, OUTPUT);
  pinMode(57, OUTPUT);
  pinMode(58, OUTPUT);
  pinMode(59, OUTPUT);
  pinMode(60, OUTPUT);
  pinMode(61, OUTPUT);

  // Configure port K
  pinMode(62, INPUT);
  pinMode(63, INPUT);
  pinMode(64, INPUT);
  pinMode(65, INPUT);
  pinMode(66, INPUT);
  pinMode(67, INPUT);
  pinMode(68, INPUT);
  pinMode(69, INPUT);

 uint8_t bus_value = *port_K_input_reg;

  Serial.print("Initial port value: ");
  Serial.println(bus_value);
}

void loop()
{
  for (uint8_t i = 0; i < 256; i++)
  {
    *port_F_data_reg = i;

    uint8_t data = *port_K_input_reg;
    if (data != 0)
    {
      Serial.print("ERROR 1: enable high, but data is ");
      Serial.print(data);
      Serial.println(" instead of zero");
      delay(10000);
    }

    digitalWrite(enable_pin, LOW);
    data = *port_K_input_reg;
    if (data != i)
    {
      Serial.print("ERROR 2: enable is low, but data is ");
      Serial.print(data);
      Serial.print(" instead of ");
      Serial.println(i);
      delay(10000);
    }

    delay(100);

    digitalWrite(enable_pin, HIGH);
    data = *port_K_input_reg;
    if (data != 0)
    {
      Serial.print("ERROR 3: enable high, but data is ");
      Serial.print(data);
      Serial.println(" instead of zero");
      delay(10000);
    }

    delay(100);
  }
}
