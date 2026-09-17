#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h> 
#include <ESP32_FTPClient.h>
#include <string>
#include "Sensors.h"

#define WIFI_SSID "hotspot"
#define WIFI_PASS ""

char ftp_server[] = "192.168.225.57"; //"192.168.225.31";
char ftp_user[]   = "esp";  // "pi"; // 
char ftp_pass[]   = "esp485"; // "raspberry"; //   

char* data_file_name = "PeltierTemperature_1.csv";

const unsigned int sensors_read_interval = 10000; // 10 seconds
const unsigned int connection_try_treshold = 20;
unsigned int connection_try_counter = 0;

// you can pass a FTP timeout and debbug mode on the last 2 arguments
ESP32_FTPClient ftp (ftp_server,ftp_user,ftp_pass, 5000, 2);


//////////////////// FUNCTIONS /////////////////////

void InitializeDataTransfer();
void handleFTP();

void setup() {

  Serial.begin(115200);
  sensors.begin();

  WiFi.begin( WIFI_SSID, WIFI_PASS );
  
  Serial.println("Connecting Wifi...");
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(WiFi.status());
      connection_try_counter++;

      if (connection_try_counter >= connection_try_treshold) esp_restart();

  }

  connection_try_counter = 0;

  Serial.println("");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  InitializeDataTransfer();

  Serial.println("\nESP START");
  delay(50);
}

void loop() {
  handlePeltierTemp();
  printPeltierTemp();
  Serial.println("//////////////////////////////////\n");
  handleFTP();
  delay(sensors_read_interval);
}

void InitializeDataTransfer(){

  ftp.OpenConnection();
  ftp.ChangeWorkDir("Data");
  String fileList[128];
  ftp.InitFile("Type A");
  ftp.ContentListWithListCommand(" /Data", fileList);

  bool fileExists = false;

  for (int i = 0; i < sizeof(fileList); i++) {
    if(fileList[i].length() > 0){
      fileList[i].trim();
      if (fileList[i] == data_file_name) {
      fileExists = true;
      Serial.printf("File %s already exists. Appending data to it.\n", data_file_name);
      break;
      } 
    
    } else break;
  }

  if(!fileExists){
    ftp.InitFile("Type A");
    ftp.NewFile(data_file_name);
    ftp.Write("SensIn, SensOut\n");
    ftp.CloseFile();
    Serial.printf("File %s created. Writing header.\n", data_file_name);
  }

}

void handleFTP(){
    if (ftp.isConnected()){
      ftp.InitFile("Type A");
      ftp.AppendFile(data_file_name);
      char temp_s[100];
      snprintf(temp_s, sizeof(temp_s), "%.2f, %.2f\n", peltierTemp_0, peltierTemp_1);
      ftp.Write(temp_s);
      ftp.CloseFile();
    }
    else {
        InitializeDataTransfer();
        connection_try_counter++;
    }

    if (connection_try_counter >= connection_try_treshold) esp_restart();
}
