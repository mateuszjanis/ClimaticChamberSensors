#pragma once

//dla CO2 I2C 
#include <Wire.h>
#include "SparkFun_SCD4x_Arduino_Library.h" // <-- get from github https://github.com/sparkfun/SparkFun_SCD4x_Arduino_Library/tree/main/src

#include <mutex>

#define PREHEATING (10 * 60 * 1000)  // (10 * 60 * 1000)  konwersja minut na milisekundy
#define SAMPLES 60 

//-------------ADC ch
#define MG811_OKY_CO2 26 //A0 GPIO26 ADC2_CH9 VCC6V, A - 0–2 V. | num 3

#define MQ3_EtOH 33 // GPIO33 - ADC1_CH5,         R-div-old   | num 2
#define MQ4_CH4 32 // GPIO33 - ADC1_CH4,          R-div-old   | num 1

#define SEN0565_CH4 25 //  A1 GPIO25  ADC2_CH8    R-div-old   | num 5
#define SEN0568_H2S 34 //A2 GPIO34  ADC1_CH6      R-div-new   | num 8
#define SEN0569_EtOH 39 //A3   GPIO39  ADC1_CH3   R-div-old   | num 4
#define SEN0567_NH3 36 //A4 GPIO36 ADC1_CH0       R-div-new   | num 7

#define SEN0467_H2S 35 // GPIO35 ADC2_SEN0467_H2S logic-conv  | num 9
#define SEN0469_NH3 27 // ADC2                    logic-conv  | num 6

// R-div voltage scale: Vout = Vin * (4.7 / (2.4 + 4.7)) ≈ Vin * 0.662
#define R_DIV_RATIO_OLD 0.662
// R-div voltage scale: Vout = Vin * (2.2 / (1.2 + 2.2)) ≈ Vin * 0.647
#define R_DIV_RATIO_NEW 0.647

#define ADC_RESOLUTION 4095.0
#define ADC_VREF       3.3
#define ADC_SCALE      (ADC_VREF / ADC_RESOLUTION)
//---------------------------------------------------------------------

//CO2, T, H
#define SCD41_CO2_I2C_SCL 22        // logic-conv | num 4
#define SCD41_CO2_I2C_SDA 21        // logic-conv | num 4

extern uint16_t ERROR_SYSTEM;
extern bool gasSensorsReady;

extern std::mutex gasSensorsMutex;

extern const unsigned int gasSensors_read_interval;
extern char gasSensorsPayload[256];

//-------------- FUNCTIONS ----------------

void ADC_init();
void readSensor(uint8_t pin, bool useRdiv_old, bool useRdiv_new, float *voltage);
bool SCD41_CO2_I2C_init();
void readSensorI2C (float *CO2, float *T, float *H);
void gasSensors_init();
void runMeasurementCycle();
void runGasSensors();