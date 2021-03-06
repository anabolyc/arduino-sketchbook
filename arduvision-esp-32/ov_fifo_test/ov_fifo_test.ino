/*********************************************
     Forked from ARDUVISION project
     by David Sanz Kirbis at http://www.arduvision.com
*********************************************/

#include "IO_config.h"
#include "sensor.h"
#include "fifo.h"
#include <Wire.h>

static const byte LF = 10;
static const unsigned long _BAUDRATE = 230400;
HardwareSerial *serialPtr = &Serial;

uint8_t rcvbuf[16], rcvbufpos = 0, c;

#define QQVGA
//#define QQQVGA

#ifdef QQVGA
static const uint8_t fW = 160;
static const uint8_t fH = 120;
static const frameFormat_t frameFormat = FF_QQVGA;
#else ifdef QQQVGA
static const uint8_t fW = 80;
static const uint8_t fH = 60;
static const frameFormat_t frameFormat = FF_QQQVGA;
#endif

static const uint8_t TRACK_BORDER = 4; // always multiple of 2 (YUYV: 2 pixels)
static const uint8_t YUYV_BPP = 2; // bytes per pixel
static const unsigned int MAX_FRAME_LEN = fW * YUYV_BPP;
byte rowBuf[MAX_FRAME_LEN];
unsigned int volatile nRowsSent = 0;
boolean volatile bRequestPending = false;
boolean volatile bNewFrame = false;

const String ASCII_ART = " .:-=+*#%@";

enum serialRequest_t {
  SEND_NONE = 0,
  SEND_DARK,
  //SEND_FPS,
  SEND_0PPB = MAX_FRAME_LEN,
  SEND_1PPB = fW,
  //SEND_2PPB = fW / 2,
  //SEND_4PPB = fW / 4,
  //SEND_8PPB = fW / 8
};

serialRequest_t serialRequest = SEND_NONE;


// *****************************************************
//                          SETUP
// *****************************************************
void setup()
{
  setup_IO_ports();

  serialPtr->begin(_BAUDRATE);
  serialPtr->println("Initializing sensor...");

  for (int i = 0; i < 10; i ++) {
    unsigned int result = sensor_init(frameFormat);
    if (result != 0) {
      serialPtr->print("inited OK, sensor PID: ");
      serialPtr->println(result, HEX);
      break;
    }
    else if (i == 5) {
      serialPtr->println("PANIC! sensor init keeps failing!");
      while (1);
    } else {
      serialPtr->println("retrying...");
      delay(300);
    }
  }

  attachInterrupt(VSYNC_INT, &vsyncIntFunc, FALLING);
  delay(100);
}

// *****************************************************
//                          LOOP
// *****************************************************
void loop() {
  while (serialPtr->available()) {

    c = serialPtr->read();
    if (c != LF) {
      rcvbuf[rcvbufpos++] = c;
    } else if (c == LF) {
      rcvbuf[rcvbufpos++] = 0;
      rcvbufpos = 0;
      parseSerialBuffer();
    }
  }
}

// *****************************************************
//               VSYNC INTERRUPT HANDLER
// *****************************************************
void __inline__ vsyncIntFunc() {

  // disable writing to fifo
  DISABLE_WREN;

  if (bRequestPending && bNewFrame) {
    detachInterrupt(VSYNC_INT);
    processRequest();
    bRequestPending = false;
    bNewFrame = false;
    attachInterrupt(VSYNC_INT, &vsyncIntFunc, FALLING);
  }
  else {
    ENABLE_WRST;
    _delayMicroseconds(1);
    SET_RCLK_H;
    _delayMicroseconds(1);
    SET_RCLK_L;
    DISABLE_WRST;
    _delay_cycles(10);

    ENABLE_WREN; // enable writing to fifo
    bNewFrame = true;
  }
}

// **************************************************************
//                      PROCESS SERIAL REQUEST
// **************************************************************
void processRequest() {
  fifo_rrst();

  switch (serialRequest) {

    case SEND_0PPB:
      for (int i = 0; i < fH; i++) {
        fifo_readRow0ppb(rowBuf, rowBuf + serialRequest);
        serialPtr->write(rowBuf, serialRequest);
        serialPtr->write(LF);
      }
      ///serialPtr->println("<<< SEND_0PPB");
      break;

    case SEND_1PPB:
      for (int i = 0; i < fH; i++) {
        fifo_readRow1ppb(rowBuf, rowBuf + serialRequest);
        serialPtr->write(rowBuf, serialRequest);
        serialPtr->write(LF);
      }
      ///serialPtr->println("<<< SEND_1PPB");
      break;

    default:
      break;
  }
}

// **************************************************************
//                      CALCULATE FPS
// **************************************************************
/*static unsigned int oneSecond = 1000;
  unsigned int volatile frameCount = 0;
  unsigned int fps = 0;
  unsigned long volatile lastTime = 0;
  unsigned long volatile timeStamp = 0;
*/
/*
  void calcFPS(unsigned int &currentFPS) {
  unsigned long currTime = millis();
  unsigned long currTimeDiff = currTime - lastTime;
  if (currTimeDiff >= oneSecond) {
    lastTime = currTime;
    currentFPS = (oneSecond * frameCount) / currTimeDiff;
    frameCount = 0;

  }
  while (GET_VSYNC); // wait for an old frame to end
  while (!GET_VSYNC);// wait for a new frame to start
  frameCount++;
  }*/

// *****************************************************
//               PARSE SERIAL BUFFER
// ****************************************************

void parseSerialBuffer(void) {
  //serialPtr->print(" << ");
  //serialPtr->println((char *)rcvbuf);

  if (strcmp((char *) rcvbuf, "hello") == 0) {
    serialPtr->print("Hello to you too!\n");
  }

  else if ( strlen((char *) rcvbuf) > 5 &&
            strncmp((char *) rcvbuf, "send ", 5) == 0) {
    serialRequest = (serialRequest_t)atoi((char *) (rcvbuf + 5));
    serialPtr->print("ACK\n");
    bRequestPending = true;
  }
  /*
    else if (strlen((char *) rcvbuf) > 5 &&
           strncmp((char *) rcvbuf, "dark ", 5) == 0) {
    thresh = atoi((char *) (rcvbuf + 5));
    serialPtr->print("ACK\n");
    serialRequest = SEND_DARK;
    bRequestPending = true;
    }
  */
  //else if (strlen((char *) rcvbuf) > 7 &&
  //         strncmp((char *) rcvbuf, "thresh ", 7) == 0) {
  //  serialPtr->print("ACK\n");
  //  thresh = atoi((char *) (rcvbuf + 7));
  //}
}
