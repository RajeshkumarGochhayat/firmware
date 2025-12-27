#include <DHT.h>

//--------------WEATHER SENSOR -----------------
#define DHTPIN 2
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

void setupWeather() {
  dht.begin();
}

void loopWeather() {
  float temperature = dht.readTemperature();
  float humidity    = dht.readHumidity();

  if (isnan(temperature) || isnan(humidity)) {
    return;
  }

  String data =
    "{\"temperature\":" + String(temperature) +
    ",\"humidity\":" + String(humidity) + "}";

  String payload = envelope("weather", data);

  if (!sendPost(payload)) {
    logToSD(payload);
  }
}
