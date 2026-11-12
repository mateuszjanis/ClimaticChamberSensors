#include <WiFi.h>
#include <WiFiClient.h> 
#include <ESP32_FTPClient.h>
// #include <esp_task_wdt.h>
#include "GasSensors.h"

// #define WDT_TIMEOUT_SECONDS 10

#define WIFI_SSID "hotspot_chamber"
#define WIFI_PASS ""

char ftp_server[] = "192.168.225.57"; //"192.168.225.31";
char ftp_user[]   = "esp";  // "pi"; // 
char ftp_pass[]   = "esp485"; // "raspberry"; //   

char gasSensors_file_name[] = "GasRecords.csv";

const unsigned int gasSensors_read_interval = 1 * 60 * 1000; // 1 minute // from config

const unsigned int connection_try_treshold = 20;

// FTP timeout and debbug mode on the last 2 arguments
ESP32_FTPClient ftp (ftp_server,ftp_user,ftp_pass, 5000, 2);

//////////////////// FUNCTIONS /////////////////////

void connectWIFI(); // loop inside trying to connect
void writeToFTP(const char* payload);
// void getConfig(unsigned int connection_try_treshold, char gasSensors_file_name[], unsigned int gasSensors_read_interval);

/////////////////// ACTUAL CODE ////////////////////

void setup() {

  // esp_task_wdt_init(WDT_TIMEOUT_SECONDS, true);
  // esp_task_wdt_add(NULL);

  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin( WIFI_SSID, WIFI_PASS );
  WiFi.disconnect();
  
  connectWIFI();
  // get config
  // gasSensors_init();

  Serial.println("\nESP START");
  delay(50);
}

void loop() {

  // esp_task_wdt_reset();

  char payload[256];
  runMeasurementCycle( payload );

  Serial.println("----------------------------------\n");
  connectWIFI();
  writeToFTP(payload);
  WiFi.disconnect();
  delay(gasSensors_read_interval);

}

void connectWIFI(){

  unsigned int connection_try_counter = 0;
  WiFi.begin( WIFI_SSID, WIFI_PASS );

  Serial.println("Connecting Wifi...");
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(WiFi.status());
      connection_try_counter++;

      if (connection_try_counter >= connection_try_treshold) esp_restart();

  }

  Serial.println("");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

}

// in main loop new payload created after every cycle 
void writeToFTP(const char* payload){
  
  unsigned int connection_try_counter = 0;
  bool send_status = false;

  ftp.OpenConnection();
  delay(100);
  ftp.ChangeWorkDir("Data");

  while(!send_status) {

    if (ftp.isConnected()){

      ftp.InitFile("Type A");
      // ftp.AppendFile(fileName);
      ftp.NewFile(gasSensors_file_name);
      ftp.Write(payload);
      ftp.CloseFile();
      send_status = true;

    } else {

      connection_try_counter++;
      delay(1000);
            
    }

    if (connection_try_counter >= connection_try_treshold) esp_restart();
  
  }

}