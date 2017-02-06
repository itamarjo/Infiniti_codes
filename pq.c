/*
 * pq_heap.c
 *
 *  Created on: Sep 12, 2016
 *      Author: pop
 */


/* return NULL on fail */
#include <assert.h>
#include "pq.h"
#include "heap.h"

typedef struct before
{
	void *param;
	int (*is_before) (const void *data1, const void *data2, void *param);
} before_t;

struct pqueue
{
	heap_t *heap;
	before_t before;
};


static int WrapBefore(const void *data1, const void *data2, void *param)
{
	before_t *wrap = (before_t *)param;

	return(wrap->is_before(*(const void **)data1, *(const void **)data2, wrap->param));
}
pqueue_t *PQCreate(int (*is_before) (const void *data1, const void *data2, void *param),
					void *param)
{
	pqueue_t *new = 0;

	assert(is_before != 0);

	new = (pqueue_t *)malloc(sizeof(pqueue_t));

	if (0 == new)
	{
		return (0);
	}

	new->before.is_before = is_before;
	new->before.param = param;

	new->heap = HeapCreate(WrapBefore, sizeof(void *), (void *)&new->before);
	if (0 == new->heap)
	{
		free(new);
		return(0);
	}

	return(new);
}

void PQDestroy(pqueue_t *queue)
{
	assert(queue != 0);

	HeapDestroy(queue->heap);
	free(queue);
}

/* return 0 on succses or 1 if fail */
int PQEnqueue(pqueue_t *queue, void *data)
{
	assert(queue != 0 );

	return(HeapPush(queue->heap, &data));
}

/* return 0 on succses or 1 if fail (i.e. queue is empty ) */
int PQDequeue(pqueue_t *queue)
{
	assert(queue != 0 );

	if (HeapIsEmpty(queue->heap))
	{
		return (1);
	}

	HeapPop(queue->heap);

	return(0);
}

typedef struct wrap
{
	void *param;
	int (*is_match) (const void *data, void *param);
} wrap_t;

static int is_match_wrap(const void *data, const void *param)
{
	wrap_t *wrap = (wrap_t *)param;
	return(wrap->is_match(*(const void **)data, wrap->param));
}

/* Remove the data according to different priority criteria
return the data from the removed item or NULL if not found */
void *PQRemove(pqueue_t *queue,
				int (*is_match) (const void *data, void *param),
				void *param)
{
	void *ret_data = 0;


	assert(queue != 0 );
	assert(is_match != 0);

	wrap_t wrap;
	wrap.is_match = is_match;
	wrap.param = param;

	if (1 == HeapRemove(queue->heap, is_match_wrap, &wrap, &ret_data))
	{
		return(0);
	}



	return(ret_data);
}

/* make the queue empty */
void PQClear(pqueue_t *queue)
{
	assert(queue != 0);

	while(HeapIsEmpty(queue->heap) != 1)
	{
		HeapPop(queue->heap);
	}
}

size_t PQSize(const pqueue_t *queue)
{
	return(HeapSize(queue->heap));
}

/* return 1 if empty or 0 if full */
int PQIsEmpty(const pqueue_t *queue)
{
	assert(queue);

	return(HeapIsEmpty(queue->heap));
}

void *PQPeek(const pqueue_t *queue)
{
	assert(queue->heap);

	return(*(void **)HeapPeek(queue->heap));
}
