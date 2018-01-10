#include <VirtualWire.h>

#ifdef __AVR_ATmega328P__
#define PIN_LED 13
#define PIN_RFR 4
#endif
#ifdef __AVR_ATtiny85__
#define PIN_LED 1
#define PIN_RFR 3
#endif

#define RF_SPEED_BPS 1024

//#define __SERIAL__

void setup()
{
  #ifdef __SERIAL__
  Serial.begin(9600);
  #endif
  vw_set_ptt_inverted(true); // Required for DR3100
  vw_set_rx_pin(PIN_RFR);
  vw_setup(RF_SPEED_BPS);

  pinMode(PIN_LED, OUTPUT);
  vw_rx_start(); // Start the receiver PLL running
}

void loop()
{
  uint8_t buf[VW_MAX_MESSAGE_LEN];
  uint8_t buflen = VW_MAX_MESSAGE_LEN;

  if (vw_get_message(buf, &buflen)) // Non-blocking
  {
    #ifdef __SERIAL__
    Serial.print(" < ");
    Serial.println((char)buf[0]);
    #endif

    if (buf[0] == '1') {
      digitalWrite(PIN_LED, HIGH);
    }
    if (buf[0] == '0') {
      digitalWrite(PIN_LED, LOW);
    }
  }
}
