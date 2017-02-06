#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sorted_list.h"
#include "dlist.h"

int IsBefore (const void *data_a, const void *data_b, void *params);

int SLPrint(const void *data, void *param);

void PrintSList(sl_t *list);

int Add10 (const void *data, void *param);

int main(int argc, char *argv[])
{
		void *params = 0;
		int array[] = {1, 6, 3, 0, -1, 5};
		int array_b[] = {15, 346, 553, 20, -331, 45};
		int i = 0;

		sl_iter_t location;
		sl_t *sort_list = SLCreate(IsBefore, params);
		sl_t *sort_list_b = SLCreate(IsBefore, params);
		for (i = 0; i < 6; ++i)
		{
			SLInsert(sort_list, (void *)&array[i]);
			SLInsert(sort_list_b, (void *)&array_b[i]);

		}
		printf("\nTesting CREATE and INSERT");
		PrintSList(sort_list);
		printf("expected:\n-1,0,1,3,5,6,\n");
		PrintSList(sort_list_b);
		printf("expected:\n-331,15,20,45,346,553,\n\n");
		printf("Testing END, PREV, REMOVE POPHEAD, POPTAIL, BEGIN");

		location = SLEnd(sort_list);
		location = SLPrev(location);
		location = SLPrev(location);
		SLRemove(location);
		SLPopHead(sort_list);
		SLPopTail(sort_list);
		location = SLBegin(sort_list);
		location = SLNext(location);
		SLRemove(location);
		PrintSList(sort_list);
		printf("expected:\n0,3, \n\nTesting MERGE");

		SLMerge(sort_list, sort_list_b);
		PrintSList(sort_list);
		printf("expected:\n-331,0,3,15,20,45,346,553\n\nTesting FIND");

		printf(" trying to find %d and remove it", array_b[0]);
		location = SLFind(sort_list, &array_b[0]);

		SLRemove(location);
		PrintSList(sort_list);
		printf("expected:\n-331,0,3,20,45,346,553  \n\n Testing FOREACH");

		SLForEach(SLBegin(sort_list), SLEnd(sort_list), Add10, 0);
		PrintSList(sort_list);
		printf("expected:\n-321,10,13,30,55,346,563  \n\n Testing FOREACH");



		SLDestroy(sort_list);


	return(0);
}


int IsBefore (const void *data_a, const void *data_b, void *params)
{
	if (*(int *)data_a < *(int *)data_b)
	{
		return (1);
	}
	return (0);
}

int SLPrint(const void *data, void *param)
{
	printf("%d,",*(int *)data);
	return (0);
}

/* print list */
void PrintSList(sl_t *list)
{
	printf("\nSLIST:\n");
	SLForEach(SLBegin(list), SLEnd(list), SLPrint, 0);
	printf("\n");
}

int Add10(const void *data, void *param)
{
	*(int *) data +=10;
	return(0);
}
