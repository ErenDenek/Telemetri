#ifndef _raspiSer_H
#define _raspiSer_H

#include "mbed.h"
#include "configuration.h"
#include "definitions.h"

class raspiSerial
{
public:
	raspiSerial(PinName tx, PinName rx);
	void startSend();
	void sendData(uint8_t p_id, uint8_t p_val);

private:
    uint16_t waitTime;
		RawSerial raspiSer;
		uint8_t sendIndexRaspi;
		uint8_t generateChecksum(char p_data[]);
    static void sendRaspiAsync(void const *argument);
    void receiveRaspi();
    //void receiveRaspi();
    Thread *raspiThread;
};

void raspiSerial::startSend()
{
	raspiThread = new Thread(sendRaspiAsync, this);
}

void raspiSerial::sendData(uint8_t p_id, uint8_t p_val)
{
		raspiSer.printf("&|%d|%d\r\n", p_id, p_val);
}

void raspiSerial::receiveRaspi()
{
    NVIC_DisableIRQ(UART4_IRQn);
		if(raspiSer.readable())
    {
        //raspiSer.putc(raspiSer.getc());
    }
	
		NVIC_EnableIRQ(UART4_IRQn);
}

void raspiSerial::sendRaspiAsync(void const *argument)
{
	raspiSerial* self = (raspiSerial*)argument;
	int k = 0;

	while(true)
	{		
		//self->raspiSer.printf("Hello World %d\n", k++);
		
		
		self->raspiSer.printf("#|");
		for(int i = 0; i < 9; i++)
		{
			self->raspiSer.printf("%d|", sendBuffBattery1[i]);
		}
		
		for(int i = 0; i < 9; i++)
		{
			self->raspiSer.printf("%d|", sendBuffBattery2[i]);
		}
		
		for(int i = 0; i < 9; i++)
		{
			self->raspiSer.printf("%d|", sendBuffBattery3[i]);
		}
		
		for(int i = 0; i < 9; i++)
		{
			self->raspiSer.printf("%d|", sendBuffMotorL[i]);
		}
		
		for(int i = 0; i < 9; i++)
		{
			self->raspiSer.printf("%d|", sendBuffMotorR[i]);
		}
		
		for(int i = 0; i < 9; i++)
		{
			self->raspiSer.printf("%d|", sendBuffCabin[i]);
		}
		
		for(int i = 0; i < 9; i++)
		{
			self->raspiSer.printf("%d|", sendBuffFuelCell[i]);
		}
		
		self->raspiSer.printf("\r\n");
		
		asd3++;
		Thread::wait(self->waitTime);
	}
}

raspiSerial::raspiSerial(PinName _tx, PinName _rx) : raspiSer(_tx, _rx)
{
    raspiSer.baud(115200);
		waitTime = 100;
		sendIndexRaspi = 0;
    raspiSer.attach(this, &raspiSerial::receiveRaspi);
}

uint8_t raspiSerial::generateChecksum(char p_data[])
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