#include <Ultrasonic.h>

#define PIN_DIST_TRIG 3
#define PIN_DIST_ECHO 4
#define SERIAL_SPEED 57600

Ultrasonic ultrasonic(PIN_DIST_TRIG, PIN_DIST_ECHO);

void setup() {
  Serial.begin(SERIAL_SPEED);
  pinMode(PIN_DIST_TRIG, OUTPUT);
  pinMode(PIN_DIST_ECHO, INPUT);
  ultrasonic.setTimeout(40000UL);
}

void loop() {
  unsigned long dist = ultrasonic.distanceRead();
  if (dist  != 0) {
    Serial.print(dist);
    Serial.print("\n");
  }
  delay(10);
}
