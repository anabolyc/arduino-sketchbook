
/*******************************************************************
    by David Sanz Kirbis

    Defines ports and pins used to connect an omnivision sensor+al422
    fifo camera module to an ATmega 328p Pro Mini Arduino clone board.
    The module tested with this setup has a 12MHz clock connected to
    the sensor XCLK and the following header / pinout:

                       GND --- 1     2 --- VCC3.3
                    OV_SCL --- 3     4 --- FIFO_WRST
                    OV_SDA --- 5     6 --- FIFO_RRST
                   FIFO_D0 --- 7     8 --- FIFO_OE
                   FIFO_D2 --- 9    10 --- FIFO_D1
                   FIFO_D4 --- 11   12 --- FIFO_D3
                   FIFO_D6 --- 13   14 --- FIFO_D5
                 FIFO_RCLK --- 15   16 --- FIFO_D7
                  FIFO_WEN --- 17   18 --- OV_VSYNC

    The definitions and implementation design is based on:
    arndtjenssen's ov7670_ports.h (Aug 18, 2013)
    see: https://github.com/arndtjenssen/ov7670

    2014 Aug 19 - Modified original file to be used with an
                  Atmega 328p
         Aug 21 - Changed pin mapping to put all data pins in
                  the same port, to save 1/3rd of the time employed
                  to read data from the fifo IC (i.e., to read each
                  whole byte in just one instruction).
                  The LS bits of the pixel data coming from the modules
                  are discarded in order to be able to communicate to
                  the 328p through the hardware UART TX and RX pins
                  and allow uploding Arduino sketches.
                  Also, interrupt pin INT0 is kept available.
                  The resulting image quality shows the expected
                  banding due to the reduction of depth but it is
                  fair enough for detection purposes.

     Wire the module as follows:

         OV+FIFO  ------  ATMEGA 328P (3.3V/8MHz) -------  ESPRESSIF ESP-32

             GND  ------  GND                     -------  GND
            3.3v  ------  VCC                     -------  3V3
         FIFO_OE  ------  GND                     -------  GND
        FIFO_WEN  ------  PB0/D8                  -------  GPIO19
       FIFO_RCLK  ------  PB1/D9 /PWM             -------  GPIO16
                  ------  PB2/D10/PWM
                  ------  PB3/D11/PWM
       FIFO_WRST  ------  PB4/D12                 -------  GPIO18
       FIFO_RRST  ------  PB5/D13                 -------  GPIO17
                          PB6 - crystal
                          PB7 - crystal
                  ------  PC0/A0
                  ------  PC1/A1
                  ------  PC2/A2
                  ------  PC3/A3
          OV_SDA  ------  PC4/A4/SDA
          OV_SCL  ------  PC5/A5/SCL
                          PC6/A6 - TQFP readonly
                          PC7/A7 - TQFP readonly
                          PD0/D0 - TX
                          PD1/D1 - RX
        OV_VSYNC  ------  PD2/D2/INT0             ------- GPIO23
         FIFO_D3  ------  PD3/D3/INT1/PWM
         FIFO_D4  ------  PD4/D4
         FIFO_D5  ------  PD5/D5/PWM
         FIFO_D6  ------  PD6/D6/PWM
         FIFO_D7  ------  PD7/D7
 ********************************************/

#ifndef IO_CONFIG_H_
#define IO_CONFIG_H_

#if (defined(__AVR__))
#include <avr/io.h>
#endif

//#define DEBUG             0

/*
// data pins --------------------
#define DATA_DDR	    DDRD
#define DATA_PORT	    PORTD
#define DATA_PINS	    PIND
*/

#define DATA_PINS	    get_data_pins_value() // GPIO_OUT1_REG  //PIND

// control pins --------------------
#if (defined(__AVR__))
#define VSYNC_INT_PIN      D2
#define VSYNC_INT 0
#else
#define VSYNC_INT_PIN      23
#define VSYNC_INT digitalPinToInterrupt(VSYNC_INT_PIN)
#endif

/*
#ifdef VSYNC_INT
#define OV_V2, 220SYNC      _BV(PIND2)
#else
#define OV_VSYNC            _BV(PINB0)
#endif
*/

#define FIFO_WREN_PIN      19
#define FIFO_WRST_PIN      18
#define FIFO_RRST_PIN      17
#define FIFO_RCLK_PIN      16

#define FIFO_DATA_D0       32
#define FIFO_DATA_D1       33
#define FIFO_DATA_D2       34
#define FIFO_DATA_D3       35

#define FIFO_DATA_D4       25
#define FIFO_DATA_D5       26
#define FIFO_DATA_D6       27
#define FIFO_DATA_D7       39

/*
#define FIFO_WREN          _BV(PINB0)          // Write Enable (active low) 
#define FIFO_RCLK          _BV(PINB1)          // Read clock
#define FIFO_WRST          _BV(PINB4)          // Write Reset (active low)
#define FIFO_RRST          _BV(PINB5)          // Read Reset (active low)

#define WREN_DDR          DDRB
#define WREN_PORT         PORTB

#define RCLK_DDR          DDRB
#define RCLK_PORT         PORTB

#define WRST_DDR          DDRB
#define WRST_PORT         PORTB

#define RRST_DDR          DDRB
#define RRST_PORT         PORTB

#ifdef VSYNC_INT
#define VSYNC_PIN         PIND
#define VSYNC_DDR         DDRD
#define VSYNC_PORT        PORTD
#else
#define VSYNC_PIN         PINB
#define VSYNC_DDR         DDRB
#define VSYNC_PORT        PORTB
#endif
*/

#define GET_VSYNC           digitalRead(VSYNC_INT_PIN)        //(VSYNC_PIN & OV_VSYNC)

#define DISABLE_RRST        digitalWrite(FIFO_RRST_PIN, HIGH) //RRST_PORT |= FIFO_RRST
#define ENABLE_RRST         digitalWrite(FIFO_RRST_PIN, LOW ) //RRST_PORT &= ~FIFO_RRST

#define DISABLE_WRST        digitalWrite(FIFO_WRST_PIN, HIGH) //WRST_PORT |= FIFO_WRST
#define ENABLE_WRST         digitalWrite(FIFO_WRST_PIN, LOW )  //WRST_PORT &= ~FIFO_WRST

#define SET_RCLK_H          digitalWrite(FIFO_RCLK_PIN, HIGH) //RCLK_PORT |= FIFO_RCLK
#define SET_RCLK_L          digitalWrite(FIFO_RCLK_PIN, LOW )  //RCLK_PORT &= ~FIFO_RCLK

#define ENABLE_WREN         digitalWrite(FIFO_WREN_PIN, HIGH) //WREN_PORT |= FIFO_WREN
#define DISABLE_WREN        digitalWrite(FIFO_WREN_PIN, LOW )  //WREN_PORT &= ~FIFO_WREN


// *************************************


uint8_t static inline get_data_pins_value() {
  uint8_t result = 0;
  result |= (uint8_t)(digitalRead(FIFO_DATA_D0) << 0);
  result |= (uint8_t)(digitalRead(FIFO_DATA_D1) << 1);
  result |= (uint8_t)(digitalRead(FIFO_DATA_D2) << 2);
  result |= (uint8_t)(digitalRead(FIFO_DATA_D3) << 3);

  result |= (uint8_t)(digitalRead(FIFO_DATA_D4) << 4);
  result |= (uint8_t)(digitalRead(FIFO_DATA_D5) << 5);
  result |= (uint8_t)(digitalRead(FIFO_DATA_D6) << 6);
  result |= (uint8_t)(digitalRead(FIFO_DATA_D7) << 7);

  #ifdef DEBUG
  Serial.print("# 0b");
  bool b0 = digitalRead(FIFO_DATA_D0);
  bool b1 = digitalRead(FIFO_DATA_D1);
  bool b2 = digitalRead(FIFO_DATA_D2);
  bool b3 = digitalRead(FIFO_DATA_D3);

  bool b4 = digitalRead(FIFO_DATA_D4);
  bool b5 = digitalRead(FIFO_DATA_D5);
  bool b6 = digitalRead(FIFO_DATA_D6);
  bool b7 = digitalRead(FIFO_DATA_D7);
  
  Serial.print(b0);
  Serial.print(b1);
  Serial.print(b2);
  Serial.print(b3);

  Serial.print(b4);
  Serial.print(b5);
  Serial.print(b6);
  Serial.print(b7);

  Serial.print(" = ");
  Serial.print(result, BIN);

  Serial.print(" = ");
  Serial.print(result, HEX);
  
  Serial.println();
  #endif
  
  return result;
}

void static inline setup_IO_ports() {
  /*
  // reset registers and register directions
  DDRB = DDRD = PORTB = PORTD = 0;
  // set fifo data pins as inputs
  DATA_DDR  = 0;  // set pins as INPUTS

  WREN_DDR  |= FIFO_WREN; // set pin as OUTPUT
  RCLK_DDR  |= FIFO_RCLK; // set pin as OUTPUT
  RRST_DDR  |= FIFO_RRST; // set pin as OUTPUT
  WRST_DDR  |= FIFO_WRST; // set pin as OUTPUT
  */

  pinMode(FIFO_DATA_D0, INPUT);
  pinMode(FIFO_DATA_D1, INPUT);
  pinMode(FIFO_DATA_D2, INPUT);
  pinMode(FIFO_DATA_D3, INPUT);

  pinMode(FIFO_DATA_D4, INPUT);
  pinMode(FIFO_DATA_D5, INPUT);
  pinMode(FIFO_DATA_D6, INPUT);
  pinMode(FIFO_DATA_D7, INPUT);

  pinMode(FIFO_RCLK_PIN, OUTPUT);
  pinMode(FIFO_WRST_PIN, OUTPUT);
  pinMode(FIFO_RRST_PIN, OUTPUT);
  pinMode(FIFO_WREN_PIN, OUTPUT);

  #ifdef VSYNC_INT
  pinMode(VSYNC_INT_PIN, INPUT_PULLUP);
  #else
  pinMode(VSYNC_INT_PIN, INPUT);
  #endif
}

#endif /* IO_CONFIG_H_ */
