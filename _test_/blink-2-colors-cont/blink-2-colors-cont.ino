#define PIN_RED D0
#define PIN_BLU D4

void setup() {
  Serial.begin(9600);
  delay(5);
  
  pinMode(PIN_RED, OUTPUT);
  pinMode(PIN_BLU, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  Serial.println(PIN_BLU);
  
  digitalWrite(PIN_BLU, HIGH);
  digitalWrite(PIN_RED, LOW);
  delay(1000);                

  Serial.println(PIN_RED);
  
  digitalWrite(PIN_BLU, LOW); 
  digitalWrite(PIN_RED, HIGH);
  delay(1000);                
}
