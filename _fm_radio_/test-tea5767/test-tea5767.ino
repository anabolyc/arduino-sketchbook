#include <Arduino.h>
#include <Wire.h>
#include <radio.h>
#include <TEA5767.h>

#define FIX_BAND RADIO_BAND_FM
#define FIX_STATION 10230

TEA5767 radio;

void setup() {
  Serial.begin(9600);
  Serial.println("Radio...");
  delay(200);

  radio.init();
  radio.debugEnable();

  radio.setBandFrequency(FIX_BAND, FIX_STATION);
  radio.setVolume(2);
  radio.setMono(false);
}

void loop() {
  char s[12];
  radio.formatFrequency(s, sizeof(s));
  Serial.print("Station:"); 
  Serial.println(s);
  
  Serial.print("Radio:"); 
  radio.debugRadioInfo();
  
  Serial.print("Audio:"); 
  radio.debugAudioInfo();

  delay(3000);
}
