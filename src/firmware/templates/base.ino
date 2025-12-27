#include <WiFi.h>
#include <HTTPClient.h>
#include <SPI.h>
#include <SD.h>

const char* ssid = "Powerhouse";
const char* password = "coolie no 5821";
const char* BASE_URL = "http://46.202.160.176:2020/agrifield/save";

#define DEVICE_ID "ESP32-AGRIFIELD-01"

bool sendPost(const String& payload) {
  if (WiFi.status() != WL_CONNECTED) return false;
  HTTPClient http;
  http.begin(BASE_URL);
  http.addHeader("Content-Type", "application/json");
  int code = http.POST(payload);
  Serial.print("📡 HTTP Status: ");
  Serial.println(code);
  http.end();
  return code >= 200 && code < 300;
}

void logToSD(const String& payload) {
  File f = SD.open("/agrifield_log.txt", FILE_APPEND);
  if (f) {
    f.println(payload);
    f.close();
  }
}

String envelope(const char* type, const String& data) {
  return String("{\"deviceId\":\"") + DEVICE_ID +
         "\",\"type\":\"" + type +
         "\",\"data\":" + data + "}";
}

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) delay(500);

  if (!SD.begin(5)) {
    Serial.println("❌ SD Card failed");
  } else {
    Serial.println("✅ SD Card ready");
  }

  setupWeather();
  setupSoil();
  setupNPK();
  setupGPS();
}

void loop() {
  loopWeather();
  loopSoil();
  loopNPK();
  loopGPS();
  delay(10000);
}
