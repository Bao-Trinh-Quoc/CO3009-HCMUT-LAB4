/*
 * scheduler.c
 *
 *  Created on: Nov 14, 2024
 *      Author: DELL
 */

#include "scheduler.h"

SCH_Task tasks[SCH_TASKNUMBER];

void SCH_Init(void) {
	for (uint8_t i = 0; i < SCH_TASKNUMBER; i++) {
		tasks[i].functionPointer = 0;
		tasks[i].id = SCH_TASKNUMBER - i - 1;
		tasks[i].delay = 0;
		tasks[i].period = 0;
		tasks[i].flag = 0;
	}
}

void SCH_Update(void) {
	if (tasks[0].functionPointer == 0)
		return;

	if (tasks[0].delay > 0) {
		if (tasks[0].delay > SCH_TIMERTICK)
			tasks[0].delay -= SCH_TIMERTICK;
		else
			tasks[0].delay = 0;
	}

	if (tasks[0].delay == 0)
		tasks[0].flag = 1;
}

void SCH_Dispatch(void) {}
uint8_t SCH_AddTask(void (*functionPointer)(void), uint32_t delay, uint32_t period) {}
unsigned char SCH_DeleteTask(uint8_t id) {}
