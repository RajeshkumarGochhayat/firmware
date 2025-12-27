//--------------SOIL MOISTURE SENSOR -----------------
#define SOIL_PIN 34
#define SOIL_RAW_DRY 4095
#define SOIL_RAW_WET 1011

void setupSoil() {
  analogSetPinAttenuation(SOIL_PIN, ADC_11db);
}

void loopSoil() {
  int raw = analogRead(SOIL_PIN);

  float moisturePercent =
    (SOIL_RAW_DRY - raw) * 100.0 /
    (SOIL_RAW_DRY - SOIL_RAW_WET);

  moisturePercent = constrain(moisturePercent, 0, 100);
  Serial.print("Soil Moisture: ");
  Serial.print(moisturePercent);
  Serial.println(" %");

  String data =
    "{\"moistureLevel\":" + String(moisturePercent, 1) + "}";

  String payload = envelope("soil", data);

  if (!sendPost(payload)) {
    logToSD(payload);
  }
}
