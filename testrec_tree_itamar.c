/*
 * testrec_tree.c
 *
 *  Created on: Sep 4, 2016
 *      Author: pop
 */

#include "rec_tree.h"


int comp(const void *data_a, const void *data_b)
{
	return((*(int *)data_a) - (*(int *)data_b));
}

int main()
{
	rec_tree_t *tree = 0;
	int ar[] = { 3,7,2,6,1,5};
	int i = 0;

	tree = RTCreate(comp);
	if (0 == tree)
	{
		return(1);
	}

	for (i=0; i < 5; ++i)
	{
		RTInsert(tree, (void *)&ar[i]);
	}

	return(0);
}

