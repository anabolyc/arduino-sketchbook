#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

const int D3 = 0;
const int D4 = 2;
const int D5 = 14;
const int D6 = 12;
const int D7 = 13;

Adafruit_PCD8544 display = Adafruit_PCD8544(D7, D6, D5, D4, D3);

void setup() {
  display.begin();
  display.clearDisplay();
  Serial.begin(9600);

  Serial.println();
  Serial.print("LED_BUILTIN = ");
  Serial.println(LED_BUILTIN);
}

void loop() {
    display.setTextSize(1);
    display.setTextColor(WHITE, BLACK);
    display.println("Hello, world!");

    display.setTextColor(BLACK, WHITE);
    display.setTextSize(2);
    display.println(random(1000,10000));
    
    display.setTextSize(3);
    display.print(random(1000,10000));
    
    display.display();
    delay(200);
    display.clearDisplay();
}
