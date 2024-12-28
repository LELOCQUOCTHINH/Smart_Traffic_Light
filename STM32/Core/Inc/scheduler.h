/*
 * Scheduler.h
 *
 *  Created on: Nov 25, 2024
 *      Author: 84898
 */

#ifndef INC_SCHEDULER_H_
#define INC_SCHEDULER_H_

//#include "global.h"
// MUST BE ADJUSTED FOR EACH NEW PROJECT
#define SCH_MAX_TASKS 40

#if SCH_MAX_TASKS > 255
	typedef uint32_t AmountTasks;
#else
	typedef uint8_t AmountTasks;
#endif

void SCH_Init ( void );

void SCH_Update(void);
//update software timer

void SCH_Dispatch_Tasks(void);
//run software timer

uint32_t SCH_Add_Task(void (* pFunctionAdd)(), int64_t DELAY, uint32_t PERIOD);
//add software timer return taskID a task which just added

uint8_t SCH_Delete_Task(uint32_t taskID);
//delete software timer based on address of function we want to delete return value 1 (successfully) or 0 (failed)


#endif /* INC_SCHEDULER_H_ */
