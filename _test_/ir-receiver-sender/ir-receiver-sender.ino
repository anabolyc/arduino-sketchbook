#include <IRremoteESP8266.h>

#define LED_RED  D0
#define PIN_IR_R D3
#define PIN_IR_T D2

IRrecv irrecv(PIN_IR_R);
IRsend irsend(PIN_IR_T); 
decode_results results;

unsigned int Samsung_power_toggle[71] = {38000,1,1,170,170,20,63,20,63,20,63,20,20,20,20,20,20,20,20,20,20,20,63,20,63,20,63,20,20,20,20,20,20,20,20,20,20,20,20,20,63,20,20,20,20,20,20,20,20,20,20,20,20,20,63,20,20,20,63,20,63,20,63,20,63,20,63,20,63,20,1798};

void setup() {

  Serial.begin(115200);
  
  irrecv.enableIRIn();
  irsend.begin();
  
  Serial.println("IR started");

}

void loop() {
  if (irrecv.decode(&results)) {
    Serial.print("IR says: ");
    Serial.println(results.value, HEX);

    irrecv.resume(); // Receive the next value
  }
  
  delay(100);

  irsend.sendGC(Samsung_power_toggle, 71);
}


