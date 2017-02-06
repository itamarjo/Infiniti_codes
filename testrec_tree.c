#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "rec_tree.h"

#define ARR_SZ(arr) (sizeof(arr)/sizeof(arr[0]))

typedef struct arr_tst
{
	size_t count;
	void *data;
} arr_tst_t;

typedef struct tst_trees
{
	size_t sz;
	int *data;
	int *sorted;
	char *descr;
} tst_trees_t;

static int CompareInts(const void *a, const void *b);
static int PrintInts(void *a, void *b);

static int TestBasic(void);
static int TestPrintTree(void);
static int TestAdvanced(void);

/****************************************************************/
int main(int argc, char *argv[])
{
	TestBasic();
	TestPrintTree();
	 TestAdvanced();

	return (0);
}
/****************************************************************/

static int CompareInts(const void *a, const void *b)
{
	return (*(const int *)a - *(const int *)b);
}

static int PrintInts(void *a, void *b)
{
	arr_tst_t *w = (arr_tst_t *) b;

	printf("%d : %d", ((int *) w->data)[w->count],  *(int *) a);

	if (((int *) w->data)[w->count] !=  *(int *) a)
	{
		return (1);
	}

	++w->count;

	if (w->count & 0x07)
	{
		printf(" || ");
	}
	else
	{
		putchar('\n');
	}

	return (0);
}

static int VerifyInts(void *a, void *b)
{
	arr_tst_t *w = (arr_tst_t *) b;

	if (((int *) w->data)[w->count] != *(int *) a)
	{
		printf("ERROR at (%lu): %d , expected %d",
						w->count, *(int *) a, ((int *) w->data)[w->count]);
		return (1);
	}

	++w->count;

	return (0);
}
/****************************************************************/


static int TestBasic(void)
{
	rec_tree_t *tree = 0;
	int tst_set[] =
	{0, 5, -5, 3, -3, 11, -11, 1, -1, 2, -2, 7, -7, 13, -13, 4, -4, 8, 9, 10};
	size_t i; /* running index */

	puts("=============== TestBasic ================");

	printf("1) ");

	printf(" Create");
	tree = RTCreate(&CompareInts);
	if (0 == tree)
	{
		puts(" failed");
		return (1);
	}

	RTDestroy(tree);
	puts(" OK");
	/****************/

	printf("2) ");

	printf(" Create");
	tree = RTCreate(&CompareInts);
	if (0 == tree)
	{
		puts(" failed");
		return (1);
	}
	puts(" OK");

	printf(" Insert");
	for (i = 0; i < ARR_SZ(tst_set); ++i)
	{
		if (0 != RTInsert(tree, tst_set + i))
		{
			puts(" failed");
			return (1);
		}
	}

	RTDestroy(tree);
	puts(" OK");
	/****************/

	printf("3) ");

	printf(" Create");
	tree = RTCreate(&CompareInts);
	if (0 == tree)
	{
		puts(" failed");
		return (1);
	}
	puts(" OK");

	printf(" Insert");
	for (i = 0; i < ARR_SZ(tst_set); ++i)
	{
		if (0 != RTInsert(tree, tst_set + i))
		{
			puts(" failed");
			return (1);
		}
	}
	puts(" OK");

	printf(" Find");
	for (i = 0; i < ARR_SZ(tst_set); ++i)
	{
		if (0 == RTIsFound(tree, tst_set + i))
		{
			puts(" failed");
			return (1);
		}
	}
	puts(" OK");

	printf(" Count");

	i = RTSize(tree);
	if (i != ARR_SZ(tst_set))
	{
		printf(" ! ERROR ! Count: %lu, expected %lu\n", i, ARR_SZ(tst_set));
	}

	RTDestroy(tree);
	puts(" OK");
	/****************/

	return (0);
}

/*************/

static int TestPrintTree(void)
{
	int tst[] = {1, 3, 0, -3, -5, 2, 4, -11, 7, 5, 6, 9, -2, -1, -10, 8, -7, 32,\
				33, 16, 14, 18, 17, -19, -12, -18, -17, -16, -14, -15, 10, 11, 12};
	int ref[] = {-19, -18, -17, -16, -15, -14, -12, -11, -10, -7, -5, -3, -2, -1,\
					0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 14, 16, 17, 18, 32, 33};

	rec_tree_t *tree = RTCreate(&CompareInts);
	arr_tst_t w;
	size_t i; /* running index */

	puts("=============== TestPrintTree ================");

	for (i = 0; i < ARR_SZ(tst); ++i)
	{
		RTInsert(tree, tst + i);
	}

	printf(" Total count of nodes: %lu (expected %lu)\n", RTSize(tree), ARR_SZ(tst));

	/* Print and verify */
	w.count = 0;
	w.data = ref;
	RTForEach(tree, PrintInts, &w);

	putchar('\n');

	RTRemove(tree, tst + 2);
	RTRemove(tree, tst + 12);

	printf(" IsFound(%d) 0? %d\n", tst[12], RTIsFound(tree, tst + 12));
	printf(" IsFound(%d) 1? %d\n", tst[22], RTIsFound(tree, tst + 22));
	printf(" IsFound(%d) 0? %d\n", tst[2], RTIsFound(tree, tst + 2));

	printf(" Height: 5? %d\n", RTHeight(tree));

	RTRemove(tree, tst + 24);
	RTRemove(tree, tst + 25);
	RTRemove(tree, tst + 26);
	RTRemove(tree, tst + 27);
	RTRemove(tree, tst + 23);
	RTRemove(tree, tst + 28);
	printf(" Height: 5? %d\n", RTHeight(tree));

	RTRemove(tree, tst + 29);
	printf(" Height: 4? %d\n", RTHeight(tree));

	RTDestroy(tree);

	return (0);
}

static int TestAdvanced(void)
{
	/* In this test we insert a sorted list, a BST's worst case scenario */
	/* Then we delete in inverse order */

	int tst[1024];
	rec_tree_t *tree = RTCreate(&CompareInts);
	arr_tst_t w;
	size_t i; /* running index */
	int height, cur_height;

	puts("=============== TestAdvanced ================");

	for (i = 0, height = -1; i < ARR_SZ(tst); ++i)
	{
		tst[i] = i;
		RTInsert(tree, tst + i);

		if (height != (cur_height = RTHeight(tree)))
		{
			height = cur_height;
			printf("After inserting %lu, Tree height changed to %d\n", i, height);
		}
	}

	/* Print and verify */
	printf(" Total count of nodes: %lu (expected %lu)\n", RTSize(tree), ARR_SZ(tst));
	w.count = 0;
	w.data = tst;
	RTForEach(tree, VerifyInts, &w);
	putchar('\n');
	/**/

	while (i)
	{
		--i;

		RTRemove(tree, tst + i);

		if (height != (cur_height = RTHeight(tree)))
		{
			height = cur_height;
			printf("After removing %lu, Tree height changed to %d\n", i, height);
		}
	}

	RTDestroy(tree);

	return (0);
}



