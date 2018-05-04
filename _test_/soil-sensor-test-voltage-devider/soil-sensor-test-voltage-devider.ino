#define PIN_SENSOR A3
#define SERIAL_SPEED 57600

void setup() {
  pinMode(PIN_SENSOR, INPUT);
  Serial.begin(SERIAL_SPEED);
}

void loop() {
  long sens = analogRead(PIN_SENSOR);
  Serial.println(sens);
  delay(10);
}
