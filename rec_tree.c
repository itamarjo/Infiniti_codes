/*
 * rec_tree.c
 *
 *  Created on: Sep 1, 2016
 *      Author: pop
 */


#include <stdio.h> /*  AVL tree implementation */
#include <stdlib.h>
#include <assert.h>
#include "rec_tree.h"


#define MAX(a,b) (((a) >= (b)) ? (a) : (b))


typedef struct rec_tree rec_tree_t;
typedef struct rt_node rt_node_t;

typedef enum
{
	LEFT,
	RIGHT
} direction_t;

struct rt_node
{
	int bfactor; /* balance factor */
	void *data;
	rt_node_t *child[2];
};


struct rec_tree
{
	comp_t comp;
	rt_node_t *root;
};



/* a comparison function, aka a - b */
typedef int (*comp_t) (const void *a, const void *b);

/* the 'job' function should return 0 on success
 * otherwise the iteration stops and returns 'job' return value
 */
typedef int (*job_t)(void* data, void *params);

int RecHeight(rt_node_t *node);

static rt_node_t *Balance(rt_node_t *node);

static void BalanceFactor(rt_node_t *node)
{
	assert (node != 0);

	node->bfactor = RecHeight(node->child[LEFT]) - RecHeight(node->child[RIGHT]);
}


rec_tree_t *RTCreate(comp_t comp)
{
	rec_tree_t *tree = 0;

	assert(comp != 0);

	tree = (rec_tree_t *)malloc(sizeof(rec_tree_t));
	if (0 == tree)
	{
		return(0);
	}

	tree->comp = comp;
	tree->root = 0;

	return(tree);
}

void RecDestory(rt_node_t *node)
{
	if (node->child[LEFT] != 0)
	{
		RecDestory(node->child[LEFT]);
	}
	else if (node->child[RIGHT] != 0)
	{
		RecDestory(node->child[RIGHT]);
	}

	free(node); /*in case there is no children */
}

/* postorder */
void RTDestroy(rec_tree_t *tree)
{
	if (RTIsEmpty(tree) == 0)
	{
		RecDestory(tree->root);
	}
	free(tree);
}


rt_node_t *DeleteNode(rt_node_t *node, rt_node_t **next)
 {
		if (node->child[LEFT] == 0)
		{
			*next = node;
			return (node->child[RIGHT]);
		}
		node->child[LEFT] = DeleteNode(node->child[LEFT], next);
		node = Balance(node); /* p is balanced if is not NULL */

		return(node);
}



rt_node_t *RecRemove(rt_node_t *node, void *data, comp_t comp)
{
	rt_node_t *next = 0;

	if (comp(data, node->data) < 0)     /* the key that wish to be removed is less than the node information */
	{ /* the key is searches in the left subtree of current the node */



		node->child[LEFT] = RecRemove(node->child[LEFT], data, comp);




	}
	else if (comp(data, node->data) > 0) /* if the key is greater */
	{


		node->child[RIGHT] = RecRemove(node->child[RIGHT], data, comp);/* searches in the right subtree*/



	}
	else /* if the key is equal with the information from the current node*/
	{
		if ((node->child[RIGHT] != 0) && (node->child[LEFT] != 0))
		{
			node->child[RIGHT] = DeleteNode(node->child[RIGHT], &next);



			node->data = next->data;
			free(next);
			return(Balance(node));
		}
		if (node->child[RIGHT] != 0) /*in case it has a right kid (only one) */
		 {
			 next = node->child[RIGHT];



			 free(node);
			 return(next);
		 }

		 /* in case there is left kid or no kid at all */
		 next = node->child[LEFT];


		 free(node);
		 return(next);

	}

	node = Balance(node); /* p is balanced if is not NULL */
	return(node);
}

void RTRemove(rec_tree_t *tree, void *data)
{



	if (RTIsEmpty(tree) == 0)
	{
		tree->root = RecRemove(tree->root, data, tree->comp);
	}
}

rt_node_t *SingleFromRight(rt_node_t *node)
{
	rt_node_t *pivot = node->child[LEFT];
	node->child[LEFT] = pivot->child[RIGHT];
	pivot->child[RIGHT] = node;
	BalanceFactor(node);
	BalanceFactor(pivot);
	node = pivot;
	return(node);
}

rt_node_t *SingleFromLeft(rt_node_t *node)
{
	rt_node_t *pivot = node->child[RIGHT];
	node->child[RIGHT] = pivot->child[LEFT];
	pivot->child[LEFT] = node;
	BalanceFactor(node);
	BalanceFactor(pivot);
	node = pivot;
	return(node);
}

rt_node_t *DoublyFromRight(rt_node_t *node)
{
	node->child[LEFT] = SingleFromLeft(node->child[LEFT]);
	node = SingleFromRight(node);

	return(node);
}

rt_node_t *DoublyFromLeft(rt_node_t *node)
{
	node->child[RIGHT] = SingleFromRight(node->child[RIGHT]);
	node = SingleFromLeft(node);

	return(node);
}

static rt_node_t *Balance(rt_node_t *node)
{
	rt_node_t *ret = 0;

	/* Update Balance Factor of node */
	BalanceFactor(node);

	if (2 == node->bfactor)
	{
		ret = node->child[LEFT];
		if (-1 == ret->bfactor)
		{
			node = DoublyFromRight(node);
		}
		else
		{
			node = SingleFromRight(node);
		}
	}
	else if (-2 == node->bfactor)
	{
		ret = node->child[RIGHT];
		if (1 == ret->bfactor)
		{
			node = DoublyFromLeft(node);
		}
		else
		{
			node = SingleFromLeft(node);
		}
	}

	return (node);
 }



static rt_node_t *Insert (rt_node_t *node, rt_node_t *new_node, rec_tree_t *tree)
{
	 if(0 == node) /*if the current node is NULL then */
	 {
		 return (new_node);
	 }
	 else if (tree->comp(node->data, new_node->data) > 0) /* if the key that wish to be inserted is smaller than the information value of the current node*/
	 {
		 node->child[LEFT] = Insert(node->child[LEFT], new_node, tree); /* then is inserted into the current node's left subtree */
	 }
	 else if (tree->comp(node->data, new_node->data) < 0) /* if the key that wish to be inserted is greater than the information value of the current node */
	{
		 node->child[RIGHT] = Insert(node->child[RIGHT], new_node, tree); /* then is inserted into the current node's right subtree */
	}

	node = Balance(node); /* if there will be cases of imbalance it will balance the tree or even the subtree */
	return (node);
}

int RTInsert(rec_tree_t *tree, void *data)
{
	rt_node_t *new_node	= 0;

	assert(0 != tree);

	new_node = (rt_node_t *) malloc(sizeof(struct rt_node));
	if (0 == new_node)/* malloc failed */
	{
		return (1);
	}
	new_node->data = data;
	new_node->child[RIGHT] = 0;
	new_node->child[LEFT] = 0;
	new_node->bfactor = 0;

	if (RTIsEmpty(tree))
	{
		tree->root = new_node;
		return (0);
	}
	else
	{
		tree->root = Insert(tree->root, new_node,tree);
	}

	return (0);
}



int RecForEach(rt_node_t *node, job_t job, void *params)
{

	int res = 0;

	if (0 == node)
	{
		return (0);
	}
	RecForEach(node->child[LEFT], job, params);

	res =  (job(node->data, params));

	RecForEach(node->child[RIGHT], job, params);

	return(res);
}

/* inorder */
/* the 'job' function should return 0 on success
 * otherwise the iteration stops and returns 'job' return value
 */
int RTForEach(rec_tree_t *tree, job_t job, void *params)
{
if (RTIsEmpty(tree) == 1)
	{
		return(0);
	}
	return(RecForEach(tree->root, job, params));
}


int RecHeight(rt_node_t *node)
{
	if (node == 0)
	{
		return(-1);
	}
	if (node->bfactor > 0)
	{
		return (1 + RecHeight(node->child[LEFT]));
	}

	return (1 + RecHeight(node->child[RIGHT]));

}

/*
int RecHeight(rt_node_t *node)
{
	if (node == 0)
	{
		return(-1);
	}

	return (1 + MAX(RecHeight(node->child[LEFT]), RecHeight(node->child[RIGHT])));
}
*/
int RTHeight(rec_tree_t *tree)
{
	return(RecHeight(tree->root));
}

size_t RecSize(rt_node_t *node)
{
	if (node == 0)
	{
		return(0);
	}

	return(1 + RecSize(node->child[RIGHT]) + RecSize(node->child[LEFT]));

}

size_t RTSize(rec_tree_t *tree)
{
	if (RTIsEmpty(tree) == 1)
	{
		return(0);
	}
	return(RecSize(tree->root));

}

int RTIsEmpty(rec_tree_t *tree)
{
	return(tree->root == 0);
}

int RecIsFound(rt_node_t *node, const void *data, comp_t comp)
{

	if (comp(node->data, data) < 0)
	{
		if(node->child[RIGHT] != 0)
		{
			return(RecIsFound(node->child[RIGHT], data, comp));
		}
		else
		{
			return(0);
		}
	}
	else if (comp(node->data, data) > 0)
	{
		if(node->child[LEFT] != 0)
		{
			return(RecIsFound(node->child[LEFT], data, comp));
		}
		else
		{
			return(0);
		}
	}
	return (1);
}

int RTIsFound(rec_tree_t *tree, const void *data)
{
	if (RTIsEmpty(tree) == 1)
		{
			return(0);
		}
	return(RecIsFound(tree->root, data, tree->comp));
}

