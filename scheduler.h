#ifndef __SCHEDULER_H__
#define __SCHEDULER_H__

#include <stddef.h>	/* size_t */

#include "uuid.h"

typedef struct scheduler sch_t;

sch_t *SchCreate(void);

void SchDestroy(sch_t *scheduler);

/* return invalid_uid if fail
 	Task return:
		success and stop future task: -1,
		success and continue:  0,
		fail: 1 */
uuid_t SchCreateTask(	sch_t *scheduler,
						size_t period_in_secs,
						int (*func_task) (void *param),
						void *param);

int SchRemoveTask(sch_t *scheduler, uuid_t uuid);

size_t SchSize(const sch_t *scheduler);

int SchIsEmpty(const sch_t *scheduler);

int SchRun(sch_t *scheduler);

void SchStop(sch_t *scheduler);

void SchClear(sch_t *scheduler);

#endif /*__SCHEDULER_H__*/
