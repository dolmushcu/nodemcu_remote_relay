#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <ESP8266WebServer.h>
#include <FS.h>
#include <LittleFS.h>
#include "wifi.h"
#include "littleFs.h"
#include "webServer.h"

int disconnectedCount = 0;
unsigned long disconnectedTime = 0;
unsigned int disconnectedTreshold = 300000; // 5 minutes = 300000 millisecond

void setup() {
  pinMode(relay, OUTPUT); 
  digitalWrite(relay, HIGH);
  
  Serial.begin(115200);
  
  Serial.println("Mounting FS...");
  if (!LittleFS.begin()) {
    Serial.println("Failed to mount file system");
    ESP.restart();
  }

  whenDisconnected = WiFi.onStationModeDisconnected(disconnectedWifi);

  server.on("/", handleRoot);
  server.on("/save", handleSave);
  server.onNotFound(handleNotFound);
  server.begin();

  autoConnect();
}

void loop() {
  // Serve the web server
  server.handleClient();

  if(WiFi.isConnected())
    fetch();
  else {
    if(disconnectedCount = 0) {
      disconnectedCount++;
      disconnectedTime = millis();
    }
    if(millis() - disconnectedTime > disconnectedTreshold) {
      Serial.println("Esp is disconnected from wifi for a long time. Restarting...");
      ESP.restart();
    }
  }
}