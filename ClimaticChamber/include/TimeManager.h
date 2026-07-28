#pragma once
#include "RTClib.h"

//------------------- OBJECTS --------------------//

extern DateTime now;
extern RTC_DS3231 rtc;

//----------------- GLOBAL VARIABLES -------------//

extern char daysOfTheWeek[7][12];

//------------------ FUNCTIONS -------------------//

void setRTC();
void printRTCinfo();