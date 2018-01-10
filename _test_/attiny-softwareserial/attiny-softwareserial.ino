unsigned long time = 0;

void setup() {
  Serial.begin(9600);
}

void loop() {
  unsigned long time0 = micros();
  Serial.print("+");
  Serial.println(time0 - time);
  time = time0;
  //delay(10);
}
