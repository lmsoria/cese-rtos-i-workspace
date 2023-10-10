/*
 * task_Monitor.c
 *
 *  Created on: Oct 10, 2023
 *      Author: lsoria
 */

/* Demo includes. */


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
#include "task_Monitor.h"

// ------ Macros and definitions ---------------------------------------

// ------ internal data declaration ------------------------------------

// ------ internal functions declaration -------------------------------

// ------ internal data definition -------------------------------------

static const char *pcTextForTask_Monitor = " | Running\r\n\n";

static const char *pcTextForTask_Monitor_message_received = " | Message received!\r\n\n";

// ------ external data definition -------------------------------------

// ------ internal functions definition --------------------------------

static void print_message_data(VehicleEventMsg* msg)
{
	/* Print the string, using a critical section as a crude method of mutual
	 * exclusion. */
	taskENTER_CRITICAL();
	{
		printf( "---------- [Message Data] ----------\r\n");
		printf( "\t> timestamp: %ld\r\n", msg->timestamp);
		printf( "\t> type: %s\r\n", event_type_to_str(msg->type));
		printf( "\t> vehicle_type: %s\r\n", vehicle_to_str(msg->vehicle_type));
		printf( "\t> id: %s\r\n", (msg->type == ENTRY ? entry_to_str(msg->id.entry) : exit_to_str(msg->id.exit)));
		printf( "------------------------------------\r\n");
		fflush( stdout );
	}
	taskEXIT_CRITICAL();
}

// ------ external functions definition --------------------------------

/*------------------------------------------------------------------*/
/* Task Monitor thread */
void vTaskMonitor( void *pvParameters )
{
	MonitorTaskData* const DATA = (MonitorTaskData*)(pvParameters);

	const QueueHandle_t MESSAGE_QUEUE = *DATA->message_queue;

	vPrintTwoStrings(DATA->name, pcTextForTask_Monitor);

	VehicleEventMsg last_message;

	while(1)
	{
		if(xQueueReceive( MESSAGE_QUEUE, &last_message, portMAX_DELAY ) == pdPASS) {
			vPrintTwoStrings(DATA->name, pcTextForTask_Monitor_message_received);
			print_message_data(&last_message);
		}
	}
}

/*------------------------------------------------------------------*-
  ---- END OF FILE -------------------------------------------------
-*------------------------------------------------------------------*/
