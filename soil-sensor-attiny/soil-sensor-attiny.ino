#include <avr/sleep.h>
#include <avr/interrupt.h>

#define PIN_SENS_MOIS 3
#define PIN_POWR_MOIS 4
#define LED_PIN_INV    false

#define MIN_SENS_VALUE 200
#define MAX_SENS_VALUE 900

#define LED_ON_MD      0
#define LED_OFF_MD     8

const int PIN_MAP_LEN = 3;
const int PIN_MAP[] = {
  2, 1, 0
};

void setup() {
  pinMode(PIN_SENS_MOIS, INPUT);
  pinMode(PIN_POWR_MOIS, OUTPUT);

  digitalWrite(PIN_POWR_MOIS, LOW);

  for (int i = 0; i < PIN_MAP_LEN; i++) {
    pinMode(PIN_MAP[i], OUTPUT);
    led_off(PIN_MAP[i]);
  }
}

void loop() {
  int sens_value = map(get_sens_value(), MIN_SENS_VALUE, MAX_SENS_VALUE, 0, PIN_MAP_LEN);
  int led_value0 = constrain(sens_value, 0, PIN_MAP_LEN - 1);

  int led_value = PIN_MAP[led_value0];
  led_on(led_value);
  sleep(LED_ON_MD);
  led_off(led_value);
  sleep(LED_OFF_MD);
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

long get_sens_value() {
  digitalWrite(PIN_POWR_MOIS, HIGH);
  delay(1);
  long result = analogRead(PIN_SENS_MOIS);
  digitalWrite(PIN_POWR_MOIS, LOW);
  return result;
}

// =============== SLEEP MODE ===============
//volatile boolean f_wdt = 1;

#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

void sleep(int mode) {
  setup_watchdog(mode);
  cbi(ADCSRA, ADEN);                   // switch Analog to Digitalconverter OFF
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  sleep_mode();                        // System actually sleeps here
  sleep_disable();                     // System continues execution here when watchdog timed out
  sbi(ADCSRA, ADEN);                   // switch Analog to Digitalconverter ON
}

//  0 16  ms
//  1 32  ms
//  2 64  ms
//  3 128 ms
//  4 256 ms
//  5 512 ms
//  6 1024  ms
//  7 2048  ms
//  8 4096  ms
//  9 8192  ms
void setup_watchdog(int ii) {
  byte bb;
  int ww;
  if (ii > 9 ) ii = 9;
  bb = ii & 7;
  if (ii > 7) bb |= (1 << 5);
  bb |= (1 << WDCE);
  ww = bb;

  MCUSR &= ~(1 << WDRF);
  // start timed sequence
  WDTCR |= (1 << WDCE) | (1 << WDE);
  // set new watchdog timeout value
  WDTCR = bb;
  WDTCR |= _BV(WDIE);
}

// Watchdog Interrupt Service / is executed when watchdog timed out
ISR(WDT_vect) {
  //f_wdt = 1;
}

