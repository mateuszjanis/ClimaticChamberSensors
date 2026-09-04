#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h> 
#include <ESP32_FTPClient.h>
#include <string>
#include "Sensors.h"

#define WIFI_SSID "test_hotspot_3" // "rpiAP" // "rpiAccessPoint" //"HUAWEi-PLAY" //
#define WIFI_PASS ""// "Al4bi&83" // "Al4bi&83" //"ZLKNshiq" //"Al4bi&83"

char ftp_server[] = "192.168.225.58"; //"192.168.225.31";
char ftp_user[]   = "esp";  // "pi"; // 
char ftp_pass[]   = "esp485"; // "raspberry"; //   

// you can pass a FTP timeout and debbug mode on the last 2 arguments
ESP32_FTPClient ftp (ftp_server,ftp_user,ftp_pass, 5000, 2);


void setup() {
  Serial.begin(115200);
  sensors.begin();

  WiFi.begin( WIFI_SSID, WIFI_PASS );
  
 Serial.println("Connecting Wifi...");
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(WiFi.status());
  }
  Serial.println("");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  ftp.OpenConnection();
  ftp.ChangeWorkDir("Data");

  // Create the file new and write a string into it
  ftp.InitFile("Type A");
  ftp.NewFile("PeltierTemperature.txt");
  ftp.Write("Sens1, Sens2\n");
  ftp.CloseFile();



  Serial.println("\nESP START");
  delay(50);
}

void loop() {
  handlePeltierTemp();
  printPeltierTemp();

  ftp.AppendFile("PeltierTemperature.txt");
  std::string temp_s = std::to_string(peltierTemp_1) + ", " + std::to_string(peltierTemp_2) + "\n";
  ftp.Write(temp_s);
  Serial.println("----------------------------------\n");
  delay(3000);

  // ftp.CloseConnection();
}