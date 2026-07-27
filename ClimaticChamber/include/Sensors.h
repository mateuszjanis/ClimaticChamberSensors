#include "DHT.h"
#include <OneWire.h>
#include <DallasTemperature.h>

////////////////////////////////////////////////////
//--------------------- PINS ---------------------//
////////////////////////////////////////////////////

const int oneWireBus = 17; //DS18B20
const int DHT_PIN_1 = 4;
const int DHT_PIN_2 = 16;

#define DHT_TYPE DHT22

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

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

////////////////////////////////////////////////////
//------------------ FUNCTIONS -------------------//
////////////////////////////////////////////////////

void handleDHT();
void printDHT();
void handlePeltierTemp();
void printPeltierTemp();