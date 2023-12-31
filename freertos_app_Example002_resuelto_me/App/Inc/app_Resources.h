/* Copyright 2022, Juan Manuel Cruz.
 * All rights reserved.
 *
 * This file is part of Project => freertos_app_Example002
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

    app_Resources.h (Released 2022-06)

  --------------------------------------------------------------------

    This is the Tasks Header file.

-*--------------------------------------------------------------------*/


#ifndef __APP_RESOURCES_H
#define __APP_RESOURCES_H

#ifdef __cplusplus
 extern "C" {
#endif

// ------ inclusions ---------------------------------------------------

// ------ macros -------------------------------------------------------

// ------ typedef ------------------------------------------------------

 typedef enum {
	 ENTRADA_A  = 0,
	 ENTRADA_B,
	 ENTRADA_C,
	 TOTAL_ENTRADAS // Keep this value always at the bottom!
 } EntryType;

 typedef enum {
	 SALIDA_A  = 0,
	 SALIDA_B,
	 SALIDA_C,
	 TOTAL_SALIDAS // Keep this value always at the bottom!
 } ExitType;

 typedef struct {
	 char name[30];
	 xSemaphoreHandle* entry_semaphore;
	 xSemaphoreHandle* continue_semaphore;
 } EntryTaskData;

 typedef struct {
	 char name[30];
	 xSemaphoreHandle* exit_semaphore;
	 xSemaphoreHandle* continue_semaphore;
 } ExitTaskData;

typedef enum {
	EVENT_ENTRY,
	EVENT_EXIT,
	EVENT_ERROR,
	TOTAL_EVENTS // Keep this value always at the bottom!
} EventType;

typedef enum {
	CAR = 0,
	VAN,
	TRUCK,
	MOTORCYCLE,
	TOTAL_VEHICLES // Keep this value always at the bottom!
} VehicleType;

typedef struct {
	uint32_t timestamp;
	EventType type;
	VehicleType vehicle_type;
	union
	{
		EntryType entry;
		ExitType exit;
	} id;
 } VehicleEventMsg;

typedef struct {
	char name[30];
	QueueHandle_t* message_queue;
} MonitorTaskData;

// ------ external data declaration ------------------------------------
/* Declare a variable of type xSemaphoreHandle.  This is used to reference the
 * semaphore that is used to synchronize a task with other task. */
extern xSemaphoreHandle xBinarySemaphoreContinue;

extern xSemaphoreHandle EntrySemaphores[TOTAL_ENTRADAS];
extern xSemaphoreHandle ExitSemaphores[TOTAL_SALIDAS];

/* Declare a variable of type xSemaphoreHandle.  This is used to reference the
 * mutex type semaphore that is used to ensure mutual exclusive access to...*/
extern xSemaphoreHandle xMutex;

/* Used to hold the handle of Tasks. */
extern xTaskHandle EntryTasks[TOTAL_ENTRADAS];
extern xTaskHandle ExitTasks[TOTAL_SALIDAS];
extern xTaskHandle TaskMonitor_Handle;
extern xTaskHandle vTask_TestHandle;

/* Task A & B Counter	*/
#define lTasksCntMAX	3
extern uint32_t	lugares_ocupados;

extern QueueHandle_t VehicleQueue;

// ------ external functions declaration -------------------------------


char* event_type_to_str(EventType event_type);

char* entry_to_str(EntryType entry);

char* exit_to_str(ExitType exit);

char* vehicle_to_str(VehicleType vehicle_type);

#ifdef __cplusplus
}
#endif

#endif /* __APP_RESOURCES_H */

/*------------------------------------------------------------------*-
  ---- END OF FILE -------------------------------------------------
-*------------------------------------------------------------------*/
