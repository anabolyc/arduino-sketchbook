// OUTPUT is pin PB3

unsigned long time = 0;

void setup() {
  Serial.begin(115200);
  Serial.println();
}

void loop() {
  unsigned long time0 = micros();
  Serial.print("+");
  Serial.println(time0 - time);
  time = time0;
  //delay(10);
}
