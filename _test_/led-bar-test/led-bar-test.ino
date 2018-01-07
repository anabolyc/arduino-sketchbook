#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306_32.h>

const int PIN_MAP_LEN = 10;
const int PIN_MAP[] = { 
  10, 11, 9, 8, 7, 
   6,  5, 4, 3, 2
};
const int PIN_SENSOR = A6;

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

const int MIN_SENS_VALUE = 300;
const int MAX_SENS_VALUE = 900;

void setup() {
  Serial.begin(9600);

  pinMode(PIN_SENSOR, INPUT);

  for (int i = 0; i < PIN_MAP_LEN; i++) {
    pinMode(PIN_MAP[i], OUTPUT);
    digitalWrite(PIN_MAP[i], HIGH);
    delay(25);
  }

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
}


void loop() {
  int sensorValue = analogRead(PIN_SENSOR);
  Serial.println(sensorValue);
  
  int ledValue = constrain(
    map(sensorValue, MIN_SENS_VALUE, MAX_SENS_VALUE, 0, PIN_MAP_LEN)
  , 0, PIN_MAP_LEN);

  int pctValue = constrain(
    map(sensorValue, MIN_SENS_VALUE, MAX_SENS_VALUE, 100, 0)
  , 0, 100);
  
  for (int i = 0; i < PIN_MAP_LEN; i++)
    digitalWrite(PIN_MAP[i], i < ledValue ? HIGH : LOW);

  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(100, 0);
  display.print(sensorValue);
  
  display.setTextSize(4);
  display.setCursor(25, 5);
  display.print(pctValue);
  display.print("%");
  display.display();
  display.clearDisplay();

  delay(250);
}
