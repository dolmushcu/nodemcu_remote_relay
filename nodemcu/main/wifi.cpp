#include "wifi.h"

WiFiEventHandler whenDisconnected;

IPAddress local_ip(192,168,0,1);
IPAddress gateway(192,168,0,1);
IPAddress netmask(255,255,255,0);

char* AP_SSID = "SMMM_RECEP_ESP8266";
char* AP_PASS = "";

char STA_SSID[33];
char STA_PASS[65];

char HOST[70];
char DIR[70];

bool AP_MODE = true;

const int relay = 16;
int count = 0;

void switchToAp() {
  AP_MODE = true;

  WiFi.softAPdisconnect(true);
  WiFi.disconnect();
  delay(1000);
  
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(local_ip, gateway, netmask);
  WiFi.softAP(AP_SSID, AP_PASS);
  delay(500);

  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());
}

void autoConnect(int timeout) {
  if (!loadConfig()) {
    Serial.println("Couldn't load config file!");
    switchToAp();
    return;
  }
  
  // reset connection settings just in case
  WiFi.softAPdisconnect(true);
  WiFi.disconnect();
  delay(1000);

  WiFi.mode(WIFI_STA);
  WiFi.begin(STA_SSID, STA_PASS);

  unsigned long beginTime = millis();

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");

    if (millis() - beginTime > timeout * 1000) {
      Serial.println("TIMEOUT");
      switchToAp();
      return;
    }
  }

  AP_MODE = false;
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void fetch() {
  // Https connection client
  WiFiClientSecure client;
  const int httpPort = 443; // 80 for http 443 for https

  // Connect https without need of security finger print
  client.setInsecure();
  
  if (!client.connect(HOST, httpPort)) {
    Serial.println("Connection failed");
    return;
  }
  
  Serial.print("Requesting URL: ");
  Serial.println(DIR);
 
  // This will send the request to the server
  client.print(String("GET ") + DIR + " HTTP/1.1\r\n" +
               "Host: " + HOST + "\r\n" +
               "Connection: close\r\n\r\n");
  delay(500);

  // Parse the response
  int section = 0;
  while(client.available()){
    String line = client.readStringUntil('\r');
    if (section == 0) {
      if (line == "\n")
        section = 1;
    }
    else if (section == 1) {
      section = 2;
      String getResponse = line.substring(1);

      StaticJsonDocument<500> json_parsed;
      deserializeJson(json_parsed, getResponse);

      if (json_parsed["status"] == "on" && count == 0) {
        digitalWrite(relay, LOW);
        delay(600);
        digitalWrite(relay, HIGH);
        count++;
      }
      else if (json_parsed["status"] == "off")
        count = 0;
    }
  }
  
  Serial.println("Closing connection.");
  delay(500);
}

void disconnectedWifi(const WiFiEventStationModeDisconnected& event) {
  if(!AP_MODE) {
    Serial.println("Esp is disconnected wifi. Restarting...");
    ESP.restart();
  }
}