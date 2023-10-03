/*
 * task_button.c
 *
 *  Created on: Oct 1, 2023
 *      Author: lsoria
 */
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
#include "task_button.h"

// ------ Macros and definitions ---------------------------------------

// ------ internal data declaration ------------------------------------

// ------ internal functions declaration -------------------------------

// ------ internal data definition -------------------------------------

#define 		buttonTickCntMAX	500

static const char *pcTextForTask_IsRunning 	= " - is running\r\n";
static const char *pcTextForTask_BlinkingOn	= " - Blinking turn On \r\n";
static const char *pcTextForTask_BlinkingOff	= " - Blinking turn Off\r\n";

void vTaskButton(void* pvParameters)
{
	/*  Declare & Initialize Task Function variables for argument, led, button and task */
	TaskData* const DATA = (TaskData*)(pvParameters);

	// Let's assume we won't change the target LED nor the button during program execution.
	const BoardButtons BUTTON = DATA->button;

	char *pcTaskName = (char *) pcTaskGetName( NULL );

	ledFlag_t led_status = NOT_BLINKING;

	/* Print out the name of this task. */
	vPrintTwoStrings( pcTaskName, pcTextForTask_IsRunning );

	/* As per most tasks, this task is implemented in an infinite loop. */
	for( ;; )
	{
		/* Check HW Button State */
		if( button_read(BUTTON) == BUTTON_PRESSED )	{
			/* Check, Update and Print Led Flag */
			if( led_status == NOT_BLINKING ) {
				led_status = BLINKING;
				vPrintTwoStrings( pcTaskName, pcTextForTask_BlinkingOn );
			} else {
				led_status = NOT_BLINKING;
				vPrintTwoStrings( pcTaskName, pcTextForTask_BlinkingOff );
			}

			xQueueSend(QueueHandle, &led_status, portMAX_DELAY);
		}

		vTaskDelay( pdMS_TO_TICKS(buttonTickCntMAX) );
	}
}
