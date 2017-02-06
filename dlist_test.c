#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "dlist.h"

const char *txt[] =
{
	"A doubly linked list is a linked data structure that consists of",
	" a set of sequentially linked records called nodes.",
	" Each node contains two fields, called links, that are references to",
	" the previous and to the next node in the sequence of nodes.",
	" The beginning and ending nodes' previous and next links, respectively,",
	" point to some kind of terminator, typically a sentinel node or null,",
	" to facilitate traversal of the list. If there is only one sentinel node,",
	" then the list is circularly linked via the sentinel node.",
	" It can be conceptualized as two singly linked lists formed from",
	" the same data items, but in opposite sequential orders.",
	0
};

typedef enum errors
{
	SUCCESS,
	CREATE_FAILED,
	INSERT_FAILED,
	REMOVE_FAILED,
	SIZE_FAILED,
	FIND_FAILED,
	SPLICE_FAILED,
	EMPTY_FAILED,
	UNDEFINED
} err_t;

/* print list */
void TestPrintDList(dlist_t *list);
void PrintError(err_t e);

/* a wrapper for 'strcmp' to be used with DlistFind */
int ComparatorStr(const void *data, const void *seek);

/* a wrapper for 'strstr' to be used with DlistFind */
int ComparatorStrStr(const void *data, const void *seek);

/* returns 0 on success */
err_t TestDlistCreateDestroy(void);

/* returns 0 on success */
err_t TestDlistSize(void);

/* returns 0 on success */
err_t TestDlistSplice(void);



int main(int argc, char *argv[])
{
	err_t res;

	printf("Test Dlist create and destroy ");
	res = TestDlistCreateDestroy();
	PrintError(res);

	printf("Test Dlist size ");
	res = TestDlistSize();
	PrintError(res);

	printf("Test Dlist splice ");
	res = TestDlistSplice();
	PrintError(res);

	return(0);
}

/* returns 0 on success */
err_t TestDlistCreateDestroy(void)
{
	const char **p_txt = 0;
	dlist_t *lst = DlistCreate();
	dlist_iter_t node;
	err_t result = SUCCESS;

	if (0 == lst)
	{
		return (CREATE_FAILED);
	}

	for (p_txt = txt; 0 != *p_txt; ++p_txt)
	{
		node = DlistPushBack(lst, (void*)*p_txt);

		if (DlistIsSameIter(node, DlistEnd(lst)))
		{
			result = INSERT_FAILED;
			break;
		}
	}

	DlistDestroy(lst);

	return (result);
}

/* returns 0 on success */
err_t TestDlistSize(void)
{
	const char **p_txt = 0;
	dlist_t *lst = DlistCreate();
	dlist_iter_t node = 0;
	err_t result = SUCCESS;
	size_t sz;

	if (0 == lst)
	{
		return (CREATE_FAILED);
	}

	for (p_txt = txt; 0 != *p_txt; ++p_txt)
	{
		sz = DlistSize(lst);
		if (sz != (p_txt - txt))
		{
			result = SIZE_FAILED;
			break;
		}

		node = DlistPushBack(lst, (void*)*p_txt);
		if (DlistIsSameIter(node, DlistEnd(lst)))
		{
			result = INSERT_FAILED;
			break;
		}
	}

	if (SUCCESS == result)
	{
		for (; p_txt != (txt+1); --p_txt)
		{
			sz = DlistSize(lst);
			if (sz != (p_txt - txt))
			{
				result = SIZE_FAILED;
				break;
			}

			DlistPopFront(lst);
		}

		if (DlistGetData(DlistBegin(lst)) != txt[9])
		{
			TestPrintDList(lst);
			printf("\nEXPECTED:\n%s\n", txt[9]);
		}
	}

	if (SUCCESS == result)
	{
		if (0 == DlistIsEmpty(lst))
		{
			DlistPopFront(lst);

			if (0 == DlistIsEmpty(lst))
			{
				result = EMPTY_FAILED;
			}
		}
		else
		{
			result = EMPTY_FAILED;
		}
	}

	DlistDestroy(lst);

	return (result);
}
/*************************************************************/
err_t TestDlistSplice(void)
{
	const char **p_txt = 0;
	dlist_t *lst = DlistCreate();
	dlist_iter_t node = 0;
	dlist_iter_t node_fnd = 0;
	err_t result = SUCCESS;
	size_t i;

	if (0 == lst)
	{
		return (CREATE_FAILED);
	}

	for (p_txt = txt; 0 != *p_txt; ++p_txt)
	{
		node = DlistPushBack(lst, (void*)*p_txt);
		if (DlistIsSameIter(node, DlistEnd(lst)))
		{
			result = INSERT_FAILED;
			break;
		}
	}

	if (SUCCESS == result)
	{
		/* illegal ?
		DlistSplice(DlistBegin(lst), DlistEnd(lst),	DlistBegin(lst));
		*/
		/* illegal ?
		DlistSplice(DlistBegin(lst), DlistPrev(DlistEnd(lst)),
					DlistBegin(lst));
		*/

		/* take first 2 lines */
		DlistSplice(DlistEnd(lst), DlistBegin(lst),
					DlistNext(DlistNext(DlistBegin(lst))));

		node_fnd = DlistFind(DlistBegin(lst), DlistEnd(lst),
							&ComparatorStrStr, "terminator");

		/* TestPrintDList(lst); */

		/* 1 */
		node = DlistBegin(lst);
		for (i = 0; i < 3; ++i)
		{
			node = DlistNext(node);
		}

		if (node != node_fnd)
		{
			result = FIND_FAILED;
		}
	}

	if (SUCCESS == result)
	{
		node_fnd = DlistRemove(node_fnd);
		node_fnd = DlistRemove(node_fnd);

		node_fnd = DlistFind(DlistBegin(lst), DlistEnd(lst),
							&ComparatorStrStr, "terminator");

		if (node_fnd != DlistEnd(lst))
		{
			result = FIND_FAILED;
		}

		node_fnd = DlistFind(DlistBegin(lst), DlistEnd(lst),
							&ComparatorStrStr, "facilitate");

		if (node_fnd != DlistEnd(lst))
		{
			result = FIND_FAILED;
		}
	}

	if (SUCCESS == result)
	{
		DlistPopBack(lst);

		for (i = 0; i < 6; ++i)
		{
			DlistPopFront(lst);
		}

		if (DlistGetData(DlistBegin(lst)) != txt[0])
		{
			TestPrintDList(lst);
			printf("\nEXPECTED:\n%s\n", txt[0]);
		}
	}

	DlistDestroy(lst);

	return (result);
}


int CllBkPrint(void *data, void *param)
{
	puts(data);
	return (0);
}

/* print list */
void TestPrintDList(dlist_t *list)
{
	printf("\nPRINT DLIST:\n");
	DlistForEach(DlistBegin(list), DlistEnd(list), &CllBkPrint, 0);
}

/* a wrapper for 'strcmp' to be used with DlistFind */
int ComparatorStr(const void *data, const void *seek)
{
	return (strcmp(data, seek));
}

/* a wrapper for 'strstr' to be used with DlistFind */
int ComparatorStrStr(const void *data, const void *seek)
{
	char *res = strstr(data, seek);

	return (0 == res);
}


void PrintError(err_t e)
{
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

		case REMOVE_FAILED:
			printf(": Remove failed\n");
			break;

		case SIZE_FAILED:
			printf(": Size failed\n");
			break;

		case FIND_FAILED:
			printf(": Find failed\n");
			break;

		case SPLICE_FAILED:
			printf(": Splice failed\n");
			break;

		case EMPTY_FAILED:
			printf(": Empty failed\n");
			break;

		case UNDEFINED:
			printf(": UNDEFINED\n");
			break;
	}
}
