#pragma once

//dla CO2 I2C 
#include <Wire.h>
#include <Adafruit_ADS1X15.h>
#include "SparkFun_SCD4x_Arduino_Library.h" // <-- get from github https://github.com/sparkfun/SparkFun_SCD4x_Arduino_Library/tree/main/src

#include <mutex>

#define PREHEATING (10 * 60 * 1000)  // (10 * 60 * 1000)  konwersja minut na milisekundy
#define SAMPLES 60 

//-------------ADC ch
#define MG811_OKY_CO2 32 // ADC1, VCC6V, A - 0–2 V. | num 3     32

#define MQ3_EtOH 39     // ADC1, R-div  | num 2       VN (39)
#define MQ4_CH4 36      // ADC1, R-div  | num 1       VP (36)

               // channel
#define SEN0567_NH3  0              // logic-conv    | num 7    external ADC
#define SEN0565_CH4  3              // logic-conv    | num 5    external ADC 
#define SEN0568_H2S  1              // logic-conv    | num 8    external ADC 
#define SEN0569_EtOH 2             // logic-conv    | num 4    external ADC 

#define SEN0467_H2S 35  // ADC1, R-div  | num 9       35
#define SEN0469_NH3 34  // ADC1, R-div  | num 6       34

//------------------------------- SUGGESTED PINS --^^^^^^^^^^

// R-div voltage scale: Vout = Vin * (4.7 / (2.4 + 4.7)) ≈ Vin * 0.662
#define R_DIV_RATIO 0.662

#define ADC_RESOLUTION 4095.0
#define ADC_VREF       3.3
#define ADC_SCALE      (ADC_VREF / ADC_RESOLUTION)
//---------------------------------------------------------------------

//CO2, T, H
// define SCD41_CO2_I2C_SCL 22        // logic-conv | num 4
// define SCD41_CO2_I2C_SDA 21        // logic-conv | num 4

// 5 sensors using I2C: SCD41_CO2, 27, 26
#define I2C_SCL 22
#define I2C_SDA 21

extern uint16_t ERROR_SYSTEM;
extern bool gasSensorsReady;

extern std::mutex gasSensorsMutex;

extern const unsigned int gasSensors_read_interval;
extern char gasSensorsPayload[256];

//-------------- FUNCTIONS ----------------

void ADC_init();
void readSensor (uint8_t pin, bool useRdiv, float *voltage);
bool I2C_init();
void readSensorSCD41_CO2 (float *CO2, float *T, float *H);
void readSensorI2C (uint16_t channel, float *voltage);
void gasSensors_init();
void runMeasurementCycle();
void runGasSensors();