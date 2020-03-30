#ifndef _CANCOMM_H
#define _CANCOMM_H

#include "mbed.h"
#include "configuration.h"
#include "definitions.h"
#include "raspiSerial.h"

class canComm
{
public:
    canComm(PinName rd1, PinName td1, 
                PinName rd2, PinName td2);

    void startSend();
    void canComm::sendArrayCan(uint8_t p_id, uint8_t p_len, uint8_t *p_val);
    void canComm::sendCan(uint8_t p_id, uint8_t p_val);
		uint8_t getReceiveId();
		uint8_t * getReceiveData();
		bool isDataAvailable();
		void setRaspiComm(raspiSerial *r_ser);

private:
    DigitalOut canSendLed;
    DigitalOut canReceiveLed;
    CAN canRx, canTx;
    uint16_t waitTime;
    static void sendCanAsync(void const *argument);
		void receiveCan();
    Thread *canThread;
		bool isReceive;
    CANMessage can_MsgRx, can_MsgTx;
		uint8_t r_id, len;
		uint8_t r_data[8];
		raspiSerial *_raspiSer;
};

#endif

uint8_t canComm::getReceiveId()
{
		return r_id;
}

uint8_t *canComm::getReceiveData()
{
		isReceive = false;
		return r_data;
}

void canComm::setRaspiComm(raspiSerial *r_ser)
{
		_raspiSer = r_ser;
}

bool canComm::isDataAvailable()
{
		return isReceive;
}

void canComm::receiveCan()
{
		
		NVIC_DisableIRQ(CAN1_RX0_IRQn);
		NVIC_DisableIRQ(CAN1_RX1_IRQn);
	
		canRx.read(can_MsgRx);
		r_id = can_MsgRx.id;
		len = can_MsgRx.len;
	
		for(int i = 0; i < len; i++)
		{
			r_data[i] = can_MsgRx.data[i];
		}

		//r_msg[i] = can_MsgRx.data[i];

    //chose date from can id
    switch (r_id) {
			case ALARM_ANS:
				sendAlarmStatus = false;
				break;
			case BATTERY_FAN_ANS:
				if(can_MsgRx.data[0] == CAN_CMD_ON)
				{
					batteryStatusFan = CAN_CMD_ON_RES;
				}else if(can_MsgRx.data[0] == CAN_CMD_OFF)
				{
					batteryStatusFan = CAN_CMD_OFF_RES;
				}
				batteryFanCounter = 0;
				break;
			case DRIVER1_FAN_ANS:
				if(can_MsgRx.data[0] == CAN_CMD_ON)
				{
					driver1FanStatus = CAN_CMD_ON_RES;
				}else if(can_MsgRx.data[0] == CAN_CMD_OFF)
				{
					driver1FanStatus = CAN_CMD_OFF_RES;
				}
				break;
			case DRIVER2_FAN_ANS:
				if(can_MsgRx.data[0] == CAN_CMD_ON)
				{
					driver2FanStatus = CAN_CMD_ON_RES;
				}else if(can_MsgRx.data[0] == CAN_CMD_OFF)
				{
					driver2FanStatus = CAN_CMD_OFF_RES;
				}
				break;
			case BATTERY1:
				sendBuffBattery1[1] = BATTERY1;
				for(int i = 1; i < len; i++)
				{
					sendBuffBattery1[i] = can_MsgRx.data[i - 1];
				}
				if(!battery1FirstCheck) battery1FirstCheck = true;
				break;
			case BATTERY2:
				sendBuffBattery2[0] = BATTERY2;
				for(int i = 1; i < len; i++)
				{
					sendBuffBattery2[i] = can_MsgRx.data[i - 1];
				}
				if(!battery2FirstCheck) battery2FirstCheck = true;
				break;
			case BATTERY3:
				sendBuffBattery3[0] = BATTERY3;
				for(int i = 1; i < len; i++)
				{
					sendBuffBattery3[i] = can_MsgRx.data[i - 1];
				}
				if(!battery3FirstCheck) battery3FirstCheck = true;
				break;
			case LEFT_MOTOR_DRIVER:
				sendBuffMotorL[0] = LEFT_MOTOR_DRIVER;
				for(int i = 1; i < len; i++)
				{
					sendBuffMotorL[i] = can_MsgRx.data[i - 1];
				}
				if(!driverLeftFirstCheck) driverLeftFirstCheck = true;
				break;
			case RIGHT_MOTOR_DRIVER:
				sendBuffMotorR[0] = RIGHT_MOTOR_DRIVER;
				for(int i = 1; i < len; i++)
				{
					sendBuffMotorR[i] = can_MsgRx.data[i - 1];
				}
				if(!driverRightFirstCheck) driverRightFirstCheck = true;
				break;
			case FUELCELL:
				sendBuffFuelCell[0] = FUELCELL;
				for(int i = 1; i < len; i++)
				{
					sendBuffFuelCell[i] = can_MsgRx.data[i - 1];
				}
				if(!fuelCellFirstCheck) fuelCellFirstCheck = true;
				break;
			case CABIN:
				sendBuffCabin[0] = CABIN;
				for(int i = 1; i < len; i++)
				{
					sendBuffCabin[i] = can_MsgRx.data[i - 1];
				}
				if(!cabinFirstCheck) cabinFirstCheck = true;
				break;
			case SHORT_LIGHT_ANS:
				_raspiSer->sendData(SHORT_LIGHT_ANS, can_MsgRx.data[0]);
				break;
			case LONG_LIGHT_ANS:
				_raspiSer->sendData(LONG_LIGHT_ANS, can_MsgRx.data[0]);
				break;
			case LEFT_SIGNAL_ANS:
				_raspiSer->sendData(LEFT_SIGNAL_ANS, can_MsgRx.data[0]);
				break;
			case RIGHT_SIGNAL_ANS:
				_raspiSer->sendData(RIGHT_SIGNAL_ANS, can_MsgRx.data[0]);
				break;
			case SIGNAL_ANS:
				_raspiSer->sendData(SIGNAL_ANS, can_MsgRx.data[0]);
				break;
			case NEO_PIXEL1_ANS:
				_raspiSer->sendData(NEO_PIXEL1_ANS, can_MsgRx.data[0]);
				break;
			case NEO_PIXEL2_ANS:
				_raspiSer->sendData(NEO_PIXEL2_ANS, can_MsgRx.data[0]);
				break;
			case WIPER_ANS:
				_raspiSer->sendData(WIPER_ANS, can_MsgRx.data[0]);
				break;
			case TOP_LIGHT_ANS:
				_raspiSer->sendData(TOP_LIGHT_ANS, can_MsgRx.data[0]);
				break;
			case RIGTH_DOOR_SW:
				_raspiSer->sendData(RIGTH_DOOR_SW, can_MsgRx.data[0]);
				break;
			case LEFT_DOOR_SW:
				_raspiSer->sendData(LEFT_DOOR_SW, can_MsgRx.data[0]);
				break;
    }
		
		canReceiveLed = !canReceiveLed;
		
		isReceive = true;
		
		NVIC_EnableIRQ(CAN1_RX0_IRQn);
		NVIC_EnableIRQ(CAN1_RX1_IRQn);
}

void canComm::startSend()
{
    canThread = new Thread(sendCanAsync, this);
}


canComm::canComm(PinName rd1, PinName td1, 
                PinName rd2, PinName td2) : canRx(rd1, td1), 
                                            canTx(rd2, td2),
                                            canSendLed(CAN_SEND_LED),
                                            canReceiveLed(CAN_RECEIVE_LED) 
{
    waitTime = 500;
    canRx.frequency(CAN_SPEED);
  	canTx.frequency(CAN_SPEED);
		can_MsgRx.format = CANStandard;
		can_MsgRx.type = CANData;
		can_MsgTx.format = CANStandard;
		can_MsgTx.type = CANData;
		canRx.attach(this, &canComm::receiveCan);
		isReceive = false;
}

void canComm::sendCanAsync(void const *argument)
{
    canComm* self = (canComm*)argument;

    while(true)
	{
		if(alarmStatus == CAN_CMD_ON)
		{
			self->sendCan(ALARM, CAN_CMD_ON);
			batteryFanCounter++;
			if(alarmCounter > 5)
			{
				errorAlarmStatus = true;
			}
		}
		
		if(batteryStatusFan == CAN_CMD_ON)
		{
			self->sendCan(BATTERY_FAN, CAN_CMD_ON);
			batteryFanCounter++;
			if(batteryFanCounter > 5)
			{
				errorBattertFanStatus = true;
			}
		}else if(batteryStatusFan == CAN_CMD_OFF)
		{
			self->sendCan(BATTERY_FAN, CAN_CMD_OFF);
			batteryFanCounter++;
			if(batteryFanCounter > 5)
			{
				errorBattertFanStatus = true;
			}
		}
		
		if(driver1FanStatus == CAN_CMD_ON)
		{
			self->sendCan(DRIVER1_FAN, CAN_CMD_ON);
			driver1FanCounter++;
			if(driver1FanCounter > 5)
			{
				 errorDriver1FanStatus = true;
			}
		}else if(driver1FanStatus == CAN_CMD_OFF)
		{
			self->sendCan(DRIVER1_FAN, CAN_CMD_OFF);
			driver1FanCounter++;
			if(driver1FanCounter > 5)
			{
				 errorDriver1FanStatus = true;
			}
		}
		
		if(driver2FanStatus == CAN_CMD_ON)
		{
			self->sendCan(DRIVER2_FAN, CAN_CMD_ON);
			driver2FanCounter++;
			if(driver2FanCounter > 5)
			{
				 errorDriver2FanStatus = true;
			}
		}else if(driver2FanStatus == CAN_CMD_OFF)
		{
			self->sendCan(DRIVER2_FAN, CAN_CMD_OFF);
			driver2FanCounter++;
			if(driver2FanCounter > 5)
			{
				 errorDriver2FanStatus = true;
			}
		}
		
		asd2++;
		Thread::wait(self->waitTime);
	}    
}

//send data over can bus with ID
void canComm::sendCan(uint8_t p_id, uint8_t p_val)
{
		can_MsgTx.id = p_id;
		
		can_MsgTx.data[0] = p_val;
		
		can_MsgTx.len = 1;

		if(canTx.write(can_MsgTx)) {
			canSendLed = !canSendLed;
		}else{
			canErrorStatus = true;
		}
}

//send array value over can bus
void canComm::sendArrayCan(uint8_t p_id, uint8_t p_len, uint8_t *p_val)
{
		can_MsgTx.id = p_id;
	
		for(int i = 0; i < p_len; i++)
		{
			can_MsgTx.data[i] = p_val[i];
		}
		
		can_MsgTx.len = p_len;
	
		if(canTx.write(can_MsgTx)) {
			canSendLed = !canSendLed;
		}else{
			canErrorStatus = true;
		} 	
}