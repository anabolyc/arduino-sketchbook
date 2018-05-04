#include <Wire.h>

#define SERIAL_SPEED 57600
#define I2C_ADDR 0x0a

void setup() {
  Serial.begin(SERIAL_SPEED);
  Serial.println("...");

  Wire.begin(I2C_ADDR);
  Wire.onReceive(receiveEvent);
}

void loop() {
  delay(100);
}

void receiveEvent(int howMany) {
  while (Wire.available() > 0) {
    int x = Wire.read();
    Serial.println(x);
  }
}
