#include <TinyWire.h>
#define led_pin 3

byte slave_address = 0x0a;

void setup() {
  pinMode( led_pin, OUTPUT);
  TinyWire.begin();
}

void loop() {
  digitalWrite(led_pin, HIGH);
  TinyWire.beginTransmission(slave_address);
  TinyWire.send('c');
  if (TinyWire.endTransmission() != 0) {
    digitalWrite(led_pin, HIGH);
  }
  digitalWrite(led_pin, LOW);

  delay(500);
}
