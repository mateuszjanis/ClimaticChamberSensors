#include <WiFi.h>
#include <WiFiClient.h> 
#include <ESP32_FTPClient.h>
// #include <esp_task_wdt.h>
#include "Sensors.h"
#include "GasSensors.h"
#include <thread>
#include <mutex>

// #define WDT_TIMEOUT_SECONDS 10

#define WIFI_SSID "hotspot"
#define WIFI_PASS ""

char ftp_server[] = "192.168.225.57"; //"192.168.225.31";
char ftp_user[]   = "esp";  // "pi"; // 
char ftp_pass[]   = "esp485"; // "raspberry"; //   

char peltierTemp_file_name[] = "PeltierTemperature.csv";
char gasSensors_file_name[] = "GasSensors.csv";

char peltierTempPayload[128];
char gasSensorsPayload[256];

const unsigned int peltierTemp_read_interval = 10 * 1000; // 10 seconds
const unsigned int gasSensors_read_interval = 10 * 60 * 1000; // 10 minutes

const unsigned int connection_try_treshold = 20;
unsigned int connection_try_counter = 0;

// you can pass a FTP timeout and debbug mode on the last 2 arguments
ESP32_FTPClient ftp (ftp_server,ftp_user,ftp_pass, 5000, 2);

std::mutex gasSensorsMutex;

//////////////////// FUNCTIONS /////////////////////

void initializeWIFI();
void initializeDataTransfer();
void handleFTP();
void writeToFTP(char* fileName, const char* payload);

/////////////////// ACTUAL CODE ////////////////////

void setup() {

  // esp_task_wdt_init(WDT_TIMEOUT_SECONDS, true);
  // esp_task_wdt_add(NULL);

  Serial.begin(115200);
  initializeWIFI();
  initializeDataTransfer();

  // get configuration from conf file (RPI) - .txt ?????? or .json

  peltierSensors.begin();
  std::thread gasSensorsThread(runGasSensors);

  Serial.println("\nESP START");
  delay(50);

  while (true)
  {
    // esp_task_wdt_reset();

    handlePeltierTemp();
    printPeltierTemp();
    Serial.println("----------------------------------\n");
    handleFTP();
    delay(peltierTemp_read_interval);

  }
}

void loop() {}   // <- EMPTY

void initializeWIFI(){
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
}

void initializeDataTransfer(){

  ftp.OpenConnection();
  ftp.ChangeWorkDir("Data");

  /* ---------- is appending to file needed? 
                or creating new file is better?

  String fileList[128];
  ftp.InitFile("Type A");
  ftp.ContentListWithListCommand(" /Data", fileList);

  bool gasFileExists = false;
  bool peltierTempFileExists = false;

  for (int i = 0; i < sizeof(fileList); i++) {
    if(fileList[i].length() > 0){
      fileList[i].trim();
      
      String gasSensors_fn_to_compare(gasSensors_file_name);
      String peltierTemp_fn_to_compare(peltierTemp_file_name);

      if (fileList[i] == gasSensors_fn_to_compare) {
      gasFileExists = true;
      Serial.printf("File %s already exists. Appending data to it.\n", gasSensors_fn_to_compare);
      break;
      }

      if (fileList[i] == peltierTemp_fn_to_compare) {
      peltierTempFileExists = true;
      Serial.printf("File %s already exists. Appending data to it.\n", peltierTemp_fn_to_compare);
      break;
      }
      
    } else break;
  }

  if(!gasFileExists){
    ftp.InitFile("Type A");
    ftp.NewFile(gasSensors_file_name);
    ftp.Write("MG811_OKY_CO2_f; MQ3_EtOH_f; MQ4_CH4_f; SEN0565_CH4_f; SEN0568_H2S_f; SEN0569_EtOH_f; SEN0567_NH3_f; SEN0467_H2S_f; SEN0469_NH3_f; SCD41_CO2_f; SCD41_T_f; SCD41_H_f\n");
    ftp.CloseFile();
    // Serial.printf("File %s created. Writing header.\n", gasSensors_file_name);
  }

  if(!peltierTempFileExists){
    ftp.InitFile("Type A");
    ftp.NewFile(peltierTemp_file_name);
    ftp.Write("PeltIn; PeltOut\n");
    ftp.CloseFile();
    // Serial.printf("File %s created. Writing header.\n", peltierTemp_file_name);
  }
  */
}

void handleFTP(){
    
  if (ftp.isConnected()){

    writeToFTP(peltierTemp_file_name, peltierTempPayload);

    if (gasSensorsReady) {
        std::unique_lock<std::mutex> gasLock(gasSensorsMutex);
        char temp_payload[256];
        strncpy(temp_payload, gasSensorsPayload, sizeof(temp_payload));
        gasLock.unlock();

        writeToFTP(gasSensors_file_name, temp_payload);

        gasSensorsReady = false;
    }

  }
  else {

      initializeDataTransfer();
      connection_try_counter++;
      
  }

  if (connection_try_counter >= connection_try_treshold) esp_restart();
}

void writeToFTP(char* fileName, const char* payload){
      ftp.InitFile("Type A");
      // ftp.AppendFile(fileName);
      ftp.NewFile(fileName);
      ftp.Write(payload);
      ftp.CloseFile();
}