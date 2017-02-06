#include <stdlib.h>
#include <assert.h>
#include <time.h> 	/*time_t*/

#include "task.h"

struct task
{
	uuid_t		uuid;
	size_t		period_in_secs;
	time_t		next_run_time;
	int (*func_task) (void *param); /* task to do */
	void 		*param;
};
/* Func sorted list
int (*is_before) (const void *data_a, const void *data_b, void *params)
int (*is_match) (const void *data, void *params)
*/
task_t *TaskCreate(size_t period_in_sec,
	int (*func_task) (void *param), void *param)
{
	task_t *task = 0;

	assert(func_task != 0);

	task = (task_t *)malloc(sizeof(task_t));
	if (0 == task)
	{
		return(0);
	}

	task->uuid = UIDCreate();
	if (UIDIsSameId(task->uuid, UIDGetInvalid()))
	{
		TaskDestroy(task);
		return(0);
	}
	task->period_in_secs = period_in_sec;
	task->func_task = func_task;
	task->param = param;
	TaskUpdate(task);

	return(task);
}

void TaskDestroy(task_t *task)
{
	assert(task != 0);

	free(task);
}

/* retun: success and stop future task: -1, success and continue:  0, fail: 1*/
int TaskRun(task_t *task)
{
	assert(task != 0);

	return(task->func_task(task->param));
}

/* if params is null then
	returns 1 if runtime of 'task_a' is before 'task_b'
otherwise
	returns 1 if runtime of 'task_a' is before 'params'
*/
int TaskIsBefore(const void *task_a, const void *task_b, void *params)
{
	assert(task_a != 0);

	if(0 == params)
	{
		assert(task_b != 0);
		return(((task_t *)task_a)->next_run_time
				< ((task_t *)task_b)->next_run_time);
	}
	else
	{
		return(((task_t *)task_a)->next_run_time <= *(time_t *)params);
	}
}

/*	Compare between uid's task
 	return 1, if is same uid */
int TaskIsSameId(const void *task, void *uid)
{
	assert(task != 0);
	assert(uid != 0);

	return(UIDIsSameId(TaskGetID((task_t *)task), *(uuid_t *)uid));
}

uuid_t TaskGetID(const task_t *task)
{
	assert(task != 0);

	return(task->uuid);
}

void TaskUpdate(task_t *task) /* update next_run_time */
{
	assert(task != 0);

	task->next_run_time = time(0) + (time_t)task->period_in_secs;
}
