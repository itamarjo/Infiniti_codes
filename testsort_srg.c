#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> /* clock_t clock() */

#include "sort.h"

int tst_ints[7] = {3, 5, -3, 0, 2, -7, 7};
int *p_ints[7];


static int CompareInts (const void *a, const void *b)
{
	return (*(const int *)a - *(const int *)b);
}


int main(int argc, char *argv[])
{
	stack_t *stk = 0;
	size_t i;

	for (i = 0; i < 7; ++i)
	{
		p_ints[i] = tst_ints + i;
	}

	stk = StackCreate(sizeof(void *), 32);
	puts("\n==================");



	for (i = 0; i < 7; ++i)
	{
		InsertStack(stk, (const void *)(tst_ints + i), &CompareInts);
	}

	for (i = 0; i < 7; ++i)
	{
		printf(" %d ", **(int **)StackPeek(stk));
		StackPop(stk);
	}

	puts("\n==================");

	for (i = 0; i < 7; ++i)
	{
		StackPush(stk, (const void *)(p_ints + i));
	}

	for (i = 0; i < 7; ++i)
	{
		printf(" %d ", **(int **)StackPeek(stk));
		StackPop(stk);
	}
	puts("\n==================");

	for (i = 0; i < 7; ++i)
	{
		StackPush(stk, (const void *)(p_ints + i));
	}

	SortStack(stk, &CompareInts);

	for (i = 0; i < 7; ++i)
	{
		printf(" %d ", **(int **)StackPeek(stk));
		StackPop(stk);
	}

	return (0);
}














