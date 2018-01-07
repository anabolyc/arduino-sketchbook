#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#define USE_SERIAL Serial

const char* ssid     = "wifi-12-private";
const char* password = "9263777101";

void setup() {

  USE_SERIAL.begin(115200);

  Serial.print("Connecting...");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

}

bool wifi_displayed = false;

void loop() {
  // wait for WiFi connection
  if (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
  } else {
    if (!wifi_displayed) {
      Serial.print("IP: ");
      Serial.println(WiFi.localIP());
      wifi_displayed  = true;
    }
    
    doRequest("192.168.1.42", "/", 80);
  }

  delay(10000);
}

void doRequest(char* host, char* path, const int port) {
  Serial.print("connecting to ");
  Serial.println(host);

  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  if (!client.connect(host, port)) {
    Serial.println("connection failed");
    return;
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
      return;
    }
  }

  // Read all the lines of the reply from server and print them to Serial
  while (client.available()) {
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }

  Serial.println();
  Serial.println("closing connection");
}
