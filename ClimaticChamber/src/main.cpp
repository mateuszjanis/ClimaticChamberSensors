#include <WiFi.h>
#include "esp_wifi.h" 
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

unsigned int gasSensors_read_interval = 10 * 60 * 1000; // updated from config

const unsigned int connection_try_treshold = 20;

int64_t last_sensor_read = 0;

// FTP timeout and debbug mode on the last 2 arguments
ESP32_FTPClient ftp (ftp_server,ftp_user,ftp_pass, 5000, 2);

//////////////////// FUNCTIONS /////////////////////

void connectWIFI();
void writeToFTP(const char* payload);
unsigned int readIntervalFromConfig();

/////////////////// ACTUAL CODE ////////////////////

void setup() {

  // esp_task_wdt_init(WDT_TIMEOUT_SECONDS, true);
  // esp_task_wdt_add(NULL);

  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin( WIFI_SSID, WIFI_PASS );
  delay(1000);
  gasSensors_read_interval = readIntervalFromConfig() * 60 * 1000 * 1000; // [microseconds] updated from config
  Serial.println("Gas sensors read interval: " + String(gasSensors_read_interval / 1000 / 1000 / 60) + " minutes");
  
  WiFi.disconnect(true);  // Odłącza i wyłącza WiFi (true = całkowite wyłączenie) - ADC2 włączony
  esp_wifi_stop();        // Zatrzymuje stos WiFi

  last_sensor_read = esp_timer_get_time();

  gasSensors_init();

  Serial.println("\nESP START");
  delay(50);
}

void loop() {

  // esp_task_wdt_reset();

  if (esp_timer_get_time() - last_sensor_read >= gasSensors_read_interval) {
    
    last_sensor_read = esp_timer_get_time();
    Serial.println("🔄 Starting new measurement cycle... after " + String(esp_timer_get_time() - last_sensor_read / 1000));

    char payload[256];
    runMeasurementCycle( payload, sizeof(payload) );

    Serial.println("----------------------------------\n");
    connectWIFI();
    delay(100);
    writeToFTP(payload);
    delay(100);
    WiFi.disconnect(true);
    esp_wifi_stop();
    Serial.println("------- WIFI DISCONNECTED --------\n");
    Serial.println("----------------------------------\n");

  }
  
  delay(30 * 1000);

}

void connectWIFI(){

  unsigned int connection_try_counter = 0;
  esp_wifi_start();
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

unsigned int readIntervalFromConfig(){

  unsigned int interval;

  unsigned int connection_try_counter = 0;
  bool read_status = false;

  ftp.OpenConnection();
  delay(100);
  // ftp.ChangeWorkDir("/home/esp");

  while(!read_status) {

    if (ftp.isConnected()){

      String response = "";
      ftp.InitFile("Type A");
      ftp.DownloadString("GAS_DATA_RECORD_INTERVAL.txt", response);
      interval = response.toInt();
      ftp.CloseFile();
      read_status = true;

    } else {

      connection_try_counter++;
      delay(1000);
            
    }

    if (connection_try_counter >= connection_try_treshold) esp_restart();
  
  }

  return interval; // convert to milliseconds

}
