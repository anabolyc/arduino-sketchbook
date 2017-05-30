#include "pi_calc.c"

const int PIN_LED = 13;

void setup() {
  Serial.begin(9600);
  delay(100);

  pinMode(PIN_LED, OUTPUT);
  /*
    Serial.print("sizeof(int) = ");
    Serial.println(sizeof(int), DEC);

    Serial.print("sizeof(long) = ");
    Serial.println(sizeof(long), DEC);


    Serial.print("sizeof(long long) = ");
    Serial.println(sizeof(long long), DEC);

    Serial.print("sizeof(float) = ");
    Serial.println(sizeof(float), DEC);

    Serial.print("sizeof(double) = ");
    Serial.println(sizeof(double), DEC);
  */
  Serial.print("PI = 3.");
}

bool state = false;
long n = 1;
long r = 0;
long prev_c = 0;

void loop() {
  digitalWrite(PIN_LED, state);
  state = !state;

  if (n % 5 == 1) {
    r += 1;
    Serial.print("\n");
    if (r < 10)
      Serial.print(0);
    Serial.print(r);
    Serial.print(":");
  }

  long c = calc(n);
  int d0 = c / 1E8;
  Serial.print(d0, DEC);
  int d1 = c / 1E7 - 10 * d0;
  Serial.print(d1, DEC);
  n += 2;
  /*
    if (prev_c != 0) {
    long error = abs(prev_c - c * 100);
    Serial.print(" err:");
    Serial.println(error / 1e9);
    } */
  prev_c = c;
}
