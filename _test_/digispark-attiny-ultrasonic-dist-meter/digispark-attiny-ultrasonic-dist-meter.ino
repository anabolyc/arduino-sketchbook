#include <Ultrasonic.h>

#define PIN_LED       1
#define PIN_DIST_TRIG 2
#define PIN_DIST_ECHO 0

#define DIST_THRESHOLD 80

Ultrasonic ultrasonic(PIN_DIST_TRIG, PIN_DIST_ECHO);

void setup() {
  pinMode(PIN_LED, OUTPUT);
  ultrasonic.setTimeout(10000UL);
}

void loop() {
  unsigned long dist = ultrasonic.read();
  if (dist != 0) {
    int res = dist > DIST_THRESHOLD;
    digitalWrite(PIN_LED, res);
  }
  delay(100);
}
