#include <TinyWireM.h>
#define Wire TinyWireM

#define I2C_ADDR 0x0a
#define PIN_LED_ERR 3
#define PIN_LED_OK  4

void setup() {
  pinMode(PIN_LED_ERR, OUTPUT);
  pinMode(PIN_LED_OK, OUTPUT);
  Wire.begin();
}

uint8_t x = 0;

void loop() {
  digitalWrite(PIN_LED_OK, HIGH);

  Wire.beginTransmission(I2C_ADDR);
  Wire.write(x);
  Wire.endTransmission();

  uint8_t res = Wire.requestFrom(I2C_ADDR, 1);
  if ( res == 0 ) {
    while (Wire.available()) {
      uint8_t value = Wire.read();
      digitalWrite(PIN_LED_ERR, value % 2 == 0);
    }
  } else {
    for (int i = 0; i < res; i++) {
      digitalWrite(PIN_LED_ERR, HIGH);
      delay(250);
      digitalWrite(PIN_LED_ERR, LOW);
      delay(250);
    }
  }

  x++;
  digitalWrite(PIN_LED_OK, LOW);

  delay(1000);
}
