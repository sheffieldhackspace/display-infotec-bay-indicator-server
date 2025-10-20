/**
 * Ethernet test - fetch
 *
 * fetch content from a web-page and print to serial1
 * copied from
 * https://docs.arduino.cc/tutorials/ethernet-shield-rev2/dns-web-client
 */

#include <Arduino.h>
#define W5500_ETHERNET_SHIELD
#include <Ethernet.h>
#include <SPI.h>

byte mac[] = {0x60, 0xF2, 0xEC, 0x18, 0x05, 0xFE};
char serverName[] = "server.alifeee.net";

EthernetClient client;

void setup() {
  Serial.begin(115200);

  // disable SD SPI
  pinMode(4, OUTPUT);
  digitalWrite(4, HIGH);

  // start ethernet
  Serial.println(F("Starting ethernet..."));
  if (!Ethernet.begin(mac))
    Serial.println(F("failed"));
  else {
    Serial.print(F("Connected to… "));
    Serial.print(Ethernet.localIP());
    Serial.print(F(" on "));
    Serial.println(Ethernet.gatewayIP());
  }

  // wait for ethernet to catch up
  delay(1000);

  // connect to server
  Serial.println("connecting...");
  if (client.connect(serverName, 80)) {
    Serial.println("connected");
    // Make a HTTP request:
    client.println("GET /bench/full.cgi");
    client.println();
  } else {
    Serial.println("connection failed");
  }
}

void loop() {
  // read and print any incoming bytes
  if (client.available()) {
    char c = client.read();
    Serial.print(c);
  }
  // stop if disconnected
  if (!client.connected()) {
    Serial.println();
    Serial.println("disconnecting.");
    client.stop();
    while (true)
      ;
  }
}
