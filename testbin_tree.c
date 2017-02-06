#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "bin_tree.h"

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


int main(int argc, char *argv[])
{
	TestBasic();
	TestPrintTree();
	TestAdvanced();

	return (0);
}

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

static int TestBasic(void)
{
	bt_iter_t iter, end;
	bin_tree_t *tree = 0;
	int tst_complete[] = {0, 5, -5, 3, -3, 11, -11, 1, -1, 2, -2, 7, -7, 13, -13};
	size_t i;
	int result;

	puts("=============== TestBasic ================");

	printf("1) ");

	printf(" Create");
	tree = BinTCreate(&CompareInts);
	if (0 == tree)
	{
		puts(" failed");
		return (1);
	}

	BinTDestroy(tree);
	puts(" OK");
	/****************/

	printf("2) ");

	printf(" Create");
	tree = BinTCreate(&CompareInts);
	if (0 == tree)
	{
		puts(" failed");
		return (1);
	}
	puts(" OK");

	printf(" Insert");
	for (i = 0, end = BinTEnd(tree); i < ARR_SZ(tst_complete); ++i)
	{
		iter = BinTInsert(tree, tst_complete + i);
		if (0 != BinTIsSameIter(iter, end))
		{
			puts(" failed");
			return (1);
		}
	}

	BinTDestroy(tree);
	puts(" OK");
	/****************/

	printf("3) ");

	printf(" Create");
	tree = BinTCreate(&CompareInts);
	if (0 == tree)
	{
		puts(" failed");
		return (1);
	}
	puts(" OK");

	printf(" Insert");
	for (i = 0, end = BinTEnd(tree); i < ARR_SZ(tst_complete); ++i)
	{
		iter = BinTInsert(tree, tst_complete + i);
		if (0 != BinTIsSameIter(iter, end))
		{
			puts(" failed");
			return (1);
		}
	}
	puts(" OK");

	printf(" Find");
	for (i = 0, end = BinTEnd(tree); i < ARR_SZ(tst_complete); ++i)
	{
		iter = BinTFind(tree, tst_complete + i);
		if (0 != BinTIsSameIter(iter, end))
		{
			puts(" failed");
			return (1);
		}
		result = *(int *) BinTGetData(iter);
		if (result != tst_complete[i])
		{
			printf(" ! ERROR ! Found %d, expected %d\n", result, tst_complete[i]);
		}
	}
	puts(" OK");

	printf(" Count");

	i = BinTCount(tree);
	if (i != ARR_SZ(tst_complete))
	{
		printf(" ! ERROR ! Count: %lu, expected %lu\n", i, ARR_SZ(tst_complete));
	}

	BinTDestroy(tree);
	puts(" OK");
	/****************/

	return (0);
}

static int TestPrintTree(void)
{
	int tst[] = {1, 3, 0, -3, -5, 2, 4, -11, 7, 5, 6, 9, -2, -1, -10, 8, -7, 32,\
				33, 16, 14, 18, 17, -19, -12, -18, -17, -16, -14, -15, 10, 11, 12};
	int ref[] = {-19, -18, -17, -16, -15, -14, -12, -11, -10, -7, -5, -3, -2, -1,\
					0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 14, 16, 17, 18, 32, 33};

	bin_tree_t *tree = BinTCreate(&CompareInts);
	arr_tst_t w;
	size_t i;
	puts("=============== TestPrintTree ================");

	for (i = 0; i < ARR_SZ(tst); ++i)
	{
		BinTInsert(tree, tst + i);
	}

	printf(" Total count of nodes: %lu (expected %lu)\n", BinTCount(tree), ARR_SZ(tst));

	w.count = 0;
	w.data = ref;
	BinTForEach(tree, PrintInts, &w);

	BinTDestroy(tree);
	return (0);
}

static int *MinInt(int *p, size_t sz)
{
	int *p_end = p + sz;
	int *p_min = p;

	for (; p < p_end; ++p)
	{
		if (*p < *p_min)
		{
			p_min = p;
		}
	}

	return (p_min);
}

static int TestAdvanced(void)
{
	int tst_list[] = {0, 1, 2, 3};
	int ref_list[] = {0, 1, 2, 3};

	int tst_inv_list[] = {0, -1, -2, -3};
	int ref_inv_list[] = {-3, -2, -1, 0};

	int tst_two_lists[] = {0, -1, 1, -2, 2, -3, 3};
	int ref_two_lists[] = {-3, -2, -1, 0, 1, 2, 3};

	int tst_min_complete[] = {0, 1, -1};
	int ref_min_complete[] = {-1, 0, 1};

	int tst_complete[] = {0, 5, -5, 3, -3, 11, -11, 1, -1, 2, -2, 7, -7, 13, -13};
	int ref_complete[] = {-13, -11, -7, -5, -3, -2, -1, 0, 1, 2, 3, 5, 7, 11, 13};

	tst_trees_t tests[] =
	{
		{ARR_SZ(tst_list), tst_list, ref_list, "list"},
		{ARR_SZ(tst_inv_list), tst_inv_list, ref_inv_list, "inverse list"},
		{ARR_SZ(tst_two_lists), tst_two_lists, ref_two_lists, "two lists"},
		{ARR_SZ(tst_min_complete), tst_min_complete, ref_min_complete, "minimum complete"},
		{ARR_SZ(tst_complete), tst_complete, ref_complete, "complete"},
		{0,0,0}
	};

	bin_tree_t *tree = BinTCreate(&CompareInts);
	arr_tst_t w; /* for print */
	tst_trees_t *test;
	for ( test = tests; test->sz > 0; ++test)
	{
		int *p_num = test->data;
		int *p_num_end = p_num + test->sz;

		printf("\n================ %s ================\n", test->descr);

		for (;	p_num < p_num_end; ++p_num)
		{
			BinTInsert(tree, p_num);
		}

		w.count = 0;
		w.data = test->sorted;
		if (BinTForEach(tree, PrintInts, &w))
		{
			puts("\n     ! ERROR !");
		}

		bt_iter_t iter, prev;

		iter = BinTPrev(BinTEnd(tree));
		while (0 == BinTIsSameIter(iter, BinTBegin(tree)))
		{
			prev = BinTPrev(iter);
			BinTRemove(iter);
			iter = prev;
		}
		putchar('\n');

		printf("  Count 1? %lu\n", BinTCount(tree));

		if (BinTIsSameIter(BinTFind(tree, MinInt(test->data, test->sz)), BinTBegin(tree)))
		{
			BinTRemove(BinTBegin(tree));
		}
		else
		{
			puts(" 137: WASTED!");
			return (1);
		}
		
		if (0 == BinTIsEmpty(tree))
		{
			puts(" 13: WASTED!");
			return (1);
		}
		puts("OK");
	}

	BinTDestroy(tree);
	return (0);
}














