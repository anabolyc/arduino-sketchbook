#include <VirtualWire.h>
#define PIN_LED 1
#define PIN_RFR 3

#define RF_SPEED_BPS 1024
volatile uint8_t* Port[] = {&OCR0A, &OCR0B, &OCR1B};

#define PIN_COL0B       _BV(PINB0)
#define PIN_COL1B       _BV(PINB1)
#define PIN_COL2B       _BV(PINB4)

#define PWM_DIRECT      B10
#define PWM_INVERT      B11
#define PWM_MODE        PWM_DIRECT

#define PWM_FAST_A      B11
#define PWM_FAST_B      B0
#define PWM_B_ENABLE    B1
#define PWM_CLK_64_A    B011
#define PWM_CLK_64_B    B111
#define PWM_OC1A_ENABLE B11
#define PWM_OC1B_ENABLE B11

#define COL_MAX         255
#define FRAME_DURATION 5

void setup()
{
  // Pin mode - output
  DDRB = DDRB | PIN_COL0B | PIN_COL1B | PIN_COL2B;
  // Setup all PWMs
  TCCR0A = PWM_MODE<<COM0A0 | PWM_MODE<<COM0B0 | PWM_FAST_A<<WGM00;
  TCCR0B = PWM_FAST_B<<WGM02 | PWM_CLK_64_A<<CS00;
  //GTCCR  = PWM_B_ENABLE<<PWM1B | PWM_OC1B_ENABLE<<COM1B0;
  //TCCR1  = PWM_OC1A_ENABLE<<COM1A0 | PWM_CLK_64_B<<CS10;
  
  vw_set_ptt_inverted(true); // Required for DR3100
  vw_set_rx_pin(PIN_RFR);
  vw_setup(RF_SPEED_BPS);
  vw_rx_start(); // Start the receiver PLL running
}

int ledState = 1;

void loop()
{
  uint8_t buf[VW_MAX_MESSAGE_LEN];
  uint8_t buflen = VW_MAX_MESSAGE_LEN;

  if (vw_get_message(buf, &buflen)) 
  {
    if (buf[0] == '1') {
      ledState = 1;
    }
    if (buf[0] == '0') {
      ledState = 0;      
    }
  }

  int value = ledState ==0 ? 0 : COL_MAX;
  setColor(0,  value);
}
/*

int j = 0;
int i = 0;

void loop() {
  setColor(0, ledState == 0 ? 0 : COL_MAX);
  if (ledState) {
      if (j == 3) {
        j = 0;
      } else {
        if (i == COL_MAX) {
          i = 0;
          setColor(j, 0);
          j++;  
        } else {
          setColor(j, i);
          delay(FRAME_DURATION);  
          i++;
        }
      }
  }
}
*/

void setColor(unsigned int index, unsigned int color) {
  switch (index) {
    case 0:
    case 1:
      *Port[index] = COL_MAX - color;
      break;
    case 2:
      *Port[index] = color;
  }
}

