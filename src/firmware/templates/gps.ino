#include <TinyGPSPlus.h>

//--------------GPS SENSOR -------------------
#define GPS_RX 13
#define GPS_TX 12

HardwareSerial GPS_Serial(1);
TinyGPSPlus gps;

void setupGPS() {
  GPS_Serial.begin(9600, SERIAL_8N1, GPS_RX, GPS_TX);
}

void loopGPS() {
  while (GPS_Serial.available()) {
    gps.encode(GPS_Serial.read());
  }

  if (!gps.location.isValid()) {
    return;
  }

  String data =
    "{\"latitude\":"        + String(gps.location.lat(), 6) +
    ",\"longitude\":"       + String(gps.location.lng(), 6) +
    ",\"altitude\":"        + String(gps.altitude.meters(), 1) +
    ",\"satelliteCount\":"  + String(gps.satellites.value()) + "}";

  String payload = envelope("gps", data);

  if (!sendPost(payload)) {
    logToSD(payload);
  }
}
