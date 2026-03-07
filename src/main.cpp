#include <DotWidget.h>
#include <Arduino.h>
#include <ArduinoJson.h>
#include <Fonts/Org_01.h>
#include <WiFi.h>

#include "Credentials.h"

constexpr const char *HTTP_SEPARATOR = "\r\n\r\n";
constexpr size_t MAX_PAYLOAD_SIZE = 4096;
constexpr int SERVER_PORT = 80;

constexpr const char *RESPONSE_200 = "HTTP/1.1 200 OK\r\nContent-Length: 0\r\nConnection: close\r\n\r\n";
constexpr const char *RESPONSE_400 = "HTTP/1.1 400 Bad Request\r\nContent-Length: 0\r\nConnection: close\r\n\r\n";
constexpr const char *RESPONSE_413 = "HTTP/1.1 413 Payload Too Large\r\nContent-Length: 0\r\nConnection: close\r\n\r\n";

inline uint8_t getAlign(std::string_view text) {
  if (text == "left") return ALIGN_LEFT;
  if (text == "center") return ALIGN_CENTER;
  if (text == "right") return ALIGN_RIGHT;
  if (text == "top") return ALIGN_TOP;
  if (text == "middle") return ALIGN_MIDDLE;
  if (text == "bottom") return ALIGN_BOTTOM;

  return 0;
}

unsigned long last = 0;
WiFiServer server(SERVER_PORT);

#ifdef USE_OLED
#include <Adafruit_SSD1306.h>
Adafruit_SSD1306 display(128, 64, &Wire, -1);
#else
#include <AS1100.h>
AS1100 display1 = AS1100(D5, D6, D4);
AS1100 display2 = AS1100(D8, D7, D9);
#endif

DotWidget widget1(&display1);
DotWidget widget2(&display2);

void setup() {
  Serial.begin(115200);
  delay(1000);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

#ifdef USE_OLED
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
#else
  display1.begin();
  display2.begin();
#endif

  widget1.begin();
  widget2.begin();

  widget1.setAlign(ALIGN_LEFT | ALIGN_MIDDLE);
  widget1.setInverted(true);
  widget1.setText("SSID: " + WIFI_SSID);
  widget1.render();
  display1.display();

  widget2.setAlign(ALIGN_LEFT | ALIGN_MIDDLE);
  widget2.setInverted(true);
  widget2.setText("Connecting...");
  widget2.render();
  display2.display();

  while (!WiFi.isConnected()) {
    if (WiFiClass::status() == WL_DISCONNECTED) {
      WiFi.reconnect();
    }

    delay(1000);
  }

  server.begin();

  widget1.setAlign(ALIGN_LEFT | ALIGN_MIDDLE);
  widget1.setInverted(false);
  widget1.setText("SSID: " + WIFI_SSID + "\n");
  widget1.render();
  display1.display();

  widget2.setAlign(ALIGN_LEFT | ALIGN_MIDDLE);
  widget2.setInverted(false);
  widget2.setText(WiFi.localIP().toString() + ":" + SERVER_PORT);
  widget2.render();
  display2.display();
}

void loop() {
  if (auto client = server.accept()) {
    if (client.available() > MAX_PAYLOAD_SIZE) {
      client.print(RESPONSE_413);
      client.stop();
    } else if (client.available()) {
      client.find(HTTP_SEPARATOR);

      JsonDocument document;

      if (deserializeJson(document, client)) {
        client.print(RESPONSE_400);
        client.stop();
        return;
      }

      // top display
      widget1.setFlashing(document["top"]["flashing"] | false);
      widget1.setInverted(document["top"]["inverted"] | false);
      widget1.setSpeed(document["top"]["speed"] | 5);

      widget1.setAlign(getAlign(document["top"]["horizontal_align"] | "left") | ALIGN_MIDDLE);
      widget1.setImage(
        document["top"]["image"] | "",
        document["top"]["image_width"] | 0,
        document["top"]["image_height"] | 0
      );
      widget1.setText(document["top"]["text"] | "");

      // bottom display
      widget2.setFlashing(document["bottom"]["flashing"] | false);
      widget2.setInverted(document["bottom"]["inverted"] | false);
      widget2.setSpeed(document["bottom"]["speed"] | 5);

      widget2.setAlign(getAlign(document["bottom"]["horizontal_align"] | "left") | ALIGN_MIDDLE);
      widget2.setImage(
        document["bottom"]["image"] | "",
        document["bottom"]["image_width"] | 0,
        document["bottom"]["image_height"] | 0
      );
      widget2.setText(document["bottom"]["text"] | "");

      client.print(RESPONSE_200);
      client.stop();
    }
  }

  widget1.render();
  display1.display();
  widget1.advanceFrame();

  widget2.render();
  display2.display();
  widget2.advanceFrame();

  if (millis() - last >= 5000) {
    Serial.printf("Free: %dB, Min free: %dB\n", ESP.getFreeHeap(), ESP.getMinFreeHeap());
    last = millis();
  }
}
