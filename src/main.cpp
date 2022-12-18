#include <Arduino.h>
#include <Wifi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid = "Riya";
const char* password = "25112004";
const char* serverPOST = "http://192.168.200.107:8000/api/data/save";
const int port = 8000;

int ok_Count = 0;
int not_ok_Count = 0;
const char* snfg = "KSABDAKSJDSF";
int line = 19;

unsigned long prevTime_T1 = millis();
long interval_T1 = 15000;

void WiFiStationConnected(WiFiEvent_t event, WiFiEventInfo_t info){
  Serial.println("Connected to AP successfully!");
}

void WiFiGotIP(WiFiEvent_t event, WiFiEventInfo_t info){
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void WiFiStationDisconnected(WiFiEvent_t event, WiFiEventInfo_t info){
  Serial.println("Disconnected from WiFi access point");
  Serial.print("WiFi lost connection. Reason: ");
  Serial.println(info.wifi_sta_disconnected.reason);
  Serial.println("Trying to Reconnect");
  WiFi.begin(ssid, password);
}

void setup() {
  Serial.begin(115200);
  WiFi.disconnect(true);
  delay(1000);
  WiFi.onEvent(WiFiStationConnected, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_CONNECTED);
  WiFi.onEvent(WiFiGotIP, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_GOT_IP);
  WiFi.onEvent(WiFiStationDisconnected, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_DISCONNECTED);
  WiFi.begin(ssid,password);
  Serial.println("Wait for WiFi... ");
}

void loop() {
  unsigned long currentTime = millis();
  if(currentTime - prevTime_T1 > interval_T1){
    WiFiClient client;
    HTTPClient http;

    http.begin(client, serverPOST);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    String var_snfg = "snfg=";
    String var_line = "&line=";
    String var_ok = "&ok=";
    String var_not_ok = "&not_ok=";
    int httpResponseCode = http.POST(var_snfg+snfg+var_line+line+var_ok+String(ok_Count)+var_not_ok+String(not_ok_Count));
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    http.end();

    prevTime_T1 = currentTime;
  }
}