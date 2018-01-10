
#define PIN_COL0B       _BV(PINB0)
#define PIN_COL1B       _BV(PINB1)
#define PIN_COL2B       _BV(PINB4)

volatile uint8_t* Port[] = {&OCR0A, &OCR0B, &OCR1B};

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
#define COL_FPS         60
#define COL_DURATION    9000

long frameDuration = 1000 / COL_FPS;
long transitionFrames = COL_DURATION / frameDuration;

void setup() {
  // Pin mode - output
  DDRB = DDRB | PIN_COL0B | PIN_COL1B | PIN_COL2B;

  // Setup all PWMs
  TCCR0A = PWM_MODE<<COM0A0 | PWM_MODE<<COM0B0 | PWM_FAST_A<<WGM00;
  TCCR0B = PWM_FAST_B<<WGM02 | PWM_CLK_64_A<<CS00;
  GTCCR  = PWM_B_ENABLE<<PWM1B | PWM_OC1B_ENABLE<<COM1B0;
  TCCR1  = PWM_OC1A_ENABLE<<COM1A0 | PWM_CLK_64_B<<CS10;
}

long frameIndex = -1;

unsigned int lastColor[3] = {0, 0, 0};
unsigned int nextColor[3] = {0, 0, 0};
unsigned int currColor[3] = {0, 0, 0};
unsigned int COL_BLACK[] = {0, 0, 0};

void loop() {
  bool ledState = true;
  
  if (ledState) {
    if (frameIndex >= transitionFrames || frameIndex == -1) {
      frameIndex = 0;
      copyColor(nextColor, lastColor);
      getRandomColor(nextColor);
    }

    getCurrentColor(lastColor, nextColor, currColor, frameIndex, transitionFrames);
    setColor(currColor);

    frameIndex++;
    delay(frameDuration);
  } else {
    copyColor(COL_BLACK, nextColor);
    setColor(COL_BLACK);
    frameIndex = -1;
  }
}

void setColor(unsigned int color[]) {
  for ( int i = 0; i < 3; i++)
    *Port[i] = color[i];
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

void getRandomColor(unsigned int dest[]) {
  while (true) {
    unsigned int r = random(COL_MAX);
    unsigned int g = random(COL_MAX);
    unsigned int b = random(COL_MAX);
    long total = r + g + b;
    if (total > COL_MAX && total <= COL_MAX * 2) {
      unsigned int avg = total / 3;
      if (abs(avg - r) > COL_MAX / 2 || abs(avg - g) > COL_MAX / 2 || abs(avg - b) > COL_MAX / 2) {
        dest[0] = r;
        dest[1] = g;
        dest[2] = b;
        break;
      }
    }
  }
}

