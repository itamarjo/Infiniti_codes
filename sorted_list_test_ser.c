#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "sorted_list.h"

/* check if b is subset of a */
#define IS_SUBSET(a, b) (strlen(b) == strspn((b), (a)))
#define SIZEOF_ARR(x) (sizeof(x)/sizeof(x[0]))

typedef enum errors
{
	SUCCESS,
	CREATE_FAILED,
	INSERT_FAILED,
	BEGIN_FAILED,
	REMOVE_FAILED,
	POPHEAD_FAILED,
	POPTAIL_FAILED,
	SIZE_FAILED,
	FIND_FAILED,
	SORT_FAILED,
	EMPTY_FAILED,
	UNDEFINED
} err_t;

/* print list - str */
static void TestPrintStrings(sl_t *list);
/* print list - int */
static void TestPrintInts(sl_t *list);
static void PrintError(err_t e);

/* print series of strings */
static void TestPrintExpectedStr(const char *expected[], size_t sz);
/* print series of ints */
static void TestPrintExpectedInt(const int *expected[], size_t sz);
/* print series of strings */
static void TestPrintResultStr(const char *expected[], size_t sz);
/* print series of ints */
static void TestPrintResultInt(const int *expected[], size_t sz);
/* compare pointers in list with expected array of pointers */
static int TestVerify(sl_t *list, const void *expected[], size_t sz);

/* comparison function */
/* B is subset of A   iff  strlen(B) == strspn(B, A) */
static int IsStrictSubSet(const void* a, const void *b, void *patam);

/* integer array comparison function */
static int IsStrictSmall(const void* a, const void *b, void *patam);

/* returns 0 on success */
static err_t TestSLCreateDestroy(void);

/* returns 0 on success */
static err_t TestSLCreateDestroyWithParam(void);

/* returns 0 on success */
static err_t TestSLRemove(void);

/* returns 0 on success */
static err_t TestSLSizeIsEmpty(void);

static err_t TestSLFindInt(void);

/* returns 0 on success */
static err_t TestSLFind(void);

/* returns 0 on success */
static err_t TestSLMerge(void);


int main(int argc, char *argv[])
{
	err_t res;
	res = TestSLCreateDestroy();
	PrintError(res);

	res = TestSLCreateDestroyWithParam();
	PrintError(res);

	res = TestSLRemove();
	PrintError(res);

	res = TestSLSizeIsEmpty();
	PrintError(res);

	res = TestSLFindInt();
	PrintError(res);

	res = TestSLFind();
	PrintError(res);

	res = TestSLMerge();
	PrintError(res);

	return(0);
}
/****************************************************************************/


/****************************************************************************/
static err_t TestSLCreateDestroy(void)
{
	sl_t *lst = 0;
	sl_iter_t node;
	err_t result = SUCCESS;
	size_t i;
	/* set inclusion ordered items */
	const char* items[] =
	{
		"  ",
		" >> ",
		" (*)===(|):::::::::::::::> ",
		" *cxxx=(|);:;:;:;:;:;:;> ",
		" (*)cxx=|{;:::::::> "
	};
	const size_t tot_size = 5;
	size_t insrt1_idx[] = {0, 1, 2, 3, 4};
	size_t insrt2_idx[] = {3, 1, 2, 4, 0};
	size_t srtd_idx[] = {0, 1, 2, 3, 4};
	const char *expected[5] = {0};

	if (0 == (lst = SLCreate(&IsStrictSubSet, 0)))
	{
		return (CREATE_FAILED);
	}

	for (i = 0; i < tot_size; ++i)
	{
		node = SLInsert(lst, (void *) items[insrt1_idx[i]]);
		if (SLIsSameIter(node, SLEnd(lst)))
		{
			result = INSERT_FAILED;
			break;
		}
	}

	if (result != SUCCESS)
	{
		SLDestroy(lst);
		return (result);
	}

	for (i = 0; i < tot_size; ++i)
	{
		expected[i] = items[srtd_idx[i]];
	}
	if (0 != TestVerify(lst, (const void **) expected, tot_size))
	{
		result = SORT_FAILED;
		/* verification failed - print the difference */
		TestPrintExpectedStr(expected, tot_size);
		TestPrintStrings(lst);
	}

	SLDestroy(lst);

	if (result != SUCCESS)
	{
		return (result);
	}

	/**** try to insert in different order ***/

	if (0 == (lst = SLCreate(&IsStrictSubSet, 0)))
	{
		return (CREATE_FAILED);
	}

	for (i = 0; i < tot_size; ++i)
	{
		node = SLInsert(lst, (void *) items[insrt2_idx[i]]);
		if (SLIsSameIter(node, SLEnd(lst)))
		{
			result = INSERT_FAILED;
			break;
		}
	}

	if (result != SUCCESS)
	{
		SLDestroy(lst);
		return (result);
	}

	for (i = 0; i < tot_size; ++i)
	{
		expected[i] = items[srtd_idx[i]];
	}
	if (0 != TestVerify(lst, (const void **) expected, tot_size))
	{
		result = SORT_FAILED;
		/* verification failed - print the difference */
		TestPrintExpectedStr(expected, tot_size);
		TestPrintStrings(lst);
	}

	SLDestroy(lst);

	return (result);
}
/****************************************************************************/


/****************************************************************************/
static err_t TestSLCreateDestroyWithParam(void)
{
	sl_t *lst1 = 0;
	sl_t *lst2 = 0;
	sl_iter_t node;
	err_t result = SUCCESS;
	size_t i;
	const int items[][2] =
	{
		{-2, 5}, {-5, 2}, {1, 3}, {0, 4}, {7, 1}
	};
	const size_t tot_size = 5;
	size_t insrt1_idx[] = {4, 1, 2, 3, 0};
	size_t insrt2_idx[] = {4, 1, 2, 3, 0};
	size_t srtd1_idx[] = {1, 0, 3, 2, 4};
	size_t srtd2_idx[] = {4, 1, 2, 3, 0};
	const int *expected[5] = {0};

	if (0 == (lst1 = SLCreate(&IsStrictSmall, (void *) 0)))
	{
		return (CREATE_FAILED);
	}
	for (i = 0; i < tot_size; ++i)
	{
		node = SLInsert(lst1, (void *) items[insrt1_idx[i]]);
		if (SLIsSameIter(node, SLEnd(lst1)))
		{
			result = INSERT_FAILED;
			break;
		}
	}

	if (result != SUCCESS)
	{
		SLDestroy(lst1);
		return (result);
	}

	/** VERIFY and PRINT if FAILS ********/
	for (i = 0; i < tot_size; ++i)
	{
		expected[i] = items[srtd1_idx[i]];
	}
	if (0 != TestVerify(lst1, (const void **) expected, tot_size))
	{
		result = SORT_FAILED;
		/* verification failed - print the difference */
		TestPrintExpectedInt(expected, tot_size);
		TestPrintInts(lst1);
	}

	if (result != SUCCESS)
	{
		SLDestroy(lst1);
		return (result);
	}

	/** Second list ********/

	if (0 == (lst2 = SLCreate(&IsStrictSmall, (void *) 1)))
	{
		SLDestroy(lst1);
		return (CREATE_FAILED);
	}
	for (i = 0; i < tot_size; ++i)
	{
		node = SLInsert(lst2, (void *) items[insrt2_idx[i]]);
		if (SLIsSameIter(node, SLEnd(lst2)))
		{
			result = INSERT_FAILED;
			break;
		}
	}

	if (result != SUCCESS)
	{
		SLDestroy(lst1);
		SLDestroy(lst2);
		return (result);
	}

	/** VERIFY and PRINT if FAILS ********/
	for (i = 0; i < tot_size; ++i)
	{
		expected[i] = items[srtd2_idx[i]];
	}
	if (0 != TestVerify(lst2, (const void **) expected, tot_size))
	{
		result = SORT_FAILED;
		/* verification failed - print the difference */
		TestPrintExpectedInt(expected, tot_size);
		TestPrintInts(lst2);
	}

	SLDestroy(lst1);
	SLDestroy(lst2);

	return (result);
}
/****************************************************************************/


/****************************************************************************/
static err_t TestSLRemove(void)
{
	sl_t *lst = 0;
	sl_iter_t node;
	err_t result = SUCCESS;
	size_t i;
	void *reference = 0, *data = 0;
	/* set inclusion ordered items */
	const char* items[] =
	{
		"  ",
		" >> ",
		" (*)===(|):::::::::::::::> ",
		" *cxxx=(|);:;:;:;:;:;:;> ",
		" (*)cxx=|{;:::::::> "
	};
	const size_t tot_size = 5;
	size_t insrt_idx[] = {2, 0, 1, 3, 4};
	size_t srtd_idx[] = {0, 1, 2, 3, 4};
	const char *expected[5] = {0};

	if (0 == (lst = SLCreate(&IsStrictSubSet, 0)))
	{
		return (CREATE_FAILED);
	}
	for (i = 0; i < tot_size; ++i)
	{
		node = SLInsert(lst, (void *) items[insrt_idx[i]]);
		if (SLIsSameIter(node, SLEnd(lst)))
		{
			result = INSERT_FAILED;
			break;
		}
	}

	if (result != SUCCESS)
	{
		SLDestroy(lst);
		return (result);
	}

	for (i = 0; i < tot_size; ++i)
	{
		expected[i] = items[srtd_idx[i]];
	}
	if (0 != TestVerify(lst, (const void **) expected, tot_size))
	{
		result = SORT_FAILED;
		/* verification failed - print the difference */
		TestPrintExpectedStr(expected, tot_size);
		TestPrintStrings(lst);
	}

	if (result != SUCCESS)
	{
		SLDestroy(lst);
		return (result);
	}


	/* verify return values of variuos functions */
	reference = (void *) items[srtd_idx[1]];

	data = SLGetData(SLNext(SLBegin(lst)));
	if (reference != data)
	{
		result = BEGIN_FAILED;
		TestPrintExpectedStr((const char **)&reference, 1);
		TestPrintResultStr((const char **)&data, 1);
	}

	if (result != SUCCESS)
	{
		SLDestroy(lst);
		return (result);
	}


	data = SLGetData(SLRemove(SLBegin(lst)));
	if (reference != data)
	{
		result = REMOVE_FAILED;
		TestPrintExpectedStr((const char **)&reference, 1);
		TestPrintResultStr((const char **)&data, 1);
	}

	if (result != SUCCESS)
	{
		SLDestroy(lst);
		return (result);
	}


	data = SLGetData(SLBegin(lst));
	if (reference != data)
	{
		result = BEGIN_FAILED;
		TestPrintExpectedStr((const char **)&reference, 1);
		TestPrintResultStr((const char **)&data, 1);
	}

	if (result != SUCCESS)
	{
		SLDestroy(lst);
		return (result);
	}

	data = SLPopHead(lst);
	if (reference != data)
	{
		result = POPHEAD_FAILED;
		TestPrintExpectedStr((const char **)&reference, 1);
		TestPrintResultStr((const char **)&data, 1);
	}

	if (result != SUCCESS)
	{
		SLDestroy(lst);
		return (result);
	}


	reference = (void *) items[srtd_idx[tot_size - 1]];
	data = SLPopTail(lst);
	if (reference != data)
	{
		result = POPTAIL_FAILED;
		TestPrintExpectedStr((const char **)&reference, 1);
		TestPrintResultStr((const char **)&data, 1);
	}

	/* TestPrintStrings(lst); */

	if (result != SUCCESS)
	{
		SLDestroy(lst);
		return (result);
	}

	/** VERIFY and PRINT if FAILS ********/
	expected[0] = items[srtd_idx[2]];
	expected[1] = items[srtd_idx[3]];
	if (0 != TestVerify(lst, (const void **) expected, 2))
	{
		result = REMOVE_FAILED;
		/* verification failed - print the difference */
		TestPrintExpectedStr(expected, 2);
		TestPrintStrings(lst);
	}

	SLDestroy(lst);
	return (result);
}
/****************************************************************************/


/****************************************************************************/
static err_t TestSLSizeIsEmpty(void)
{
	sl_t *lst = 0;
	sl_iter_t node;
	err_t result = SUCCESS;
	size_t i;
	void *reference = 0, *data = 0;
	/* set inclusion ordered items */
	const char* items[] =
	{
		"  ",
		" >> ",
		" (*)===(|):::::::::::::::> ",
		" *cxxx=(|);:;:;:;:;:;:;> ",
		" (*)cxx=|{;:::::::> "
	};
	const size_t tot_size = 5;
	size_t insrt_idx[] = {2, 1, 0, 3, 4};
	size_t srtd_idx[] = {0, 1, 2, 3, 4};
	const char *expected[5] = {0};

	if (0 == (lst = SLCreate(&IsStrictSubSet, 0)))
	{
		return (CREATE_FAILED);
	}

	if (0 == SLIsEmpty(lst))
	{
		SLDestroy(lst);
		return (EMPTY_FAILED);
	}
	if (0 != SLSize(lst))
	{
		SLDestroy(lst);
		return (SIZE_FAILED);
	}
	/*----------------------------------------------------*/
	for (i = 0; i < tot_size; ++i)
	{
		node = SLInsert(lst, (void *) items[insrt_idx[i]]);
		if (SLIsSameIter(node, SLEnd(lst)))
		{
			result = INSERT_FAILED;
			break;
		}

		if ((i + 1) != SLSize(lst))
		{
			result = SIZE_FAILED;
			break;
		}
		if (0 != SLIsEmpty(lst))
		{
			result = EMPTY_FAILED;
			break;
		}
	}
	/**___________________**/
	if (result != SUCCESS)
	{
		SLDestroy(lst);
		return (result);
	}

	/*----------------------------------------------------*/
	for (i = 0; i < tot_size; ++i)
	{
		expected[i] = items[srtd_idx[i]];
	}
	if (0 != TestVerify(lst, (const void **) expected, tot_size))
	{
		result = SORT_FAILED;
		/* verification failed - print the difference */
		TestPrintExpectedStr(expected, tot_size);
		TestPrintStrings(lst);
	}
	/**___________________**/
	if (result != SUCCESS)
	{
		SLDestroy(lst);
		return (result);
	}


	/* verify return values of variuos functions */
	reference = (void *) items[srtd_idx[1]];
	/*----------------------------------------------------*/
	data = SLGetData(SLRemove(SLBegin(lst)));
	if (reference != data)
	{
		result = REMOVE_FAILED;
		TestPrintExpectedStr((const char **)&reference, 1);
		TestPrintResultStr((const char **)&data, 1);
	}
	/**___________________**/
	if (result != SUCCESS)
	{
		SLDestroy(lst);
		return (result);
	}
	/*----------------------------------------------------*/
	if ((tot_size - 1) != SLSize(lst))
	{
		result = SIZE_FAILED;
	}
	/**___________________**/
	if (result != SUCCESS)
	{
		SLDestroy(lst);
		return (result);
	}

	/*----------------------------------------------------*/
	data = SLPopHead(lst);
	if (reference != data)
	{
		result = POPHEAD_FAILED;
		TestPrintExpectedStr((const char **)&reference, 1);
		TestPrintResultStr((const char **)&data, 1);
	}
	/**___________________**/
	if (result != SUCCESS)
	{
		SLDestroy(lst);
		return (result);
	}
	/*----------------------------------------------------*/
	if ((tot_size - 2) != SLSize(lst))
	{
		result = SIZE_FAILED;
	}
	/**___________________**/
	if (result != SUCCESS)
	{
		SLDestroy(lst);
		return (result);
	}

	/*----------------------------------------------------*/
	reference = (void *) items[srtd_idx[tot_size - 1]];
	data = SLPopTail(lst);
	if (reference != data)
	{
		result = POPTAIL_FAILED;
		TestPrintExpectedStr((const char **)&reference, 1);
		TestPrintResultStr((const char **)&data, 1);
	}
	/**___________________**/
	if (result != SUCCESS)
	{
		SLDestroy(lst);
		return (result);
	}
	/*----------------------------------------------------*/
	if ((tot_size - 3) != SLSize(lst))
	{
		result = SIZE_FAILED;
	}
	/**___________________**/
	if (result != SUCCESS)
	{
		SLDestroy(lst);
		return (result);
	}

	/*----------------------------------------------------*/
	if (0 != SLIsEmpty(lst))
	{
		result = EMPTY_FAILED;
	}
	/**___________________**/
	if (result != SUCCESS)
	{
		SLDestroy(lst);
		return (result);
	}

	/*----------------------------------------------------*/
	node = SLInsert(lst, (void *) " (*)===(|);::::> ");
	if (SLIsSameIter(node, SLEnd(lst)))
	{
		result = INSERT_FAILED;
	}
	/**___________________**/
	if (result != SUCCESS)
	{
		SLDestroy(lst);
		return (result);
	}
	/*----------------------------------------------------*/
	if ((tot_size - 2) != SLSize(lst))
	{
		result = SIZE_FAILED;
	}
	/**___________________**/
	if (result != SUCCESS)
	{
		SLDestroy(lst);
		return (result);
	}

	/*----------------------------------------------------*/
	reference = (void *) items[srtd_idx[3]];
	data = SLPopTail(lst);
	if (reference != data)
	{
		result = POPTAIL_FAILED;
		TestPrintExpectedStr((const char **)&reference, 1);
		TestPrintResultStr((const char **)&data, 1);
	}
	/**___________________**/
	if (result != SUCCESS)
	{
		SLDestroy(lst);
		return (result);
	}

	/*----------------------------------------------------*/
	reference = (void *) items[srtd_idx[2]];
	data = SLPopHead(lst);
	if (reference != data)
	{
		result = POPTAIL_FAILED;
		TestPrintExpectedStr((const char **)&reference, 1);
		TestPrintResultStr((const char **)&data, 1);
	}
	/**___________________**/
	if (result != SUCCESS)
	{
		SLDestroy(lst);
		return (result);
	}

	SLPopHead(lst);

	/*----------------------------------------------------*/
	if (0 != SLSize(lst))
	{
		result = SIZE_FAILED;
	}
	/**___________________**/
	if (result != SUCCESS)
	{
		SLDestroy(lst);
		return (result);
	}
	/*----------------------------------------------------*/
	if (0 == SLIsEmpty(lst))
	{
		result = EMPTY_FAILED;
	}


	SLDestroy(lst);
	return (result);
}
/****************************************************************************/


/****************************************************************************/
static err_t TestSLFindInt(void)
{
	sl_t *lst = 0;
	sl_iter_t node;
	err_t result = SUCCESS;
	void *reference = 0, *data = 0;
	size_t i;
	const int items[][2] =
	{
		{-5, 1}, {-2, 2}, {0, 3}, {1, 4}, {7, 5}
	};

	const int item0_to_find[] = {-5, 0};
	const int item2_to_find[] = {0, 0};
	const int item_not_to_find[] = {-1, 0};

	const size_t tot_size = 5;
	size_t insrt_idx[] = {1, 4, 0, 3, 2};
	const int *expected[5] = {0};

	if (0 == (lst = SLCreate(&IsStrictSmall, 0)))
	{
		return (CREATE_FAILED);
	}
	for (i = 0; i < tot_size; ++i)
	{
		node = SLInsert(lst, (void *) items[insrt_idx[i]]);
		if (SLIsSameIter(node, SLEnd(lst)))
		{
			result = INSERT_FAILED;
			break;
		}
	}

	if (result != SUCCESS)
	{
		SLDestroy(lst);
		return (result);
	}

	/** VERIFY and PRINT if FAILS ********/
	for (i = 0; i < tot_size; ++i)
	{
		expected[i] = items[i];
	}
	if (0 != TestVerify(lst, (const void **) expected, tot_size))
	{
		result = SORT_FAILED;
		/* verification failed - print the difference */
		TestPrintExpectedInt(expected, tot_size);
		TestPrintInts(lst);
	}

	if (result != SUCCESS)
	{
		SLDestroy(lst);
		return (result);
	}

	/*----------------------------------------------------*/
	node = SLFind(lst, (void *)item_not_to_find);
	if (0 == SLIsSameIter(node, SLEnd(lst)))
	{
		printf("\n ERROR no such item, but the following was found: \n");
		printf("%d\n", ((int*)SLGetData(node))[0]);
		result = FIND_FAILED;
	}
	/**___________________**/
	if (result != SUCCESS)
	{
		SLDestroy(lst);
		return (result);
	}



	/*----------------------------------------------------*/
	reference = (void *) items[0];
	node = SLFind(lst, (void *) item0_to_find);
	if (0 != SLIsSameIter(node, SLEnd(lst)))
	{
		result = FIND_FAILED;
	}
	/**___________________**/
	if (result != SUCCESS)
	{
		SLDestroy(lst);
		return (result);
	}
	/*------------------------------------*/
	data = SLGetData(node);
	if (reference != data)
	{
		result = FIND_FAILED;
		TestPrintExpectedInt((const int **)&reference, 1);
		TestPrintResultInt((const int **)&data, 1);
	}
	/**___________________**/
	if (result != SUCCESS)
	{
		SLDestroy(lst);
		return (result);
	}

	/*----------------------------------------------------*/
	reference = (void *) items[2];
	node = SLFind(lst, (void *) item2_to_find);
	if (0 != SLIsSameIter(node, SLEnd(lst)))
	{
		result = FIND_FAILED;
	}
	/**___________________**/
	if (result != SUCCESS)
	{
		SLDestroy(lst);
		return (result);
	}
	/*------------------------------------*/
	data = SLGetData(node);
	if (reference != data)
	{
		result = FIND_FAILED;
		TestPrintExpectedInt((const int **)&reference, 1);
		TestPrintResultInt((const int **)&data, 1);
	}
	/**___________________**/
	if (result != SUCCESS)
	{
		SLDestroy(lst);
		return (result);
	}



	SLDestroy(lst);
	return (result);
}
/****************************************************************************/


/****************************************************************************/
static err_t TestSLFind(void)
{
	sl_t *lst = 0;
	sl_iter_t node;
	err_t result = SUCCESS;
	size_t i;
	void *reference = 0, *data = 0;
	/* set inclusion ordered items */
	const char* items[] =
	{
		"  ",
		" >> ",
		" (*)===(|):::::::::::::::> ",
		" *cxxx=(|);:;:;:;:;:;:;> ",
		" (*)cxx=|{;:::::::> "
	};
	const size_t tot_size = 5;
	size_t insrt_idx[] = {2, 1, 3, 0, 4};
	size_t srtd_idx[] = {0, 1, 2, 3, 4};
	const char *expected[5] = {0};

	if (0 == (lst = SLCreate(&IsStrictSubSet, 0)))
	{
		return (CREATE_FAILED);
	}

	if (0 == SLIsEmpty(lst))
	{
		SLDestroy(lst);
		return (EMPTY_FAILED);
	}
	if (0 != SLSize(lst))
	{
		SLDestroy(lst);
		return (SIZE_FAILED);
	}
	/*----------------------------------------------------*/
	for (i = 0; i < tot_size; ++i)
	{
		node = SLInsert(lst, (void *) items[insrt_idx[i]]);
		if (SLIsSameIter(node, SLEnd(lst)))
		{
			result = INSERT_FAILED;
			break;
		}

		if ((i + 1) != SLSize(lst))
		{
			result = SIZE_FAILED;
			break;
		}
		if (0 != SLIsEmpty(lst))
		{
			result = EMPTY_FAILED;
			break;
		}
	}
	/**___________________**/
	if (result != SUCCESS)
	{
		SLDestroy(lst);
		return (result);
	}

	/*----------------------------------------------------*/
	for (i = 0; i < tot_size; ++i)
	{
		expected[i] = items[srtd_idx[i]];
	}
	if (0 != TestVerify(lst, (const void **) expected, tot_size))
	{
		result = SORT_FAILED;
		/* verification failed - print the difference */
		TestPrintExpectedStr(expected, tot_size);
		TestPrintStrings(lst);
	}
	/**___________________**/
	if (result != SUCCESS)
	{
		SLDestroy(lst);
		return (result);
	}



	node = SLFind(lst, (void *) "  ");


	printf("%s\n", (char *) SLGetData(node));






	/*----------------------------------------------------*/
	node = SLFind(lst, (void *) " (*)===(|);::::> ");
	if (0 == SLIsSameIter(node, SLEnd(lst)))
	{
		printf("\n ERROR no such item, but the following was found: \n");
		printf("%s\n", (char *) SLGetData(node));
		result = FIND_FAILED;
	}
	/**___________________**/
	if (result != SUCCESS)
	{
		SLDestroy(lst);
		return (result);
	}




	/*----------------------------------------------------*/
	reference = (void *) items[srtd_idx[2]];
	node = SLFind(lst, (void *) " (*)=====(|)::::>  ");
	if (SLIsSameIter(node, SLEnd(lst)))
	{
		printf("\nE1\n");
		result = FIND_FAILED;
	}
	/**___________________**/
	if (result != SUCCESS)
	{
		SLDestroy(lst);
		return (result);
	}
	/*------------------------------------*/
	data = SLGetData(node);
	if (reference != data)
	{
		result = FIND_FAILED;
		TestPrintExpectedStr((const char **)&reference, 1);
		TestPrintResultStr((const char **)&data, 1);
	}
	/**___________________**/
	if (result != SUCCESS)
	{
		SLDestroy(lst);
		return (result);
	}



	SLDestroy(lst);
	return (result);
}


/*******************************************************************/
static err_t TestSLMerge(void)
{
	sl_t *lst_to = 0;
	sl_t *lst_from = 0;
	sl_iter_t node;
	err_t result = SUCCESS;
	size_t i;
	const int items_to[][2] =
	{
		{-5, 1}, {-2, 2}, {0, 3}, {1, 4}, {7, 5}
	};
	const int items_from[][2] =
	{
		{-4, 1}, {-3, 2}, {-1, 3}, {8, 4}, {9, 5}
	};
	const size_t tot_size = 5;
	size_t insrt_idx[] = {1, 4, 0, 3, 2};
	const int *expected[5] = {0};

	if (0 == (lst_to = SLCreate(&IsStrictSmall, 0)))
	{
		return (CREATE_FAILED);
	}
	for (i = 0; i < tot_size; ++i)
	{
		node = SLInsert(lst_to, (void *) items_to[insrt_idx[i]]);
		if (SLIsSameIter(node, SLEnd(lst_to)))
		{
			result = INSERT_FAILED;
			break;
		}
	}

	if (result != SUCCESS)
	{
		SLDestroy(lst_to);
		return (result);
	}

	/** VERIFY and PRINT if FAILS ********/
	for (i = 0; i < tot_size; ++i)
	{
		expected[i] = items_to[i];
	}
	if (0 != TestVerify(lst_to, (const void **) expected, tot_size))
	{
		result = SORT_FAILED;
		/* verification failed - print the difference */
		TestPrintExpectedInt(expected, tot_size);
		TestPrintInts(lst_to);
	}

	if (result != SUCCESS)
	{
		SLDestroy(lst_to);
		return (result);
	}

	/** Second list ********/

	if (0 == (lst_from = SLCreate(&IsStrictSmall, 0)))
	{
		SLDestroy(lst_to);
		return (CREATE_FAILED);
	}
	for (i = 0; i < tot_size; ++i)
	{
		node = SLInsert(lst_from, (void *) items_from[insrt_idx[i]]);
		if (SLIsSameIter(node, SLEnd(lst_from)))
		{
			result = INSERT_FAILED;
			break;
		}
	}

	if (result != SUCCESS)
	{
		SLDestroy(lst_to);
		SLDestroy(lst_from);
		return (result);
	}

	/** VERIFY and PRINT if FAILS ********/
	for (i = 0; i < tot_size; ++i)
	{
		expected[i] = items_from[i];
	}
	if (0 != TestVerify(lst_from, (const void **) expected, tot_size))
	{
		result = SORT_FAILED;
		/* verification failed - print the difference */
		TestPrintExpectedInt(expected, tot_size);
		TestPrintInts(lst_from);
	}


	return (result);
}



/********************************************************************/
/* print series of strings */
static void TestPrintExpectedStr(const char *expected[], size_t sz)
{
	printf("\nPRINT EXPECTED STRINGS:\n");
	while (0 != sz)
	{
		printf("\"%s\"\n", *expected);
		++expected;
		--sz;
	}
}
/* print series of ints */
static void TestPrintExpectedInt(const int *expected[], size_t sz)
{
	printf("\nPRINT EXPECTED INTS:\n");
	while (0 != sz)
	{
		printf("{%d, %d}, ", (*expected)[0], (*expected)[1]);
		++expected;
		--sz;
	}
}
/* print series of strings */
static void TestPrintResultStr(const char *expected[], size_t sz)
{
	printf("\nPRINT RESULT STRINGS:\n");
	while (0 != sz)
	{
		printf("\"%s\"\n", *expected);
		++expected;
		--sz;
	}
}
/* print series of ints */
static void TestPrintResultInt(const int *expected[], size_t sz)
{
	printf("\nPRINT RESULT INTS:\n");
	while (0 != sz)
	{
		printf("{%d, %d}, ", (*expected)[0], (*expected)[1]);
		++expected;
		--sz;
	}
}


/********************************************************************/
/* compare pointers in list with expected array of pointers */

typedef struct vrf
{
	const void **ref;
	size_t idx;
} vrf_t;

static int Verificator(const void* data, void *param)
{
	vrf_t *vrf_pack = (vrf_t *) param;
	int result = 0;

	if (vrf_pack->ref[vrf_pack->idx] != data)
	{
		result = 1;
	}

	++(vrf_pack->idx);

	return (result);
}
static int TestVerify(sl_t *list, const void *expected[], size_t sz)
{
	vrf_t vrf_pack;

	vrf_pack.ref = expected;
	vrf_pack.idx = 0;


	SLForEach(SLBegin(list), SLEnd(list), &Verificator, &vrf_pack);


	return (vrf_pack.idx - sz);
}

/********************************************************************/
/* print list - str */
static int CllBkPrintStr(const void *data, void *param)
{
	printf("\"%s\"\n", (const char *)data);
	return (0);
}
static void TestPrintStrings(sl_t *list)
{
	printf("\nPRINT SORTED LIST STRINGS:\n");
	SLForEach(SLBegin(list), SLEnd(list), &CllBkPrintStr, 0);
}

/********************************************************************/
/* print list - int */
static int CllBkPrintInt(const void *data, void *param)
{
	printf("{%d, %d}, ", ((int*)data)[0], ((int*)data)[1]);
	return (0);
}
static void TestPrintInts(sl_t *list)
{
	printf("\nPRINT SORTED LIST INTS:\n");
	SLForEach(SLBegin(list), SLEnd(list), &CllBkPrintInt, 0);
}

/********************************************************************/
/* comparison functions */
/* B is subset of A   iff  strlen(B) == strspn(B, A) */
static int IsStrictSubSet(const void* a, const void *b, void *patam)
{
	/* check if b is subset of a */
	return (!IS_SUBSET(a, b) && IS_SUBSET(b, a));
}

/* integer array comparison function */
static int IsStrictSmall(const void* a, const void *b, void *patam)
{
	size_t i = (size_t)patam;

	return (((int *)a)[i] < ((int *)b)[i]);
}

/*********************************************************************/
void PrintError(err_t e)
{
	putchar('\n');

	switch (e)
	{
		case SUCCESS:
			printf(": OK\n");
			break;
		case CREATE_FAILED:
			printf(": Create failed\n");
			break;

		case INSERT_FAILED:
			printf(": Insert failed\n");
			break;

		case BEGIN_FAILED:
			printf(": Begin failed\n");
			break;

		case REMOVE_FAILED:
			printf(": Remove failed\n");
			break;

		case POPHEAD_FAILED:
			printf(": PopHead failed\n");
			break;

		case POPTAIL_FAILED:
			printf(": PopTail failed\n");
			break;

		case SIZE_FAILED:
			printf(": Size failed\n");
			break;

		case FIND_FAILED:
			printf(": Find failed\n");
			break;

		case SORT_FAILED:
			printf(": Sort failed\n");
			break;

		case EMPTY_FAILED:
			printf(": Empty failed\n");
			break;

		case UNDEFINED:
			printf(": UNDEFINED\n");
			break;
	}
}
