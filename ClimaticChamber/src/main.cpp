#include <Arduino.h>
#include "DHT.h"

#define DHT_TYPE DHT22
#define DHT_PIN_1 4
#define DHT_PIN_2 16

DHT dht1(DHT_PIN_1, DHT_TYPE);
DHT dht2(DHT_PIN_2, DHT_TYPE);

void setup() {
  Serial.begin(115200);
  Serial.println("\nESP START");
  delay(500);
  dht1.begin();
  dht2.begin();
}

void loop() {

  float hum1 = dht1.readHumidity();
  float temp1 = dht1.readTemperature();

  float hum2 = dht2.readHumidity();
  float temp2 = dht2.readTemperature();

  if (isnan(hum1) || isnan(temp1)) {

    Serial.println(F("Failed to read from DHT 1 sensor!"));
    return;

  } else if (isnan(hum2) || isnan(temp2)) {

    Serial.println(F("Failed to read from DHT 2 sensor!"));
    return;

  }

  Serial.printf("DHT1: Humidity: %f;  Temperature: %f\n", hum1, temp1);
  Serial.printf("DHT2: Humidity: %f;  Temperature: %f\n\n", hum2, temp2);
  delay(2000);
}