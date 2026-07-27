#include "Peltier.h"
#include "Sensors.h"
#include "RTClib.h"

////////////////////////////////////////////////////
//------------------- OBJECTS --------------------//
////////////////////////////////////////////////////

DateTime now;

RTC_DS3231 rtc;

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

void setRTC(){
  if (! rtc.begin()) {
  
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1) delay(10);
  
  }

  if (rtc.lostPower()) {
    
    Serial.println("RTC lost power, let's set the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

  }
}

void printRTCinfo(){
  // Get the current time from the RTC
  now = rtc.now();
  
  // Getting each time field in individual variables
  // And adding a leading zero when needed;
  String yearStr = String(now.year(), DEC);
  String monthStr = (now.month() < 10 ? "0" : "") + String(now.month(), DEC);
  String dayStr = (now.day() < 10 ? "0" : "") + String(now.day(), DEC);
  String hourStr = (now.hour() < 10 ? "0" : "") + String(now.hour(), DEC); 
  String minuteStr = (now.minute() < 10 ? "0" : "") + String(now.minute(), DEC);
  String secondStr = (now.second() < 10 ? "0" : "") + String(now.second(), DEC);
  String dayOfWeek = daysOfTheWeek[now.dayOfTheWeek()];

  // Complete time string
  String formattedTime = dayOfWeek + ", " + yearStr + "-" + monthStr + "-" + dayStr + " " + hourStr + ":" + minuteStr + ":" + secondStr;

  // Print the complete formatted time
  Serial.println(formattedTime);

  // Getting temperature
  Serial.print(rtc.getTemperature());
  Serial.println("ºC");

  Serial.println();
}

void setup() {
  Serial.begin(115200);
  dht1.begin();
  dht2.begin();
  sensors.begin();
  setPeltierPins();
  setRTC();

  Serial.println("\nESP START");
  delay(500);
  Serial.println("Gotowe. Wpisz 0, 1 lub 2:");
}

void loop() {
  handleDHT();
  printDHT();
  handlePeltierTemp();
  printPeltierTemp();
  handleInput();
  printRTCinfo();
  
  delay(3000);
}