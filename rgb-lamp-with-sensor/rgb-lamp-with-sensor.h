#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

const int PIN_RED  = 9;
const int PIN_GRN  = 5;
const int PIN_BLU  = 6;

const int PIN_RADIO_CNS = 7;
const int PIN_RADIO_CE  = 8;

const int TCH_DELAY_MS = 100;
const int TCH_DELAY_NOJMP = 500;

const int PIN_LED    = 13;
const int PIN_SENSOR = 3;
const int PIN_RELAY  = 7;

RF24 radio(PIN_RADIO_CNS, PIN_RADIO_CE);

const uint64_t RADIO_ADDR = 0xF0F0F0F0AA;
