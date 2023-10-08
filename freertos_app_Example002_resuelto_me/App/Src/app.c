/* Copyright 2020, Juan Manuel Cruz.
 * All rights reserved.
 *
 * This file is part of Project => freertos_app_Example002
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

    app.c (Released 2022-06)

--------------------------------------------------------------------

    app file for FreeRTOS - Event Driven System (EDS) - Project for
    STM32F429ZI_NUCLEO_144.

    See readme.txt for project information.

-*--------------------------------------------------------------------*/


// ------ Includes -----------------------------------------------------
/* Project includes. */
#include "main.h"
#include "cmsis_os.h"

/* Standard includes. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* Demo includes. */
#include "supporting_Functions.h"

/* Application & Tasks includes. */
#include "app.h"
#include "app_Resources.h"
#include "task_A.h"
#include "task_B.h"
#include "task_Test.h"

// ------ Macros and definitions ---------------------------------------

// ------ internal data declaration ------------------------------------
/* Declare a variable of type xSemaphoreHandle.  This is used to reference the
 * semaphore that is used to synchronize a task with other task. */
xSemaphoreHandle xBinarySemaphoreContinue;

xSemaphoreHandle EntrySemaphores[TOTAL_ENTRADAS];
xSemaphoreHandle ExitSemaphores[TOTAL_SALIDAS];

/* Declare a variable of type xSemaphoreHandle.  This is used to reference the
 * mutex type semaphore that is used to ensure mutual exclusive access to ........ */
xSemaphoreHandle xMutex;

/* Declare a variable of type xTaskHandle. This is used to reference tasks. */
xTaskHandle EntryTasks[TOTAL_ENTRADAS];
xTaskHandle ExitTasks[TOTAL_SALIDAS];
xTaskHandle vTask_TestHandle;

/* Task A & B Counter	*/
uint32_t	lugares_ocupados;


EntryTaskData ENTRY_TASK_DATA_ARRAY[TOTAL_ENTRADAS] =
{
	[ENTRADA_A] = {
			.name = "ENTRADA_A",
			.entry_semaphore = &EntrySemaphores[ENTRADA_A],
			.continue_semaphore = &xBinarySemaphoreContinue,
	},
	[ENTRADA_B] = {
			.name = "ENTRADA_B",
			.entry_semaphore = &EntrySemaphores[ENTRADA_B],
			.continue_semaphore = &xBinarySemaphoreContinue,
	},
	[ENTRADA_C] = {
			.name = "ENTRADA_C",
			.entry_semaphore = &EntrySemaphores[ENTRADA_C],
			.continue_semaphore = &xBinarySemaphoreContinue,
	},
};

ExitTaskData EXIT_TASK_DATA_ARRAY[TOTAL_SALIDAS] =
{
	[SALIDA_A] = {
			.name = "SALIDA_A",
			.exit_semaphore = &ExitSemaphores[SALIDA_A],
			.continue_semaphore = &xBinarySemaphoreContinue,
	},
	[SALIDA_B] = {
			.name = "SALIDA_B",
			.exit_semaphore = &ExitSemaphores[SALIDA_B],
			.continue_semaphore = &xBinarySemaphoreContinue,
	},
	[SALIDA_C] = {
			.name = "SALIDA_C",
			.exit_semaphore = &ExitSemaphores[SALIDA_C],
			.continue_semaphore = &xBinarySemaphoreContinue,
	},
};

// ------ internal functions declaration -------------------------------

// ------ internal data definition -------------------------------------
const char *pcTextForMain = "freertos_app_Example002 is running: parking lot\r\n\n";
const char* SEMAPHORE_NAME = "Semaphore_";

static char entry_semaphores_names[TOTAL_ENTRADAS][30] = {0};
static char exit_semaphores_names[TOTAL_SALIDAS][30] = {0};

// ------ external data definition -------------------------------------

// ------ internal functions definition --------------------------------
static char* entry_to_str(EntryType entry) {
	 switch (entry) {
		case ENTRADA_A:
			return "ENTRADA_A";
		case ENTRADA_B:
			return "ENTRADA_B";
		case ENTRADA_C:
			return "ENTRADA_C";
		default:
			return "";
	}
}

static char* exit_to_str(ExitType exit) {
	 switch (exit) {
		case SALIDA_A:
			return "SALIDA_A";
		case SALIDA_B:
			return "SALIDA_B";
		case SALIDA_C:
			return "SALIDA_C";
		default:
			return "";
	}
}
// ------ external functions definition --------------------------------


/*------------------------------------------------------------------*/
/* App Initialization */
void appInit( void )
{
	/* Print out the name of this Example. */
  	vPrintString( pcTextForMain );

  	for(uint8_t i = 0; i < TOTAL_ENTRADAS; i++) {
  		snprintf(entry_semaphores_names[i], 30, "%s%s", SEMAPHORE_NAME, entry_to_str(i));
  		vSemaphoreCreateBinary(EntrySemaphores[i]);
  		configASSERT(EntrySemaphores[i] !=  NULL);
  		vQueueAddToRegistry(EntrySemaphores[i], entry_semaphores_names[i]);
  	}

  	for(uint8_t i = 0; i < TOTAL_SALIDAS; i++) {
  		snprintf(exit_semaphores_names[i], 30, "%s%s", SEMAPHORE_NAME, exit_to_str(i));
  		vSemaphoreCreateBinary(ExitSemaphores[i]);
  		configASSERT(ExitSemaphores[i] !=  NULL);
  		vQueueAddToRegistry(ExitSemaphores[i], exit_semaphores_names[i]);
  	}

    /* Before a semaphore is used it must be explicitly created.
     * In this example a binary semaphore is created. */
    vSemaphoreCreateBinary( xBinarySemaphoreContinue );

    /* Check the semaphore was created successfully. */
	configASSERT( xBinarySemaphoreContinue !=  NULL );

    /* Add semaphore to registry. */
    vQueueAddToRegistry(xBinarySemaphoreContinue, "xBinarySemaphoreContinue");

    /* Before a semaphore is used it must be explicitly created.
     * In this example a mutex semaphore is created. */
    xMutex = xSemaphoreCreateMutex();

    /* Check the mutex was created successfully. */
    configASSERT( xMutex !=  NULL );

    /* Add mutex to registry. */
	vQueueAddToRegistry(xMutex, "xMutex");

	BaseType_t ret;

    /* Task A thread at priority 2 */
    ret = xTaskCreate( vTask_A,						/* Pointer to the function thats implement the task. */
					   entry_to_str(ENTRADA_A),					/* Text name for the task. This is to facilitate debugging only. */
					   (2 * configMINIMAL_STACK_SIZE),	/* Stack depth in words. 				*/
					   &ENTRY_TASK_DATA_ARRAY[0],						/* We are not using the task parameter.		*/
					   (tskIDLE_PRIORITY + 2UL),	/* This task will run at priority 1. 		*/
					   &EntryTasks[ENTRADA_A] );				/* We are using a variable as task handle.	*/

    /* Check the task was created successfully. */
    configASSERT( ret == pdPASS );

    /* Task B thread at priority 2 */
    ret = xTaskCreate( vTask_B,						/* Pointer to the function thats implement the task. */
					   exit_to_str(SALIDA_A),					/* Text name for the task. This is to facilitate debugging only. */
					   (2 * configMINIMAL_STACK_SIZE),	/* Stack depth in words. 				*/
					   &EXIT_TASK_DATA_ARRAY[0],						/* We are not using the task parameter.		*/
					   (tskIDLE_PRIORITY + 2UL),	/* This task will run at priority 1. 		*/
					   &ExitTasks[SALIDA_A] );				/* We are using a variable as task handle.	*/

    /* Check the task was created successfully. */
    configASSERT( ret == pdPASS );

	/* Task Test at priority 1, periodically excites the other tasks */
    ret = xTaskCreate( vTask_Test,					/* Pointer to the function thats implement the task. */
					   "Task Test",					/* Text name for the task. This is to facilitate debugging only. */
					   (2 * configMINIMAL_STACK_SIZE),	/* Stack depth in words. 				*/
					   NULL,						/* We are not using the task parameter.		*/
					   (tskIDLE_PRIORITY + 1UL),	/* This task will run at priority 2. 		*/
					   &vTask_TestHandle );			/* We are using a variable as task handle.	*/

    /* Check the task was created successfully. */
    configASSERT( ret == pdPASS );
}

/*------------------------------------------------------------------*-
  ---- END OF FILE -------------------------------------------------
-*------------------------------------------------------------------*/
