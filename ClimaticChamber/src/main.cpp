#include <Arduino.h>
#include "DHT.h"

#define DHT_TYPE DHT22
#define DHT_PIN_1 4
#define DHT_PIN_2 16

DHT dht1(DHT_PIN_1, DHT_TYPE);
DHT dht2(DHT_PIN_2, DHT_TYPE);

float hum_1, DHT_temp_1, hum_2, DHT_temp_2, hum_mean, DHT_temp_mean;

void handleDHT(){
  hum_1 = dht1.readHumidity();
  DHT_temp_1 = dht1.readTemperature();

  hum_1 = dht2.readHumidity();
  DHT_temp_2 = dht2.readTemperature();

  if (isnan(hum_1) || isnan(DHT_temp_1)) {

    Serial.println(F("Failed to read from DHT 1 sensor!"));
    return;

  } else if (isnan(hum_2) || isnan(DHT_temp_1)) {

    Serial.println(F("Failed to read from DHT 2 sensor!"));
    return;

  }

  hum_mean = (hum_1 + hum_2) / 2;
  DHT_temp_mean = (DHT_temp_1 + DHT_temp_2) / 2;
}

void printDHT(){

  Serial.printf("DHT1: Humidity: %f;  Temperature: %f\n", hum_1, DHT_temp_1);
  Serial.printf("DHT2: Humidity: %f;  Temperature: %f\n", hum_2, DHT_temp_2);
  Serial.printf("DHT mean: Humidity: %f;  Temperature: %f\n\n", hum_mean, DHT_temp_mean);

}

void setup() {
  Serial.begin(115200);
  Serial.println("\nESP START");
  delay(500);
  dht1.begin();
  dht2.begin();
}

void loop() {
  handleDHT();
  printDHT();
  delay(2000);
}