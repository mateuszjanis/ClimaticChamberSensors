#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>

const int oneWireBus = 17;     

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(oneWireBus);

// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);

float peltierTemp_1;
float peltierTemp_2;

void handlePeltierTemp(){

  sensors.requestTemperatures(); 
  peltierTemp_1 = sensors.getTempCByIndex(0);
  peltierTemp_2 = sensors.getTempCByIndex(1);
 
}

void printPeltierTemp(){

  Serial.printf("Sens1: %f ºC, Sens2: %f\n", peltierTemp_1, peltierTemp_2);
}

void setup() {
  Serial.begin(115200);
  sensors.begin();
  
  Serial.println("\nESP START");
  delay(500);
  
}

void loop() {

  handlePeltierTemp();
  printPeltierTemp();

  delay(5000);
}