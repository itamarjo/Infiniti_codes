/*
 * bin_tree.c
 *
 *  Created on: Aug 28, 2016
 *      Author: pop
 */

#include<stdlib.h>
#include <stdio.h>
#include <strings.h>
#include <assert.h>

#include "bin_tree.h"

struct bt_node
{
	bt_node_t *parent;
	bt_node_t *left;
	bt_node_t *right;
	void *data;
};

struct bin_tree
{
	int (*compar) (const void *data_a, const void *data_b);
	bt_node_t dummy;
};



bin_tree_t *BinTCreate(int (*compar) (const void *, const void *))
{
	bin_tree_t *tree = 0;

	assert(compar != 0);

	tree = (bin_tree_t *)malloc(sizeof(bin_tree_t));
	if (0 == tree)
	{
		return(0);
	}


	tree->dummy.parent = 0;
	tree->dummy.left = 0;
	tree->dummy.right = 0;
	tree->dummy.data = 0;
	tree->compar = compar;

	return(tree);

}

void BinTDestroy(bin_tree_t *tree)
{
	bt_iter_t destroy;



	while (0 == BinTIsEmpty(tree))
	{
		destroy.node= tree->dummy.left;
		while ((destroy.node->left != 0) || (destroy.node->right != 0) )
		{
			while (destroy.node->left != 0)
			{
				destroy.node = destroy.node->left;
			}
			if (destroy.node->right != 0)
			{
				destroy.node = destroy.node->right;
			}
		}
		if (destroy.node->parent->right == destroy.node)
		{
			destroy.node->parent->right = 0;
		}
		else
		{
			destroy.node->parent->left = 0;
		}
		free(destroy.node);

	}
}

size_t BinTCount(bin_tree_t *tree)
{
	size_t count = 0;
	bt_iter_t iter;

	iter = BinTBegin(tree);
	while(iter.node != &tree->dummy)
	{
		iter = BinTNext(iter);
		++count;
	}
	return(count);
}

int BinTIsEmpty(bin_tree_t *tree)
{
	return(0 == tree->dummy.left);
}

bt_iter_t BinTBegin(bin_tree_t *tree)
{
	bt_iter_t scan;
	assert(tree != 0);

	scan.node = tree->dummy.left;
	if (0 == scan.node)
	{
		scan.node = &tree->dummy;
		return(scan);
	}
	while(0 != scan.node->left)
	{
		scan.node = scan.node->left;
	}
	return(scan);
}

bt_iter_t BinTEnd(bin_tree_t *tree)
{
	bt_iter_t end;
	end.node = &tree->dummy;
	return(end);
}


bt_iter_t BinTPrev(bt_iter_t iter)
{

	if (iter.node->left != 0)
	{
		iter.node = iter.node->left;
		while (iter.node->right != 0)
		{
			iter.node = iter.node->right;
		}
	}
	else
	{
		while (iter.node->parent->left == iter.node)
		{
			iter.node = iter.node->parent;
		}
		iter.node = iter.node->parent;
	}
	return(iter);
}

bt_iter_t BinTNext(bt_iter_t iter)
{
	if (iter.node->right != 0)
	{
		iter.node = iter.node->right;
		while (iter.node->left != 0)
		{
			iter.node = iter.node->left;
		}
	}
	else
	{
		while ((iter.node->parent->right == iter.node))
		{
			iter.node = iter.node->parent;
		}
		iter.node = iter.node->parent;
	}
	return(iter);
}


void *BinTGetData(bt_iter_t iter)
{
	return(iter.node->data);
}


bt_iter_t BinTInsert(bin_tree_t *tree, void *data)
{
	bt_node_t *node = 0;
	bt_iter_t scan;
	int insert = 0;
	assert (tree != 0 );

	node = (bt_node_t *)malloc(sizeof(bt_node_t));
	if (0 == node)
	{
		return (BinTEnd(tree));
	}


	node->data = data;
	node->left = 0;
	node->right = 0;

	/* check if the tree is empty */
	if (0 == tree->dummy.left)
	{
		node->parent = &tree->dummy;
		tree->dummy.left = node;
		scan.node = node;
		return(scan);
	}

	scan.node = tree->dummy.left;
	while (0 == insert)
	{
		if (tree->compar(data, scan.node->data) > 0)
		{
			if (scan.node->right != 0)
			{
				scan.node = scan.node->right;
			}
			else
			{
				scan.node->right = node;
				insert = 1;
			}
		}
		else if (scan.node->left != 0)
		{
			scan.node = scan.node->left;
		}
		else
		{
			/*insert new node */
			scan.node->left = node;
			insert = 1;
		}
	}
	node->parent = scan.node;
	scan.node = node;
	return(scan);
}


/* return next ?*/
void BinTRemove(bt_iter_t iter)
{
	if ((0 == iter.node->right) && (0 == iter.node->left))/* if there is no child */
	{
		if (iter.node->parent-> right == iter.node)
		{
			iter.node->parent->right = 0;
		}
		else
		{
			iter.node->parent->left = 0;
		}
	}
	else if ((0 != iter.node->right) && (0 == iter.node->left)) /* if there is a right child */
	{
		if (iter.node->parent-> right == iter.node) /* if the parent is a parent from the right */
		{
			iter.node->parent->right = iter.node->right;

		}
		else
		{
			iter.node->parent->left = iter.node->right;

		}
		iter.node->right->parent = iter.node->parent;
	}
	else if ((0 != iter.node->left) && (0 == iter.node->right)) /* if there is a left child */
	{
		if (iter.node->parent-> right == iter.node)
		{
			iter.node->parent->right = iter.node->left;
		}
		else
		{
			iter.node->parent->left = iter.node->left;
		}
		iter.node->left->parent = iter.node->parent;
	}
	else if ((0 != iter.node->left) && (0 != iter.node->right)) /* if the element has two children */
	{
		iter.node->data = BinTNext(iter).node->data; /*switch data with next */
		iter = BinTNext(iter); /* remove iter */
		if (0 != iter.node->right) /* if there is childern on the right */
		{
			if (iter.node->parent-> right == iter.node)
			{
				iter.node->parent->right = iter.node->right; /*connect the parent to it */
			}else
			{
				iter.node->parent->left = iter.node->right; /*connect the parent to it */
			}
			iter.node->right->parent = iter.node->parent;
		}
		else /*in case there is no child */
		{
			if (iter.node->parent-> right == iter.node)
			{
				iter.node->parent->right = 0;
			}
			else
			{
				iter.node->parent->left = 0;
			}
		}
	}

	free(iter.node);
}


/* const ?  */
int BinTForEach(bin_tree_t *tree, int (*func)( void* data,  void *params), void *params)
{
	bt_iter_t scan = BinTBegin(tree);
	int ret = 0;
	while ((scan.node != &tree->dummy) && (ret == 0))
	{
		ret = func((const void *)BinTGetData(scan), params);
		scan= BinTNext(scan);
	}
	return (ret);
}

int BinTIsSameIter(bt_iter_t a, bt_iter_t b)
{
	return(a.node == b.node);
}

bt_iter_t BinTFind(bin_tree_t *tree, const void *data)
{
	bt_iter_t scan;
	assert (tree != 0 );

	scan.node = tree->dummy.left;
	while (1)
	{
		if (tree->compar(data, scan.node->data) > 0)
		{
			if (scan.node->right != 0)
			{
				scan.node = scan.node->right;
			}
			else
			{
				break;
			}
		}
		else if (tree->compar(data, scan.node->data) < 0)
		{
			if (scan.node->left != 0)
			{
				scan.node = scan.node->left;
			}
			else
			{
				break;
			}
		}
		else if (tree->compar(data, scan.node->data) == 0)
		{
			return(scan);
		}
	}
	return(BinTEnd(tree));
}

int print(const void *data, void *params)
{
	printf("%d,", *(int *)data);

	return (0);
}

void TreePrint(bin_tree_t *tree)
{
	BinTForEach(tree, print, (void *)0);
}

