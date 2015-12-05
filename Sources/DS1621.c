/*
 * DS1621.c
 *
 *  Created on: Nov 25, 2015
 *      Author: Student
 */

#include "DS1621.h"

static DS1621_TDataState deviceData;

int ConfigureThermometer()
{
	uint8_t buf[2];
	int result;
	
	buf[0] = 0xAC; //send configuration command
	buf[1] = 0x02; //configure polarity & set continuous mode
	
	result = CI2C1_MasterSendBlock(deviceData.handle,&buf,sizeof(buf),LDD_I2C_SEND_STOP); 
	if (result != ERR_OK)
	{
		return ERR_FAILED;
	}
	while(deviceData.dataTransmittedFlg == FALSE) {} //wait until transmitted
	deviceData.dataTransmittedFlg = FALSE;
	return 0;
}

int StartConversion()
{
	uint8_t buf;
	int result;
	
	buf = 0xEE; //send start conversion command
	
	result = CI2C1_MasterSendBlock(deviceData.handle,&buf,sizeof(buf),LDD_I2C_SEND_STOP); 
	if (result != ERR_OK)
	{
		return ERR_FAILED;
	}
	while(deviceData.dataTransmittedFlg == FALSE) {} //wait until transmitted
	deviceData.dataTransmittedFlg = FALSE;
	return 0;
}

int ReadTemperature()
{
	uint8_t buf;
	short temp_read;
	int result;
	
	buf = 0xAA; //send command to read temperature
		
	result = CI2C1_MasterSendBlock(deviceData.handle,&buf,sizeof(buf),LDD_I2C_NO_SEND_STOP);
	if (result != ERR_OK)
	{
		return ERR_FAILED;
	}
	while(deviceData.dataTransmittedFlg == FALSE) {} //wait until transmitted
	deviceData.dataTransmittedFlg = FALSE;
	
	result = CI2C1_MasterReceiveBlock(deviceData.handle,&temp_read,sizeof(temp_read),LDD_I2C_SEND_STOP); //receive 2 bytes of temperature data
	if (result != ERR_OK)
	{
		return ERR_FAILED;
	}
	while(deviceData.dataReceivedFlg == FALSE) {} //wait until received
	deviceData.dataReceivedFlg = FALSE;
	
	temp_read = temp_read & 0x00FF; //truncate .5 C bit
	return temp_read;
}

void InitDS1621()
{
	deviceData.handle = CI2C1_Init(&deviceData);
	deviceData.dataReceivedFlg = FALSE;
	deviceData.dataTransmittedFlg = FALSE;
	
	while(ConfigureThermometer() != 0) {}
	while(StartConversion() != 0) {}
}
