#define PIN_DIST_SENS A3
#define SERIAL_SPEED 57600

void setup() {
  Serial.begin(SERIAL_SPEED);
  pinMode(PIN_DIST_SENS, INPUT);
}

void loop() {
  uint16_t dist = analogRead(PIN_DIST_SENS);
  Serial.println(dist);
  delay(20);
}
