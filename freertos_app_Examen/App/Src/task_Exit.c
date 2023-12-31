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

    task_B.c (Released 2022-06)

--------------------------------------------------------------------

    task file for FreeRTOS - Event Driven System (EDS) - Project for
    STM32F429ZI_NUCLEO_144.

    See readme.txt for project information.

-*--------------------------------------------------------------------*/


// ------ Includes -------------------------------------------------
/* Project includes. */
#include "main.h"
#include "cmsis_os.h"

/* Standard includes. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <task_Exit.h>

/* Demo includes. */
#include "supporting_Functions.h"

/* Application includes. */
#include "app_Resources.h"

// ------ Macros and definitions ---------------------------------------

// ------ internal data declaration ------------------------------------
uint32_t lTask_BCnt = 0;

// ------ internal functions declaration -------------------------------

// ------ internal data definition -------------------------------------
/* Define the strings that will be passed in as the Supporting Functions parameters.
 * These are defined const and off the stack to ensure they remain valid when the
 * tasks are executing. */
static const char *pcTextForTask_X_Exit    			    = " - Running\r\n";

static const char *pcTextForTask_X_lTasksCnt			= " | Lugares ocupados: ";

static const char *pcTextForTask_X_Exit_WaitExit	    = " | Espero que salga un auto...\r\n\n";
static const char *pcTextForTask_X_Exit_SignalContinue 	= " | Aviso que hay lugar disponible\r\n\n";


static const char *pcTextForTask_X_Exit_WaitMutex	    = " | Espero que la seccion critica este libre\r\n\n";
static const char *pcTextForTask_X_Exit_SignalMutex	    = " | Fin seccion critica. Devuelvo el Mutex\r\n\n";

// ------ external data definition -------------------------------------

// ------ internal functions definition --------------------------------

// ------ external functions definition --------------------------------

/*------------------------------------------------------------------*/
/* Task A thread (Exit) */
void vTask_X_Exit( void *pvParameters )
{
	ExitTaskData* const DATA = (ExitTaskData*)(pvParameters);

	const xSemaphoreHandle EXIT_SEMAPHORE = *DATA->exit_semaphore;
	const xSemaphoreHandle CONTINUE_SEMAPHORE = *DATA->continue_semaphore;

	uint32_t counter = 0;

	Vehicle last_vehicle;

	last_vehicle.task_handle = xTaskGetCurrentTaskHandle();


	/* Print out the name of this task. */
	vPrintTwoStrings(pcTaskGetName(NULL), pcTextForTask_X_Exit );

	/* As per most tasks, this task is implemented within an infinite loop.
	 *
	 * Take the semaphore once to start with so the semaphore is empty before the
	 * infinite loop is entered.  The semaphore was created before the scheduler
	 * was started so before this task ran for the first time.*/
    xSemaphoreTake( EXIT_SEMAPHORE, (portTickType) 0 );
    while( 1 )
    {
		/* Use the semaphore to wait for the event.  The task blocks
         * indefinitely meaning this function call will only return once the
         * semaphore has been successfully obtained - so there is no need to check
         * the returned value. */
    	vPrintTwoStrings(pcTaskGetName(NULL), pcTextForTask_X_Exit_WaitExit );
        xSemaphoreTake( EXIT_SEMAPHORE, portMAX_DELAY );
        {
        	vPrintTwoStrings(pcTaskGetName(NULL), " | Salio un auto!\r\n");
        	/* The semaphore is created before the scheduler is started so already
    		 * exists by the time this task executes.
    		 *
    		 * Attempt to take the semaphore, blocking indefinitely if the mutex is not
    		 * available immediately.  The call to xSemaphoreTake() will only return when
    		 * the semaphore has been successfully obtained so there is no need to check
    		 * the return value.  If any other delay period was used then the code must
    		 * check that xSemaphoreTake() returns pdTRUE before accessing the resource. */
        	vPrintTwoStrings(pcTaskGetName(NULL), pcTextForTask_X_Exit_WaitMutex);
			xSemaphoreTake(xBridgeMutex, portMAX_DELAY );
        	{
        		/* The following line will only execute once the semaphore has been
        		 * successfully obtained. */
        		vPrintTwoStrings(pcTaskGetName(NULL), " | Entre a la seccion critica\r\n");

        		/* Update Task A & B Counter */
        		if(lTasksCnt > 0) {
        			lTasksCnt--;
        		}

        		vPrintStringAndNumber(pcTextForTask_X_lTasksCnt, lTasksCnt);

    			/* 'Give' the semaphore to unblock the tasks. */
    			vPrintString(pcTextForTask_X_Exit_SignalMutex);

    			snprintf(last_vehicle.number, sizeof(last_vehicle.number)/sizeof(char), "%ld", counter);
    			counter++;

    			xQueueSend(xQueueVehicle, (void*)&last_vehicle, 0);

        		xSemaphoreGive( xBridgeMutex );

    			/* 'Give' the semaphore to unblock the task A. */
        		vPrintTwoStrings(pcTaskGetName(NULL), pcTextForTask_X_Exit_SignalContinue);
   	        	xSemaphoreGive( CONTINUE_SEMAPHORE );
        	}
        }
	}
}

/*------------------------------------------------------------------*-
  ---- END OF FILE -------------------------------------------------
-*------------------------------------------------------------------*/
