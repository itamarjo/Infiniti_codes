#ifndef __TASK_H__
#define __TASK_H__

#include <stddef.h>	/* size_t */

#include "uuid.h"

typedef struct task task_t;

task_t *TaskCreate(size_t period_in_sec,
	int (*func_task) (void *param), void *param);

void TaskDestroy(task_t *task);

/* retun: success and stop future task: -1, success and continue:  0, fail: 1*/
int TaskRun(task_t *task);

/* if task_a < task_b return 1 */
int TaskIsBefore(const void *task_a, const void *task_b, void *params);

/*	Compare between uid's task
 	return 1, if is same uid */
int TaskIsSameId(const void *task, void *uid);

uuid_t TaskGetID(const task_t *task);

void TaskUpdate(task_t *task); /* update next_run_time */

#endif /*__TASK_H__*/
