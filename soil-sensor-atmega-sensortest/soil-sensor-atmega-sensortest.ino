#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SERIAL_SPEED 57600

#define PIN_SENS_MOIS A0
#define PIN_POWR_MOIS 2
#define LED_PIN_INV    false

#define MIN_SENS_VALUE 0
#define MAX_SENS_VALUE 1024

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);


void setup() {
  pinMode(PIN_SENS_MOIS, INPUT);
  pinMode(PIN_POWR_MOIS, OUTPUT);

  digitalWrite(PIN_POWR_MOIS, LOW);
  Serial.begin(SERIAL_SPEED);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
}

void loop() {
  int sens_value = get_sens_value();
  Serial.println(sens_value);
  print_value(sens_value);
  delay(100);
}


long get_sens_value() {
  digitalWrite(PIN_POWR_MOIS, HIGH);
  delay(1);
  long result = analogRead(PIN_SENS_MOIS);
  digitalWrite(PIN_POWR_MOIS, LOW);
  return result;
}

bool text_init = false;

void print_value(long value) {

  if (!text_init) {
    display.setTextSize(3);
    display.setTextColor(WHITE);
    text_init = true;
  }
  
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println(value);
  display.display();
}
