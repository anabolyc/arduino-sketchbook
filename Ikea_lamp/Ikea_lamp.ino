#define DEBUG true
#define BUTTONS_COUNT 2

const int SW_DELAY_MS = 100;

const int PIN_SENS_1 = 2;
const int PIN_SENS_2 = 4;

const int PIN_SW_1   = 0;
const int PIN_SW_2   = 1;

typedef struct _btn_state {
  int pin_sensor;
  int pin_switch;
  int  _stored;
  int  _switch;
  int  _switch_state;
  long _time;
} btn_state;

btn_state buttons[BUTTONS_COUNT] = {
  {PIN_SENS_1, PIN_SW_1, LOW, LOW, 0, millis()},
  {PIN_SENS_2, PIN_SW_2, LOW, LOW, 0, millis()}
};

void setup() {
  for (int i = 0; i < BUTTONS_COUNT; i++) {
    pinMode(buttons[i].pin_sensor, INPUT);
    pinMode(buttons[i].pin_switch  , OUTPUT);
  }

  if (DEBUG) {
    Serial.begin(9600);
    delay(1);
  }
}

void loop() {
  for (int i = 0; i < BUTTONS_COUNT; i++)
    button_check(buttons + i);
}

void button_check(btn_state* state) {
  
  int current = digitalRead(state->pin_sensor);
  /*
  Serial.print(state->pin_sensor);
  Serial.print(" = ");
  Serial.print(current, DEC);
  */
  
  if (state->_stored == LOW && current == HIGH) {
    long time_c = millis();
    print("pressed: ");
    println(time_c - state->_time);
    
    state->_time = time_c;
    state->_stored = current;
  }

  if (state->_stored == HIGH && current == HIGH) {
      long time_c = millis();
      if (state->_switch == 0 && time_c - state->_time > SW_DELAY_MS) {
          print("switching ON/OFF ");

          state->_switch_state = !state->_switch_state;
          state->_switch = 1;
      }
  }
  
  if (state->_stored == HIGH && current == LOW) {
    long time_c = millis();
    print("released: ");
    println(time_c - state->_time);
    
    state->_time = time_c;
    state->_stored = current;
    state->_switch = 0;
  }

  /*
  Serial.print(" | ");
  Serial.print(state->pin_switch);
  Serial.print(" = ");
  Serial.println(state->_switch_state, DEC);*/
  digitalWrite(state->pin_switch, state->_switch_state);
}

void print(char* s) {
  if (DEBUG) {
    Serial.print(s);
  }  
}

void println(long i) {
  if (DEBUG) {
    Serial.println(i, DEC);
  }  
}

