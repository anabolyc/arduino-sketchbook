#define SERIAL_SPEED 57600

/*
#include <Adafruit_GFX.h>
#include "Adafruit_SSD1306.h"
#include "Adafruit_INA219.h"


#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);
Adafruit_INA219 ina219;

void setup(void)
{
  Serial.begin(SERIAL_SPEED);
  while (!Serial) {
    // will pause Zero, Leonardo, etc until serial console opens
    delay(1);
  }

  uint32_t currentFrequency;

  Serial.println("Hello!");

  // Initialize the INA219.
  // By default the initialization will use the largest range (32V, 2A).  However
  // you can call a setCalibration function to change this range (see comments).
  ina219.begin();
  // To use a slightly lower 32V, 1A range (higher precision on amps):
  //ina219.setCalibration_32V_1A();
  // Or to use a lower 16V, 400mA range (higher precision on volts and amps):
  ina219.setCalibration_16V_400mA();

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  // Clear the buffer.
  display.clearDisplay();
  display.display();

  display.setTextSize(1);
  display.setTextColor(WHITE);

}

void loop(void)
{
  float shuntvoltage = 0;
  float busvoltage = 0;
  float current_mA = 0;
  float loadvoltage = 0;
  float power_mW = 0;

  shuntvoltage = ina219.getShuntVoltage_mV();
  busvoltage = ina219.getBusVoltage_V();
  current_mA = ina219.getCurrent_mA();
  power_mW = ina219.getPower_mW();
  loadvoltage = busvoltage + shuntvoltage / 1000;

  Serial.print("Bus Voltage:   "); Serial.print(busvoltage); Serial.println(" V");
  Serial.print("Shunt Voltage: "); Serial.print(shuntvoltage); Serial.println(" mV");
  Serial.print("Load Voltage:  "); Serial.print(loadvoltage); Serial.println(" V");
  Serial.print("Current:       "); Serial.print(current_mA); Serial.println(" mA");
  Serial.print("Power:         "); Serial.print(power_mW); Serial.println(" mW");
  Serial.println("");


  display.clearDisplay();

  display.setCursor(0, 0);
  display.print("Vb:");
  display.print(busvoltage);
  display.println(" V");

  display.setCursor(0, 10);
  display.print("Vs:");
  display.print(shuntvoltage);
  display.println("mV");

  display.setCursor(0, 20);
  display.print("Vl:");
  display.print(loadvoltage);
  display.println(" V");

  display.setCursor(64, 0);
  display.print("I:");
  display.print(current_mA);
  display.println(" mA");

  display.setCursor(64, 10);
  display.print("P:");
  display.print(power_mW);
  display.println(" mW");
  
  display.display();
  delay(50);
}
*/
