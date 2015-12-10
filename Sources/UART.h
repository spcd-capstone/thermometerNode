/*
 * UART.h
 *
 *  Created on: Dec 1, 2015
 *      Author: Student
 */

#ifndef UART_H_
#define UART_H_

#include "PE_Types.h"
#include "PE_LDD.h"
#include "PE_Error.h"
#include "stdio.h"

typedef enum {
	PDT_STRING,
	PDT_INTEGER
} parser_dataType;

typedef enum {
	PC_GET,
	PC_SET
}parser_command;

typedef enum {
	PS_COMMAND,
	PS_KEYLENGTH,
	PS_KEY,
	PS_DATATYPE,
	PS_DATALENGTH,
	PS_DATASTRING,
	PS_DATAINT,
	PS_READY
} parser_state;

typedef struct {
	parser_state state;
	parser_dataType dataType;
	parser_command command;
	unsigned int keyLength;
	unsigned int dataLength;
	char key[32];
	char dataString[64];
	unsigned int dataInt;
	unsigned int stateLength;
} parser;

char GetChar();
void InitParser(parser* parser);
void ParseChar(parser* parser, char c);

void SendError(unsigned char* str);
void SendSuccess(unsigned char* str);
void SendInteger(unsigned int num);
void SendString(unsigned char* str);

#endif /* UART_H_ */
