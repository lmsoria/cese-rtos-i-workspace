/*
 * API_button.h
 *
 *  Created on: Sep 1, 2023
 *      Author: lsoria
 */

#pragma once

/// @brief Possible Button status
typedef enum
{
    BUTTON_RELEASED = 0,
    BUTTON_PRESSED = 1,
} ButtonStatus;

/// @brief Enum that keeps track of the available LEDs
typedef enum
{
    USER_BUTTON = 0, ///< User Button (the blue one)
	BUTTON_S1, ///< Breakout board button S1 (connected to PG.14)
	BUTTON_S2, ///< Breakout board button S2 (connected to PG.9)
	BUTTON_S3, ///< Breakout board button S3 (connected to PE.8)
	BUTTON_S4, ///< Breakout board button S4 (connected to PE.7)
    BUTTONS_TOTAL,   ///< Total amount of buttons. Keep this value always at the bottom!
} BoardButtons;

/// @brief Read the status of the specified button
/// @param button Must be one of the defined in BoardButtons
/// @return BUTTON_PRESSED if the button is pressed, and BUTTON_RELEASED otherwise.
ButtonStatus button_read(const BoardButtons button);
