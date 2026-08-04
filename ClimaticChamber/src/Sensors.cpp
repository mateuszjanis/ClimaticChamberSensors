#include "Sensors.h"

////////////////////////////////////////////////////
//--------------------- PINS ---------------------//
////////////////////////////////////////////////////

const int oneWireBus = 17; //DS18B20
const int DHT_PIN_1 = 4;
const int DHT_PIN_2 = 16;

#ifndef DHT_TYPE
  #define DHT_TYPE DHT22
#endif

////////////////////////////////////////////////////
//------------------- OBJECTS --------------------//
////////////////////////////////////////////////////

DHT dht1(DHT_PIN_1, DHT_TYPE);
DHT dht2(DHT_PIN_2, DHT_TYPE);

OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);

////////////////////////////////////////////////////
//----------------- GLOBAL VARIABLES -------------//
////////////////////////////////////////////////////

float peltierTemp_1;
float peltierTemp_2;
float hum_1, DHT_temp_1, hum_2, DHT_temp_2, hum_mean, DHT_temp_mean;

////////////////////////////////////////////////////
//------------------ FUNCTIONS -------------------//
////////////////////////////////////////////////////

void handleDHT(){
  hum_1 = dht1.readHumidity();
  DHT_temp_1 = dht1.readTemperature();

  hum_2 = dht2.readHumidity();
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

  // Serial.printf("DHT1: Humidity: %f;  Temperature: %f\n", hum_1, DHT_temp_1);
  // Serial.printf("DHT2: Humidity: %f;  Temperature: %f\n", hum_2, DHT_temp_2);
  // Serial.printf("DHT mean: Humidity: %f;  Temperature: %f\n\n", hum_mean, DHT_temp_mean);

  Serial.printf("TempUp: %f, ",DHT_temp_1);
  Serial.printf("TempDown: %f, ",DHT_temp_2);
  Serial.printf("HumUp: %f, ",hum_1);
  Serial.printf("HumDown: %f, ",hum_2);

}

void handlePeltierTemp(){

  sensors.requestTemperatures(); 
  peltierTemp_1 = sensors.getTempCByIndex(1);
  peltierTemp_2 = sensors.getTempCByIndex(0);
 
}

void printPeltierTemp(){

  // Serial.printf("Sens1: %f ºC, Sens2: %f\n", peltierTemp_1, peltierTemp_2);
  
  Serial.printf("PeltUp: %f, ",peltierTemp_1);
  Serial.printf("PeltDown: %f, ",peltierTemp_2);
}
