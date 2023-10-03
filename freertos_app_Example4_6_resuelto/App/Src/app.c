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

    app.c (Released 2022-10)

--------------------------------------------------------------------

    app file for FreeRTOS - Event Driven System (EDS) - Project for
    STM32F429ZI_NUCLEO_144.

    See readme.txt for project information.

-*--------------------------------------------------------------------*/


// ------ Includes -----------------------------------------------------
/* Project includes. */
#include "cmsis_os.h"

/* Standard includes. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <task_led.h>

/* Demo includes. */
#include "supporting_Functions.h"

/* Application & Tasks includes. */
#include "app.h"
#include "app_Resources.h"
#include "task_button.h"

// ------ Macros and definitions ---------------------------------------

// ------ internal data declaration ------------------------------------
/* Declare a variable of type xTaskHandle. This is used to reference tasks. */
TaskHandle_t xTaskLed1Handle;
TaskHandle_t xTaskButton1Handle;

TaskHandle_t xTaskLed2Handle;
TaskHandle_t xTaskButton2Handle;

TaskHandle_t xTaskLed3Handle;
TaskHandle_t xTaskButton3Handle;

QueueHandle_t QueueHandle;

// Data passed to future tasks. TODO: List available tasks inside an enum so we don't hardcode the indices
static TaskData TASK_DATA_ARRAY[3] =
{
    [0] = {
    		.led = LED_D1,
            .button = BUTTON_S1,
			.blinking_period_ms = 250,
          },
    [1] = {
    		.led = LED_D2,
            .button = BUTTON_S2,
			.blinking_period_ms = 500,
           },
    [2] = {
    		.led = LED_D3,
            .button = BUTTON_S3,
			.blinking_period_ms = 1000,
           },
};

// ------ internal functions declaration -------------------------------

// ------ internal data definition -------------------------------------
const char *pcTextForMain = "freertos_app_Example3_6 is running: PO (3 de 6) (Resuelto)\r\n\n";

// ------ external data definition -------------------------------------

// ------ internal functions definition --------------------------------

// ------ external functions definition --------------------------------


/*------------------------------------------------------------------*/
/* App Initialization */
void appInit( void )
{
    uint32_t index;
    BaseType_t ret;

    /* Print out the name of this Example. */
    vPrintString( pcTextForMain );

    /* Before a queue is used it must be explicitly created.
     * The queue is created to hold a maximum of 5 long values. */
	QueueHandle = xQueueCreate( 5, sizeof( TaskMessage ) );
	/* Check the queues was created successfully */
	configASSERT( QueueHandle != NULL );
    /* We want this queue to be viewable in a RTOS kernel aware debugger, so register it. */
    vQueueAddToRegistry( QueueHandle, "QueueHandle" );


    index = 0;
    /* Task 1 thread at priority 1 */
    ret = xTaskCreate( vTaskLed,                    /* Pointer to the function thats implement the task. */
                       "TaskLed 1",                         /* Text name for the task. This is to facilitate debugging only. */
                       (2 * configMINIMAL_STACK_SIZE),   /* Stack depth in words. */
                       (void*)(&TASK_DATA_ARRAY[index]), /* Pass the index as the task parameter. */
                       (tskIDLE_PRIORITY + 1UL),         /* This task will run at priority 1. */
                       &xTaskLed1Handle );                  /* We are using a variable as task handle. */

    /* Check the task was created successfully. */
    configASSERT( ret == pdPASS );

    /* Task 1 thread at priority 1 */
    ret = xTaskCreate( vTaskButton,                    /* Pointer to the function thats implement the task. */
                       "TaskButton 1",                         /* Text name for the task. This is to facilitate debugging only. */
                       (2 * configMINIMAL_STACK_SIZE),   /* Stack depth in words. */
                       (void*)(&TASK_DATA_ARRAY[index]), /* Pass the index as the task parameter. */
                       (tskIDLE_PRIORITY + 1UL),         /* This task will run at priority 1. */
                       &xTaskButton1Handle );                  /* We are using a variable as task handle. */

    /* Check the task was created successfully. */
    configASSERT( ret == pdPASS );


//    index = 1;
//    /* Task 2 thread at priority 1 */
//    ret = xTaskCreate( vTaskLed,                    /* Pointer to the function thats implement the task. */
//                       "TaskLed 2",                         /* Text name for the task. This is to facilitate debugging only. */
//                       (2 * configMINIMAL_STACK_SIZE),   /* Stack depth in words. */
//                       (void*)(&TASK_DATA_ARRAY[index]), /* Pass the index as the task parameter. */
//                       (tskIDLE_PRIORITY + 1UL),         /* This task will run at priority 1. */
//                       &xTaskLed2Handle );                  /* We are using a variable as task handle. */
//
//    /* Check the task was created successfully. */
//    configASSERT( ret == pdPASS );
//
//    ret = xTaskCreate( vTaskButton,                    /* Pointer to the function thats implement the task. */
//                       "TaskButton 2",                         /* Text name for the task. This is to facilitate debugging only. */
//                       (2 * configMINIMAL_STACK_SIZE),   /* Stack depth in words. */
//                       (void*)(&TASK_DATA_ARRAY[index]), /* Pass the index as the task parameter. */
//                       (tskIDLE_PRIORITY + 1UL),         /* This task will run at priority 1. */
//                       &xTaskButton2Handle );                  /* We are using a variable as task handle. */
//
//    /* Check the task was created successfully. */
//    configASSERT( ret == pdPASS );
//
//
//    index = 2;
//    /* Task 3 thread at priority 1 */
//    ret = xTaskCreate( vTaskLed,                    /* Pointer to the function thats implement the task. */
//                       "TaskLed 3",                         /* Text name for the task. This is to facilitate debugging only. */
//                       (2 * configMINIMAL_STACK_SIZE),   /* Stack depth in words. */
//                       (void*)(&TASK_DATA_ARRAY[index]), /* Pass the index as the task parameter. */
//                       (tskIDLE_PRIORITY + 1UL),         /* This task will run at priority 1. */
//                       &xTaskLed3Handle );                  /* We are using a variable as task handle. */
//
//    /* Check the task was created successfully. */
//    configASSERT( ret == pdPASS );
//
//    ret = xTaskCreate( vTaskButton,                    /* Pointer to the function thats implement the task. */
//                       "TaskButton 3",                         /* Text name for the task. This is to facilitate debugging only. */
//                       (2 * configMINIMAL_STACK_SIZE),   /* Stack depth in words. */
//                       (void*)(&TASK_DATA_ARRAY[index]), /* Pass the index as the task parameter. */
//                       (tskIDLE_PRIORITY + 1UL),         /* This task will run at priority 1. */
//                       &xTaskButton3Handle );                  /* We are using a variable as task handle. */
//
//    /* Check the task was created successfully. */
//    configASSERT( ret == pdPASS );
}

/*------------------------------------------------------------------*-
  ---- END OF FILE -------------------------------------------------
-*------------------------------------------------------------------*/
