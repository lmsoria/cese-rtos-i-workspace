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

    task_Test.c (Released 2022-06)

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
#include "task_Test.h"

// ------ Macros and definitions ---------------------------------------

// ------ internal data declaration ------------------------------------
/* Events to excite tasks */
typedef enum eTask_Test{ Error, Entry, Exit } eTask_Test_t;

typedef struct
{
	EventType event_type;
	VehicleType vehicle_type;
	union
	{
		EntryType entry;
		ExitType exit;
	} id;
} TestStimulus;

// ------ internal functions declaration -------------------------------

// ------ internal data definition -------------------------------------
/* Define the strings that will be passed in as the Supporting Functions parameters.
 * These are defined const and off the stack to ensure they remain valid when the
 * tasks are executing. */
const char *pcTextForTask_Test						= "  <=> Task Test - Running\r\n\n";

const char *pcTextForTask_Test_TEST_X				= "[TEST] Corriendo Tarea - Variante: ";
const char *pcTextForTask_Test_priority			 	= "  <=> Tesk Test - priority:";
const char *pcTextForTask_Test_eTask_TestArrayIndex	= "  <=> Task Test - eTask_TestArray Index :";

const char *pcTextForTask_Test_SignalEntry 			= "[TEST] Entro un auto por";
const char *pcTextForTask_Test_SignalExit			= "[TEST] Salio un auto por";
const char *pcTextForTask_Test_SignalError			= "  <=> Task Test - Signal: Error  <=>\r\n";
const char *pcTextForTask_Test_Wait5000mS			= "  <=> Task Test - Wait:   5000mS <=>\r\n\n";



#define TEST_X ( 4 )

#if( TEST_X == 0 )
/* Array of events to excite tasks */
const eTask_Test_t eTask_TestArray[] = { Error, Exit+1, Exit+2 };
#endif

#if( TEST_X == 1 )
/* Array of events to excite tasks */
const eTask_Test_t eTask_TestArray[] = { Entry, Exit };
#endif

#if( TEST_X == 2 )
/* Array of events to excite tasks */
const eTask_Test_t eTask_TestArray[] = { Entry, Entry, Exit, Exit };
#endif

#if( TEST_X == 3 )
/* Array of events to excite tasks */
const eTask_Test_t eTask_TestArray[] = { Entry, Entry, Entry, Entry, Exit, Exit, Exit, Exit };
const TestStimulus eTask_TestStimulusArray[] =
{
		{.type = Entry, .id.entry = ENTRADA_A},
		{.type = Entry, .id.entry = ENTRADA_B},
		{.type = Entry, .id.entry = ENTRADA_A},
		{.type = Entry, .id.entry = ENTRADA_B},
		{.type = Exit, .id.exit = SALIDA_A},
		{.type = Exit, .id.exit = SALIDA_A},
		{.type = Exit, .id.exit = SALIDA_A},
		{.type = Exit, .id.exit = SALIDA_A},
};
#endif

#if( TEST_X == 4 )
/* Array of events to excite tasks */
const eTask_Test_t eTask_TestArray[] = { Entry, Entry, Entry, Entry, Entry, Exit, Exit, Exit, Exit, Exit };

const TestStimulus eTask_TestStimulusArray[] =
{
		{.event_type = EVENT_ENTRY, .vehicle_type = CAR, .id.entry = ENTRADA_A},
		{.event_type = EVENT_ENTRY, .vehicle_type = CAR, .id.entry = ENTRADA_B},
		{.event_type = EVENT_ENTRY, .vehicle_type = CAR, .id.entry = ENTRADA_A},
		{.event_type = EVENT_ENTRY, .vehicle_type = CAR, .id.entry = ENTRADA_B},
		{.event_type = EVENT_ENTRY, .vehicle_type = CAR, .id.entry = ENTRADA_A},
		{.event_type = EVENT_EXIT, .vehicle_type = CAR, .id.exit = SALIDA_B},
		{.event_type = EVENT_EXIT, .vehicle_type = CAR, .id.exit = SALIDA_B},
		{.event_type = EVENT_EXIT, .vehicle_type = CAR, .id.exit = SALIDA_A},
		{.event_type = EVENT_EXIT, .vehicle_type = CAR, .id.exit = SALIDA_A},
		{.event_type = EVENT_EXIT, .vehicle_type = CAR, .id.exit = SALIDA_A},
};


#endif

#if( TEST_X == 5 )
/* Array of events to excite tasks */
const eTask_Test_t eTask_TestArray[] = { Entry, Entry, Entry, Entry, Entry, Entry, Exit, Exit, Exit, Exit, Exit, Exit };
#endif

// ------ external data definition -------------------------------------

// ------ internal functions definition --------------------------------

// ------ external functions definition --------------------------------

/*------------------------------------------------------------------*/
/* Task Test thread */
void vTask_Test( void *pvParameters )
{
	uint32_t i = TEST_X;
	portTickType xLastWakeTime;
	UBaseType_t uxPriority;

	VehicleEventMsg msg;

	/* Print out the name, parameters and TEST_X of this task. */
	vPrintStringAndNumber( pcTextForTask_Test_TEST_X, i);

	/* The xLastWakeTime variable needs to be initialized with the current tick
	 * count.  Note that this is the only time we access this variable. From this
	 * point on xLastWakeTime is managed automatically by the vTaskDelayUntil()
	 * API function. */
	xLastWakeTime = xTaskGetTickCount();

	/* This task will run the first time after other tasks as it has the lower
	 * priority.
	 *
	 * Query the priority at which this task is running - passing in NULL means
	 * "return our own priority". */
	uxPriority = uxTaskPriorityGet( vTask_TestHandle ) + 2UL;

	/* Setting the TestingTask priority above the other tasks priority will
	 * cause TestingTask to immediately start running (as then TestingTask
	 * will have the higher priority of the three created tasks). */
	vTaskPrioritySet( vTask_TestHandle, uxPriority );

	while( 1 )
	{
		/* Scanning the array of events to excite tasks */
		for ( i = 0; i < (sizeof(eTask_TestStimulusArray)/sizeof(TestStimulus)); i++ )
		{
			const EntryType ENTRY = eTask_TestStimulusArray[i].id.entry;
			const ExitType EXIT = eTask_TestStimulusArray[i].id.exit;

			msg.timestamp = xTaskGetTickCount();
			msg.type = eTask_TestStimulusArray[i].event_type;
			msg.vehicle_type = eTask_TestStimulusArray[i].vehicle_type;

			switch( eTask_TestStimulusArray[i].event_type ) {

	    		case EVENT_ENTRY:
				    /* 'Give' the semaphore to unblock the task A. */
	    			vPrintTwoStrings( pcTextForTask_Test_SignalEntry, entry_to_str(ENTRY));
					xSemaphoreGive( EntrySemaphores[ENTRY] );
	    			msg.id.entry = ENTRY;
	    			xQueueSend(VehicleQueue, (void*)&msg, 0);
	    			break;

	    		case EVENT_EXIT:
				    /* 'Give' the semaphore to unblock the task B. */
		    		vPrintTwoStrings( pcTextForTask_Test_SignalExit, exit_to_str(EXIT) );
		    		xSemaphoreGive( ExitSemaphores[EXIT] );
	    			msg.id.entry = EXIT;
	    			xQueueSend(VehicleQueue, (void*)&msg, 0);
	    			break;

	    		case EVENT_ERROR:
		    	default:
		    		vPrintString( pcTextForTask_Test_SignalError );
		    		break;
		    }
		    /* About a 5000 mS delay here */
			/* We want this task to execute exactly every 5000 milliseconds.  As per
			 * the vTaskDelay() function, time is measured in ticks, and the
			 * portTICK_RATE_MS constant is used to convert this to milliseconds.
			 * xLastWakeTime is automatically updated within vTaskDelayUntil() so does not
			 * have to be updated by this task code. */
    		vTaskDelayUntil( &xLastWakeTime, (5000 / portTICK_RATE_MS) );
		}
	}
}

/*------------------------------------------------------------------*-
  ---- END OF FILE -------------------------------------------------
-*------------------------------------------------------------------*/
