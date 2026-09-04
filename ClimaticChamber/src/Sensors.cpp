#include "Sensors.h"

////////////////////////////////////////////////////
//--------------------- PINS ---------------------//
////////////////////////////////////////////////////

const int oneWireBus = 17; //DS18B20

////////////////////////////////////////////////////
//------------------- OBJECTS --------------------//
////////////////////////////////////////////////////

OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);

////////////////////////////////////////////////////
//----------------- GLOBAL VARIABLES -------------//
////////////////////////////////////////////////////

float peltierTemp_1;
float peltierTemp_2;

////////////////////////////////////////////////////
//------------------ FUNCTIONS -------------------//
////////////////////////////////////////////////////

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
