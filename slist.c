#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "slist.h"



/* Create a new instance of a node
	data: pointer to the data
	return: pointer to the node upon success
			0 - upon failure
*/
slist_node_t *SListCreateAndInitNode(void *data, slist_node_t *next)
{
	/*allocate memory to the new node */
	slist_node_t *new = (slist_node_t *) malloc(sizeof(slist_node_t));;

	if (0 == new)
	{
		return(0); /* exit if the allocation failed */
	}

	/* insert values to the new node */
	new->data = data;
	new->next = next;
	/* return the new node */
	return (new);
}


/* Insert a new node before the 'where' node
	where : existing node
	new_node: a new node to Insert
	return : void
	note: this function invalidate exsisting pointers to nodes
*/
void SListInsert(slist_node_t *where, slist_node_t *new_node)
{
	void *buffer_data = 0;
	slist_node_t *buffer_next = 0;

	assert(where != 0);
	assert(new_node != 0);

	 /* using buffer to insert the new node after where */
	 /* copy where node to buffer node */
	buffer_data = where->data;
	buffer_next = where->next;
	/*where gets the new node data */
	where->data = new_node->data;
	where->next = new_node;

	/*new_node gets the new node data */
	new_node->data = buffer_data;
	new_node->next = buffer_next;
}

/* Insert a new node after the where node
	where : existing node
	new_node: a new node to Insert
	return : void
*/

void SListInsertAfter(slist_node_t *where, slist_node_t *new_node)
{
	slist_node_t *buffer_next = 0;

	assert (where != 0);
	assert (new_node != 0);

	 /* using buffer to insert the new mode after where */
	buffer_next = where->next;
	/* where points to new_node */
	where->next = new_node;
	/* new node points to what the orifinal location pointed to */
	new_node->next = buffer_next;
}


/* Remove a node
	node : node to remove
	return : the removed node
	note: this function invalidate pointers
		  it is not possible to remove last node in the list
		  the data received is not the data removed
*/
slist_node_t *SListRemove(slist_node_t *node)
{
	slist_node_t *deleted_n = 0;

	assert (node != 0);
	assert (node->next != 0);

	deleted_n = node->next;


	node->data = node->next->data;
	node->next = node->next->next;

	return(deleted_n);
}


/* Remove the node after the node pointed
	node : node before the node to remove
	return : the removed node
	note: it is not possible to remove the node before the last node in the list
		  the data received is not the data removed
*/
slist_node_t *SListRemoveAfter(slist_node_t *node)
{
	slist_node_t *deleted_n = 0;
	assert(node != 0);
	assert(node->next != 0);

	deleted_n = node->next;
	node->next = node->next->next;

	return(deleted_n);
}


/* Free the list starting with the pointed node
	node : first node to free
	return : void
*/
void SListFreeList(slist_node_t *node)
{
	slist_node_t *slist_cpy = 0;

	while (node != 0)
	{
		slist_cpy = node;
		node = node->next;
		free(slist_cpy);
	}
}


/* Count the number of nodes starting with the pointed node
	node : first node to count
	return : number of nodes
	note : cannot receive a list with a loop
*/
size_t SListCount(const slist_node_t *node)
{
	size_t counter = 0;

	while (node != 0)
	{
		++counter;
		node = node->next;
	}

	return (counter);
}


/* Flip the list starting with the pointed node
	node : the node to start flip from
	return : pointer to the new first node
	note : cannot receive a list with a loop
*/
slist_node_t *SListFlip(slist_node_t *node)
{
	slist_node_t *help;
	slist_node_t *help2;

	assert(node != 0 );

	help = node->next;
	node->next = 0;

	while (help != 0)
	{
		help2 = help->next;
		help->next = node;
		node = help;
		help = help2;
	}

	return (node);
}


/* Check if a node in the list is pointed by more then one node
	node : first node in the list
	return : 0 if no loop , 1 if loop
*/
int SListIsLoop(const slist_node_t *node)
{
    slist_node_t  *slow_p = (slist_node_t *) node, *fast_p =(slist_node_t *) node;

    while (slow_p && fast_p && fast_p->next )
    {
        slow_p = slow_p->next;
        fast_p  = fast_p->next->next;
        if (slow_p == fast_p)
        {
            return (1);
        }
    }
    return (0);
}


/* Check if two lists share a node
	node_a : node of first list
	node_b : node of second list
	return : 0 if no intersect , 1 if intersect
	note : cannot receive a list with a loop
*/
int SListIsIntersect(const slist_node_t *node_a, const slist_node_t *node_b)
{
	slist_node_t *scan_a = (slist_node_t *) node_a;
	slist_node_t *scan_b = (slist_node_t *) node_b;

	assert(0 != node_a);
	assert(0 != node_b);

	while (scan_a->next != 0 )
	{
		scan_a = scan_a->next;
	}

	while (scan_b->next != 0 )
	{
		scan_b = scan_b->next;
	}

	return (scan_a->data == scan_b->data);
}


/* Find a node acording to criteria
	node : the start node
	ptr_func : pointer to the comparison function (see typedef)
	data_cmp : the data to search in all nodes
	return : return pointer to the node containing the data
			or NULL if not found
	note : cannot receive a list with a loop
*/
 slist_node_t *SListFind(const slist_node_t *node, predicator_t ptr_func,
 						 const void *data_cmp)
{
	assert(0 != ptr_func);
	/* note: 'data_cmp' may be null */

	while ( node != 0)
	{
		if (ptr_func(node->data,(void *) data_cmp) == 0)
		{
			return ((slist_node_t *)node);
		}
		node = node->next;
	}
	return (0);
}


 /* Applining func (with the param) to each node in the list
 	node : the start node
 	ptr_func : pointer to the function
 	param : parameter to be used by the function
 	return : return pointer to the last node used in the function,
			if function failed it will not continue,
			0 if the function wass applied on all nodes in the list
	note : cannot receive a list with a loop
 */
  slist_node_t *SListForEach(slist_node_t *node, action_t ptr_func, void *param)
  {
	slist_node_t *node_scan = node;

	assert(0 != ptr_func);

	while ((node_scan != 0) && (ptr_func(node_scan->data, param) == 0))
	{
		node_scan = node_scan->next;
	}

	return (node_scan);
  }
