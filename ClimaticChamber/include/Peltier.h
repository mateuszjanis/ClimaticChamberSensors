#pragma once
#include <Arduino.h>

////////////////////////////////////////////////////
//--------------------- PINS ---------------------//
////////////////////////////////////////////////////

extern const int HEAT_PIN_1;
extern const int HEAT_PIN_2;
extern const int COOL_PIN_1;
extern const int COOL_PIN_2;

////////////////////////////////////////////////////
//------------------ FUNCTIONS -------------------//
////////////////////////////////////////////////////

void setPeltierPins();
void setCOOLING();
void setHEATING();
void setIDLE();