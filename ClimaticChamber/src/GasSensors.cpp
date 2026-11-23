#include "GasSensors.h"

uint16_t ERROR_SYSTEM = 0x0000;

//SCD41_CO2_I2C
SCD4x scd4x;

void ADC_init() {
  analogReadResolution(12);  // 12-bit = 0–4095

  // Tłumienie dla czujników 0–2V
  analogSetPinAttenuation(MG811_OKY_CO2, ADC_6db); // MG811

  // Tłumienie dla czujników z R-div (do ~4V)
  analogSetPinAttenuation(SEN0565_CH4, ADC_6db); // CH4
  analogSetPinAttenuation(SEN0568_H2S, ADC_6db); // H2S
  analogSetPinAttenuation(SEN0569_EtOH, ADC_6db); // EtOH
  analogSetPinAttenuation(SEN0567_NH3, ADC_6db); // NH3
  
  analogSetPinAttenuation(SEN0467_H2S, ADC_6db); // H2S
  analogSetPinAttenuation(SEN0469_NH3, ADC_6db); // NH3

  analogSetPinAttenuation(MQ3_EtOH, ADC_6db); // MQ3
  analogSetPinAttenuation(MQ4_CH4, ADC_6db); // MQ4
}

void readSensor(uint8_t pin, bool useRdiv_old, bool useRdiv_new, float *voltage) {
  int raw = analogRead(pin);
  // int raw = random(0, 4095); // Simulacja odczytu ADC dla testów
  float v = raw * ADC_SCALE;

  if (useRdiv_old) {              // Korekta napięcia z dzielnika
  
    v = v / R_DIV_RATIO_OLD;
  
  } else if (useRdiv_new) {

    v = v / R_DIV_RATIO_NEW;

  }
  // Serial.printf("Raw: %d, V: %.3f V\n", raw, v);

  *voltage += v;  // Dodaj do sumy uśredniania
}

bool I2C_init()
{
  Wire.begin(SCD41_CO2_I2C_SDA, SCD41_CO2_I2C_SCL);

  Serial.println("🔍 Skanowanie magistrali I2C...");
  for (uint8_t addr = 1; addr < 127; addr++) {
    
    Wire.beginTransmission(addr);
    if (Wire.endTransmission() == 0) {
      
      Serial.print("✅ Urządzenie I2C znalezione: 0x");
      Serial.println(addr, HEX);

    }
  
  }

  // Inicjalizacja czujnika
  Serial.println("📡 Inicjalizacja czujnika SCD41...");
  if (!scd4x.begin(Wire)) {
  
    Serial.println("❌ Nie wykryto czujnika SCD41.");
    return false;
  
  } else {

    Serial.println("✅ Czujnik SCD41 wykryty.");
    scd4x.stopPeriodicMeasurement();
    delay(500);
    scd4x.startPeriodicMeasurement();
  
  }

  return true; 
}

void readSensorSCD41_CO2 (float *CO2, float *T, float *H) {
  if (scd4x.readMeasurement()) {
    *CO2 += scd4x.getCO2();
    *T += scd4x.getTemperature();
    *H += scd4x.getHumidity();

    } 
  else {
    Serial.println("⚠️ Brak danych z SCD41");
  }

  // *CO2 += random(10);
  // *T += random(10);
  // *H += random(10);
}

void gasSensors_init()
{
  ADC_init();
  delay(5);

  if(I2C_init()==false)
  ERROR_SYSTEM =  ERROR_SYSTEM | 0x0040; 
  delay(5); 
}

void runMeasurementCycle(char* gasPayload, size_t payloadSize){

  // Preheating
  Serial.println("Preheating start\n");
  delay(PREHEATING); 
  Serial.println("Preheating stop\n");

  gasSensors_init();

  float MG811_OKY_CO2_f   = 0.0f;
  float MQ3_EtOH_f        = 0.0f;
  float MQ4_CH4_f         = 0.0f;

  float SEN0565_CH4_f     = 0.0f;
  float SEN0568_H2S_f     = 0.0f;
  float SEN0569_EtOH_f    = 0.0f;
  float SEN0567_NH3_f     = 0.0f;

  float SEN0467_H2S_f     = 0.0f;
  float SEN0469_NH3_f     = 0.0f;

  float SCD41_CO2_f  = 0.0f;
  float SCD41_T_f  = 0.0f;
  float SCD41_H_f  = 0.0f;


  /*
  * Parametr Wartość
  Preheating  5 minut
  Częstotliwość odczytu co 2 sekundy
  Liczba próbek 30 próbek (1 minuta)
  Uśrednienie średnia arytmetyczna
  */

  Serial.println("🔍 Rozpoczęcie pomiarów...");

  // 5 minut = 60 próbek co 5s - tyle potrzebuje SCD41_CO2_f

  for (int i = 0; i < SAMPLES; i++) {  

    //                         R-div-use
    //                        OLD     NEW
    readSensor(MG811_OKY_CO2, false, false, &MG811_OKY_CO2_f);
    readSensor(MQ3_EtOH,      true,  false, &MQ3_EtOH_f);
    readSensor(MQ4_CH4,       true,  false, &MQ4_CH4_f);
    readSensor(SEN0467_H2S,   true,  false, &SEN0467_H2S_f);
    readSensor(SEN0469_NH3,   true,  false, &SEN0469_NH3_f);

    readSensor(SEN0565_CH4,   false,  true, &SEN0565_CH4_f);
    readSensor(SEN0568_H2S,   false,  true, &SEN0568_H2S_f);
    readSensor(SEN0569_EtOH,  false,  true, &SEN0569_EtOH_f);
    readSensor(SEN0567_NH3,   false,  true, &SEN0567_NH3_f);

    readSensorSCD41_CO2(&SCD41_CO2_f, &SCD41_T_f, &SCD41_H_f);

    delay(5000);
  }

  MQ3_EtOH_f /= SAMPLES;
  MQ4_CH4_f /= SAMPLES;
  MG811_OKY_CO2_f /= SAMPLES;
  SEN0565_CH4_f /= SAMPLES;
  SEN0568_H2S_f /= SAMPLES;
  SEN0569_EtOH_f /= SAMPLES;
  SEN0567_NH3_f /= SAMPLES;
  SEN0467_H2S_f /= SAMPLES;
  SEN0469_NH3_f /= SAMPLES;

  SCD41_CO2_f /= SAMPLES; 
  SCD41_T_f /= SAMPLES; 
  SCD41_H_f /= SAMPLES;

  snprintf(gasPayload, payloadSize, 
      "%.3f;%.3f;%.3f;%.3f;%.3f;%.3f;%.3f;%.3f;%.3f;%.3f;%.3f;%.3f;\r\n",
      MG811_OKY_CO2_f,    // %.5f
      MQ3_EtOH_f,         // %.5f
      MQ4_CH4_f,          // %.5f
      SEN0565_CH4_f,      // %.5f
      SEN0568_H2S_f,      // %.5f
      SEN0569_EtOH_f,     // %.5f
      SEN0567_NH3_f,      // %.5f
      SEN0467_H2S_f,      // %.5f
      SEN0469_NH3_f,
      SCD41_CO2_f,        // %.0f
      SCD41_T_f,          // %.2f
      SCD41_H_f           // %.1f
  );

  Serial.println("🔍 Pomiary zakończone. Wyniki:");
  // Serial.println(gasPayload);

    Serial.printf(
    "MQ3_EtOH: %.3f V, MQ4_CH4: %.3f V, MG811_CO2: %.3f V, \n"
    "SEN0565_CH4: %.3f V, SEN0568_H2S: %.3f V, SEN0569_EtOH: %.3f V, \n"
    "SEN0567_NH3: %.3f V, SEN0467_H2S: %.3f V, \n"
    "SEN0469_NH3: %.3f V, \n"
    "SCD41_CO2: %.0f ppm, Temp: %.2f °C, Humidity: %.1f %%\n",
    MQ3_EtOH_f, MQ4_CH4_f, MG811_OKY_CO2_f,
    SEN0565_CH4_f, SEN0568_H2S_f, SEN0569_EtOH_f,
    SEN0567_NH3_f, SEN0467_H2S_f,SEN0469_NH3_f,
    SCD41_CO2_f, SCD41_T_f, SCD41_H_f 
    );


}