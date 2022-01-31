#ifndef _WEBSERVER_
#define _WEBSERVER_

#include <ESP8266WebServer.h>
#include "littleFs.h"
#include "wifi.h"

extern ESP8266WebServer server;

void handleRoot();
void handleSave();
void handleNotFound();
String createHTML();

#endif