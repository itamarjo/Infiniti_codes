#include <stdio.h>
#include <stdlib.h>


#include "sorted_list.h"
#include "dlist.h"
#include "pq.h"

int IsBefore (const void *data_a, const void *data_b, void *params);

int PQPrint(const void *data, void *param);

void PQPrintList(pqueue_t *list);

int Add10 (const void *data, void *param);

int main(int argc, char *argv[])
{
		void *params = 0;
		int array[] = {1, 6, 3, 0, -1, 5};
		int array_b[] = {15, 346, 553, 20, -331, 45};
		int i = 0;

		pqueue_t *sort_list = PQCreate(IsBefore, params);
		pqueue_t *sort_list_b = PQCreate(IsBefore, params);
		for (i = 0; i < 6; ++i)
		{
			PQEnqueue(sort_list, (void *)&array[i]);
			PQEnqueue(sort_list_b, (void *)&array_b[i]);

		}
		printf("\nTesting CREATE and INSERT");
		PQPrintList(sort_list);
		printf("expected:\n-1,0,1,3,5,6,\n");
		PQPrintList(sort_list_b);
		printf("expected:\n-331,15,20,45,346,553,\n\n");
		PQDequeue(sort_list_b);
		PQDequeue(sort_list_b);
		PQClear(sort_list);

		printf("\nlist1:%lu list2: %lu\n", PQSize(sort_list), PQSize(sort_list_b));
		printf("expected:\nlist1: 0 list2: 4\n\n");

		PQDestroy(sort_list);
		PQDestroy(sort_list_b);

	return	(0);
}


int IsBefore (const void *data_a, const void *data_b, void *params)
{
	if (*(int *)data_a < *(int *)data_b)
	{
		return (1);
	}
	return (0);
}



int Add10(const void *data, void *param)
{
	*(int *) data +=10;
	return(0);
}
