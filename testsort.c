/*
 * testsort.c
 *
 *  Created on: Aug 17, 2016
 *      Author: radio
 */
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "sort.h"
#include "stack.h"

int intcmp(const void *a, const void *b)
{
	return (*(int *)a - *(int *)b);
}

size_t key_int(const void *data, const void *args)
{
	return(*(int *)data);
}


size_t intcmprd(const void *data, const void *args)
{
	size_t mask = 0x0F << (*(int *)args * 4);
	return( (*(int *)data & mask) >> (*(int *)args * 4));
}

int comp(const void *a, const void *b)
{
	return(*(char *)a - *(char*)b);
}

int main()
{
	int ar_a[5000] = {0};
	int ar_b[5000] = {0};
	int ar_c[5000] = {0};
	int ar_d[5000] = {0};
	int ar_e[5000] = {0};
	int ar_f[5000]={0};
	int ar_g[5000]={0};
	int ar_h[5000]={0};
	int ar_i[5000]={0};
	int ar_original[5000]={0};
	char *st[] = {"abcd", "ddd", "bbb", "rrr", "cwew"};

	stack_t *stack = 0;

	int i = 0;
	int ok = 1;
	clock_t start_t = 0;
	clock_t end_t = 0;

	for (i = 0; i < 5000; ++i)
	{
		ar_a[i] = rand()  % 100;
		ar_b[i] = ar_a[i];
		ar_c[i] = ar_a[i];
		ar_d[i] = ar_a[i];
		ar_e[i] = ar_a[i];
		ar_f[i] = ar_a[i];
		ar_g[i] = ar_a[i];
		ar_h[i] = ar_a[i];
		ar_i[i] = ar_a[i];
		ar_original[i] = ar_a[i];
	}

	printf("****Insert sort****\n");
	start_t = clock();
	insert_sort(ar_a, 10, sizeof(int), intcmp);
	end_t = clock();
	printf("running time is: %g \n", (double) (end_t - start_t) / CLOCKS_PER_SEC);


	printf("****Select sort****\n");
	start_t = clock();
	select_sort(ar_b, 10, sizeof(int), intcmp);
	end_t = clock();
	printf("running time is: %g \n", (double) (end_t - start_t) / CLOCKS_PER_SEC);


	printf("****Bubble sort****\n");
	start_t = clock();
	bubble_sort(ar_c, 10, sizeof(int), intcmp);
	end_t = clock();
	printf("running time is: %g \n", (double) (end_t - start_t) / CLOCKS_PER_SEC);

	printf("****counting sort****\n");
	start_t = clock();
	counting_sort(ar_e, sizeof(int), 10, key_int, 0, 100 );
	end_t = clock();
	printf("running time is: %g \n", (double) (end_t - start_t) / CLOCKS_PER_SEC);


	printf("****quick sort****\n");
	start_t = clock();
	qsort(ar_d, 10, sizeof(int), intcmp);
	end_t = clock();
	printf("running time is: %g \n", (double) (end_t - start_t) / CLOCKS_PER_SEC);

	printf("****Radix sort****\n");
	start_t = clock();
	RadixSort(ar_f, 10, sizeof(int), intcmprd, 0, sizeof(int));
	end_t = clock();
	printf("running time is: %g \n", (double) (end_t - start_t) / CLOCKS_PER_SEC);

	printf("****Merge sort****\n");
	start_t = clock();
	MergeSort(ar_g, 10, sizeof(int), intcmp);

	end_t = clock();
	printf("running time is: %g \n", (double) (end_t - start_t) / CLOCKS_PER_SEC);

	printf("****Quick sort****\n");
	start_t = clock();
	QuickSort(ar_h, 10, sizeof(int), intcmp);

	end_t = clock();
	printf("running time is: %g \n", (double) (end_t - start_t) / CLOCKS_PER_SEC);

	printf("****Heap sort****\n");
	start_t = clock();
	QuickSort(ar_i, 10, sizeof(int), intcmp);

	end_t = clock();
	printf("running time is: %g \n", (double) (end_t - start_t) / CLOCKS_PER_SEC);


	for( i = 0; i < 5000; ++i)
		{
			/*printf("original : %d expected: %d, result: %d \n", ar_original[i],  ar_d[i], ar_h[i]); */
			if (ar_i[i] != ar_d[i])
			{
				ok = 0;
			}
		}
	if (ok == 0)
	{
		printf("bad bad/n");
	}

	printf("=================sort stack===================");
	stack = StackCreate(sizeof(char *), 10);
	for (i =0; i < 5; ++i)
	{
		StackPush(stack, (void *)&st[i]);
	}

	SortStack(stack, comp);

	for (i =0; i < 5; ++i)
	{
		printf("\n%s, ", *(char **)StackPeek(stack));
		StackPop(stack);
	}

	StackDestroy(stack);

	return(0);
}


