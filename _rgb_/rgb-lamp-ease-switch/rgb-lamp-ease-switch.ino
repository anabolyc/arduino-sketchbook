#include "rgb-lamp-ease-switch.h"

#define DEBUG true

const long MS_IN_SECOND = 1000;
const long FPS = 30;
const long DURATION_MS = MS_IN_SECOND * 1;
const long FRAME_DUR = MS_IN_SECOND / FPS;
const long FRAMES_CNT = DURATION_MS * FPS / MS_IN_SECOND;
const long MAX_DUTY_CYCLE = 255;

void setup() {
  pinMode(PIN_SWITCH, OUTPUT);
  //pinMode(PIN_LED, OUTPUT);

  pinMode(PIN_BTN, INPUT);

  digitalWrite(PIN_SWITCH, LOW);
  //digitalWrite(PIN_LED, LOW);

  if (DEBUG)
    Serial.begin(9600);
  delay(1);

  radio.begin();
  radio.openReadingPipe(0, RADIO_ADDR);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();

}

int cur_state = TURN_ON;
int cur_frame = 0;

int prev_btn = HIGH;

void loop() {

  if (radio.available()) {
    // CHECK RADIO COMMUNICATION
    bool ledOn;
    radio.read(&ledOn, sizeof(char));
    if (DEBUG) {
      Serial.print("RADIO: LED = ");
      Serial.println(ledOn);
    }

    if (cur_state == ON) {
      if (ledOn) {
        cur_state = TURN_OFF;
      }
    } else if (cur_state == OFF) {
      if (!ledOn) {
        cur_state = TURN_ON;
      }
    } else if (cur_state == TURN_ON) {
      if (ledOn) {
        cur_state = TURN_OFF;
        cur_frame = FRAMES_CNT - cur_frame;
      }
    } else if (cur_state == TURN_OFF) {
      if (!ledOn) {
        cur_state = TURN_ON;
        cur_frame = FRAMES_CNT - cur_frame;
      }
    }
  } else {
    // CHECK LOCAL BUTTON
    int btn = digitalRead(PIN_BTN);
    if (btn != prev_btn && btn == LOW) {
      if (cur_state == ON) {
        cur_state = TURN_OFF;
      } else if (cur_state == OFF) {
        cur_state = TURN_ON;
      } else if (cur_state == TURN_ON) {
        cur_state = TURN_OFF;
        cur_frame = FRAMES_CNT - cur_frame;
      } else if (cur_state == TURN_OFF) {
        cur_state = TURN_ON;
        cur_frame = FRAMES_CNT - cur_frame;
      }
    }
    prev_btn = btn;
  }

  long value = 0;
  switch (cur_state) {
    case TURN_ON:
      value = sinus_ease(++cur_frame, 0, FRAMES_CNT);
      analogWrite(PIN_SWITCH, value);
      //digitalWrite(PIN_LED, value > 0 ? HIGH : LOW);

      Serial.print("value = ");
      Serial.print(value, DEC);
      Serial.print("; frame = ");
      Serial.print(cur_frame, DEC);
      Serial.print(" out of ");
      Serial.println(FRAMES_CNT, DEC);

      if (cur_frame == FRAMES_CNT) {
        cur_state = ON;
        cur_frame = 0;
      }
      break;
    case TURN_OFF:
      value = sinus_ease(++cur_frame, FRAMES_CNT, 0);
      analogWrite(PIN_SWITCH, value);
      //digitalWrite(PIN_LED, value > 0 ? HIGH : LOW);

      Serial.print("value = ");
      Serial.print(value, DEC);
      Serial.print("; frame = ");
      Serial.print(cur_frame, DEC);
      Serial.print(" out of ");
      Serial.println(FRAMES_CNT, DEC);

      if (cur_frame == FRAMES_CNT) {
        cur_state = OFF;
        cur_frame = 0;
      }
      break;
  }

  if (DEBUG) {
    //Serial.print("cur_state = ");
    //Serial.println(cur_state, DEC);
  }

  delay(FRAME_DUR);
}

long linear_ease(long val, long min_v, long max_v) {
  return map(val, min_v, max_v, 0, MAX_DUTY_CYCLE);
}

long sinus_ease(long val, long min_v, long max_v) {
  return MAX_DUTY_CYCLE * sin(M_PI * abs(val - min_v) / abs(max_v - min_v) / 2);
}
