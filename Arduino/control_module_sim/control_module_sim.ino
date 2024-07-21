
#include <IC_595.h>
#include <Port_8.h>

// Multiplexed input signals
#define INPUT_MASK 0b0000000000000111

#define NI  0b0000000000000111 // no bus load
#define MI  0b0000000000000000 // MAR in
#define RI  0b0000000000000001 // RAM in
#define II  0b0000000000000010 // Inst. reg in
#define AI  0b0000000000000011 // A reg in
#define BI  0b0000000000000100 // B reg in
#define OI  0b0000000000000101 // Output reg in
#define J   0b0000000000000110 // PC in (jump)

// Other low-byte signals
#define SU  0b0000000000001000 // Subtract
#define CE  0b0000000000010000 // PC increment
#define FI  0b0000000000100000 // Flags in

// Multiplexed output signals
#define OUTPUT_MASK 0b0000011100000000
#define NO  0b0000011100000000 // no bus enable
#define TO  0b0000000000000000 // RAM out
#define RO  0b0000000100000000 // RAM out
#define IO  0b0000001000000000 // Inst. reg out
#define AO  0b0000001100000000 // A reg out
#define EO  0b0000010100000000 // Sum out
#define CO  0b0000011000000000 // PC out

// Other high-byte signals
#define TO  0b0100000000000000 // test rig out
#define HLT 0b1000000000000000 // Halt

#define UU  0b0111100011000000 // unused signals

#define INVERT (FI|TO)         // Active low signals

// Using pin 13 will also cause the built-in LED
// to flash at the same rate as our clock signal.
const int CLOCK_PIN = 13;

const int DELAY_TIME = 2000;

IC_595 sr;

void setup()
{
  Serial.begin(9600);
  Serial.println(__cplusplus);

  sr.int16_out(0);
  pinMode(CLOCK_PIN, OUTPUT);
}

uint8_t perform_cycle(uint16_t signals, uint8_t data_out)
{
  digitalWrite(CLOCK_PIN, LOW);
  sr.int16_out(signals);
  if ((signals & OUTPUT_MASK) == TO)
  {
    // write data to bus
  }
  delay(DELAY_TIME);

  digitalWrite(CLOCK_PIN, HIGH);
  delay(DELAY_TIME);
  // read data from bus
  return 0; // replace with data from bus
}

void loop()
{
  for (int i = 0; i < 8; i++)
  {
    uint16_t signals = 0b0000011100000000 + i;
    perform_cycle(signals, 0);
  }

  for (int i = 0; i < 5; i++)
  {
    uint16_t signals = 0b0000011100000111 + (1 << (i+3));
    perform_cycle(signals, 0);
  }

  for (int i = 0; i < 8; i++)
  {
    uint16_t signals = 0b0000000000000111 + (i << 8);
    perform_cycle(signals, 0);
  }

  for (int i = 0; i < 5; i++)
  {
    uint16_t signals = 0b0000011100000111 + (1 << (i+11));
    perform_cycle(signals, 0);
  }
}
