/* ###################################################################
**     Filename    : main.c
**     Project     : themometerNode
**     Processor   : MKL25Z128VLK4
**     Version     : Driver 01.01
**     Compiler    : GNU C Compiler
**     Date/Time   : 2015-11-15, 12:59, # CodeGen: 0
**     Abstract    :
**         Main module.
**         This module contains user's application code.
**     Settings    :
**     Contents    :
**         No public methods
**
** ###################################################################*/
/*!
** @file main.c
** @version 01.01
** @brief
**         Main module.
**         This module contains user's application code.
*/         
/*!
**  @addtogroup main_module main module documentation
**  @{
*/         
/* MODULE main */


/* Including needed modules to compile this module/procedure */
#include "Cpu.h"
#include "Events.h"
#include "CI2C1.h"
#include "LED1.h"
#include "LEDpin1.h"
#include "BitIoLdd1.h"
#include "AS1.h"
#include "ASerialLdd1.h"
/* Including shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"

/* User includes (#include below this line is not maintained by Processor Expert) */
#include "DS1621.h"
#include "UART.h"
#include "string.h"

parser commandParser;

/*lint -save  -e970 Disable MISRA rule (6.3) checking. */
int main(void)
/*lint -restore Enable MISRA rule (6.3) checking. */
{
  /* Write your local variable definition here */
  unsigned int target, temperature;
  unsigned int counter;
  char name[64];
  /*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
  PE_low_level_init();
  /*** End of Processor Expert internal initialization.                    ***/

  /* Write your code here */
  InitDS1621();
  InitParser(&commandParser);
  
  target = 0; //set target so we don't turn on the furnace before we have a real target
  counter = 0;
  strcpy(name,"Thermometer Node");
  
  while(1)
  {
	  //execute command if one is ready
	  if (commandParser.state == PS_READY)
	  {
		  if (commandParser.command == PC_GET)
		  {
			  if (strcmp("temperature", commandParser.key) == 0)
			  {
				  SendInteger(temperature);
			  }
			  else if (strcmp("target", commandParser.key) == 0)
			  {
				  SendInteger(target);
			  }
			  else if (strcmp("name", commandParser.key) == 0)
			  {
				  SendString((unsigned char*)name);
			  }
			  else
			  {
				  SendError();
			  }
		  }
		  else if (commandParser.command == PC_SET)
		  {
			  if ((strcmp("target", commandParser.key) == 0) && (commandParser.dataType == PDT_INTEGER))
			  {
				  target = commandParser.dataInt;
				  SendSuccess();
			  }
			  else if ((strcmp("name", commandParser.key) == 0) && (commandParser.dataType == PDT_STRING))
			  {
				  strcpy(name,commandParser.dataString);
				  SendSuccess();
			  }
			  else
			  {
				  SendError();
			  }
		  }
		  commandParser.state = PS_COMMAND;
	  }
	  //normal operations
	  //update temperature periodically (~4-5 seconds)
	  if (counter >= 2000000) //2 mil
	  {
		  temperature = ReadTemperature();
		  counter = 0;
	  }
	  else
	  {
		  counter++;
	  }
	  //control furnace
	  if (temperature < target)
	  {
		  LED1_Put(1); //turn furnace on
	  }
	  else
	  {
		  LED1_Put(0); //shut furnace off
	  }
  }
  /* For example: for(;;) { } */

  /*** Don't write any code pass this line, or it will be deleted during code generation. ***/
  /*** RTOS startup code. Macro PEX_RTOS_START is defined by the RTOS component. DON'T MODIFY THIS CODE!!! ***/
  #ifdef PEX_RTOS_START
    PEX_RTOS_START();                  /* Startup of the selected RTOS. Macro is defined by the RTOS component. */
  #endif
  /*** End of RTOS startup code.  ***/
  /*** Processor Expert end of main routine. DON'T MODIFY THIS CODE!!! ***/
  for(;;){}
  /*** Processor Expert end of main routine. DON'T WRITE CODE BELOW!!! ***/
} /*** End of main routine. DO NOT MODIFY THIS TEXT!!! ***/

/* END main */
/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.3 [05.09]
**     for the Freescale Kinetis series of microcontrollers.
**
** ###################################################################
*/
