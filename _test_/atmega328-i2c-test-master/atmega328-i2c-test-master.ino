#include <Wire.h>

#define SERIAL_SPEED 57600
#define I2C_ADDR 0x0a
#define PIN_LED 13

void setup() {
  Serial.begin(SERIAL_SPEED);
  Serial.println("...");
  
  pinMode(PIN_LED, OUTPUT);
  Wire.begin();
}

byte x = 0;

void loop() {
  Serial.print(">");
  digitalWrite(PIN_LED, HIGH);
  
  Wire.beginTransmission(I2C_ADDR);
  Wire.write(x);
  Wire.endTransmission();

  x++;
  digitalWrite(PIN_LED, LOW);
  Serial.println("<");
  
  delay(1000);
}
