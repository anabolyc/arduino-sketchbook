#include "BME280.h"
Adafruit_BME280 bme;
#define BME280_ADDR  0x76

#define SERIAL_SPEED 57600
#define PIN_LED 13

void setup() {
  Serial.begin(SERIAL_SPEED);
  Serial.println("...");

  if (!bme.begin(BME280_ADDR)) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1) {
      digitalWrite(PIN_LED, LOW);
      delay(100);
      digitalWrite(PIN_LED, HIGH);
      delay(100);
    }
  }

}

void loop() {
  uint16_t temp = bme.readTemperature() * 100;
  uint16_t pres = bme.readPressure() / 10;
  uint16_t humi = bme.readHumidity() * 100;

  Serial.print(temp);
  Serial.print("\t");
  Serial.print(pres);
  Serial.print("\t");
  Serial.print(humi);
  Serial.println();
  delay(200);
}
