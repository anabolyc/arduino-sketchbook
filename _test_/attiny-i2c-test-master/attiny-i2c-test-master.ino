#include <TinyWire.h>
#define Wire TinyWire

#define I2C_ADDR 0x0a
#define PIN_LED_ERR 3
#define PIN_LED_OK  4 

void setup() {
  pinMode(PIN_LED_ERR, OUTPUT);
  pinMode(PIN_LED_OK, OUTPUT);
  Wire.begin();
}

byte x = 0;

void loop() {
  digitalWrite(PIN_LED_OK, HIGH);

  Wire.beginTransmission(I2C_ADDR);
  Wire.write(x);
  Wire.endTransmission();

  x++;
  digitalWrite(PIN_LED_OK, LOW);

  delay(1000);
}
