#include <Wire.h>

#define SERIAL_SPEED 57600
#define I2C_ADDR 0x0a

void setup() {
  Serial.begin(SERIAL_SPEED);
  Serial.println("...");

  Wire.begin(I2C_ADDR);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);
}

void loop() {
  delay(100);
}

uint16_t x = 0;
uint16_t cnt = 0;

void receiveEvent(int howMany) {
  cnt++;
  int chars = 0;
  x = 0;
  while (Wire.available() > 0) {
    uint8_t c = Wire.read();
    x = (x << 8) | c;
    Serial.print(c, HEX);
    Serial.print(' ');
    chars++;
  }
  Serial.print(chars);
  Serial.print(' ');
  Serial.print(x);
  
  //if (cnt % 3 == 0)
    Serial.println();
  //else
  //  Serial.print(' ');
}

void requestEvent() {
  Serial.print("<< ");
  Serial.println(x);
  Wire.write(x);
}
