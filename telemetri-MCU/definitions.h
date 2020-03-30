#ifndef _DEFINITIONS_H
#define _DEFINITIONS_H

#include "mbed.h"

///function definitions
uint8_t generateChecksum(char p_data[]);
void checkStatus(float p_control_var, float p_val, uint8_t &p_status);

//variables
float gasValue;
float wheelValue;
float batteryTemperature = 0;
float driver1Tempeerature = 0;
float motorLTemperature = 0;
float motorRTemperature = 0;
float driver2Tempeerature = 0;
float H2Value = 0;
float motorCurrent = 0;
float h2SensValue = 0;

uint8_t sendIndexRaspi = 0;

//arrays
char sendBuffBattery1[9];
char sendBuffBattery2[9];
char sendBuffBattery3[9];
char sendBuffCabin[9];
char sendBuffMotorL[9];
char sendBuffMotorR[9];
char sendBuffFront[9];
char sendBuffFuelCell[9];

//can bus error detect counter
uint8_t alarmCounter = 0;
uint8_t batteryFanCounter = 0;
uint8_t driver1FanCounter = 0;
uint8_t driver2FanCounter = 0;
uint8_t allValuesCounter = 0;

//can bus check variables
bool canErrorStatus = false;
bool errorAlarmStatus = false;
bool errorBattertFanStatus = false;
bool errorDriver1FanStatus = false;
bool errorDriver2FanStatus = false;
bool startControl = false;

//oto control variables
uint8_t alarmStatus = 0;
uint8_t sendAlarmStatus = 0;
uint8_t batteryStatusFan = 0;
uint8_t driver1FanStatus = 0;
uint8_t driver2FanStatus = 0;

//car controls
uint8_t shortHeadlight = 0;
uint8_t longHeadlight = 0;
uint8_t leftIndicator = 0;
uint8_t rightIndicator = 0;
uint8_t wiper = 0;
uint8_t leftNeoPixel = 0;
uint8_t rightNeopixel = 0;

//first values control
bool battery1FirstCheck = 0;
bool battery2FirstCheck = 0;
bool battery3FirstCheck = 0;
bool driverLeftFirstCheck = 0;
bool driverRightFirstCheck = 0;
bool fuelCellFirstCheck = 0;
bool cabinFirstCheck = 0;

long asd = 0, asd1 = 0, asd2 = 0, asd3 = 0;

#endif
