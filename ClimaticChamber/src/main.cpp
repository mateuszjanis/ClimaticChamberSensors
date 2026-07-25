#include <Arduino.h>
#include "DHT.h"

#define DHT_TYPE DHT22
#define DHT_PIN 4

DHT dht(DHT_PIN, DHT_TYPE);

void setup() {
  Serial.begin(115200);
  Serial.println("\nESP START");
  delay(500);
  dht.begin();
}

void loop() {

  float hum = dht.readHumidity();
  float temp = dht.readTemperature();

  if (isnan(hum) || isnan(temp)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  Serial.printf("Humidity: %f;  Temperature: %f\n", hum, temp);
  delay(2000);
}