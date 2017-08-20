#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

const int PIN_BTN = 2;
const int PIN_SWITCH = 3;
//const int PIN_LED    = LED_BUILTIN;

const int PIN_RADIO_CNS = 7;
const int PIN_RADIO_CE  = 8;

const int ON = 2;
const int TURN_ON = 1;
const int TURN_OFF = -1;
const int OFF = -2;

RF24 radio(PIN_RADIO_CNS, PIN_RADIO_CE);

const uint64_t RADIO_ADDR = 0xF0F0F0F0AA;
