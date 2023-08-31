/* Copyright 2020, Juan Manuel Cruz.
 * All rights reserved.
 *
 * This file is part of Project => freertos_app_Example2_6
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */


/*--------------------------------------------------------------------*-

    task_Function.c (Released 2022-10)

--------------------------------------------------------------------

    task file for FreeRTOS - Event Driven System (EDS) - Project for
    STM32F429ZI_NUCLEO_144.

    See readme.txt for project information.

-*--------------------------------------------------------------------*/


// ------ Includes -------------------------------------------------
/* Project includes. */
#include "main.h"
#include "cmsis_os.h"
#include "API_leds.h"

/* Standard includes. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* Demo includes. */
#include "supporting_Functions.h"

/* Application includes. */
#include "app_Resources.h"
#include "task_Function.h"

// ------ Macros and definitions ---------------------------------------

// ------ internal data declaration ------------------------------------

// ------ internal functions declaration -------------------------------

// ------ internal data definition -------------------------------------
/* Define the strings that will be passed in as the Supporting Functions parameters.
 * These are defined const and off the stack to ensure they remain valid when the
 * tasks are executing. */
const char *pcTextForTask_IsRunning 	= " - is running\r\n";

const char *pcTextForTask_LDXTOn		= " - LDX turn On \r\n";
const char *pcTextForTask_LDXTOff		= " - LDX turn Off\r\n";

const char *pcTextForTask_BlinkingOn	= " - Blinking turn On \r\n";
const char *pcTextForTask_BlinkingOff	= " - Blinking turn Off\r\n";

#define 		buttonTickCntMAX	500
#define			ledTickCntMAX		500
typedef enum	ledFlag_e{ Blinking, NotBlinking } ledFlag_t;

uint16_t	  LDX_Pin[]			= { LD1_Pin,       LD2_Pin,       LD3_Pin };
GPIO_TypeDef* LDX_GPIO_Port[]	= { LD1_GPIO_Port, LD2_GPIO_Port, LD3_GPIO_Port };

// ------ external data definition -------------------------------------

// ------ internal functions definition --------------------------------

// ------ external functions definition --------------------------------

/*------------------------------------------------------------------*/
/* Task Function thread */
void vTaskFunction( void *pvParameters )
{
	/* The buffer used to hold the run time stats text needs to be quite large.  It
	   is therefore declared static to ensure it is not allocated on the task stack.
	   This makes this function non re-entrant. */
	static char cStringBuffer[ 512 ];

	/*  Declare & Initialize Task Function variables for argument, led, button and task */
	TaskData* const DATA = (TaskData*)(pvParameters);

	// Let's assume we won't change the target LED during program execution.
	const BoardLEDs LED = DATA->led;

	ledFlag_t ledFlag = NotBlinking;
	LEDStatus ledState = LED_OFF;
	TickType_t ledTickCnt = xTaskGetTickCount();

	TickType_t buttonTickCnt = xTaskGetTickCount();

	char *pcTaskName = (char *) pcTaskGetName( NULL );

	/* Print out the name of this task. */
	vPrintTwoStrings( pcTaskName, pcTextForTask_IsRunning );

	/* As per most tasks, this task is implemented in an infinite loop. */
	for( ;; )
	{
		/* Generate a text table from the run time stats.  This must fit into
		the cStringBuffer array.
		https://www.freertos.org/a00021.html#vTaskGetRunTimeStats */
		vTaskGetRunTimeStats( cStringBuffer );

		/* Check HW Button State */
		if( HAL_GPIO_ReadPin( USER_Btn_GPIO_Port, USER_Btn_Pin ) == GPIO_PIN_SET )
		{
			/* Delay for a period using Tick Count */
			if( ( xTaskGetTickCount() - buttonTickCnt ) >= buttonTickCntMAX )
			{
        		/* Check, Update and Print Led Flag */
				if( ledFlag == NotBlinking )
				{
					ledFlag = Blinking;
                	vPrintTwoStrings( pcTaskName, pcTextForTask_BlinkingOn );
				}
				else
				{
					ledFlag = NotBlinking;
                	vPrintTwoStrings( pcTaskName, pcTextForTask_BlinkingOff );
				}
				/* Update and Button Tick Counter */
        		buttonTickCnt = xTaskGetTickCount();
			}
		}

		/* Check Led Flag */
		if( ledFlag == Blinking )
		{
			/* Delay for a period using Tick Count. */
			if( ( xTaskGetTickCount() - ledTickCnt ) >= ledTickCntMAX )
			{
				/* Check, Update and Print Led State */
		    	if( ledState == LED_OFF )
		    	{
		    		ledState = LED_ON;
                	vPrintTwoStrings( pcTaskName, pcTextForTask_LDXTOn );
		    	}
		    	else
		    	{
		    		ledState = LED_OFF;
                	vPrintTwoStrings( pcTaskName, pcTextForTask_LDXTOff );
		    	}
				/* Update HW Led State */
		    	led_write(LED, ledState);

				/* Update and Led Tick Counter */
				ledTickCnt = xTaskGetTickCount();
			}
		}
	}
}

/*------------------------------------------------------------------*-
  ---- END OF FILE -------------------------------------------------
-*------------------------------------------------------------------*/
