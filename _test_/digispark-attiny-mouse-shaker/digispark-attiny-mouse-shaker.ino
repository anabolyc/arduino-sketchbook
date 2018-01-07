#include <DigiMouse.h>
const int LED_PIN = 1;
const int SHIFT_PX = 3;

void setup() {
  DigiMouse.begin();
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  digitalWrite(LED_PIN, HIGH);
  
  DigiMouse.moveY(SHIFT_PX);
  DigiMouse.moveX(SHIFT_PX);
  DigiMouse.delay(10);
  
  DigiMouse.moveY(-2 * SHIFT_PX);
  DigiMouse.moveX(-2 * SHIFT_PX);
  DigiMouse.delay(10);

  DigiMouse.moveY(SHIFT_PX);
  DigiMouse.moveX(SHIFT_PX);
  DigiMouse.delay(10);
  
  digitalWrite(LED_PIN, LOW);
  
  DigiMouse.delay(30000);
}
