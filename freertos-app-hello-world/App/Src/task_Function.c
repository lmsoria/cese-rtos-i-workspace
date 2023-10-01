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
#include "cmsis_os.h"
#include "API_button.h"
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


// ------ external data definition -------------------------------------

// ------ internal functions definition --------------------------------

// ------ external functions definition --------------------------------

/*------------------------------------------------------------------*/
/* Task Function thread */
void vTaskFunction( void *pvParameters )
{
	/*  Declare & Initialize Task Function variables for argument, led, button and task */
	TaskData* const DATA = (TaskData*)(pvParameters);

	// Let's assume we won't change the target LED nor the button during program execution.
	const BoardLEDs LED = DATA->led;
	const BoardButtons BUTTON = DATA->button;

	ledFlag_t ledFlag = NotBlinking;
	uint32_t button_held = 0;


	char *pcTaskName = (char *) pcTaskGetName( NULL );

	/* Print out the name of this task. */
	vPrintTwoStrings( pcTaskName, pcTextForTask_IsRunning );

	/* As per most tasks, this task is implemented in an infinite loop. */
	TickType_t xLastWakeTime = xTaskGetTickCount();
	for( ;; )
	{
		/* Check HW Button State */
		if( button_read(BUTTON) == BUTTON_PRESSED) {
			/* Delay for a period using Tick Count */

			if(!button_held) {
				button_held = 1;
				vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(buttonTickCntMAX));
			} else {
        		/* Check, Update and Print Led Flag */
				if( ledFlag == NotBlinking ) {
					ledFlag = Blinking;
                	vPrintTwoStrings( pcTaskName, pcTextForTask_BlinkingOn );
				} else {
					ledFlag = NotBlinking;
                	vPrintTwoStrings( pcTaskName, pcTextForTask_BlinkingOff );
				}
				button_held = 1;
			}
		} else {
			button_held = 0;
			vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(buttonTickCntMAX));
		}

		/* Check Led Flag */
		if( ledFlag == Blinking )
		{
			/* Delay for a period using Tick Count. */
			led_toggle(LED);
			vTaskDelay(ledTickCntMAX);
		}
	}
}

/*------------------------------------------------------------------*-
  ---- END OF FILE -------------------------------------------------
-*------------------------------------------------------------------*/
