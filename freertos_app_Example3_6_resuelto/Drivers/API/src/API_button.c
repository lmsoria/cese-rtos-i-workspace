/*
 * API_button.c
 *
 *  Created on: Sep 1, 2023
 *      Author: lsoria
 */

#include <assert.h>
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_gpio.h"

#include "API_button.h"

/// @brief Struct that represents a button
typedef struct
{
    GPIO_TypeDef* port;
    uint16_t pin;
    uint8_t pullup;
} ButtonStruct;

static const ButtonStruct AVAILABLE_BUTTONS[BUTTONS_TOTAL] =
{
    [USER_BUTTON] = {GPIOC, GPIO_PIN_13, GPIO_NOPULL},
	[BUTTON_S1] = {GPIOG, GPIO_PIN_14, GPIO_PULLUP},
	[BUTTON_S2] = {GPIOG, GPIO_PIN_9, GPIO_PULLUP},
	[BUTTON_S3] = {GPIOE, GPIO_PIN_8, GPIO_PULLUP},
	[BUTTON_S4] = {GPIOE, GPIO_PIN_7, GPIO_PULLUP},
};

ButtonStatus button_read(const BoardButtons button)
{
    assert(button < BUTTONS_TOTAL);
    const GPIO_PinState BUTTON_STATE = HAL_GPIO_ReadPin(AVAILABLE_BUTTONS[button].port, AVAILABLE_BUTTONS[button].pin);

    // If we have a pull-up resistor, when the button is pressed the ReadPin function should return a low state.
    // If we have a pull-down resistor (and no pull?), the opposite happens (button pressed -> GPIO_PIN_SET read)
    GPIO_PinState condition_check = (AVAILABLE_BUTTONS[button].pullup == GPIO_PULLUP) ? GPIO_PIN_RESET : GPIO_PIN_SET;

    return (BUTTON_STATE == condition_check) ? BUTTON_PRESSED : BUTTON_RELEASED;
}
