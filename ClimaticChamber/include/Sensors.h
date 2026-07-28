#pragma once
#include "DHT.h"
#include <OneWire.h>
#include <DallasTemperature.h>

////////////////////////////////////////////////////
//--------------------- PINS ---------------------//
////////////////////////////////////////////////////

extern const int oneWireBus;
extern const int DHT_PIN_1;
extern const int DHT_PIN_2;

#define DHT_TYPE DHT22

////////////////////////////////////////////////////
//------------------- OBJECTS --------------------//
////////////////////////////////////////////////////

extern DHT dht1;
extern DHT dht2;

extern OneWire oneWire;
extern DallasTemperature sensors;

////////////////////////////////////////////////////
//----------------- GLOBAL VARIABLES -------------//
////////////////////////////////////////////////////

extern float peltierTemp_1;
extern float peltierTemp_2;
extern float hum_1, DHT_temp_1, hum_2, DHT_temp_2, hum_mean, DHT_temp_mean;

////////////////////////////////////////////////////
//------------------ FUNCTIONS -------------------//
////////////////////////////////////////////////////

void handleDHT();
void printDHT();
void handlePeltierTemp();
void printPeltierTemp();