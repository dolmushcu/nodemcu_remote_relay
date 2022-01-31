#ifndef _LITTLEFS_
#define _LITTLEFS_

#include <Arduino.h>
#include <ArduinoJson.h>
#include <FS.h>
#include <LittleFS.h>
#include "wifi.h"

bool loadConfig();
bool saveConfig(String ssid, String pass, String host, String dir);

#endif