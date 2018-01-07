#include <VirtualWire.h>

#define PIN_LED 1
#define PIN_RFT 3
//#define __SERIAL__

char *controller;

void setup() {
  #ifdef __SERIAL__
  Serial.begin(9600);
  #endif
  pinMode(PIN_LED, OUTPUT);
  vw_set_ptt_inverted(true);
  vw_set_tx_pin(PIN_RFT);
  
  vw_setup(4000);
}

void loop() {
  controller = "1" ;
  #ifdef __SERIAL__
  Serial.print(" > ");
  Serial.println(controller);
  #endif
  vw_send((uint8_t *)controller, strlen(controller));
  vw_wait_tx(); 
  
  digitalWrite(PIN_LED, HIGH);
  delay(100);
  
  controller = "0" ;
  #ifdef __SERIAL__
  Serial.print(" > ");
  Serial.println(controller);
  #endif
  vw_send((uint8_t *)controller, strlen(controller));
  vw_wait_tx(); 
  
  digitalWrite(PIN_LED, LOW);
  delay(100);
}
