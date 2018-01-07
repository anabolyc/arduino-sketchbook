#include <VirtualWire.h>

#define PIN_LED 13
#define PIN_RFT 3
#define PIN_RFR 4

char *controller;

void setup() {
  Serial.begin(9600);
  pinMode(PIN_LED, OUTPUT);
  vw_set_ptt_inverted(true);
  vw_set_tx_pin(PIN_RFT);
  vw_set_rx_pin(PIN_RFR);

  vw_setup(4000);
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
  } else {
    controller = "2";
    Serial.print(" > ");
    Serial.println(controller);
    vw_send((uint8_t *)controller, strlen(controller));
    vw_wait_tx();
    delay(100);
  }
}
