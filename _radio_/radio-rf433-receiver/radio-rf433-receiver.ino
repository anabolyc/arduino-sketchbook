#include <VirtualWire.h>

#define PIN_LED 13
#define PIN_RFR 4

#define RF_SPEED_BPS 1024

void setup()
{
  Serial.begin(9600);
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
    Serial.print(" < ");
    Serial.println((char)buf[0]);
    
    if (buf[0] == '1') {
      digitalWrite(PIN_LED, HIGH);
    }
    if (buf[0] == '0') {
      digitalWrite(PIN_LED, LOW);
    }
  }
}
