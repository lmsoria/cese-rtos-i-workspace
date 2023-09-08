/*
 * task_HelloWorld.c
 *
 *  Created on: Sep 7, 2023
 *      Author: lsoria
 */

#include "cmsis_os.h"
#include "task_Function.h"
#include "supporting_Functions.h"

#define LED_PERIOD_MS 1000
#define LED_ON_TIME_INCREMENT_MS 100

// Naive approach: use xTaskGetTickCount() and toggle after the comparison is >= the period
void vTaskHelloWorldNaive( void *pvParameters )
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

// Use vTaskDelay() function. This approach will decrease dramatically the runtime usage
void vTaskHelloWorldDelay( void *pvParameters )
{
	for( ;; ){
		/* Delay for a period using vTaskDelay */
		led_toggle(LED1);
		vTaskDelay(pdMS_TO_TICKS(LED_PERIOD_MS));
	}
}


void vTaskEjercicioC2( void *pvParameters )
{
	uint32_t on_time = LED_ON_TIME_INCREMENT_MS;

	LEDStatus ledState = LED_OFF;

	TickType_t xLastWakeTime = xTaskGetTickCount();
	while(1) {

    	if( ledState == LED_OFF ) {
    		ledState = LED_ON;
    		printf("[%d] ON. Lets wait %d ms\r\n", (uint32_t)xLastWakeTime, on_time);
    		vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(on_time));

    	}
    	else {
    		ledState = LED_OFF;
    		printf("[%d] OFF. Lets wait %d ms\r\n", (uint32_t)xLastWakeTime, LED_PERIOD_MS - on_time);
    		vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(LED_PERIOD_MS - on_time));

    		// Compute new on_time
			if(on_time < LED_PERIOD_MS - LED_ON_TIME_INCREMENT_MS) {
				on_time += LED_ON_TIME_INCREMENT_MS;
			} else {
				on_time = LED_ON_TIME_INCREMENT_MS;
			}
    	}

    	led_write(LED1, ledState);
	}
}
