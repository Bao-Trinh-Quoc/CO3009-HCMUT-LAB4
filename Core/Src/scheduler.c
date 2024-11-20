/*
 * scheduler.c
 *
 *  Created on: Nov 14, 2024
 *      Author: DELL
 */

#include "scheduler.h"

SCH_Task tasks[SCH_TASKNUMBER];

void SCH_Init(void) {
	/*
	 * arrIndex 0 1 2 3 4 5 6 7
	 * id		7 6 5 4 3 2 1 0
	 * */
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
// Queue mechanism
uint8_t SCH_AddTask(void (*functionPointer)(void), uint32_t delay, uint32_t period) {
	// If there is no space left to add a new task
	if (tasks[SCH_TASKNUMBER - 1].functionPointer != 0)
		return SCH_TASKNUMBER;
	uint8_t currentID = tasks[SCH_TASKNUMBER - 1].id;
	uint32_t currentDelay = 0;
	for (uint8_t i = 0; i < SCH_TASKNUMBER; i++) {
		currentDelay += tasks[i].delay;
		if ((currentDelay > delay) || (tasks[i].functionPointer == 0)) {
			// Create an empty space at ith index by shifting to the right
			for (uint8_t j = SCH_TASKNUMBER - 1; j > i; j--) {
				tasks[j] = tasks[j - 1];
			}
			tasks[i].functionPointer = functionPointer;
			tasks[i].id = currentID;
			tasks[i].period = period;
			tasks[i].flag = 0;
			if (currentDelay > delay) {
				int newDelay = currentDelay - delay;
				tasks[i].delay = tasks[i + 1].delay - newDelay;
				if (tasks[i].delay == 0) {
					tasks[i].flag = 1;
				}
				tasks[i + 1].delay = newDelay;
				if (tasks[i + 1].delay == 0) {
					tasks[i + 1].flag = 1;
				}
			}
			else {
				tasks[i].delay = delay - currentDelay;
				if (tasks[i].delay == 0) {
					tasks[i].flag = 1;
				}
			}
			return tasks[i].id;
		}
	}
	return SCH_TASKNUMBER;
}
unsigned char SCH_DeleteTask(uint8_t id) {}
