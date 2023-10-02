/*
 * API_leds.h
 *
 *  Created on: Aug 31, 2023
 *      Author: lsoria
 */

#pragma once

/// @brief Enum that keeps track of the available LEDs
typedef enum
{
    LED1 = 0,  ///< Green LED
    LED2,      ///< Blue LED
    LED3,      ///< Red LED
	LED_D1,    ///< Breakout led D1 (connected to PE.14)
	LED_D2,    ///< Breakout led D2 (connected to PE.15)
	LED_D3,    ///< Breakout led D3 (connected to PB.10)
	LED_D4,    ///< Breakout led D4 (connected to PB.11)
    LEDS_TOTAL /// Total amount of LEDs. Keep this value always at the bottom!
} BoardLEDs;

/// @brief Posible LED status
typedef enum
{
    LED_OFF = 0,
    LED_ON = 1,
} LEDStatus;

/// @brief Toggle the status of the specified led
/// @param led Must be one of the defined in BoardLEDs.
void led_toggle(const BoardLEDs led);

/// @brief Set the specified LED with the desired status
/// @param led Must be one of the defined in BoardLEDs.
/// @param status Can be LED_ON or LED_OFF
void led_write(const BoardLEDs led, const LEDStatus status);

/// @brief Turn on the specified LED. It's equivalent to call `led_set(led, LED_ON)`
/// @param led Must be one of the defined in BoardLEDs.
void led_set(const BoardLEDs led);

/// @brief Turn off the specified LED. It's equivalent to call `led_set(led, LED_OFF)`
/// @param led Must be one of the defined in BoardLEDs.
void led_clear(const BoardLEDs led);
