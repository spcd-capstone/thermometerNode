/*
 * DS1621.h
 *
 *  Created on: Nov 25, 2015
 *      Author: Student
 */

#ifndef DS1621_H_
#define DS1621_H_

#include "PE_Types.h"
#include "PE_LDD.h"
 
typedef struct {
  volatile unsigned char dataReceivedFlg; /* set to TRUE by the interrupt if we have received data */
  volatile unsigned char dataTransmittedFlg; /* set to TRUE by the interrupt if we have set data */
  LDD_TDeviceData *handle; /* pointer to the device handle */
} DS1621_TDataState;

int ConfigureThermometer();
int StartConversion();
int ReadTemperature();

void InitDS1621();

#endif /* DS1621_H_ */
