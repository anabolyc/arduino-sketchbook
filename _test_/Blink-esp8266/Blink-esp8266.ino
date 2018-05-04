#define LED_BUILTIN  2

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  Serial.println("LOW");
  digitalWrite(LED_BUILTIN, LOW);  
  delay(1750);                      
  Serial.println("HIGH");
  digitalWrite(LED_BUILTIN, HIGH); 
  delay(250);                      
}
