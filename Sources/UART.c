/*
 * UART.c
 *
 *  Created on: Dec 1, 2015
 *      Author: Student
 */

#include "UART.h"
#include <math.h>

bool EmptyFlg;

char GetChar()
{
	AS1_TComData rec;
	byte result;
	
	result = AS1_RecvChar(&rec);
	if (result == ERR_OK)
	{
		return rec;
	}
	else
	{
		return ' ';
	}
}

void InitParser(parser* parser)
{
	parser->state = PS_COMMAND;
	parser->keyLength = 0;
	parser->dataLength = 0;
	parser->stateLength = 0;
}

void SendError(unsigned char* str)
{	
	unsigned char i;
	word size;
	word sent;
	unsigned char string[128];
	word length;
	
	for(i = 0; i < 128; i++)
	{
		if (str[i] == '\0')
		{
			length = i;
			break;
		}
	}
	
	sprintf((char*)string, "-%d:%s", length, str);
	for(i = 0; i < 128; i++)
	{
		if (string[i] == '\0')
		{
			size = i;
			break;
		}
	}
	
	AS1_SendBlock(string, size, &sent);
}

void SendSuccess(unsigned char* str)
{
	unsigned char i;
	word size;
	word sent;
	unsigned char string[128];
	word length;
	
	for(i = 0; i < 128; i++)
	{
		if (str[i] == '\0')
		{
			length = i;
			break;
		}
	}
	
	sprintf((char*)string, "+%d:%s", length, str);
	for(i = 0; i < 128; i++)
	{
		if (string[i] == '\0')
		{
			size = i;
			break;
		}
	}
	
	AS1_SendBlock(string, size, &sent);
}

void SendInteger(unsigned int num)
{
	unsigned char i;
	word size;
	word sent;
	unsigned char string[128];
	
	sprintf((char*)string, "+i%de", num);
	for(i = 0; i < 128; i++)
	{
		if (string[i] == '\0')
		{
			size = i;
			break;
		}
	}
	
	AS1_SendBlock(string, size, &sent);
}

void SendString(unsigned char* str)
{
	unsigned char i;
	word size;
	word sent;
	unsigned char string[128];
	word length;
	
	for(i = 0; i < 128; i++)
	{
		if (str[i] == '\0')
		{
			length = i;
			break;
		}
	}
	
	sprintf((char*)string, "+%d:%s", length, str);
	for(i = 0; i < 128; i++)
	{
		if (string[i] == '\0')
		{
			size = i;
			break;
		}
	}
	
	AS1_SendBlock(string, size, &sent);
}

void ParseChar(parser* parser, char c)
{
	if (parser->state == PS_COMMAND)
	{
		if (c == 'g')
		{
			parser->state = PS_KEYLENGTH;
			parser->command = PC_GET;
			parser->keyLength = 0;
		}
		else if (c == 's')
		{
			parser->state = PS_KEYLENGTH;
			parser->command = PC_SET;
			parser->keyLength = 0;
		}
	}
	else if (parser->state == PS_KEYLENGTH)
	{
		if (c == ':')
		{
			parser->state = PS_KEY;
			parser->stateLength = 0;
		}
		else
		{
			parser->keyLength = parser->keyLength * 10;
			parser->keyLength += c - '0';
		}
	}
	else if (parser->state == PS_KEY)
	{
		if (parser->stateLength >= parser->keyLength - 1)
		{
			if (parser->command == PC_GET)
			{
				parser->state = PS_READY;
				parser->key[parser->stateLength] = c;
				parser->key[parser->stateLength+1] = '\0';
			}
			else if (parser->command == PC_SET)
			{
				parser->state = PS_DATATYPE;
				parser->key[parser->stateLength] = c;
				parser->key[parser->stateLength+1] = '\0';
			}
		}
		else
		{
			parser->key[parser->stateLength] = c;
			parser->stateLength++;
		}
	}
	else if (parser->state == PS_DATATYPE)
	{
		if (c == 'i')
		{
			parser->state = PS_DATAINT;
			parser->dataType = PDT_INTEGER;
			parser->dataInt = 0;
		}
		else
		{
			parser->state = PS_DATALENGTH;
			parser->dataType = PDT_STRING;
			parser->dataLength = c - '0';
		}
	}
	else if (parser->state == PS_DATALENGTH)
	{
		if (c == ':')
		{
			parser->state = PS_DATASTRING;
			parser->stateLength = 0;
		}
		else
		{
			parser->dataLength = parser->dataLength * 10;
			parser->dataLength += c - '0';
		}
	}
	else if (parser->state == PS_DATASTRING)
	{
		if (parser->stateLength >= parser->dataLength - 1)
		{
			parser->state = PS_READY;
			parser->dataString[parser->stateLength] = c;
			parser->dataString[parser->stateLength+1] = '\0';
		}
		else
		{
			parser->dataString[parser->stateLength] = c;
			parser->stateLength++;
		}
	}
	else if (parser->state == PS_DATAINT)
	{
		if (c == 'e')
		{
			parser->state = PS_READY;
		}
		else
		{
			parser->dataInt = parser->dataInt * 10;
			parser->dataInt += c - '0';
		}
	}
	else if (parser->state == PS_READY)
	{
		//waiting for previous command to be executed, ignore (probably?) trailing chars
	}
}
