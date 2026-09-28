#include "Sensors.h"

////////////////////////////////////////////////////
//--------------------- PINS ---------------------//
////////////////////////////////////////////////////

const int oneWireBus = 27; //DS18B20

////////////////////////////////////////////////////
//------------------- OBJECTS --------------------//
////////////////////////////////////////////////////

OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);

////////////////////////////////////////////////////
//----------------- GLOBAL VARIABLES -------------//
////////////////////////////////////////////////////

float peltierTemp_0;
float peltierTemp_1;

////////////////////////////////////////////////////
//------------------ FUNCTIONS -------------------//
////////////////////////////////////////////////////

void handlePeltierTemp(){

  sensors.requestTemperatures(); 
  peltierTemp_0 = sensors.getTempCByIndex(0);
  peltierTemp_1 = sensors.getTempCByIndex(1);
 
}

void printPeltierTemp(){
  
  Serial.printf("PeltIn: %f, ",peltierTemp_0);
  Serial.printf("PeltOut: %f\n",peltierTemp_1);

}
