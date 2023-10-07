/* Copyright 2022, Juan Manuel Cruz.
 * All rights reserved.
 *
 * This file is part of Project => freertos_app_Example000
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

    dwt.h (Released 2022-06)

  --------------------------------------------------------------------

    This is the Task Header file.

-*--------------------------------------------------------------------*/


#ifndef __APP_H
#define __APP_H

#ifdef __cplusplus
 extern "C" {
#endif

// ------ inclusions ---------------------------------------------------

// ------ macros -------------------------------------------------------

// ------ typedef ------------------------------------------------------

// ------ external data declaration ------------------------------------

// ------ external functions declaration -------------------------------

/* init cycle counter */
/* DWT (Data Watchpoint and Trace) registers, only exists on ARM Cortex with a DWT unit */
/*!< DEMCR: Debug Exception and Monitor Control Register */
/*!< TRCENA: Enable trace and debug block DEMCR (Debug Exception and Monitor Control Register) */
/*!< DWT Cycle Counter register */
/*!< CYCCNTENA bit in DWT_CONTROL register */
 #define InitCycleCounter() ({\
	CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;	/* enable DWT hardware */\
	DWT->CYCCNT = 0;								/* reset cycle counter */\
	DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;			/* start counting */\
 	})

/* reset cycle counter */
/*!< DWT Cycle Counter register */
#define ResetCycleCounter() (DWT->CYCCNT = 0)

/* start counting */
/*!< CYCCNTENA bit in DWT_CONTROL register */
#define EnableCycleCounter() (DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk)

/* disable counting if not used any more */
/*!< CYCCNTENA bit in DWT_CONTROL register */
#define DisableCycleCounter() (~DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk)

/* read cycle counter */
/*!< DWT Cycle Counter register */
#define GetCycleCounter() (DWT->CYCCNT)
#define CyclesPer_uS (SystemCoreClock / 1000000)
#define GetCycleCounterTime_uS() (DWT->CYCCNT / CyclesPer_uS)

 /*  uint32_t cycleCounter = 0;
  *  uint32_t cycleCounterTime_us = 0;
  *															// PC8 (GPIO)
  *  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_RESET);	// => ______
  *  InitCycleCounter();
  *
  *  ...
  *  ...													// => ______
  *																	 ___
  *  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_SET); 	// => __/
  *  // or => HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_8);
  *  ResetCycleCounter();
  *																  ______
  *  ...														// =>
  *
  *  cycleCounter = GetCycleCounter();
  *  GetCycleCounterTime_uS = GetCycleCounterTime_uS();			  __
  *  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_RESET);	// =>   \___
  *  // or => HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_8);
  *
  *  														// => ______
  *
  *  DEBUGOUT("Cycles: %lu - Time %lu uS\r\n", cycleCounter, cycleCounterTime_us);
  */

#ifdef __cplusplus
}
#endif

#endif /* __APP_H */

/*------------------------------------------------------------------*-
  ---- END OF FILE -------------------------------------------------
-*------------------------------------------------------------------*/
