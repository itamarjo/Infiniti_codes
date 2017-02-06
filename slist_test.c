#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "slist.h"

typedef struct tst
{
	char a;
	int b;
	void *c;
} tst_data_t;

/* print list */
void TestPrintSList(slist_node_t *list);


/* create simple list and then free all */
int TestSListCreateAndFree(void);


/* 3 steps for loop detector verification:
	1) create a simple list (check that loop is not detected)
	2) connect the last one somwhere in between (loop should be detected)
	3) remove and check that loop is resolved
 */
int TestSListLoops(void);

/* a comparing function */
int TestCmp(void *data, void *seek)
{
	return (!(((tst_data_t*)data)->b == ((tst_data_t*)seek)->b));
}

/* creates intersection and tests if its detected */
int TestSListIntersect(void);

int main(int argc, char *argv[])
{
	TestSListCreateAndFree();
	TestSListLoops();
	TestSListIntersect();

	return(0);
}

int TestSListCreateAndFree(void)
{
	slist_node_t *nd1 = 0, *nd2 = 0, *nd3 = 0;
	tst_data_t t2 = {0,0,0}, t3 = {1,1,(void*)1};

	nd1 = SListCreateAndInitNode(&t2, 0);
	assert(0 != nd1);

	nd2 = SListCreateAndInitNode(&t3, nd1);
	assert(0 != nd2);

	nd3 = SListCreateAndInitNode(&t2, nd2);
	assert(0 != nd3);

	SListFreeList(nd3);

	return (0);
}

int TestSListLoops(void)
{
	slist_node_t *nd = 0, *nd_last = 0;
	tst_data_t tt[5] = {{0}};
	tst_data_t *p_stt = 0;
	size_t i;

	/* create a list with 5 nodes */
	p_stt = tt;
	p_stt->b = 5;
	nd_last = SListCreateAndInitNode(p_stt, 0);
	++p_stt;
	assert(0 != nd_last);
	nd = nd_last;
	for (i = 1; i < 5; ++i)
	{
		p_stt->b = 5-i;

		nd = SListCreateAndInitNode(p_stt, nd);
		assert(0 != nd);
		++p_stt;
	}

	/* at this point we have a simple list */
	assert(0 == SListIsLoop(nd));
	/* at this point we have a simple list */
	assert(0 == SListIsLoop(nd_last));

	/* now we connect the last one to the first to create loop */
	SListInsert(nd, nd_last);

	/* a loop should be detected */
	assert(1 == SListIsLoop(nd));
	/* a loop should be there */
	assert(1 == SListIsLoop(nd_last));

	/* this should keep the list as is */
	SListInsertAfter(nd_last, SListRemoveAfter(nd_last));

	/* will not break the loop */
	assert(nd_last == SListFlip(nd_last));

	/* we want to break a loop
	Thus we shall find appropriete first and last points */
	/* first */
	nd = SListFind(nd, &TestCmp, &(tt[0]));
	/* last */
	nd_last = SListFind(nd, TestCmp, &(tt[3]));
	/* break */
	nd_last->next = 0;

	/* a loop should be gone */
	assert(0 == SListIsLoop(nd));

	printf("\nloops:\n");
	TestPrintSList(nd);
	printf("\n5 1 4 3 2\n");

	SListFreeList(nd);

	return (0);
}

/* creates intersection and tests if its detected */
int TestSListIntersect(void)
{
	slist_node_t *nd = 0, *nd_last = 0, *nd_new = 0;
	tst_data_t tt[5] = {{0}};
	tst_data_t *p_stt = 0;
	size_t i;

	/* create a list with 5 nodes */
	p_stt = tt;
	p_stt->b = 5;
	nd_last = SListCreateAndInitNode(p_stt, 0);
	++p_stt;
	assert(0 != nd_last);
	nd = nd_last;
	for (i = 1; i < 5; ++i)
	{
		p_stt->b = 5-i;

		nd = SListCreateAndInitNode(p_stt, nd);
		assert(0 != nd);
		++p_stt;
	}

	/* a trivial intersection */
	assert(1 == SListIsIntersect(nd, nd));

	nd_new = SListFlip(nd->next->next);

	assert(1 == SListIsIntersect(nd, nd_new));

	printf("\nintersections:\n");
	TestPrintSList(nd);
	printf("\n1 2 3\n");
	TestPrintSList(nd_new);
	printf("\n5 4 3\n");

	SListFreeList(nd);
	SListFreeList(nd_new);

	return (0);
}




int CllBkPrint(void *data, void *param)
{
	printf("%d ", ((tst_data_t*)data)->b);
	return (0);
}

void TestPrintSList(slist_node_t *list)
{
	SListForEach(list, &CllBkPrint, 0);
}
