#include "wifi_config.h"

const char* ssid = "YourSSID";         // SSID của mạng WiFi
const char* password = "YourPassword"; // Mật khẩu WiFi

void setupWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected!");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}