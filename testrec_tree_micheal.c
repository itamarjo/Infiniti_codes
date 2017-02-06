#include<stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rec_tree.h"

static int CompareInts (const void *a, const void *b)
{
	return ((const int)a - (const int)b);
}

int main(int argc, char *argv[])
{
	rec_tree_t *tree = 0;
	int array_int[5000] = {83, 86, 77, 15, 93, 35, 92, 49, 21, 20, 69, 46, 10, 0};
	int i = 0;
	int sz_arr = 14;
	
	puts("=======RTCreate==========");
	tree = RTCreate(CompareInts);
	if (0 == tree)
	{
		puts("RTCreate Failed!!!");
	}
	else
	{
		puts("RTCreate Success!!!");
	}
	puts("");

	puts("Array of Ints:");
	for (i = 0; i < sz_arr; ++i)
	{
		printf("%d ", array_int[i]);
	}

	puts("\n=======RTIsEmpty==========");
	printf("%d (excepted 1)", RTIsEmpty(tree));

	puts("\n=======RTInsert==========");
	for (i = 0; i < sz_arr; ++i)
	{
		RTInsert(tree, array_int[i]);
	}

	puts("\n=======RTIsEmpty==========");
	printf("%d (excepted 0)", RTIsEmpty(tree));

	puts("\n=======RTHeight==========");
	printf("%d (excepted 3)", RTHeight(tree));

	puts("\n=======RTSize==========");
	printf("%lu (excepted 14)", RTSize(tree));

	puts("\n=======RTIsFound==========");
	printf("%d (excepted 1)", RTIsFound(tree, array_int[5]));
	printf("%d (excepted 0)", RTIsFound(tree, 47));
	puts("\n");


	puts("\n=======RTDestroy==========");
	RTDestroy(tree);
	if (0 == tree)
	{
		puts("RTDestroy Failed!!!");
	}
	else
	{
		puts("RTDestroy Success!!!");
	}

	return(0);
}
