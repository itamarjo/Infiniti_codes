#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "search.h"

static void PrintArray(int arr[], size_t num_of_elem);

int compar(const void *a, const void *b)
{
	return(*(int *)a - *(int *)b);
}

int main(int argc, char *argv[])
{
	int array[5000] = {0};
	size_t array_sz = 10;
	size_t i = 0;
	int res = 0;

	for(; i < array_sz; ++i)
	{
		array[i] = rand() % 100;
	}
	qsort(&array, array_sz, sizeof(int), compar);
	PrintArray(&array, array_sz);

	printf("/**********BinarySearch********/\n");
	for(i = 0; i < array_sz; ++i)
	{
		int index = rand() % 10;
		res = *(int *)BinarySearch(&array, array_sz, sizeof(int), &array[index], compar);
		printf("find? (%d?): %d\n", array[index],res);
	}

	printf("\n*********RecBinarySearch********\n");
	for(i = 0; i < array_sz; ++i)
	{
		int index = rand() % 10;
		res = *(int *)RecBinarySearch(&array, array_sz, sizeof(int), &array[index], compar);
		printf("find? (%d?): %d\n", array[index], res);
	}

	printf("\n*********JumpSearchum********\n");
	for(i = 0; i < array_sz; ++i)
	{
	int index = rand() % 10;
	res = *(int *)JumpSearch(&array, array_sz, sizeof(int), &array[index], compar);
	printf("find? (%d?): %d\n", array[index], res);
	}
	return(0);
}

/* A utility function ot print an array of size n */
static void PrintArray(int arr[], size_t num_of_elem)
{
   int i = 0;
   for (i = 0; i < num_of_elem; ++i)
   {
	   if(i % 10 == 0)
	   {
		   printf("\n");
	   }
	   printf("%d ", arr[i]);
   }
   printf("\n");
}
