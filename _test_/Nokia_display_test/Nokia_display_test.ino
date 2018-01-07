#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

const int D3 = 3;
const int D4 = 4;
const int D5 = 5;
const int D6 = 6;
const int D7 = 7;

Adafruit_PCD8544 display = Adafruit_PCD8544(D7, D6, D5, D4, D3);

void setup() {
  display.begin();
  display.clearDisplay();
  Serial.begin(9600);

  Serial.println();
}

void loop() {
    display.setTextSize(1);
    display.setTextColor(WHITE, BLACK);
    display.println("Hello, world!");

    display.setTextColor(BLACK, WHITE);
    display.setTextSize(2);
    long rnd = random(1000,10000);
    display.println(rnd);
    Serial.println(rnd);

    rnd = random(1000,10000);
    display.setTextSize(3);
    display.print(rnd);
    Serial.println(rnd);
    
    display.display();
    delay(200);
    display.clearDisplay();
}
