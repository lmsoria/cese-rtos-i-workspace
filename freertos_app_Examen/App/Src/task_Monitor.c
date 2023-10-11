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



// ------ external functions definition --------------------------------

/*------------------------------------------------------------------*/
/* Task Monitor thread */
void vTask_Monitor( void *pvParameters )
{
	while(1)
	{
	}
}

/*------------------------------------------------------------------*-
  ---- END OF FILE -------------------------------------------------
-*------------------------------------------------------------------*/
