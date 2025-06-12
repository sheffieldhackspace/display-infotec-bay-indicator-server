/**
 * @file get_http.cpp
 * @author alifeee (alifeee.web@outlook.com)
 * @brief Fetches text from an HTTP endpoint and sends it over SPI
 * @version 0.1
 * @date 2024-04-17
 *
 * @copyright Use freely
 */
#include <Arduino.h>
#include <CH_AS1100.h>
#include <pins.h>

// panel length
#define NUM_CHIPS 32
Panel topRow = Panel(LOAD_PIN, NUM_CHIPS);

#include <ESP8266WiFiMulti.h>
ESP8266WiFiMulti wifiMulti;

#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

#include <secrets.h>

void showText(Panel &p, const char *msg);
char *convert(const std::string &str);

void setup() {
  // SETUP SERIAL
  Serial.begin(9600);
  while (!Serial) {
    delay(100);
  }
  Serial.print("Set up!");

  // SETUP wifi
  WiFi.mode(WIFI_STA);
  wifiMulti.addAP(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to wifi");
  while (wifiMulti.run() != WL_CONNECTED) {
    Serial.print(".");
    delay(100);
  }
  Serial.println();

  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());

  topRow.begin();
  showText(topRow, (char *)"connected to Wi-Fi!");
  delay(1000);
}

void loop() {
  // Check WiFi connection and reconnect if needed
  if (wifiMulti.run() != WL_CONNECTED) {
    Serial.println("Wifi connection lost");
  } else {
    WiFiClient client;
    HTTPClient http;
    Serial.print("[HTTP] begin...\n");

    http.begin(client, FILE_URL);

    Serial.printf("[HTTP] GET... %s\n", FILE_URL);

    int httpCode = http.GET();

    if (httpCode > 0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpCode);
      String payload = http.getString();
      Serial.println(payload);

      topRow.clearDisplay();
      showText(topRow, (char *)"fetched...");
      topRow.display();

      delay(1000);
      topRow.clearDisplay();
      showText(topRow, payload.c_str());
      topRow.display();
    } else {
      Serial.print("Error code: ");
      Serial.println(httpCode);
    }

    http.end();
  }
  delay(10000);
}

void showText(Panel &p, const char *msg) {
  p.setCursor(0, 0);
  p.setTextColor(1);
  p.setTextSize(1); // pixel size multiplier
  p.println(msg);
  p.display();
}
