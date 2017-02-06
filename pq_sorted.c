#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

#include "pq.h"
#include "sorted_list.h"

struct pqueue
{
	sl_t *slist;
};

/* return NULL on fail */
pqueue_t *PQCreate(int (*is_before) (const void *data1, const void *data2, void *param),
					void *param)
{
	pqueue_t *new;

	assert(is_before != 0);

	new = (pqueue_t *)malloc(sizeof(pqueue_t));

	if (0 == new)
	{
		return (0);
	}
	new->slist = SLCreate(is_before, param);
	if (0 == new)
	{
		free(new);
		return(0);
	}

	return(new);
}

void PQDestroy(pqueue_t *queue)
{
	assert(queue != 0);
	assert(queue->slist != 0);

	SLDestroy(queue->slist);
	free(queue);
}

/* return 0 on succses or 1 if fail */
int PQEnqueue(pqueue_t *queue, void *data)
{
	sl_iter_t iter;

	assert(queue != 0 );
	assert(queue->slist != 0);

	iter = SLInsert(queue->slist, data);
	if (SLIsSameIter(iter,SLEnd(queue->slist)) != 0)
	{
		return(1);
	}

	return(0);
}

/* return 0 on succses or 1 if fail (i.e. queue is empty ) */
int PQDequeue(pqueue_t *queue)
{
	assert(queue != 0 );
	assert(queue->slist != 0);

	if (SLIsEmpty(queue->slist))
	{
		return (1);
	}
	SLPopHead(queue->slist);

	return(0);
}

/* Remove the data according to different priority criteria
return the data from the removed item or NULL if not found */
void *PQRemove(pqueue_t *queue,
				int (*is_match) (const void *data, void *param),
				void *param)
{
	sl_iter_t ret;
	void *ret_data = 0;

	assert(queue != 0 );
	assert(is_match != 0);
	assert(queue->slist != 0);

	ret = SLFindIf(queue->slist, is_match, param);

	if (SLIsSameIter(ret, SLEnd(queue->slist)) == 0)
	{
		ret_data = SLGetData(ret);
		SLRemove(ret);
		return(ret_data);
	}

	return(0);
}

/* make the queue empty */
void PQClear(pqueue_t *queue)
{
	assert(queue != 0 );
	assert(queue->slist != 0);

	while (SLIsEmpty(queue->slist) == 0)
	{
		SLPopHead(queue->slist);
	}
}

size_t PQSize(const pqueue_t *queue)
{
	assert(queue != 0 );
	assert(queue->slist != 0);

	return(SLSize(queue->slist));
}

static int SLPrint(const void *data, void *param)
{
	printf("%d,",*(int *)data);
	return (0);
}

void PQPrintList(pqueue_t *list)
{
	assert(list != 0);

	printf("\nSLIST:\n");
	SLForEach(SLBegin(list->slist), SLEnd(list->slist), SLPrint, 0);
	printf("\n");
}

/* return 1 if empty or 0 if full */
int PQIsEmpty(const pqueue_t *queue)
{
	assert(queue != 0 );
	assert(queue->slist != 0);

	return(SLIsEmpty(queue->slist));
}

void *PQPeek(const pqueue_t *queue)
{
	if (PQIsEmpty(queue))
	{
		return(0);
	}

	return(SLGetData(SLBegin(queue->slist)));
}
