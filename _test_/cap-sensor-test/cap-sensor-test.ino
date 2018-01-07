#include <CapacitiveSensor.h>
#include <EEPROM.h>

#define PIN_SRC   2
#define PIN_DRAIN 4
#define PIN_LED   1

#define CAP_THRESHLD  1

CapacitiveSensor cs_4_2 = CapacitiveSensor(PIN_DRAIN, PIN_SRC);

int   address = 0;
byte tchCount = 0;

void setup() {
  //Serial.begin(9600);
  pinMode(PIN_LED, OUTPUT);

  byte storedCount = EEPROM.read(address);
  for (int i = 0; i < storedCount; i++) {
    digitalWrite(PIN_LED, HIGH);
    delay(50);
    digitalWrite(PIN_LED, LOW);
    delay(200);
  }
}

void loop() {
  long start = millis();
  long total = cs_4_2.capacitiveSensor(30);

  //Serial.print(millis() - start);
  //Serial.print("\t");
  //Serial.println(total);
  
  if (total >= CAP_THRESHLD) {
    digitalWrite(PIN_LED,  HIGH);
    tchCount++;
    EEPROM.update(address, tchCount);
  } else {
    digitalWrite(PIN_LED,  LOW);
  }

  delay(100);
}
