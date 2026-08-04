#include "Peltier.h"
#include "Sensors.h"
#include "TimeManager.h"

/* Input handling
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
*/

void setup() {
  Serial.begin(115200);
  dht1.begin();
  dht2.begin();
  sensors.begin();
  // setPeltierPins();
  setRTC();

  Serial.println("\nESP START");
  delay(500);
  // Serial.println("Gotowe. Wpisz 0, 1 lub 2:");
}

void loop() {
  handleDHT();
  handlePeltierTemp();
  // handleInput();

  printDHT();  
  printPeltierTemp();
  // printRTCinfo();

  Serial.println("");
  delay(30000);
}