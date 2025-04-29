#include <WiFi.h>

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_MODE_STA); 
  Serial.print("ESP32 MAC Address: ");
  Serial.println(WiFi.macAddress());
}

void loop() {
  Serial.begin(115200);
  WiFi.mode(WIFI_MODE_STA);  
  Serial.print("ESP32 MAC Address: ");
  Serial.println(WiFi.macAddress()); 
}
