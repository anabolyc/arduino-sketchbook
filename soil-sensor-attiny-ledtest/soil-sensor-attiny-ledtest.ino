#define LED_PIN_INV    false

const int PIN_MAP_LEN = 3;
const int PIN_MAP[] = {
  2, 0, 1
};

void setup() {
  for (int i = 0; i < PIN_MAP_LEN; i++) {
    pinMode(PIN_MAP[i], OUTPUT);
    led_off(PIN_MAP[i]);
  }
}

void loop() {
  for (int i = 0; i < PIN_MAP_LEN; i++) {
    led_on(PIN_MAP[i]);
    delay(500 * (i + 1));
    led_off(PIN_MAP[i]);
  }
}

void led_on(int pin) {
  led_state(pin, true);
}

void led_off(int pin) {
  led_state(pin, false);
}

void led_state(int pin, bool state) {
  if (LED_PIN_INV)
    digitalWrite(pin, !state);
  else
    digitalWrite(pin, state);
}

