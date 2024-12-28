/*
 * schedulerDataStructure.c
 *
 *  Created on: Nov 25, 2024
 *      Author: 84898
 */

#include "global.h"

struct sTask
{
	// Pointer to the task (must be a ’ void ( void ) ’ function )
	void ( * pTask ) ( void ) ;
	// Delay (ticks) until the function will ( next ) be run
	int64_t Delay;
	// Interval (ticks) between subsequent runs .
	uint32_t Period;
	// Incremented (by schedulerDataStructure) when task is due to execute
//	uint8_t RunMe;
	//ID used for delete task
	uint32_t TaskID ;

	struct sTask * next;
	struct sTask * previous;

};

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

struct SDS //schedulerDataStructure data structure
{
	struct sTask * head;
	struct sTask * tail;
	AmountTasks count;
};

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

struct BufferScheduler //be used to save time remain until subsequent task
{
	int64_t Delay; //be used to save time of task that has lowest Delay in SDS
	//and if this Delay = 0, every tasks in SDS will be updated and if it has any task with Delay = 0, they will be executed
	uint32_t Period; //be used to update Delay of nodes in SDS (=Period - Delay) if it has functions that just be added to
	AmountTasks pastCount; //be used to indicate that more function be added to SDS
};

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

struct SDS * schedulerDataStructure = NULL; //global variable for schedulerDataStructure data structure

struct BufferScheduler * BufferTimer = NULL; //global variable for save time remain until subsequent task

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

struct sTask * constructorsTaskNoParameters()
{
	struct sTask * temp = NULL;

//	while(temp == NULL) //ask to allocate memory until that is granted memory
//	{
		temp = (struct sTask *)malloc(sizeof(struct sTask));
//	}

	if(temp == NULL)
		return temp;

	temp -> pTask = NULL;
	temp -> Delay = 0;
	temp -> Period = 0;
//	temp -> RunMe = 0;
	temp -> TaskID = 0;
	temp -> next = NULL;
	temp -> previous = NULL;

	return temp;
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

struct sTask * constructorsTaskWithParameters(void (*pTask)(void), int64_t Delay,
											uint32_t Period/*, uint8_t RunMe*/, uint32_t TaskID,
											struct sTask * next, struct sTask * previous)
{
	struct sTask * temp = NULL;

//	while(temp == NULL) //ask to allocate memory until that is granted memory
//	{
		temp = (struct sTask *)malloc(sizeof(struct sTask));
//	}

	if(temp == NULL)
		return temp;

	temp -> pTask = pTask;
	temp -> Delay = Delay;
	temp -> Period = Period;
//	temp -> RunMe = RunMe;
	temp -> TaskID = TaskID;
	temp -> next = next;
	temp -> previous = previous;

	return temp;
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

struct SDS * constructorSDSNoParameters()
{
	struct SDS * temp = NULL;
//	while(temp == NULL) //ask to allocate memory until that is granted memory
//	{
		temp = (struct SDS *)malloc(sizeof(struct SDS));
//	}

	if(temp == NULL)
		return temp;

	temp -> head = NULL;
	temp -> tail = NULL;
	temp -> count = 0;

	return temp;
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void destructorSDS(struct SDS * needToBeDeleted)
{
	if(needToBeDeleted == NULL)
		return;

	struct sTask * temp_head = needToBeDeleted -> head;
	struct sTask * temp_tail = needToBeDeleted -> tail;

	do
	{
		if(temp_head != NULL)
		{
			struct sTask * temp = temp_head -> next;
			free(temp_head);
			temp_head = temp;
		}

		if(temp_tail != NULL)
		{
			struct sTask * temp = temp_tail -> previous;
			free(temp_tail);
			temp_tail = temp;
		}

	}
		while(temp_head != temp_tail);

	if(temp_head != NULL)
		free(temp_head);

	needToBeDeleted -> count = 0;
//	needToBeDeleted -> head = NULL; //don't need because at the end head = tail = null
//	needToBeDeleted -> tail = NULL;
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

struct BufferScheduler * ConstructorBufferSchedulerNoParameters(void)
{
	struct BufferScheduler * temp = (struct BufferScheduler *) malloc(sizeof(struct BufferScheduler));

	if(temp == NULL)
		return NULL;

	temp -> pastCount = 0;
	temp -> Delay = INT64_MAX;
	temp -> Period = (uint32_t) -1; //because unsigned integer so -1 will be max of it

	return temp;
}



//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void SCH_Init ( void )
{
	if(BufferTimer == NULL)
		BufferTimer = ConstructorBufferSchedulerNoParameters();

	else
	{
		BufferTimer -> pastCount = 0;
		BufferTimer -> Delay = INT64_MAX;
		BufferTimer -> Period = (uint32_t) -1;
	}

	if(schedulerDataStructure == NULL)
	{
		schedulerDataStructure = constructorSDSNoParameters();
		return;
	}

	destructorSDS(schedulerDataStructure);
//	taskImplement = NULL;
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void SCH_Update(void)
{
	if(BufferTimer == NULL)
		return;

	(BufferTimer -> Delay) --;
}
//update software timer

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void dealWithTasksAdded(int32_t differenceCount)
{
	struct sTask * temp_tail = schedulerDataStructure -> tail;

	if(temp_tail == NULL)
		return;

	for(AmountTasks i = 0 ; i < differenceCount ; i++)
	{
		if(BufferTimer -> Delay > temp_tail -> Delay)
			//if current delay in buffer greater than delay of a task in SDS so we need carry it to buffer
		{
			BufferTimer -> Delay = temp_tail -> Delay;
			BufferTimer -> Period = temp_tail -> Delay;
			//just use delay because we will minus by origin delay after BufferTimer -> Delay = 0 that indicate have delayed time elapse
		}
		temp_tail = temp_tail -> previous;
	}
}

void ProcessBufferChanged(int64_t saveDelay, uint32_t savePeriod)
{
	struct sTask * temp = schedulerDataStructure -> head;

	if(temp == NULL)
		return;

	for(AmountTasks i = 0 ; i < BufferTimer -> pastCount ; i++) //don't use count in the SDS because we just need to update older tasks
	{
		temp -> Delay -= ((int64_t)savePeriod) - saveDelay;
		temp = temp -> next;
	}

	BufferTimer -> pastCount = schedulerDataStructure -> count;
}

void deleteOnTheSpot(struct sTask * needToBeDeleted)
{
	if(needToBeDeleted -> previous != NULL)
		needToBeDeleted -> previous -> next = needToBeDeleted -> next;

	if(needToBeDeleted -> next != NULL)
		needToBeDeleted -> next -> previous = needToBeDeleted -> previous;

	schedulerDataStructure -> count --;

	free(needToBeDeleted);
}

void Run_Tasks()
{
	if(schedulerDataStructure == NULL || schedulerDataStructure -> count <= 0)
		return;

	if(schedulerDataStructure -> count == 1)
	{
		schedulerDataStructure -> head -> pTask();
		schedulerDataStructure -> head -> Delay = schedulerDataStructure -> head -> Period;
		return;
	}

	struct sTask * temp_head = schedulerDataStructure -> head;
	struct sTask * temp_tail = schedulerDataStructure -> tail;

//	while(temp_head != temp_tail /*for case with odd number count*/
//			&& temp_head -> previous != temp_tail && temp_tail -> next != temp_head
//			/*for case with even number of count*/)
	AmountTasks limit = (schedulerDataStructure -> count) / 2;
	AmountTasks isOdd = (schedulerDataStructure -> count) % 2;
	for(AmountTasks i = 0 ; i < limit ; i++)
	{
		if(temp_head != NULL)
		{
			temp_head -> Delay -= (int64_t) BufferTimer -> Period; //BufferTimer -> Delay is equaling zero so we don't need minus it

			if(temp_head -> Delay <= 0)
			{
				temp_head -> pTask();
				temp_head -> Delay = temp_head -> Period;
			}

			if(temp_head -> Period <= 0)
			{
				temp_head = temp_head -> next;
				deleteOnTheSpot(temp_head -> previous); //delete at index undo
			}

			else
				temp_head = temp_head -> next;
		}

		if(temp_tail != NULL)
		{
			temp_tail -> Delay -= (int64_t) BufferTimer -> Period; //BufferTimer -> Delay is equaling zero so we don't need minus it

			if(temp_tail -> Delay <= 0)
			{
				temp_tail -> pTask();
				temp_tail -> Delay = temp_tail -> Period;
			}

			if(temp_tail -> Period <= 0)
			{
				temp_tail = temp_tail -> previous;
				deleteOnTheSpot(temp_tail -> next);//delete at index undo
			}

			else
				temp_tail = temp_tail -> previous;
		}
	}

	if(isOdd && temp_head != NULL)
		//execute remain task due to if count is odd, the iterator above will stop when still remain a task don't be executed
		//it has presented at middle index already due to iterator above so don't need get temp_head -> next
	{
		temp_head -> Delay -= (int64_t) BufferTimer -> Period;

		if(temp_head -> Delay <= 0)
		{
			temp_head -> pTask();
			temp_head -> Delay = temp_head -> Period;
		}

		if(temp_head -> Period <= 0)
		{
			temp_head = temp_head -> next;
			deleteOnTheSpot(temp_head -> previous);
		}
		//Or
//		temp_tail -> previous -> pTask();
	}

	BufferTimer -> Period = schedulerDataStructure -> head -> Delay;
//	BufferTimer -> Delay = BufferTimer -> Period;
	//assign for first element in the SDS to compare remain elements to find largest delay

	return;
}

void refreshBuffer()
{
		if(schedulerDataStructure == NULL || schedulerDataStructure -> count <= 0)
			return;

		if(schedulerDataStructure -> count == 1)
		{
			BufferTimer -> Period = schedulerDataStructure -> head -> Period;
			BufferTimer -> Delay = BufferTimer -> Period;
			return;
		}

		struct sTask * temp_head = schedulerDataStructure -> head;
		struct sTask * temp_tail = schedulerDataStructure -> tail;

	//	while(temp_head != temp_tail /*for case with odd number count*/
	//			&& temp_head -> previous != temp_tail && temp_tail -> next != temp_head
	//			/*for case with even number of count*/)
		AmountTasks limit = (schedulerDataStructure -> count) / 2;
		AmountTasks isOdd = (schedulerDataStructure -> count) % 2;
		for(AmountTasks i = 0 ; i < limit ; i++)
		{
			if(temp_head != NULL)
			{
				if(temp_head -> Delay < BufferTimer -> Period)
					BufferTimer -> Period = temp_head -> Delay;

				temp_head = temp_head -> next;
			}

			if(temp_tail != NULL)
			{
				if(temp_tail -> Delay < BufferTimer -> Period)
					BufferTimer -> Period = temp_tail -> Delay;

				temp_tail = temp_tail -> previous;
			}
		}

		if(isOdd && temp_head != NULL)
			//execute remain task due to if count is odd, the iterator above will stop when still remain task don't be executed
			//it has presented at middle index already due to iterator above so don't need get temp_head -> next
		{
			if(temp_head -> Delay < BufferTimer -> Period)
				BufferTimer -> Period = temp_head -> Delay;
		}


		BufferTimer -> Delay = BufferTimer -> Period;
}

void SCH_Dispatch_Tasks(void)
{
	if(schedulerDataStructure == NULL)
		return;

	int64_t saveDelay = BufferTimer -> Delay;
	uint32_t savePeriod = BufferTimer -> Period;

	int32_t differenceCount = (int32_t) (schedulerDataStructure -> count - BufferTimer -> pastCount);

	if(differenceCount > 0) ///more tasks be added
	{
		dealWithTasksAdded(differenceCount);
	}

	if(BufferTimer -> Delay != saveDelay && differenceCount > 0)
		//Buffer delay be changed because have task with Delay lower so we need to update tasks in the SDS
		//but we also note that if timer interrupt occur so saveDelay also difference with delay current
		//so we need a certain condition that not change even if interrupt occurs, that is add more tasks to the SDS
	{
		ProcessBufferChanged(saveDelay, savePeriod);
	}

	if(differenceCount < 0) //update count because tasks be deleted
		BufferTimer -> pastCount = schedulerDataStructure -> count;

	if(BufferTimer -> Delay <= 0)
	{
		Run_Tasks();
		refreshBuffer();
	}
}
//run software timer

uint32_t SCH_Add_Task(void (* pFunctionAdd)(), int64_t DELAY, uint32_t PERIOD)
{
	if(schedulerDataStructure == NULL)
		return 0;

	if(schedulerDataStructure -> count == 0)
	{
		schedulerDataStructure -> head = constructorsTaskWithParameters(pFunctionAdd, DELAY/TICK, PERIOD/TICK, 0, NULL, NULL);
		schedulerDataStructure -> tail = schedulerDataStructure -> head;
		schedulerDataStructure -> count ++;
		return 0;
	}

	schedulerDataStructure -> tail -> next = constructorsTaskWithParameters(pFunctionAdd, DELAY/TICK, PERIOD/TICK,
								(schedulerDataStructure -> tail -> TaskID) + 1, NULL, schedulerDataStructure -> tail);

	schedulerDataStructure -> tail = schedulerDataStructure -> tail -> next;

	schedulerDataStructure -> count ++;

	return schedulerDataStructure -> tail -> TaskID;
}
//add software timer return TaskID

uint8_t SCH_Delete_Task(uint32_t taskID)
{
	if(schedulerDataStructure == NULL || schedulerDataStructure -> count <= 0)
		return 0;

	if(schedulerDataStructure -> count == 1)
	{
		free(schedulerDataStructure -> head);
		schedulerDataStructure -> head = NULL;
		schedulerDataStructure -> tail = NULL;
		schedulerDataStructure -> count --;
		return 1;
	}

	struct sTask * temp_head = schedulerDataStructure -> head;
	struct sTask * temp_tail = schedulerDataStructure -> tail;

//	while(temp_head != temp_tail /*for case with odd number count*/
//			&& temp_head -> previous != temp_tail && temp_tail -> next != temp_head
//			/*for case with even number of count*/)
	AmountTasks limit = (schedulerDataStructure -> count) / 2;
	AmountTasks isOdd = (schedulerDataStructure -> count) % 2;
	for(AmountTasks i = 0 ; i < limit ; i++)
	{
		if(temp_head != NULL)
		{
			if(temp_head -> TaskID == taskID)
			{
				deleteOnTheSpot(temp_head);
				return 1;
			}

			temp_head = temp_head -> next;
		}

		if(temp_tail != NULL)
		{
			if(temp_tail -> TaskID == taskID)
			{
				deleteOnTheSpot(temp_tail);
				return 1;
			}

			temp_tail = temp_tail -> previous;
		}
	}

	if(isOdd && temp_head != NULL)
		//execute remain task due to if count is odd iteration above will stop when still remain task don't be executed
		//it has presented at middle index already due to iterator above, so we don't need to get temp_head -> next
	{
		if(temp_head -> TaskID == taskID)
		{
			deleteOnTheSpot(temp_head);
			return 1;
		}

	}

	return 0;
}
//delete software timer based on address of function we want to delete return value 1 (successfully) or 0 (failed)
