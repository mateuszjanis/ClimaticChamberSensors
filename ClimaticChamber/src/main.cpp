#include <Arduino.h>
#include "Sensors.h"

void setup() {
  Serial.begin(115200);
  sensors.begin();
  
  Serial.println("\nESP START");
  delay(50);
}

void loop() {
  handlePeltierTemp();
  printPeltierTemp();
  Serial.println("----------------------------------\n");
  delay(3000);
}