/*
 * API_leds.c
 *
 *  Created on: Aug 31, 2023
 *      Author: lsoria
 */

#include <assert.h>
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_gpio.h"

#include "API_leds.h"

/// @brief Polarity of the LED.
typedef enum
{
	ACTIVE_LOW,  ///< A logical 0 will turn on the LED
	ACTIVE_HIGH, ///< A logical 1 will turn on the LED
} Polarity;

/// @brief Struct that represents a HAL LED
typedef struct
{
    GPIO_TypeDef* port;
    uint16_t pin;
    Polarity polarity;
} LEDStruct;

static const LEDStruct AVAILABLE_LEDS[LEDS_TOTAL] =
{
    [LED1] = {GPIOB, GPIO_PIN_0, ACTIVE_HIGH},
    [LED2] = {GPIOB, GPIO_PIN_7, ACTIVE_HIGH},
    [LED3] = {GPIOB, GPIO_PIN_14, ACTIVE_HIGH},
	[LED_D1] = {GPIOE, GPIO_PIN_14, ACTIVE_LOW},
	[LED_D2] = {GPIOE, GPIO_PIN_15, ACTIVE_LOW},
	[LED_D3] = {GPIOB, GPIO_PIN_10, ACTIVE_LOW},
	[LED_D4] = {GPIOB, GPIO_PIN_11, ACTIVE_LOW},
};

void led_toggle(const BoardLEDs led)
{
    assert(led < LEDS_TOTAL);
    HAL_GPIO_TogglePin(AVAILABLE_LEDS[led].port, AVAILABLE_LEDS[led].pin);
}

void led_write(const BoardLEDs led, const LEDStatus status)
{
    assert(led < LEDS_TOTAL);
    assert(status == LED_ON || status == LED_OFF);

    GPIO_PinState state;
    switch(status)
    {
    case LED_ON:
    	state = AVAILABLE_LEDS[led].polarity == ACTIVE_HIGH ? GPIO_PIN_SET : GPIO_PIN_RESET;
    	break;
    case LED_OFF:
    	state = AVAILABLE_LEDS[led].polarity == ACTIVE_HIGH ? GPIO_PIN_RESET : GPIO_PIN_SET;
    	break;
    }

    HAL_GPIO_WritePin(AVAILABLE_LEDS[led].port, AVAILABLE_LEDS[led].pin, state);
}

void led_set(const BoardLEDs led) { led_write(led, LED_ON); }

void led_clear(const BoardLEDs led) { led_write(led, LED_OFF); }
