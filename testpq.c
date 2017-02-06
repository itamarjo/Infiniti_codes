#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "pq.h"

#define SIZEOF_ARR(x) (sizeof(x)/sizeof(x[0]))

/* integer array comparison function */
static int IsStrictSmall(const void* a, const void *b, void *patam);
static int IsMatch2(const void* a,  void *b);

int main (int argc, char *argv[])
{
	pqueue_t *q;
	int *rmv_res = 0;
	/* void *reference = 0, *data = 0;
	size_t i; */
	const int items[][3] =
	{
		{-5, 1, -10},
		{-2, -1, -11},
		{ 0, 3, -12}
	};
	/* const int item_to_find[] = {-5, 0};
	const int item_not_to_find[] = {-1, 0}; */

	if (0 == (q = PQCreate(&IsStrictSmall, 0)))
	{
		return (0);
	}

	printf(" IsEmpty 1? %d\n", PQIsEmpty(q));
	printf(" Size 0? %lu\n", PQSize(q));


	printf(" Enqueue 0? %d\n", PQEnqueue(q, (void *) items[0]));
	printf(" Peek -5? %d\n", *(int *) PQPeek(q));

	printf(" Enqueue 0? %d\n", PQEnqueue(q, (void *) items[0]));
	printf(" Peek -5? %d\n", *(int *) PQPeek(q));

	printf(" Enqueue 0? %d\n", PQEnqueue(q, (void *) items[0]));
	printf(" Peek -5? %d\n", *(int *) PQPeek(q));


	printf(" IsEmpty 0? %d\n", PQIsEmpty(q));
	printf(" Size 3? %lu\n", PQSize(q));


	PQClear(q);

	printf(" Size 0? %lu\n", PQSize(q));

	PQDestroy(q);

	/**********************************************************************/

	if (0 == (q = PQCreate(&IsStrictSmall, (void *) 1)))
	{
		return (0);
	}

	printf("\n----------\n");

	printf(" Enqueue 0? %d\n", PQEnqueue(q, (void *) items[0]));
	printf(" Peek 1? %d\n", ((int *) PQPeek(q))[1]);
	printf(" Enqueue 0? %d\n", PQEnqueue(q, (void *) items[1]));
	printf(" Peek -1? %d\n", ((int *) PQPeek(q))[1]);
	printf(" Enqueue 0? %d\n", PQEnqueue(q, (void *) items[2]));
	printf(" Peek -1? %d\n", ((int *) PQPeek(q))[1]);
	printf(" Dequeue 0? %d\n", PQDequeue(q));
	printf(" Peek 1? %d\n", ((int *) PQPeek(q))[1]);

	printf(" Size 2? %lu\n", PQSize(q));

	PQClear(q);

	printf(" Size 0? %lu\n", PQSize(q));
	printf(" IsEmpty 1? %d\n", PQIsEmpty(q));
	printf(" Dequeue 1? %d\n", PQDequeue(q));

	PQDestroy(q);

	/**********************************************************************/

	if (0 == (q = PQCreate(&IsStrictSmall, (void *) 2)))
	{
		return (0);
	}

	printf("\n----------\n");

	printf(" Enqueue 0? %d\n", PQEnqueue(q, (void *) items[0]));
	printf(" Peek -10? %d\n", ((int *) PQPeek(q))[2]);
	printf(" Enqueue 0? %d\n", PQEnqueue(q, (void *) items[1]));
	printf(" Peek -11? %d\n", ((int *) PQPeek(q))[2]);
	printf(" Enqueue 0? %d\n", PQEnqueue(q, (void *) items[2]));
	printf(" Peek -12? %d\n", ((int *) PQPeek(q))[2]);
	printf(" Size 3? %lu\n", PQSize(q));

	rmv_res = (int *)PQRemove(q, &IsMatch2, (void *) items[1]);
	if (rmv_res)
	{
		printf(" Removed: %d, expected %d \n", rmv_res[2], items[1][2]);
	}
	else
	{
		printf(" Removal of %d failed \n", *(int *) items[1]);
	}

	printf(" Size 2? %lu\n", PQSize(q));
	printf(" IsEmpty 0? %d\n", PQIsEmpty(q));

	PQDestroy(q);

	return(0);
}


/* integer array comparison function */
static int IsStrictSmall(const void* a, const void *b, void *patam)
{
	size_t i = (size_t)patam;

	return (((int *)a)[i] < ((int *)b)[i]);
}

/* integer array comparison function */
static int IsMatch2(const void* a,  void *b)
{
	return (((int *)a)[2] == ((int *)b)[2]);
}



