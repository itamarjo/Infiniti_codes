#include <stdlib.h>
#include <assert.h>

#include "scheduler.h"
#include "task.h"
#include "pq.h"

struct scheduler
{
	pqueue_t 	*pqueue;
	task_t		*current_task;
	int			flag_stop;
};


sch_t *SchCreate(void)
{
	sch_t *sch = (sch_t *)malloc(sizeof(sch_t));

	if(0 == sch)
	{
		return(0);
	}

	sch->pqueue = PQCreate(TaskIsBefore, 0);
	if(0 == sch->pqueue)
	{
		free(sch);
		return(0);
	}

	sch->current_task = 0;
	sch->flag_stop = 0;

	return(sch);
}

void SchDestroy(sch_t *scheduler)
{
	assert(scheduler != 0);
	assert(scheduler->current_task == 0);

	SchClear(scheduler);
	PQDestroy(scheduler->pqueue);
	/* to add tasks destory */
	free(scheduler);
}

/* return invalid_uid if fail
 	Task return:
		success and stop future task: -1,
		success and continue:  0,
		fail: 1 */
uuid_t SchCreateTask(	sch_t *scheduler,
						size_t period_in_secs,
						int (*func_task) (void *param),
						void *param)
{
	task_t *task = 0;

	assert(scheduler != 0);
	assert(func_task != 0);

	task = TaskCreate(period_in_secs, func_task, param);

	if (0 == task)
	{
		return(UIDGetInvalid());
	}

	if (1 == PQEnqueue(scheduler->pqueue, (void *)task))
	{
		TaskDestroy(task);
		return(UIDGetInvalid());
	}

	return(TaskGetID(task));
}

/*
*/
static int SchTaskIsSameId(const void *data, void *param)
{
	/* return (!TaskIsSameId(data, (const void *)param)); */
	return (0 == TaskIsSameId(data, param));
}

int SchRemoveTask(sch_t *scheduler, uuid_t uuid)
{
	task_t *result= 0;

	assert(scheduler != 0);

	result = (task_t *)PQRemove(scheduler->pqueue, TaskIsSameId, (void *)&uuid);

	if (0 == result)
	{
		return(1);
	}

	TaskDestroy(result);

	return(0);
}

size_t SchSize(const sch_t *scheduler)
{
	assert(scheduler != 0);

	return(PQSize(scheduler->pqueue) + (scheduler->current_task != 0));
}

int SchIsEmpty(const sch_t *scheduler)
{
	assert(scheduler != 0);

	return((PQIsEmpty(scheduler->pqueue)) && (scheduler->current_task == 0));
}

int SchRun(sch_t *scheduler)
{
	time_t now = 0;;
	int result = 0;

	/* run the loop as long as there are tasks in queue and the flags are not
		raised */
	while((scheduler->flag_stop == 0) && (SchIsEmpty(scheduler) == 0))
	{
		now = time(0);
		/* check if the time to run the task has arrive */
		if (TaskIsBefore((task_t *)(PQPeek(scheduler->pqueue)), 0,
			(void *)&now) == 1)
		{
			/* run the task and save the result */
			result = TaskRun((task_t *)PQPeek(scheduler->pqueue));
			/* save the pointer to the task that are currently running */
			scheduler->current_task = (task_t *)PQPeek(scheduler->pqueue);
			/* remove the task from the queue */
			PQDequeue(scheduler->pqueue);


			/* check if the task needs to be added back to the queue */
			if (0 == result)
			{
				/* update running time of the task */
				TaskUpdate(scheduler->current_task);
				/* put the task back in the queue */
				PQEnqueue(scheduler->pqueue, scheduler->current_task);
			}
			else
			{
				TaskDestroy(scheduler->current_task);
			}
			/*clear the current task field */
			scheduler->current_task = 0;
		}
		else /* go to sleep for 1 second */
		{
			sleep(1);
		}
	}

	return(0);
}

void SchStop(sch_t *scheduler)
{
	assert(scheduler != 0);

	/* raise the flag for stop */
	scheduler->flag_stop = 1;
}

void SchClear(sch_t *scheduler)
{
	assert(scheduler != 0);
	assert(scheduler->current_task == 0);

	/* clear the queue */
	while(SchIsEmpty == 0)
	{
		TaskDestroy(PQPeek(scheduler->pqueue)); /*destory task */
		PQDequeue(scheduler->pqueue); /*removing node iq queue */
	}
}
