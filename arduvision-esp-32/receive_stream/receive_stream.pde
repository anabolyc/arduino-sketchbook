/**************************************************************
 *
 *  Part of the ARDUVISION project
 *
 *  by David Sanz Kirbis
 *
 *  Receive the image and/or tracking data from a CMOS sensor
 *  controlled by an Arduino board.
 *  In the arduino side use the example sketch "ov_fifo_test.ino"
 *
 *  More info: http://www.arduvision.com
 **************************************************************/

import processing.serial.*;
import java.util.*;

Serial serialPort;     

request_t       request;
requestStatus_t reqStatus = requestStatus_t.IDLE;

// incoming serial 
byte[]   ackBuff = new byte[G_DEF.ACK_BUF_LEN];
byte[][] pix     = new byte[G_DEF.F_H][G_DEF.MAX_ROW_LEN + 1];

double waitTimeout    = 0;
double fpsTimeStamp   = 0;

PFont  myFont;
PImage currFrame;
PImage yuvFrame;
int frameIndex = 0;

int currRow = 0;

PrintWriter log;
boolean bSerialDebug = false;

// ************************************************************
//                          SETUP
// ************************************************************
void setup() {
  log = createWriter("last_log.txt");
  
  size(640, 507);
  frameRate(30);
  currFrame = createImage(G_DEF.F_W, G_DEF.F_H, RGB);
  yuvFrame  = createImage(G_DEF.F_W, G_DEF.F_H, RGB);

  // create a font with the third font available to the system:
  myFont = createFont("Arial", G_DEF.FONT_SIZE);
  textFont(myFont);

  //delay(500);
  serialPort = new Serial(this, "/dev/ttyUSB0", G_DEF.BAUDRATE);
  serialPort.bufferUntil(G_DEF.LF);
  serialPort.clear();
  delay(500);
  reqStatus = reqStatus.IDLE;
  request = request.STREAM1PPB;
}

void stop() {
  log.close();
}

// ************************************************************
//                          DRAW
// ************************************************************
void draw() {
  switch (request) {
  case NONE:        
    reqStatus = requestStatus_t.IDLE;
    background(0);
    drawInfo();
    break;
  case STREAM0PPB:
  case STREAM1PPB:
    switch (reqStatus) {
    case RECEIVED:
      reqStatus = requestStatus_t.PROCESSING;
      buff2pixFrame(pix, currFrame, yuvFrame, request);
      noSmooth();
      image(currFrame, 0, 0, G_DEF.F_W * G_DEF.DRAW_SCALE, G_DEF.F_H * G_DEF.DRAW_SCALE);
      frameIndex++;
      smooth();
      drawFPS();
      reqStatus = requestStatus_t.IDLE;
      break;
    case TIMEOUT:   
    case IDLE:      
      reqImage(request);
      reqStatus = requestStatus_t.REQUESTED; 
      waitTimeout = G_DEF.SERIAL_TIMEOUT + millis();
      break;
    case REQUESTED:  
      if (millis() > waitTimeout) reqStatus = requestStatus_t.TIMEOUT;
      break;
    case ARRIVING:  
      break;
    case PROCESSING:  
      break;
    default : 
      break;
    }   
    drawInfo();
    break;
  default :        
    break;
  }
}

// ************************************************************
//                     SERIAL EVENT HANDLER
// ************************************************************
void serialEvent(Serial serialPort) {

  if (reqStatus == requestStatus_t.REQUESTED) {
    int numBytesRead = serialPort.readBytesUntil(G_DEF.LF, ackBuff);
    if ((numBytesRead >= 4) && 
      (ackBuff[numBytesRead-4] == 'A') &&
      (ackBuff[numBytesRead-3] == 'C') &&
      (ackBuff[numBytesRead-2] == 'K') ) {
      reqStatus = requestStatus_t.ARRIVING;
    } else if (millis() > waitTimeout) {
      reqStatus = requestStatus_t.TIMEOUT;
    }
  } else if (reqStatus == requestStatus_t.ARRIVING) {
    parseSerialData();
  } else {
    if (bSerialDebug) 
      print(serialPort.readStringUntil(G_DEF.LF));
  }
}

// ************************************************************
//                       PARSE SERIAL DATA
// ************************************************************
void parseSerialData() {
  switch (request) {
  case NONE:         
    break;
  case STREAM0PPB:
  case STREAM1PPB:
    int bytesAvail = serialPort.available();
    if (bytesAvail >= request.getParam()) {
      int bytesRead = serialPort.readBytes(pix[currRow]);
      println("#", frameIndex, bytesRead, "bytes read, line: ", currRow);
      serialPort.clear();
      
      //if (request == STREAM0PPB) {
        log.print("#" + frameIndex + "\t" + currRow + "\t");
        for( int i = 0; i < request.getParam(); i++) {
          log.print(pix[currRow][i] + " ");
        }
        log.println();
      //}
      
      currRow++;
    } else {
      //println("#", bytesAvail, "out of", request.getParam(), "bytes available, line: ", currRow);
    }
    if (currRow >= G_DEF.F_H) {
      reqStatus = requestStatus_t.RECEIVED; // frame ready on buffer
      currRow = 0;
      if (bSerialDebug) 
        println();
    }
    break;
  default :       
    break;
  }
}

// ************************************************************
//                      REQUEST IMAGE
// ************************************************************
void reqImage(request_t req) {
  currRow = 0;
  serialPort.clear();
  serialPort.write("send " + Integer.toString(req.getParam())+G_DEF.LF);
}

// ************************************************************
//              CONVERT PIXEL STREAM BUFFER TO PIMAGE
// ************************************************************
void buff2pixFrame(byte[][] pixBuff, PImage dstImg, PImage yuvImg, request_t req) {

  int reqParam = req.getParam();
  dstImg.loadPixels();

  switch (req) {
  case STREAM0PPB: 
    for (int y = 0, l = 0, x = 0; y < G_DEF.F_H; y++, x = 0)
      while (x < reqParam) {

        if (x == 0) {
          int d = pixBuff[y][x];
          if (abs(d) > 50)
            x++;
        }
          
        int Y0 = int(pixBuff[y][x++]);
        int V  = int(pixBuff[y][x++]);
        int Y1 = int(pixBuff[y][x++]);
        int U  = int(pixBuff[y][x++]);

        yuvImg.pixels[l]   = color(Y0, U, V); 
        dstImg.pixels[l++] = YUV2RGB(Y0, U, V);
        yuvImg.pixels[l]   = color(Y1, U, V);
        dstImg.pixels[l++] = YUV2RGB(Y1, U, V);

        //print("#", frameIndex, "(", x, ",", y, ") = ");
        //log.print("#" + frameIndex + "\tx\t" + x + "\ty\t" + y);
        //println(hex(Y0), Y0, hex(Y1), Y1, hex(U), U, hex(V), V);
        //log.println("\tY0\t" + Y0 + "\tY1\t" + Y1 + "\tU\t" + U + "\tV\t" + V); 
      }
    break;
  case STREAM1PPB: 
    for (int y = 0, l = 0, x = 0; y < G_DEF.F_H; y++, x = 0)
      while (x < reqParam) {
        int Y0 = 0x08 | ((0x0f & int(pixBuff[y][x])) << 4);
        int U  = 0x00 | ((0xf0 & int(pixBuff[y][x++])));
        int Y1 = 0x08 | ((0x0f & int(pixBuff[y][x])) << 4);
        int V  = 0x00 | ((0xf0 & int(pixBuff[y][x++])));
        yuvImg.pixels[l]   = color(Y0, U, V);
        dstImg.pixels[l++] = YUV2RGB(Y0, U, V);
        yuvImg.pixels[l]   = color(Y1, U, V);
        dstImg.pixels[l++] = YUV2RGB(Y1, U, V);
      }
    break;
  }
  dstImg.updatePixels();
  yuvImg.updatePixels();
  log.flush();
}

// ************************************************************
//                     YUV TO RGB
// ************************************************************

color YUV2RGB(int Y, int U, int V) {
  float R = Y + 1.13983 * (V - 128);
  float G = Y - 0.39465 * (U - 128) - 0.58060 * (V - 128);
  float B = Y + 2.03211 * (U - 128);
  return color(R, G, B);
}

// ************************************************************
//
// ************************************************************
void keyPressed() {

  switch (key) {
  case ' ':  
    int i = request.ordinal()+1;
    if (i >= request_t.values().length) i=0;
    request = request_t.values()[i];
    serialPort.clear();
    reqStatus = requestStatus_t.IDLE;
    break; 
  case 's':  
    saveFrame(); 
    break; 
  default: 
    break;
  }
}

// ************************************************************
//
// ************************************************************
//void mouseMoved() {
//  text("x: " + mouseX + " y: " + mouseY, 20, 40);
//}

// ************************************************************
//                      DRAW SCREEN INFO
// ************************************************************
void drawInfo() {
  String modeStr = "MODE: " + request;
  pushStyle();
  pushMatrix();
  noStroke();
  fill(64);
  rect(0, height - G_DEF.FONT_BKG_SIZE, width, G_DEF.FONT_BKG_SIZE);
  fill(255);
  textAlign(LEFT, TOP);
  text(modeStr, 20, height - G_DEF.FONT_BKG_SIZE);

  if (request.getParam() != 0) {
    int coordsKoef = G_DEF.SCR_W / G_DEF.F_W;
    int picX = mouseX / coordsKoef;
    int picY = mouseY / coordsKoef;
    color p = currFrame.get(picX, picY);
    color p0 = yuvFrame.get(picX, picY);
    String coordsStr = "F:" + (frameIndex - 1) + " x:" + picX + " y:" + picY;
    String rgbString = "RGB:" + int(red(p)) + " " + int(green(p)) + " " + int(blue(p));
    String yuvString = "YUV:" + int(red(p0)) + " " + int(green(p0)) + " " + int(blue(p0));
    text(coordsStr, 200, height-G_DEF.FONT_BKG_SIZE);
    text(rgbString, 320, height-G_DEF.FONT_BKG_SIZE);
    text(yuvString, 450, height-G_DEF.FONT_BKG_SIZE);
  }

  popMatrix();
  popStyle();
}

// ************************************************************
//                           DRAW FPS
// ************************************************************
void drawFPS() {
  long currTime = millis();
  float fps =1000.0 / (float)(currTime - fpsTimeStamp);

  pushStyle();
  pushMatrix();
  noStroke();
  fill(0);
  rect(20, 20, textWidth("FPS: " + fps), G_DEF.FONT_SIZE);
  fill(255);
  translate(0, -2);
  textAlign(LEFT, TOP);
  text("FPS: " + fps, 20, 20);
  popMatrix();
  popStyle();
  fpsTimeStamp = currTime;
}