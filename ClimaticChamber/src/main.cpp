#include <Arduino.h>

const int HEAT_PIN_1 = 25;
const int HEAT_PIN_2 = 26;
const int COOL_PIN_1 = 19;
const int COOL_PIN_2 = 18; 

void setPeltierPins(){
  pinMode(HEAT_PIN_1,OUTPUT);
  pinMode(HEAT_PIN_2,OUTPUT);
  pinMode(COOL_PIN_1,OUTPUT);
  pinMode(COOL_PIN_2,OUTPUT);

  digitalWrite(HEAT_PIN_1,HIGH);
  digitalWrite(HEAT_PIN_2,HIGH);
  digitalWrite(COOL_PIN_1,HIGH);
  digitalWrite(COOL_PIN_2,HIGH);
}

void setCOOLING(){

  digitalWrite(HEAT_PIN_1,HIGH); Serial.printf("%d HIGH, ",HEAT_PIN_1);
  digitalWrite(HEAT_PIN_2,HIGH); Serial.printf("%d HIGH, ",HEAT_PIN_2);
  digitalWrite(COOL_PIN_1,LOW); Serial.printf("%d LOW, ",COOL_PIN_1);
  digitalWrite(COOL_PIN_2,LOW); Serial.printf("%d LOW ",COOL_PIN_2);
  Serial.println("COOLING...");

}

void setHEATING(){

  digitalWrite(HEAT_PIN_1,LOW); Serial.printf("%d LOW, ",COOL_PIN_1);
  digitalWrite(HEAT_PIN_2,LOW); Serial.printf("%d LOW, ",COOL_PIN_2);
  digitalWrite(COOL_PIN_1,HIGH); Serial.printf("%d HIGH, ",COOL_PIN_1);
  digitalWrite(COOL_PIN_2,HIGH); Serial.printf("%d HIGH ",COOL_PIN_2);
  Serial.println("HEATING...");

}

void setIDLE(){

  digitalWrite(HEAT_PIN_1,HIGH);
  digitalWrite(HEAT_PIN_2,HIGH);
  digitalWrite(COOL_PIN_1,HIGH);
  digitalWrite(COOL_PIN_2,HIGH);
  Serial.println("IDLE...");

}

void handleInput(){
  
  if (Serial.available() > 0) {
      
      // Odczytanie pojedynczego znaku
      char receivedChar = Serial.read();
      Serial.println(receivedChar);

      switch (receivedChar) {
        case '0':
          setIDLE();
          break;
        case '1':
          setHEATING();
          break;
        case '2':
          setCOOLING();
          break;
        default:
          setIDLE();
          Serial.println("Default");
          break;
      }

      Serial.println("Gotowe. Wpisz 0, 1 lub 2:");
    }

}

void setup() {
  Serial.begin(115200);
  setPeltierPins();
  Serial.println("\nESP START");
  delay(500);
  Serial.println("Gotowe. Wpisz 0, 1 lub 2:");
}

void loop() {
  handleInput();
  delay(10);
}