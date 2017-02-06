#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "heap.h"

static int IsBeforeInt(const void *data_a, const void*data_b, void *param)
{
	return(*(int *)data_a > *(int *)data_b);
}

static int PrintInt(void *data, void *params)
{
	printf("%d ", *(int *) data);
	return (0);
}
static int IsMatchInt(const void *a, const void *b)
{
	return(*(int *)a == *(int *)b);
}


int main(int argc, char *argv[])
{
	int arr[] = {10, 45, 12, 60, 5, 90, 46, 75, 50};
	size_t size_arr = 9;
	size_t i = 0;
	size_t size_heap = 0;
	heap_t *heap = 0;
	int result = 0;

	printf("1) ");
	puts("/*****HeapCreate*****/");
	heap = HeapCreate(IsBeforeInt, sizeof(int), 0);
	if (0 == heap)
	{
		printf ("HeapCreate failed\n");
	}
	else
	{
		puts(" OK");
	}
	puts("/*****HeapIsEmpty*****/");
	if (!HeapIsEmpty(heap))
	{
		printf ("HeapIsEmpty failed after create\n");
	}
	else
	{
		puts(" OK");
	}

	size_heap = HeapSize(heap);
	printf("/*****HeapSize:*****/\nsize (0?) = %lu\n", size_heap);
	puts("/*****HeapDestroy*****/");
	HeapDestroy(heap);
	puts(" OK");
	/****************/
	puts("");
	printf("2) ");
	puts("/*****HeapCreate*****/");
	heap = HeapCreate(IsBeforeInt, sizeof(int), 0);
	if (0 == heap)
	{
		printf ("HeapCreate failed\n");
	}
	else
	{
		puts(" OK");
	}
	for(; i < size_arr - 2; ++i)
	{
		HeapPush(heap, &arr[i]);
	}
	printf("excepted:\n(90 45 60 10 5 12 46)\n ");
	HeapForEach	(heap, &PrintInt, 0);
	size_heap = HeapSize(heap);
	printf("\n/****HeapSize:****/\nsize (%lu?) = %lu\n", size_arr - 2, size_heap);
	if (1 == HeapIsEmpty(heap))
	{
		printf ("HeapIsEmpty failed after create\n");
	}
	else
	{
		puts(" OK");
	}
	puts("/*****HeapPop*****/");
	if (1 == HeapPop(heap))
	{
		printf ("HeapPop failed \n");
	}
	else
	{
		puts(" OK");
	}
	size_heap = HeapSize(heap);
	printf("/****HeapSize:****/\nsize (%lu?) = %lu\n", size_arr - 1 - 2, size_heap);
	printf("excepted:\n(60 45 46 10 5 12)\n ");
	HeapForEach	(heap, &PrintInt, 0);
	for(; i < size_arr; ++i)
	{
		HeapPush(heap, &arr[i]);
	}
	printf("\nexcepted:\n(75 50 60 45 5 12 46 10)\n ");
	HeapForEach	(heap, &PrintInt, 0);
	size_heap = HeapSize(heap);
	printf("\n/****HeapSize:****/\nsize (%lu?) = %lu\n", size_arr - 1, size_heap);
	puts("/*****HeapPop*****/");
	if (1 == HeapPop(heap))
	{
		printf ("HeapPop failed \n");
	}
	else
	{
		puts(" OK");
	}
	if (1 == HeapPop(heap))
	{
		printf ("HeapPop failed \n");
	}
	else
	{
		puts(" OK");
	}
	size_heap = HeapSize(heap);
	printf("/****HeapSize:****/\nsize (6?) = %lu\n", size_heap);
	printf("\nexcepted:\n(50 45 46 10 5 12)\n ");
	HeapForEach	(heap, &PrintInt, 0);
	puts("\n/*****HeapRemove*****/");
	printf("Remove: (10)\n ");
	HeapRemove(heap, IsMatchInt, &arr[0], &result);
	printf("excepted:\n(50 45 46 12 5)\n ");
	HeapForEach	(heap, &PrintInt, 0);
	printf("\nresult after remove:(%d?) %d\n", arr[0], result);
	printf("\nRemove: (46)\n ");
	HeapRemove(heap, IsMatchInt, &arr[6], &result);
	printf("excepted:\n(50 45 5 12)\n ");
	HeapForEach	(heap, &PrintInt, 0);
	printf("\nresult after remove:(%d?) %d\n", arr[6], result);
	size_heap = HeapSize(heap);
	printf("/****HeapSize:****/\nsize (4?) = %lu\n", size_heap);
	printf("/****HeapPeek:****/\nHeapPeek after remove:(%d?) %d\n", arr[8], *(int *)HeapPeek(heap));
	puts("\n/*****HeapDestroy*****/");
	HeapDestroy(heap);
	puts("TEST IS END but You need check your results lemala!");
	/****************/
	return(0);
}
