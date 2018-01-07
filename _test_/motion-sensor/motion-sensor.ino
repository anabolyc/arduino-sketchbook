const int PIN_RED   =  3;
const int PIN_GREEN =  2;
const int PIN_MSENS =  4;


void setup() {
  pinMode(PIN_RED, OUTPUT);
  pinMode(PIN_GREEN, OUTPUT);
  pinMode(PIN_MSENS, INPUT);
}

void loop() {
  int sens_value = digitalRead(PIN_MSENS);
  digitalWrite(PIN_GREEN, sens_value == 0 ? HIGH : LOW );
  digitalWrite(PIN_RED  , sens_value == 0 ? LOW  : HIGH);
  delay(250);
}
