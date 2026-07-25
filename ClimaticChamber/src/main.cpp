#include <Arduino.h>
#define LED 2

void setup() {
  Serial.begin(115200);
  pinMode(LED,OUTPUT);
  Serial.println("\nESP START");
  delay(500);
}

void loop() {
  delay(500);
  digitalWrite(LED,HIGH);
  Serial.println("On");

  delay(500);
  digitalWrite(LED,LOW);
  Serial.println("Off");
}