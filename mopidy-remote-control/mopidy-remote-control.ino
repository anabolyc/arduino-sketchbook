#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <IRremoteESP8266.h>
#include "Buttons.h"
#include "wificreditnails.h"
#include "httpresult.h"

#define COMMANDS_COUNT 4
#define MOPIDY_HOST    "192.168.1.42"
#define MOPIDY_PORT    80

#define LEDS_COUNT     3
#define PIN_LED_RED   13 //D7
#define PIN_LED_GREEN 12 //D6
#define PIN_LED_BLUE  15 //D8

int RECV_PIN = 14;

IRrecv irrecv(RECV_PIN);
decode_results results;

CommandBind commands[COMMANDS_COUNT] = {
  { BTN_A, "/" },
  { BTN_B, "/B" },
  { BTN_C, "/C" },
  { BTN_D, "/D" }
};


int leds[LEDS_COUNT] = {
  PIN_LED_RED,
  PIN_LED_GREEN,
  PIN_LED_BLUE
};

void setup()
{
  for (int i = 0; i < LEDS_COUNT; i++)
    pinMode(leds[i], OUTPUT);

  Serial.begin(115200);
  irrecv.enableIRIn(); // Start the receiver
  Serial.println("IR started");

  Serial.print("Wifi connecting...");
  WiFi.mode(WIFI_STA);
  WiFi.begin(wifi_ssid, wifi_password);
}

bool wifi_displayed = 0;

void loop() {
  for (int i = 0; i < LEDS_COUNT; i++)
    digitalWrite(leds[i], LOW);

  if (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(10000);
  } else {
    if (!wifi_displayed) {
      Serial.print("IP: ");
      Serial.println(WiFi.localIP());
      wifi_displayed  = true;
    }

    if (irrecv.decode(&results)) {

      for (int i = 0; i < COMMANDS_COUNT; i++) {
        if (commands[i].button == results.value) {
          Serial.print("IR says: ");
          Serial.println(results.value, HEX);
          int req_result = doRequest(MOPIDY_HOST, commands[i].command, MOPIDY_PORT);
          switch (req_result) {
            case CONN_OK:
              digitalWrite(PIN_LED_GREEN, HIGH);
              break;
            case CONN_FAILED:
            case CONN_TIMEOUT:
              digitalWrite(PIN_LED_RED, HIGH);
              delay(500);
              break;
          }
        }
      }

      irrecv.resume(); // Receive the next value
    }
    delay(100);
  }
}

int doRequest(char* host, char* path, const int port) {
  Serial.print("connecting to ");
  Serial.println(host);

  WiFiClient client;
  if (!client.connect(host, port)) {
    Serial.println("connection failed");
    return CONN_FAILED;
  }

  Serial.print("Requesting URL: ");
  Serial.println(path);

  client.print(String("GET ") + path + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");

  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return CONN_TIMEOUT;
    }
  }

  // Read all the lines of the reply from server and print them to Serial
  while (client.available()) {
    String line = client.readStringUntil('\r');
    // Serial.print(line);
  }

  Serial.println();
  Serial.println("closing connection");
  return CONN_OK;
}
