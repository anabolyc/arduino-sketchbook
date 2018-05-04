#include "Receiver.h"

#define RF_SPEED_BPS 984

#define SERIAL_SPEED 9600
#define __SERIAL__

#ifdef __AVR_ATmega328P__
#define PIN_LED 13
#define PIN_RFR 4
#endif
#ifdef __AVR_ATtiny85__
#define PIN_LED 1
#define PIN_RFR 3
#endif

#define MESSAGE_LEN 4
uint8_t CMD_ON [MESSAGE_LEN] = {"sa5"};
uint8_t CMD_OFF[MESSAGE_LEN] = {"xHM"};

Receiver receiver(PIN_RFR, RF_SPEED_BPS);

void setup()
{
#ifdef __SERIAL__
  Serial.begin(SERIAL_SPEED);
  Serial.println();
  Serial.println(">_<");
#endif
  receiver.start();
  pinMode(PIN_LED, OUTPUT);
}

long msg_count = 0;

bool flag = 0;

void loop()
{
    uint8_t buf[MESSAGE_LEN];
    uint8_t buflen = MESSAGE_LEN;

    if (receiver.message(buf, &buflen)) {
    #ifdef __SERIAL__
      Serial.print(" << ");
      Serial.print(msg_count++);
      Serial.print(" ");
      for (int i = 0; i < buflen; i++)
        Serial.print((char)buf[i]);
      Serial.println();
    #endif

      if (str_cmp(buf, CMD_ON)) {
        digitalWrite(PIN_LED, HIGH);
      }

      if (str_cmp(buf, CMD_OFF)) {
        digitalWrite(PIN_LED, LOW);
      }
    }
  /*
  
  if (!flag) {
    drop_settings();
    flag = true;
  } */
}

bool str_cmp(uint8_t* a, uint8_t* b) {
  for (int i = 0; i < MESSAGE_LEN; i++)
    if (a[i] != b[i])
      return false;
  return true;
}

void drop_settings() {
#ifdef __SERIAL__

  // RF speed
  Serial.print("RF_SPEED_BPS = ");
  Serial.println(RF_SPEED_BPS);

  //prescaler
  uint8_t b = TCCR0B;
  Serial.print("TCCR0B = ");
  Serial.print(b, DEC);
  Serial.print(" ");
  Serial.print(b, HEX);
  Serial.print(" ");
  Serial.print(b, BIN);
  Serial.println(" ");

  //nticks
  uint8_t a = OCR0A;
  Serial.print("OCR0A = ");
  Serial.print(a, DEC);
  Serial.print(" ");
  Serial.print(a, HEX);
  Serial.print(" ");
  Serial.print(a, BIN);
  Serial.println(" ");
#endif
}
