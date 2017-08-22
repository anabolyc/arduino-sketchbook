#define DEBUG true
#include "rgb-lamp-constant-on.h"

long frameIndex = -1;
long frameDuration = 1000 / fps;
long transitionFrames = transitionDuration / frameDuration;
int btnState = 0xFF;
bool ledState = true;

unsigned int lastColor[3] = {0, 0, 0};
unsigned int nextColor[3] = {0, 0, 0};
unsigned int currColor[3] = {0, 0, 0};
unsigned int COL_BLACK[] = {0, 0, 0};

void setup() {
  pinMode(PIN_RED , OUTPUT);
  pinMode(PIN_GRN , OUTPUT);
  pinMode(PIN_BLU, OUTPUT);
  // pinMode(PIN_BTN, INPUT);

  randomSeed(analogRead(0));

  if (DEBUG) {
    Serial.begin(9600);
    delay(1);
  }
}

void loop() {
  bool ledState = true;
  
  if (ledState) {
    if (frameIndex >= transitionFrames || frameIndex == -1) {
      frameIndex = 0;
      copyColor(nextColor, lastColor);
      getRandomColor(nextColor);

      if (DEBUG) {
        printColor(lastColor, false);
        Serial.print("-> ");
        printColor(nextColor, false);
        Serial.print("\n");
      }
    }

    getCurrentColor(lastColor, nextColor, currColor, frameIndex, transitionFrames);
    setColor(currColor);

    if (DEBUG) {
      Serial.print(frameIndex);
      //Serial.print("\t");
      //Serial.print(transitionFrames);
      Serial.print("\t");
      printColor(currColor, true);
      Serial.print("\n");
    }

    frameIndex++;
    delay(frameDuration);
  } else {
    copyColor(COL_BLACK, nextColor);
    setColor(COL_BLACK);
    frameIndex = -1;
  }

}

void getRandomColor(unsigned int dest[]) {
  while (true) {
    unsigned int r = random(MAX_COL);
    unsigned int g = random(MAX_COL);
    unsigned int b = random(MAX_COL);
    long total = r + g + b;
    if (total > MAX_COL && total <= MAX_COL * 2) {
      unsigned int avg = total / 3;
      if (abs(avg - r) > MAX_COL / 2 || abs(avg - g) > MAX_COL / 2 || abs(avg - b) > MAX_COL / 2) {
        dest[0] = r;
        dest[1] = g;
        dest[2] = b;
        break;
      }
    }
  }
}

void getCurrentColor(unsigned int prev[], unsigned int next[], unsigned int dest[], long frameIndex, long frameCount) {
  for ( int i = 0; i < 3; i++) {
    unsigned int prev0 = prev[i];
    unsigned int next0 = next[i];
    unsigned int curr0 = 0;
    if (next0 >= prev0)
      curr0 = prev0 + (next0 - prev0) * frameIndex / frameCount;
    else
      curr0 = prev0 - (prev0 - next0) * frameIndex / frameCount;
    dest[i] = curr0;
  }
}

void copyColor(unsigned int from[], unsigned int to[]) {
  for (int i = 0; i < 3; i++)
    to[i] = from[i];
}

void setColor(unsigned int color[]) {
  analogWrite(PIN_RED, color[0]);
  analogWrite(PIN_GRN, color[1]);
  analogWrite(PIN_BLU, color[2]);
}

void printColor(unsigned int color[], bool simple) {
  if (!simple)
    Serial.print("(");
  long total = 0;
  for (int i = 0; i < 3; i++) {
    Serial.print(color[i]);
    if (i < 2)
      if (simple)
        Serial.print("\t");
      else
        Serial.print(", ");
    total += color[i];
  }
  if (!simple) {
    Serial.print(") => ");
    Serial.print(total);
  }
}
