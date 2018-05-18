#include "BME280.h"
Adafruit_BME280 bme;

#define BME280_ADDR 0x76
#define    I2C_ADDR 0x0a

#define PIN_LED_ERR 3
#define PIN_LED_OK  4 

void setup() {
  pinMode(PIN_LED_ERR, OUTPUT);
  pinMode(PIN_LED_OK, OUTPUT);
  
  digitalWrite(PIN_LED_ERR, HIGH);
  
  if (!bme.begin(BME280_ADDR)) {
    while (1) {
      digitalWrite(PIN_LED_ERR, LOW);
      delay(100);
      digitalWrite(PIN_LED_ERR, HIGH);
      delay(100);
    }
  }

  Wire.begin();

  digitalWrite(PIN_LED_ERR, LOW);
}

void loop() {

  digitalWrite(PIN_LED_OK, HIGH);

  uint16_t temp = bme.readTemperature() * 100;
  //uint16_t pres = bme.readPressure() / 10;
  //uint16_t humi = bme.readHumidity() * 100; 

  Wire.beginTransmission(I2C_ADDR);
  Wire.write((uint8_t *) temp, 2);
  Wire.endTransmission();

  //Wire.beginTransmission(I2C_ADDR);
  //Wire.write((uint8_t *) pres, 2);
  //Wire.endTransmission();

  //Wire.beginTransmission(I2C_ADDR);
  //Wire.write((uint8_t *) humi, 2);
  //Wire.endTransmission();

  digitalWrite(PIN_LED_OK, LOW);

  delay(1000);
}
