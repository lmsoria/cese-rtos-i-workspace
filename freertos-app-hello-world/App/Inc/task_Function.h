/* Copyright 2022, Juan Manuel Cruz.
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

    task_Function.h (Released 2022-10)

  --------------------------------------------------------------------

    This is the Tasks Header file.

-*--------------------------------------------------------------------*/


#ifndef __TASK_FUNCTION_H
#define __TASK_FUNCTION_H

#ifdef __cplusplus
 extern "C" {
#endif

// ------ inclusions ---------------------------------------------------
#include "API_button.h"
#include "API_leds.h"
// ------ macros -------------------------------------------------------

// ------ typedef ------------------------------------------------------

 /// @brief Struct containing information relevant for the tasks. For the moment
 /// it only holds the LED assigned to each task, but we can add more fields in the future.
 typedef struct
 {
    BoardLEDs led;       ///< LED assigned to the task.
    BoardButtons button; ///< Button assigned to the task.
 } TaskData;

// ------ external data declaration ------------------------------------

// ------ external functions declaration -------------------------------

void vTaskFunction( void *pvParameters );
void vTaskHelloWorldNaive( void *pvParameters );
void vTaskHelloWorldDelay( void *pvParameters );

// Ejercicio C2: Generar una señal cuadrada de 1s de periodo, pero que el tiempo de encendido
// de crezca de 100 a 900ms en pasos de 100 ms (de forma ciclica). Apoyarse en vTaskDelayUntil
void vTaskEjercicioC2( void *pvParameters );

// Ejercicio C3: Para comprobar el estado de funcionamiento de un pulsador, queremos reproducir
// el tiempo de pulsacion en un led.
// Implementando un mecanismo anti-rebote por software, desarrolle un programa que mida el tiempo
// de pulsacion y que luego lo reproduzca en el led azul
void vTaskEjercicioC3_button( void *pvParameters );

#ifdef __cplusplus
}
#endif

#endif /* __TASK_FUNCTION_H */

/*------------------------------------------------------------------*-
  ---- END OF FILE -------------------------------------------------
-*------------------------------------------------------------------*/
