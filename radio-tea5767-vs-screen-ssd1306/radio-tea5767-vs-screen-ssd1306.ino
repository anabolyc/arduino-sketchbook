#include <SPI.h>
#include <Wire.h>
#include <Arduino.h>

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_MOSI  D3
#define OLED_CLK   D0
#define OLED_DC    D6
#define OLED_CS    D7
#define OLED_RESET D5
Adafruit_SSD1306 display(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

#include <radio.h>
#include <TEA5767.h>

#define FIX_BAND RADIO_BAND_FM
#define FIX_STATION 10230
TEA5767 radio;

#include <ESP8266WiFi.h>

const char* WIFI_SSID = "wifi-12-private";
const char* WIFI_PASS = "9263777101";

void setup() {
  Serial.begin(9600);
  delay(10);

  radio.init();
  // DISPLAY SETUP
  display.begin(SSD1306_SWITCHCAPVCC);
  display.clearDisplay();

  // RADIO SETUP
  radio.setBandFrequency(FIX_BAND, FIX_STATION);
  radio.setVolume(1);
  radio.setMono(false);
  radio.debugEnable();
  
  // WIFI SETUP
  print("Connecting to ");
  println(WIFI_SSID);

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
}

bool wifi_displayed = 0;

void check_wifi() {
  if (WiFi.status() != WL_CONNECTED) {
    delay(500);
  } else {
    if (!wifi_displayed) {
      println("WiFi connected");
      Serial.println("IP address: ");
      Serial.println(WiFi.localIP());
      wifi_displayed = true;
    }
  }
}

void loop() { 
  check_wifi();

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);

  char s[12];
  radio.formatFrequency(s, sizeof(s));
  print("Station:");
  println(s);

  {
    RADIO_INFO info;
    radio.getRadioInfo(&info);

    print(info.rds    ? " RDS"    : " ---");
    print(info.tuned  ? " TUNED"  : " -----");
    print(info.stereo ? " STEREO" : "  MONO ");
    Serial.print("  RSSI: ");
    Serial.print(info.rssi);
    Serial.print("  SNR: ");
    Serial.print(info.snr);
    println();
  }

  {
    AUDIO_INFO info;
    radio.getAudioInfo(&info);

    print(info.bassBoost ? " BASS"  : " ----");
    print(info.mute      ? " MUTE"  : " ----");
    print(info.softmute  ? " SOFTMUTE"  : " --------");
    println();
  }

  display.display();
  delay(1000); 
}

void print(String s) {
  Serial.print(s);
  display.print(s);
}

void println(String s) {
  Serial.println(s);
  display.println(s);
}

void println() {
  Serial.println();
  display.println();
}
