#include "Sensors.h"

////////////////////////////////////////////////////
//--------------------- PINS ---------------------//
////////////////////////////////////////////////////

const int oneWireBus = 4; //DS18B20

////////////////////////////////////////////////////
//------------------- OBJECTS --------------------//
////////////////////////////////////////////////////

OneWire oneWire(oneWireBus);
DallasTemperature peltierSensors(&oneWire);

////////////////////////////////////////////////////
//----------------- GLOBAL VARIABLES -------------//
////////////////////////////////////////////////////

////////////////////////////////////////////////////
//------------------ FUNCTIONS -------------------//
////////////////////////////////////////////////////

void handlePeltierTemp(){

  peltierSensors.requestTemperatures();

  float peltierTemp_0 = peltierSensors.getTempCByIndex(0);
  float peltierTemp_1 = peltierSensors.getTempCByIndex(1);

  snprintf(peltierTempPayload, sizeof(peltierTempPayload), "%.2f;%.2f;\n",
    peltierTemp_0,
    peltierTemp_1);
 
}

void printPeltierTemp(){
  
  Serial.print("Peltier: ");
  Serial.println(peltierTempPayload);

}
