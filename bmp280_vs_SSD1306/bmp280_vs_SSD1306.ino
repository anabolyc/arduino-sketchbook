#include <SPI.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

Adafruit_BMP280 bmp; // I2C

#define OLED_MOSI   3
#define OLED_CLK   2
#define OLED_DC    5
#define OLED_CS    6
#define OLED_RESET 4

Adafruit_SSD1306 display(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2

#define LOGO16_GLCD_HEIGHT 16 
#define LOGO16_GLCD_WIDTH  16 

void setup() {
  Serial.begin(9600);
  Serial.println(F("BMP280 test"));

  if (!bmp.begin()) {
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring!"));
    while (1);
  }

  display.begin(SSD1306_SWITCHCAPVCC);
  display.display();
  delay(1000);
  display.clearDisplay();
  display.display();
}

void loop() {
  String result;
  // text display tests
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);

  float temp = bmp.readTemperature();
  Serial.print(F("Temperature = "));
  Serial.print(temp);
  Serial.println(" *C");
  result = String("T = ") + temp + String(" *C");
  display.println(result);

  float press_pa = bmp.readPressure();
  Serial.print(F("Pressure = "));
  Serial.print(press_pa);
  Serial.println(" Pa");
  
  display.setCursor(0, 12);
  result = String("P = ") + (long)press_pa + String(" Pa");
  display.println(result);

  float press_mm = press_pa * 0.00750061683;
  Serial.print(F("Pressure = "));
  Serial.print(press_mm);
  Serial.println(" mmhg");
  
  display.setCursor(0, 24);
  result = String("P = ") + press_mm + String(" mmHg");
  display.println(result);

  float alt = bmp.readAltitude(1013.25);
  Serial.print(F("Approx altitude = "));
  Serial.print(alt);
  Serial.println(" m");
  
  display.setCursor(0, 36);
  result = String("Alt = ") + alt + String(" m");
  display.println(result);

  Serial.println();
  display.display();
  delay(500);
  display.clearDisplay();
}
