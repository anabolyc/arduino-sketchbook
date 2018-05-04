#include <VirtualWire.h>
#include <avr/pgmspace.h>

#ifdef __AVR_ATmega328P__
  #define __SERIAL__
  #define SERIAL_SPEED 115200
  #define PIN_LED 13
  #define PIN_RFT 3
#endif
#ifdef __AVR_ATtiny85__
  #define PIN_LED 0
  #define PIN_RFT 3
#endif

#define RF_SPEED_BPS 984
#define ON_DELAY  4000
#define OFF_DELAY 2000

#define MESSAGE_LEN 4
uint8_t CMD_ON [MESSAGE_LEN] = {"sa5"};
uint8_t CMD_OFF[MESSAGE_LEN] = {"xHM"};

void setup() {
  #ifdef __SERIAL__
  Serial.begin(SERIAL_SPEED);
  #endif
  pinMode(PIN_LED, OUTPUT);
  vw_set_ptt_inverted(true);
  vw_set_tx_pin(PIN_RFT);
  
  vw_setup(RF_SPEED_BPS);
}

long msg_count = 0;

void loop() {
  #ifdef __SERIAL__
  Serial.print(" >> ");
  Serial.print(msg_count++);
  Serial.print(" ");
  Serial.println((char*)CMD_ON);
  #endif
  
  vw_send((uint8_t *)CMD_ON, MESSAGE_LEN);
  vw_wait_tx(); 
  
  digitalWrite(PIN_LED, HIGH);
  delay(ON_DELAY);
  
  #ifdef __SERIAL__
  Serial.print(" >> ");
  Serial.print(msg_count++);
  Serial.print(" ");
  Serial.println((char*)CMD_OFF);
  #endif
  vw_send((uint8_t *)CMD_OFF, MESSAGE_LEN);
  vw_wait_tx(); 
  
  digitalWrite(PIN_LED, LOW);
  delay(OFF_DELAY);
}
