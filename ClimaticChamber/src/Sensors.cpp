#include "Sensors.h"

////////////////////////////////////////////////////
//--------------------- PINS ---------------------//
////////////////////////////////////////////////////

const int oneWireBus = 4; //DS18B20

////////////////////////////////////////////////////
//------------------- OBJECTS --------------------//
////////////////////////////////////////////////////

OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);

////////////////////////////////////////////////////
//----------------- GLOBAL VARIABLES -------------//
////////////////////////////////////////////////////

////////////////////////////////////////////////////
//------------------ FUNCTIONS -------------------//
////////////////////////////////////////////////////

void handlePeltierTemp(){

  sensors.requestTemperatures();

  float peltierTemp_0 = sensors.getTempCByIndex(0);
  float peltierTemp_1 = sensors.getTempCByIndex(1);

  snprintf(peltierTempPayload, sizeof(peltierTempPayload), "%.2f;%.2f;\n",
    peltierTemp_0,
    peltierTemp_1);
 
}

void printPeltierTemp(){
  
  Serial.println(peltierTempPayload);

}
