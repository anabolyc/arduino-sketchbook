const int LED_PIN = 1;
const int BTN_PIN = 0;

void setup() {                
  pinMode(LED_PIN, OUTPUT); 
  pinMode(BTN_PIN, INPUT); 
}

void loop() {
  //digitalWrite(LED_PIN, HIGH);
  //delay(50);               
  //digitalWrite(LED_PIN, LOW); 
  //delay(950);               
  int value = digitalRead(BTN_PIN);
  
  digitalWrite(LED_PIN, value == LOW ? HIGH : LOW);
  delay(50);
}
