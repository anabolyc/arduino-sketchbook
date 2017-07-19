const int PINS_COUNT = 1;
const int PIN_START  = 13;

int curr_pin = 0;

void setup() {
  for (int i = PIN_START; i < PIN_START + PINS_COUNT; i++)
    pinMode(i, OUTPUT);
}

void loop() {
  int pin_number = PIN_START + (++curr_pin % PINS_COUNT);
  digitalWrite(pin_number, HIGH);
  delay(2000);
  digitalWrite(pin_number, LOW);
  delay(500);
}
