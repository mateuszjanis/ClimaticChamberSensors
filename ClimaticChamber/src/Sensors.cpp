#include "Sensors.h"

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

void handlePeltierTemp(){

  sensors.requestTemperatures(); 
  peltierTemp_1 = sensors.getTempCByIndex(0);
  peltierTemp_2 = sensors.getTempCByIndex(1);
 
}

void printPeltierTemp(){

  Serial.printf("Sens1: %f ºC, Sens2: %f\n", peltierTemp_1, peltierTemp_2);

}
