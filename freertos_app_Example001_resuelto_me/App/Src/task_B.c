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

/* Demo includes. */
#include "supporting_Functions.h"

/* Application includes. */
#include "app_Resources.h"
#include "task_B.h"

// ------ Macros and definitions ---------------------------------------

// ------ internal data declaration ------------------------------------
uint32_t lTask_BCnt = 0;

// ------ internal functions declaration -------------------------------

// ------ internal data definition -------------------------------------
/* Define the strings that will be passed in as the Supporting Functions parameters.
 * These are defined const and off the stack to ensure they remain valid when the
 * tasks are executing. */
const char *pcTextForTask_B    			= "  ==> Task    B - Running\r\n";

const char *pcTextForTask_B_WaitEntry_B	= "\t[Task B] Espero que entre un auto...\r\n\n";
const char *pcTextForTask_B_WaitExit_B	= "\t[Task B] Espero que salga el auto...\r\n\n";

const char *pcTextForTask_B_WaitMutex	= "\t[Task B] Espero que se libere el puente...\r\n\n";
const char *pcTextForTask_B_SignalMutex	= "\t[Task B] puente liberado!\r\n\n\r\n\n";

const char *pcTextForTask_B_Wait1500mS	= "  ==> Task    B - Wait:   1500mS - cnt: ";

// ------ external data definition -------------------------------------

// ------ internal functions definition --------------------------------

// ------ external functions definition --------------------------------

/*------------------------------------------------------------------*/
/* Task B thread */
void vTask_B( void *pvParameters )
{
	/* Print out the name of this task. */
	vPrintString( pcTextForTask_B );

	/* As per most tasks, this task is implemented within an infinite loop.
	 *
	 * Take the semaphore once to start with so the semaphore is empty before the
	 * infinite loop is entered.  The semaphore was created before the scheduler
	 * was started so before this task ran for the first time.*/
	xSemaphoreTake( xBinarySemaphoreEntry_B, (portTickType) 0 );
    xSemaphoreTake( xBinarySemaphoreExit_B, (portTickType) 0 );

    while( 1 )
    {
    	vPrintString( pcTextForTask_B_WaitEntry_B );
    	xSemaphoreTake( xBinarySemaphoreEntry_B, portMAX_DELAY);

    	// Seccion Critica, tengo que tomar un mutex aqui
    	vPrintString( pcTextForTask_B_WaitMutex );
    	xSemaphoreTake(xMutex, portMAX_DELAY);

    	vPrintString("\t[Task B] SECCION CRITICA - Auto yendo B -> A\r\n\n");

    	vPrintString( pcTextForTask_B_WaitExit_B );
    	xSemaphoreTake( xBinarySemaphoreExit_B, portMAX_DELAY);

    	// Fin de la Seccion Critica, Liberar Mutex
    	vPrintString( pcTextForTask_B_SignalMutex );
    	xSemaphoreGive(xMutex);

    	lTask_BCnt++;
	}
}

/*------------------------------------------------------------------*-
  ---- END OF FILE -------------------------------------------------
-*------------------------------------------------------------------*/
