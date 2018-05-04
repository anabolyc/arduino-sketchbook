
#define LED_A 3
#define LED_B 1
#define LED_C 3

void setup() {
  pinMode(LED_A, OUTPUT);
  pinMode(LED_B, OUTPUT);
  pinMode(LED_C, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(LED_A, HIGH);
  delay(1000);             
  digitalWrite(LED_A, LOW);

  digitalWrite(LED_B, HIGH);
  delay(1000);            
  digitalWrite(LED_B, LOW);
  
  digitalWrite(LED_C, HIGH);
  delay(1000);             
  digitalWrite(LED_C, LOW);
}
