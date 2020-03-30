#ifndef _XBEESER_H
#define _XBEESER_H

#include "mbed.h"
#include "configuration.h"
#include "definitions.h"

class xbeeSerial{
public:
    xbeeSerial(PinName _tx, PinName _rx);
	void startSend();

private:
    uint16_t waitTime;
	Serial xbeeSer;
	uint8_t sendIndexXbee;
	uint8_t generateChecksum(char p_data[]);
    static void sendXbeeAsync(void const *argument);
    Thread *xbeeThread;
};

void xbeeSerial::startSend()
{
	xbeeThread = new Thread(sendXbeeAsync, this);
}

void xbeeSerial::sendXbeeAsync(void const *argument)
{
	xbeeSerial* self = (xbeeSerial*)argument;
    while(true)
	{		

		self->xbeeSer.putc(SYNC);
		self->xbeeSer.putc(SYNC);
		self->xbeeSer.putc(PACKET_LEN);
		
		switch(self->sendIndexXbee)
		{
			case 0:
				sendBuffBattery1[0] = BATTERY1;
				for(int i = 0; i < 9; i++)
				{
					self->xbeeSer.putc(sendBuffBattery1[i]);
				}
				self->xbeeSer.putc(self->generateChecksum(sendBuffBattery1));
				break;
			case 1:
				sendBuffBattery2[0] = BATTERY2;
				for(int i = 0; i < 9; i++)
				{
					self->xbeeSer.putc(sendBuffBattery2[i]);
				}
				self->xbeeSer.putc(self->generateChecksum(sendBuffBattery2));
				break;
			case 2:
				sendBuffBattery3[0] = BATTERY3;
				for(int i = 0; i < 9; i++)
				{
					self->xbeeSer.putc(sendBuffBattery3[i]);
				}
				self->xbeeSer.putc(self->generateChecksum(sendBuffBattery3));
				break;
			case 3:
				sendBuffCabin[0] = CABIN;
				for(int i = 0; i < 9; i++)
				{
					self->xbeeSer.putc(sendBuffCabin[i]);
				}
				self->xbeeSer.putc(self->generateChecksum(sendBuffCabin));
				break;
			case 4:
				sendBuffMotorL[0] = LEFT_MOTOR_DRIVER;
				for(int i = 0; i < 9; i++)
				{
					self->xbeeSer.putc(sendBuffMotorL[i]);
				}
				self->xbeeSer.putc(self->generateChecksum(sendBuffMotorL));
				break;
			case 5:
				sendBuffMotorR[0] = RIGHT_MOTOR_DRIVER;
				for(int i = 0; i < 9; i++)
				{
					self->xbeeSer.putc(sendBuffMotorR[i]);
				}
				self->xbeeSer.putc(self->generateChecksum(sendBuffMotorR));
				break;
			case 6:
				sendBuffFuelCell[0] = FUELCELL;
				for(int i = 0; i < 9; i++)
				{
					self->xbeeSer.putc(sendBuffFuelCell[i]);
				}
				self->xbeeSer.putc(self->generateChecksum(sendBuffFuelCell));
				break;
				
		}
		self->xbeeSer.putc('\n');
		self->xbeeSer.putc('\r');
		self->sendIndexXbee++;
		
		if(self->sendIndexXbee > ARRAY_COUNT - 1) self->sendIndexXbee = 0;
		asd1++;
		Thread::wait(150);
	}
}

xbeeSerial::xbeeSerial(PinName _tx, PinName _rx) : xbeeSer(_tx, _rx)
{
	xbeeSer.baud(9600);
	waitTime = 500;
	sendIndexXbee = 0;
}

uint8_t xbeeSerial::generateChecksum(char p_data[])
{
	uint8_t sum = 0;
	for(int i = 1; i < 9; i++)
	{
		sum += p_data[i];
	}
	
	//sum = 256 - sum;
	return sum;
}

#endif