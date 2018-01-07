/*
  ESP8266 Blink by Simon Peter
  Blink the blue LED on the ESP-01 module
  This example code is in the public domain

  The blue LED on the ESP-01 module is connected to GPIO1
  (which is also the TXD pin; so we cannot use Serial.print() at the same time)

  Note that this sketch uses LED_BUILTIN to find the pin with the internal LED
*/

const int LED_RED  = LED_BUILTIN;
const int LED_BLUE = 2;
const int PIN_BTN  = 14; 
const int PIN_REL  = 12;

int ledState = 0xFF;
int pinState = 0xFF;

void setup() {
  Serial.begin(9600);
  delay(1);
  Serial.println("initializing");
  Serial.print("HIGH = ");
  Serial.println(HIGH, HEX);
      
  pinMode(LED_RED , OUTPUT);
  pinMode(PIN_REL , OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
  pinMode(PIN_BTN, INPUT);
  
  on();
  pinState = LOW;
}


void loop() {
  int current = digitalRead(PIN_BTN);
  if (current != pinState) {
    Serial.print("current = ");
    Serial.print(current, HEX);
    Serial.print(", pinState = ");
    Serial.println(pinState, HEX);

    if (current == HIGH && pinState == LOW)
      toggle();
    
    delay(50);
    pinState = current;  
  }
}

void on() {
  Serial.println("setting ON state");
  digitalWrite(LED_RED , LOW);  
  digitalWrite(PIN_REL , HIGH); 
  digitalWrite(LED_BLUE, HIGH);
  ledState = 1;
}

void off() {
  Serial.println("setting OFF state");
  digitalWrite(LED_RED , HIGH);
  digitalWrite(PIN_REL , LOW);
  digitalWrite(LED_BLUE, LOW);     
  ledState = 0;
}

void toggle() {
  Serial.print("toggle request, ledState = ");
  Serial.println(ledState, HEX);
  if (ledState == 0)
    on();
   else 
    off();
}
