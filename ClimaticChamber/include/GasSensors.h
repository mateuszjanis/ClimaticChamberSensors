#pragma once

//dla CO2 I2C 
#include <Wire.h>
#include <Adafruit_ADS1X15.h>
#include "SparkFun_SCD4x_Arduino_Library.h" // <-- get from github https://github.com/sparkfun/SparkFun_SCD4x_Arduino_Library/tree/main/src

#define PREHEATING (5 * 60 * 1000)  // (10 * 60 * 1000)  konwersja minut na milisekundy // from config
#define SAMPLES 60 

//-------------ADC ch
#define MG811_OKY_CO2 36 // ADC1, VCC6V, A - 0–2 V. | num 3     32

#define MQ3_EtOH 35     // ADC1, R-div-old
#define MQ4_CH4 32      // ADC1, R-div-old

#define SEN0567_NH3  27 // ADC1, R-div-new
#define SEN0565_CH4  33 // ADC1, R-div-new
#define SEN0568_H2S  26 // ADC1, R-div-new
#define SEN0569_EtOH 25 // ADC1, R-div-new

#define SEN0467_H2S 39  // ADC1, R-div-old
#define SEN0469_NH3 34  // ADC1, R-div-old

// R-div voltage scale: Vout = Vin * (4.7 / (2.4 + 4.7)) ≈ Vin * 0.662
#define R_DIV_RATIO_OLD 0.662

// R-div voltage scale: Vout = Vin * (2.2 / (1.2 + 2.2)) ≈ Vin * 0.647
#define R_DIV_RATIO_NEW 0.647

#define ADC_RESOLUTION 4095.0
#define ADC_VREF       3.3
#define ADC_SCALE      (ADC_VREF / ADC_RESOLUTION)
//---------------------------------------------------------------------

//CO2, T, H
#define SCD41_CO2_I2C_SCL 22
#define SCD41_CO2_I2C_SDA 21

extern uint16_t ERROR_SYSTEM;

//-------------- FUNCTIONS ----------------

void ADC_init();
void readSensor (uint8_t pin, bool useRdiv_old, bool useRdiv_new, float *voltage);
bool I2C_init();
void readSensorSCD41_CO2 (float *CO2, float *T, float *H);
void gasSensors_init();
void runMeasurementCycle(char* gasPayload, size_t payloadSize);