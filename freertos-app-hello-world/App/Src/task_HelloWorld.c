/*
 * task_HelloWorld.c
 *
 *  Created on: Sep 7, 2023
 *      Author: lsoria
 */

#include "cmsis_os.h"
#include "task_Function.h"

#define LED_PERIOD_MS 500

void vTaskHelloWorld( void *pvParameters )
{
	TickType_t ledTickCnt = xTaskGetTickCount();
	for( ;; ){
		/* Delay for a period using Tick Count. */
		if( ( xTaskGetTickCount() - ledTickCnt ) >= LED_PERIOD_MS )	{
			/* Update HW Led State */
			led_toggle(LED1);
			/* Update and Led Tick Counter */
			ledTickCnt = xTaskGetTickCount();
		}
	}
}
