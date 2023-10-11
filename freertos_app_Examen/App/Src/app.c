/* Copyright 2020, Juan Manuel Cruz.
 * All rights reserved.
 *
 * This file is part of Project => freertos_app_Example001
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
#include <task_Entry.h>
#include <task_Exit.h>

/* Demo includes. */
#include "supporting_Functions.h"

/* Application & Tasks includes. */
#include "app.h"
#include "app_Resources.h"
#include "task_Entry.h"
#include "task_Exit.h"
#include "task_Monitor.h"
#include "task_Test.h"

// ------ Macros and definitions ---------------------------------------

// ------ internal data declaration ------------------------------------
/* Declare a variable of type xSemaphoreHandle.  This is used to reference the
 * semaphore that is used to synchronize a task with other task. */
xSemaphoreHandle EntrySemaphores[ENTRY_TOTAL];
xSemaphoreHandle ExitSemaphores[EXIT_TOTAL];

/* Declare a variable of type xSemaphoreHandle.  This is used to reference the
 * mutex type semaphore that is used to ensure mutual exclusive access to ........ */
xSemaphoreHandle xMutexSemaphoreTask_A;

/* Counting Semaphore that controls the bridge capacity */
xSemaphoreHandle xCountingSemaphoreTask_A;

/* Declare a variable of type xTaskHandle. This is used to reference tasks. */
xTaskHandle EntryTasks[ENTRY_TOTAL];
xTaskHandle ExitTasks[EXIT_TOTAL];
xTaskHandle vTask_TestHandle;
xTaskHandle vTask_MonitorHandle;

// Queues
QueueHandle_t xQueueVehicle;
QueueHandle_t xQueueVehicleDateTime;

/* Task Entry/Exit Vehicle Counter	*/
uint32_t	lTasksCnt;

// ------ internal functions declaration -------------------------------

// ------ internal data definition -------------------------------------
static const char *pcTextForMain = "Examen is running: narrow vehicular bridge\r\n\n";
static const char* SEMAPHORE_NAME = "Semaphore_";
static char entry_semaphores_names[ENTRY_TOTAL][30] = {0};
static char exit_semaphores_names[EXIT_TOTAL][30] = {0};

EntryTaskData ENTRY_TASK_DATA_ARRAY[ENTRY_TOTAL] =
{
	[ENTRY_A] = {
			.entry_semaphore = &EntrySemaphores[ENTRY_A],
			.opposite_semaphore = &EntrySemaphores[ENTRY_B],
			.continue_semaphore = &xCountingSemaphoreTask_A,
	},
	[ENTRY_B] = {
			.entry_semaphore = &EntrySemaphores[ENTRY_B],
			.opposite_semaphore = &EntrySemaphores[ENTRY_A],
			.continue_semaphore = &xCountingSemaphoreTask_A,
	},
};

ExitTaskData EXIT_TASK_DATA_ARRAY[EXIT_TOTAL] =
{
	[EXIT_A] = {
			.exit_semaphore = &ExitSemaphores[EXIT_A],
			.continue_semaphore = &xCountingSemaphoreTask_A,
	},
	[EXIT_B] = {
			.exit_semaphore = &ExitSemaphores[EXIT_B],
			.continue_semaphore = &xCountingSemaphoreTask_A,
	},
};

// ------ external data definition -------------------------------------

// ------ internal functions definition --------------------------------

// ------ external functions definition --------------------------------

char* entry_to_str(EntryType entry) {
	 switch (entry) {
		case ENTRY_A:
			return "ENTRY_A";
		case ENTRY_B:
			return "ENTRY_B";
		default:
			return "";
	}
}

char* exit_to_str(ExitType exit) {
	 switch (exit) {
		case EXIT_A:
			return "EXIT_A";
		case EXIT_B:
			return "EXIT_B";
		default:
			return "";
	}
}



/*------------------------------------------------------------------*/
/* App Initialization */
void appInit( void )
{
	/* Print out the name of this Example. */
  	vPrintString( pcTextForMain );

    /* Before a semaphore is used it must be explicitly created.
     * In this example a binary semaphore is created. */
  	for(uint8_t i = 0; i < ENTRY_TOTAL; i++) {
  		snprintf(entry_semaphores_names[i], 30, "%s%s", SEMAPHORE_NAME, entry_to_str(i));
  		vSemaphoreCreateBinary(EntrySemaphores[i]);
  		configASSERT(EntrySemaphores[i] !=  NULL);
  		vQueueAddToRegistry(EntrySemaphores[i], entry_semaphores_names[i]);
  	}

  	for(uint8_t i = 0; i < EXIT_TOTAL; i++) {
  		snprintf(exit_semaphores_names[i], 30, "%s%s", SEMAPHORE_NAME, exit_to_str(i));
  		vSemaphoreCreateBinary(ExitSemaphores[i]);
  		configASSERT(ExitSemaphores[i] !=  NULL);
  		vQueueAddToRegistry(ExitSemaphores[i], exit_semaphores_names[i]);
  	}

    // Create Mutex
    xMutexSemaphoreTask_A = xSemaphoreCreateMutex();
    // Check the mutex was created successfully.
    configASSERT( xMutexSemaphoreTask_A !=  NULL );
    // Add mutex to registry.
	vQueueAddToRegistry(xMutexSemaphoreTask_A, "xMutexSemaphoreTask_A");


	// Create counting semaphore
    xCountingSemaphoreTask_A = xSemaphoreCreateCounting(lTasksCntMAX, lTasksCntMAX);
    // Check the semaphore was created successfully.
	configASSERT( xCountingSemaphoreTask_A !=  NULL );
	// Add Counting Semaphore to registry
	vQueueAddToRegistry(xCountingSemaphoreTask_A, "xCountingSemaphoreTask_A");

    /* Before a queue is used it must be explicitly created. */
	xQueueVehicle = xQueueCreate( 10, sizeof( Vehicle ) );
	/* Check the queues was created successfully */
	configASSERT( xQueueVehicle != NULL );
    /* We want this queue to be viewable in a RTOS kernel aware debugger, so register it. */
    vQueueAddToRegistry( xQueueVehicle, "xQueueVehicle" );

    /* Before a queue is used it must be explicitly created. */
    xQueueVehicleDateTime = xQueueCreate( 32, sizeof( VehicleDateTime ) );
	/* Check the queues was created successfully */
	configASSERT( xQueueVehicleDateTime != NULL );
    /* We want this queue to be viewable in a RTOS kernel aware debugger, so register it. */
    vQueueAddToRegistry( xQueueVehicleDateTime, "xQueueVehicleDateTime" );

    BaseType_t ret;

    /* Task A thread at priority 2 */
    ret = xTaskCreate( vTask_X_Entry,				   /* Pointer to the function thats implement the task. */
    			       entry_to_str(ENTRY_A),			   /* Text name for the task. This is to facilitate debugging only. */
					   (2 * configMINIMAL_STACK_SIZE), /* Stack depth in words. */
					   &ENTRY_TASK_DATA_ARRAY[ENTRY_A],						   /* We are not using the task parameter. */
					   (tskIDLE_PRIORITY + 2UL),	   /* This task will run at priority 1. */
					   &EntryTasks[ENTRY_A] );		   /* We are using a variable as task handle. */

    /* Check the task was created successfully. */
    configASSERT( ret == pdPASS );

    /* Task B thread at priority 2 */
    ret = xTaskCreate( vTask_X_Exit,				   /* Pointer to the function thats implement the task. */
					   exit_to_str(EXIT_A),				   /* Text name for the task. This is to facilitate debugging only. */
					   (2 * configMINIMAL_STACK_SIZE), /* Stack depth in words. */
					   &EXIT_TASK_DATA_ARRAY[EXIT_A],						   /* We are not using the task parameter. */
					   (tskIDLE_PRIORITY + 2UL),	   /* This task will run at priority 1. */
					   &ExitTasks[EXIT_A] );		   /* We are using a variable as task handle. */

    /* Check the task was created successfully. */
    configASSERT( ret == pdPASS );


    /* Task Monitor thread at priority 1 */
    ret = xTaskCreate( vTask_Monitor,				   /* Pointer to the function thats implement the task. */
					   "Task Monitor",				   /* Text name for the task. This is to facilitate debugging only. */
					   (2 * configMINIMAL_STACK_SIZE), /* Stack depth in words. */
					   NULL,						   /* We are not using the task parameter. */
					   (tskIDLE_PRIORITY + 1UL),	   /* This task will run at priority 1. */
					   &vTask_MonitorHandle);		   /* We are using a variable as task handle. */

    /* Check the task was created successfully. */
    configASSERT( ret == pdPASS );

	/* Task Test at priority 1, periodically excites the other tasks */
    ret = xTaskCreate( vTask_Test,					   /* Pointer to the function thats implement the task. */
					   "Task Test",					   /* Text name for the task. This is to facilitate debugging only. */
					   (2 * configMINIMAL_STACK_SIZE), /* Stack depth in words. */
					   NULL,						   /* We are not using the task parameter. */
					   (tskIDLE_PRIORITY + 1UL),	   /* This task will run at priority 2. */
					   &vTask_TestHandle );			   /* We are using a variable as task handle. */

    /* Check the task was created successfully. */
    configASSERT( ret == pdPASS );
}

/*------------------------------------------------------------------*-
  ---- END OF FILE -------------------------------------------------
-*------------------------------------------------------------------*/
