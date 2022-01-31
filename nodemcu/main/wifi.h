#ifndef _WIFI_
#define _WIFI_

#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include "littleFs.h"

extern WiFiEventHandler whenDisconnected;

extern IPAddress local_ip;
extern IPAddress gateway;
extern IPAddress netmask;

extern char* AP_SSID;
extern char* AP_PASS;

extern char STA_SSID[33];
extern char STA_PASS[65];

extern char HOST[70];
extern char DIR[70];

extern bool AP_MODE;

extern const int relay;
extern int count;

void switchToAp();
void autoConnect(int timeout = 30);
void fetch();
void disconnectedWifi(const WiFiEventStationModeDisconnected& event);

#endif