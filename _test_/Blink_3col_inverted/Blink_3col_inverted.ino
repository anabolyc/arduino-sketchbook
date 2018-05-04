const int PINS_COUNT = 3;
const int PIN_START  = 0;

int curr_pin = 0;

void setup() {
  for (int i = PIN_START; i < PIN_START + PINS_COUNT; i++) {
    pinMode(i, OUTPUT);
    digitalWrite(i, HIGH);
  }
}

void loop() {
  int pin_number = PIN_START + (++curr_pin % PINS_COUNT);
  digitalWrite(pin_number, LOW);
  delay(10);
  digitalWrite(pin_number, HIGH);
  delay(500);
}
