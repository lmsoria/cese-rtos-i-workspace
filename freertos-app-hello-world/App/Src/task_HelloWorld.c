/*
 * task_HelloWorld.c
 *
 *  Created on: Sep 7, 2023
 *      Author: lsoria
 */
#include <stdio.h>
#include "cmsis_os.h"
#include "task_Function.h"
#include "supporting_Functions.h"

#define LED_PERIOD_MS 1000
#define LED_ON_TIME_INCREMENT_MS 100

#define DEBOUNCE_PERIOD_MS 40

typedef enum
{
	KEY_STATE_WAIT_PRESS = 0,    ///< Key released, waiting to be pressed
	KEY_STATE_DEBOUNCE_ACTIVE,   ///< The key has been pressed and we need to filter the debouncing
	KEY_STATE_WAIT_RELEASE,      ///< Key pressed, waiting to be released
	KEY_STATE_DEBOUNCE_INACTIVE, ///< The key has been released, so we need to filter the debouncing
} KeyState;

typedef struct
{
	BoardButtons button;
	KeyState state;
    uint32_t timer_debounce;
    uint32_t timer_up;
} Key;

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
//    		printf("[%ld] ON. Lets wait %ld ms\r\n", (uint32_t)xLastWakeTime, on_time);
    		vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(on_time));

    	}
    	else {
    		ledState = LED_OFF;
//    		printf("[%ld] OFF. Lets wait %ld ms\r\n", (uint32_t)xLastWakeTime, LED_PERIOD_MS - on_time);
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


void vTaskEjercicioC3_button( void *pvParameters )
{
	TaskData* const DATA = (TaskData*)(pvParameters);

	Key key =
	{
		.button = DATA->button,
		.state = KEY_STATE_WAIT_PRESS,
		.timer_debounce = 0,
		.timer_up = 0,
	};

	while(1) {

		switch(key.state) {
		case KEY_STATE_WAIT_PRESS:
			if(button_read(key.button) == BUTTON_PRESSED) {
				key.timer_debounce = pdMS_TO_TICKS(DEBOUNCE_PERIOD_MS);
				key.state = KEY_STATE_DEBOUNCE_ACTIVE;
			}
			break;
		case KEY_STATE_DEBOUNCE_ACTIVE:
			if(button_read(key.button) == BUTTON_RELEASED) {
				key.state = KEY_STATE_WAIT_PRESS;
			} else if(key.timer_debounce) {
				key.timer_debounce--;
			} else {
				key.timer_up = 0;
				key.state = KEY_STATE_WAIT_RELEASE;
			}
			break;
		case KEY_STATE_WAIT_RELEASE:
			if(button_read(key.button) == BUTTON_RELEASED) {
				key.timer_debounce = pdMS_TO_TICKS(DEBOUNCE_PERIOD_MS);
				key.state = KEY_STATE_DEBOUNCE_INACTIVE;
			}
			break;
		case KEY_STATE_DEBOUNCE_INACTIVE:
			if(button_read(key.button) == BUTTON_PRESSED) {
				key.state = KEY_STATE_WAIT_RELEASE;
			} else if(key.timer_debounce) {
				key.timer_debounce--;
			} else {
				key.state = KEY_STATE_WAIT_PRESS;
			}
			break;
		default:
			key.state = KEY_STATE_WAIT_PRESS;
			break;
		}

		if(key.state == KEY_STATE_WAIT_RELEASE) {
			key.timer_up++;

			if((key.timer_up% 100) == 0) {
				led_toggle(DATA->led);
			}
		} else {
			key.timer_up = 0;
		}

		vTaskDelay(pdMS_TO_TICKS(1));
	}
}
