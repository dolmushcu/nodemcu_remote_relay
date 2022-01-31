#include "webServer.h"

ESP8266WebServer server(80);

void handleRoot() {
  server.send(200, "text/html", createHTML());
}
void handleSave() {
  String ssid = server.hasArg("ssid") ? server.arg("ssid") : STA_SSID;
  String pass = server.hasArg("pass") ? server.arg("pass") : STA_PASS;
  String host = server.hasArg("host") ? server.arg("host") : HOST;
  String dir = server.hasArg("dir") ? server.arg("dir") : DIR;

  if (saveConfig(ssid, pass, host, dir))
  {  
    server.send(200, "text/plain", "Your configrations are saved. The module is restarting...");
    delay(500);
  }
  else {
    server.send(500);
    Serial.println("Couldn't save configrations.");
  }
  ESP.restart();
}

void handleNotFound() {
  server.send(404, "text/plain", "404: Not found");
}

String createHTML() {
  String html = "<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"UTF-8\"><meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"><title>Configure</title><style>html, body{margin: 0;font-family: Arial, Helvetica, sans-serif;} .main{display: flex;justify-content: center;} fieldset{display: inline-block;margin-top: 5px;font-size: 14px;font-weight: 300;padding: 8px 12px;} legend{font-size: 16px;font-weight: 500;} input[type=\"submit\"]{display: block;margin: 8px auto;padding: 6px;background: #045CAA;color: white;border: 2px solid #3d81d9;border-radius: 6px;width: 100%;cursor: pointer;} input[type=\"text\"]{width: 70vw;max-width: 600px;min-width: 250px;padding: 3px;margin: 3px 0;}</style></head><body><div class=\"main\"><form action=\"/save\" method=\"get\"><fieldset><legend>WiFi Credentials</legend><label for=\"ssid\">SSID:</label><br><input required type=\"text\" id=\"ssid\" name=\"ssid\" value=\""; 
  html += STA_SSID;
  html += "\"><br><label for=\"pass\">Password:</label><br><input required type=\"text\" id=\"pass\" name=\"pass\" value=\"\"><br></fieldset><br><fieldset><legend>Advanced</legend><label for=\"host\">Host:</label><br><input required type=\"text\" id=\"host\" name=\"host\" value=\"";
  html += HOST;
  html += "\"><br><label for=\"dir\">Path:</label><br><input required type=\"text\" id=\"dir\" name=\"dir\" value=\"";
  html += DIR;
  html += "\"><br></fieldset><br><input type=\"submit\" value=\"Save & Connect\"></form></div></body></html>";

  return html;
}