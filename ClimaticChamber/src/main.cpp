#include <Arduino.h>
#include "DHT.h"
#include <OneWire.h>
#include <DallasTemperature.h>
#include "RTClib.h"

RTC_DS3231 rtc;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

DateTime now;

const int HEAT_PIN_1 = 25;
const int HEAT_PIN_2 = 26;
const int COOL_PIN_1 = 19;
const int COOL_PIN_2 = 18; 

const int oneWireBus = 17;     

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(oneWireBus);

// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);

float peltierTemp_1;
float peltierTemp_2;

#define DHT_TYPE DHT22
#define DHT_PIN_1 4
#define DHT_PIN_2 16

DHT dht1(DHT_PIN_1, DHT_TYPE);
DHT dht2(DHT_PIN_2, DHT_TYPE);

float hum_1, DHT_temp_1, hum_2, DHT_temp_2, hum_mean, DHT_temp_mean;

void handleDHT(){
  hum_1 = dht1.readHumidity();
  DHT_temp_1 = dht1.readTemperature();

  hum_1 = dht2.readHumidity();
  DHT_temp_2 = dht2.readTemperature();

  if (isnan(hum_1) || isnan(DHT_temp_1)) {

    Serial.println(F("Failed to read from DHT 1 sensor!"));
    return;

  } else if (isnan(hum_2) || isnan(DHT_temp_1)) {

    Serial.println(F("Failed to read from DHT 2 sensor!"));
    return;

  }

  hum_mean = (hum_1 + hum_2) / 2;
  DHT_temp_mean = (DHT_temp_1 + DHT_temp_2) / 2;
}

void printDHT(){

  Serial.printf("DHT1: Humidity: %f;  Temperature: %f\n", hum_1, DHT_temp_1);
  Serial.printf("DHT2: Humidity: %f;  Temperature: %f\n", hum_2, DHT_temp_2);
  Serial.printf("DHT mean: Humidity: %f;  Temperature: %f\n\n", hum_mean, DHT_temp_mean);


void handlePeltierTemp(){

  sensors.requestTemperatures(); 
  peltierTemp_1 = sensors.getTempCByIndex(0);
  peltierTemp_2 = sensors.getTempCByIndex(1);
 
}

void printPeltierTemp(){

  Serial.printf("Sens1: %f ºC, Sens2: %f\n", peltierTemp_1, peltierTemp_2);

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

  Serial.println("\nESP START");
  delay(500);
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