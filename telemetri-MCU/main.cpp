#include "mbed.h"
#include "configuration.h"
#include "definitions.h"
#include "xbeeSerial.h"
#include "raspiSerial.h"
#include "canComm.h"

xbeeSerial xbeeSer(SERIAL1_TX, SERIAL1_RX);
raspiSerial raspiSer(SERIAL4_TX, SERIAL4_RX);
canComm canComm(CAN1_RD, CAN1_TD, CAN2_RD, CAN2_TD);

#ifdef MAKE_DEBUG
//Serial pc(USBTX, USBRX);
#ifndef BLUETOOTH_DEBUG
RawSerial pc(USBTX, USBRX);
#else
RawSerial pc(SERIAL5_TX, SERIAL5_RX);
#endif
#endif

InterruptIn btn1(EXT_BTN1);
InterruptIn btn2(EXT_BTN2);

//DigitalOut led1(CAN_SEND_LED);
//DigitalOut led2(CAN_RECEIVE_LED);

AnalogIn wheel(WHEEL);
AnalogIn gasPedal(GAS_PEDAL);

int id = 0, dVal = 0;
char sVal[8];
bool asdData = false;
void receivePc()
{
		char arr[8], i = 0;
	
		char c = pc.getc();
		while(c != '\n')
		{
			sVal[i++] = c;
			c = pc.getc();
		}
		asdData = true;
		//printf("Val: %s\n", sVal);
}

void btnInterrupt1()
{
	canComm.sendCan(LONG_LIGHT, CAN_CMD_ON);
	//while(!btn1.read()) wait(0.01);
}

void btnInterrupt2()
{
	canComm.sendCan(BATTERY_FAN, CAN_CMD_ON);
	//while(!btn2.read()) wait(0.01);
}

void checkStatus(float p_control_var, float p_val, uint8_t &p_status)
{
	if(p_control_var > p_val && p_status != CAN_CMD_ON_RES)
	{
		p_status = CAN_CMD_ON;
	}else if(p_control_var < p_val && p_status != CAN_CMD_OFF_RES)
	{
		p_status = CAN_CMD_OFF;
	}
}

int main()
{
	pc.baud(9600);
	
	xbeeSer.startSend();
	raspiSer.startSend();
	canComm.startSend();
	pc.attach(&receivePc);
	
	canComm.setRaspiComm(&raspiSer);
	
	btn1.mode(PullUp);
	btn1.fall(btnInterrupt1);
	
	btn2.mode(PullUp);
	btn2.fall(btnInterrupt2);
	
	
	for(int i = 1; i < 9; i++)
	{
		sendBuffBattery1[i] = 98;
		sendBuffBattery2[i] = 99;
		sendBuffBattery3[i] = 100;
		sendBuffCabin[i] = 101;
		sendBuffMotorL[i] = 102;
		sendBuffMotorR[i] = 103;
		sendBuffFront[i] = 104;
		sendBuffFuelCell[i] = 105;
	}

	char sendIndex = BATTERY_FAN;
	bool start = true;
  while(true)
  {

		//gasValue = gasPedal.read();
		//wheelValue = wheel.read(); 
		batteryTemperature = sendBuffBattery1[1];
		driver1Tempeerature = sendBuffMotorL[1];
		motorRTemperature = sendBuffMotorL[4];
		motorLTemperature = sendBuffMotorR[4];
		driver2Tempeerature = sendBuffMotorR[1];
		h2SensValue = sendBuffCabin[5];
		//pc.printf("Hello World %d %d %d %d\n", asd++, asd1, asd2, asd3);
		
		if(asdData)
		{
			sscanf(sVal, "%d %d", &id, &dVal);
			printf("Val: %s \t %d %d\n", sVal, id, dVal);
			//if(id >= ALARM && id <= TOP_LIGHT)
			{
				canComm.sendCan(id, dVal);
			}
			asdData = false;
		}
		
#ifdef MAKE_DEBUG
		if(canComm.isDataAvailable())
		{
			pc.printf("Id: %d, Temp1: %.1f, Temp2: %.1f, Temp3: %.1f, Val: ",
								canComm.getReceiveId(), batteryTemperature, driver1Tempeerature, driver2Tempeerature);
			
			for(int i = 0; i < 9; i++)
			{
					pc.printf("%d ", canComm.getReceiveData()[i]);
			}
			pc.printf("\n");
		}
#endif
		
		allValuesCounter = battery1FirstCheck 
									+ battery2FirstCheck 
									+ battery3FirstCheck 			
									+ driverLeftFirstCheck
									+ driverRightFirstCheck
									+ fuelCellFirstCheck
									+ cabinFirstCheck;
		
		if(allValuesCounter >= ARRAY_COUNT  && !startControl)
		{
#ifdef MAKE_DEBUG
			pc.printf("Starting...\n");
#endif
			startControl = true;
		}
		
		if(startControl)
		{
			
			checkStatus(batteryTemperature, BATTERY_TEMP_FAN, batteryStatusFan);
			checkStatus(driver1Tempeerature, TEMP_MOTOR_MAX, driver1FanStatus);
			checkStatus(driver2Tempeerature, TEMP_MOTOR_MAX, driver2FanStatus);
			
			checkStatus(batteryTemperature, BATTERY_TEMP_FLASOR, alarmStatus);
			checkStatus(h2SensValue, H2_VALUE_MAX, alarmStatus);
		}

		Thread::wait(10);
		//wait(0.1);
  }
	
	return 0;
}
