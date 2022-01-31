#include "littleFs.h"

bool loadConfig() {
  File configFile = LittleFS.open("/config.json", "r");
  if (!configFile) {
    Serial.println("Failed to open config file");
    return false;
  }

  size_t size = configFile.size();
  if (size > 1024) {
    Serial.println("Config file size is too large");
    return false;
  }

  // Allocate a buffer to store contents of the file.
  std::unique_ptr<char[]> buf(new char[size]);

  // We don't use String here because ArduinoJson library requires the input
  // buffer to be mutable. If you don't use ArduinoJson, you may as well
  // use configFile.readString instead.
  configFile.readBytes(buf.get(), size);

  StaticJsonDocument<600> doc;
  auto error = deserializeJson(doc, buf.get());
  if (error) {
    Serial.println("Failed to parse config file");
    return false;
  }

  if (doc["ssid"])
    strcpy(STA_SSID, (const char*)doc["ssid"]);
  if (doc["pass"])
    strcpy(STA_PASS, (const char*)doc["pass"]);
  if (doc["host"])
    strcpy(HOST, (const char*)doc["host"]);
  if (doc["dir"])
    strcpy(DIR, (const char*)doc["dir"]);
  
  return true;
}

bool saveConfig(String ssid, String pass, String host, String dir) {
  StaticJsonDocument<600> doc;
  doc["ssid"] = ssid;
  doc["pass"] = pass;
  doc["host"] = host;
  doc["dir"] = dir;

  File configFile = LittleFS.open("/config.json", "w");
  if (!configFile) {
    Serial.println("Failed to open config file for writing");
    return false;
  }

  serializeJson(doc, configFile);
  return true;
}