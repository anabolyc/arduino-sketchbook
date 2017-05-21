const int PIN_LED = 2;

void setup() {
  pinMode(PIN_LED, OUTPUT);
}

void loop() {
  digitalWrite(PIN_LED, HIGH);  
  delay(2000);                      
  digitalWrite(PIN_LED, LOW);   
  delay(500);                       
}
