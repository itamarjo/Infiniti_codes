#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "dlist.h"

struct dlist_node
{
	void *data;
	node_t *prev;
	node_t *next;
};


struct dlist
{
	node_t head;
	node_t tail;
};

/* 	Create an new empty instance of a doubly list
	return: pointer to a pointer the list upon success
			0 - upon failure */
dlist_t *DlistCreate(void)
{
	dlist_t *new = (dlist_t *)malloc (sizeof(dlist_t));

	if (new != 0) /* check if the allocation didn't fail */
	{
		/* initialize pointer with 0, head and tail point to each other */
		new->head.prev = 0;
		new->head.data = 0;
		new->head.next = &new->tail;
		new->tail.prev = &new->head;
		new->tail.data = 0;
		new->tail.next = 0;
	}

	return(new);
}


/*	Destroy the dlist 'list'.
	list: pointer to a dlist.
	return: void */
void DlistDestroy(dlist_t *list)
{
	dlist_iter_t destroy_itr = 0;
	assert(list != 0);

	destroy_itr = DlistBegin(list);

	/* go through all elements in the list and remove them */
	while (DlistNext(destroy_itr) != 0)
	{
		destroy_itr = DlistRemove(destroy_itr);
	}
	/*free the list itself */
	free(list);
}


/*	Insert a node before the node pointed to by the iterator 'location'.
	location: the exsisting node, the new data will be inserted before it.
	data: a pointer to the content that the new node will hold.
	return: an iterator to the new node
	 		list 'end' - upon failure */
dlist_iter_t DlistInsert(dlist_t *list, dlist_iter_t location, void *data)
{
	dlist_iter_t new = 0;

	assert(list != 0);
	assert(location != 0);

	new = (dlist_iter_t)malloc(sizeof(node_t));
	if (0 == new) /* check if the allocation falied */
	{
		return (DlistEnd(list)); /*return the last element in the list */
	}
	/*connect the elements to the new element */
	new->prev = DlistPrev(location);
	new->next = location;
	new->data = data;
	DlistPrev(location)->next = new;
	location->prev = new;

	return(new);
}


/*	Remove a node 'location'.
	location: the requested node to remove.
	return: an iterator to the next node
			can not accept 'end' as iterator*/
dlist_iter_t DlistRemove(dlist_iter_t location)
{
	dlist_iter_t tmp;
	assert(location != 0);
	assert(location->next != 0);

	tmp = DlistNext(location);
	/* connect the element before and after to each other */
	DlistPrev(location)->next = location->next;
	DlistNext(location)->prev = location->prev;
	/*free the memory */
	free(location);

	return(tmp);
}


/*	Push a new node containing 'data' to the end of the dlist 'list'
 	list: a doubly linked list.
	data: the content of the new node.
	return: an iterator to the inserted node
				list 'end' if failed */
dlist_iter_t DlistPushBack(dlist_t *list, void *data)
{
	assert(list != 0);
	/* use insert with the end of the list as location */
	return(DlistInsert (list, DlistEnd(list), data));
}


/*	Push a new node containing 'data' to the begining of the dlist 'list'.
 	list: a doubly linked list.
	data: the content of the new node to be added to the begining of the list.
	return: an iterator to the last node
				list 'end' if failed */
dlist_iter_t DlistPushFront(dlist_t *list, void *data)
{
	assert(list != 0);
	/* use insert with the begin of the list as location */
	return(DlistInsert (list, DlistBegin(list), data));
}


/*	Remove the last node from the doubly list 'list'.
	list: a doubly linked list.
	return: a pointer to the data that was contained in the last item */
void *DlistPopBack(dlist_t *list)
{
	void *ret = 0;

	assert(list != 0);
	assert(DlistEnd(list) != DlistBegin(list));

	ret = DlistGetData(DlistEnd(list)->prev);
	/*remove the element before the tail */
	DlistRemove(DlistEnd(list)->prev);

	return(ret);
}


/*	Remove the first node from the doubly linked list 'list'.
	list: a doubly linked list.
	return: a pointer to the data that was contained in the first item */
void *DlistPopFront(dlist_t *list)
{
	void* ret = 0;
	assert(list != 0);
	assert(0 == DlistIsSameIter(DlistBegin(list), DlistEnd(list)));

	ret = DlistGetData(DlistBegin(list));
	/* use remove with on the begin - the element after the head */
	DlistRemove(DlistBegin(list));

	return(ret);
}

static int Count(void *data, void *counter)
{
	++*(size_t *)counter;

	return(0);
}
/*	Counts the number of nodes in doubly linked list 'list'
	list: doubly linked list.
	return: the number of nodes in doubly linked list 'list'*/
size_t DlistSize(const dlist_t *list)
{
	size_t counter = 0;

	assert(list != 0);
	/* use function count with foreach to count all members */
	DlistForEach(DlistBegin(list), DlistEnd(list), &Count, &counter);

	return(counter);
}

/*	Applying function 'callback' (with 'param') to each node in a range,
		or until falied
 	first: an iterator pointing to the first node to perform the function.
	last:  an iterator pointing to the out of range node.
	callback : pointer to the function.
 	param : parameter to be used by the function.
 	return : return exist status.
 			0 - success or returns the error number with param
 			will point to the  location of the failed node  */
int DlistForEach(dlist_iter_t first, const dlist_iter_t last,
	callback_t callback, void *param)
{
	dlist_iter_t iter = first;
	int status = 0;

	assert(first != 0);
	assert(last != 0);
	assert(callback != 0);
	/* as long as the iter didnt arrive to the enf of the list and functions
		didn't return failure (0), contine sending the iter to the func */
	while ( (0 == DlistIsSameIter(iter, last)) && (status == 0) )
	{
		status = callback(iter->data, param);
		iter = DlistNext(iter);
	}

	return (status);
}


/*	Check if the doubly linked list 'list' is empty
   	list: a doubly linked list.
	return: 1 - is empty. 0 is not empty */
int DlistIsEmpty(const dlist_t *list)
{
	assert(list != 0);
	/* check if begin and end are the same */
	return(DlistIsSameIter(DlistBegin(list), DlistEnd(list)));
}


/*	Returns a data from a node.
	iter: an iterator to the node.
	return: a pointer to the data contained in the node pointed by 'iter' */
void *DlistGetData(const dlist_iter_t iter)
{
	assert(iter != 0);

	return(iter->data);
}




/*	Return the first node in the 'list'.
	list: a doubly linked list.
	return: an iterator to the first node in the list */
dlist_iter_t DlistBegin(const dlist_t *list)
{
	assert(list != 0);

	return(list->head.next);
}


/*	Return the out of range node in the 'list'.
	list: a doubly linked list.
	return: an iterator to the end node in the list */
dlist_iter_t DlistEnd(const dlist_t *list)
{
	assert(list != 0);

	return((dlist_iter_t)&list->tail);
}


/* Find a node within a range according to criteria
	first : the start node to preform the search on.
	last: the out of range node.
	ptr_func : pointer to the comparison function (see typedef)
	data_cmp : the data sent to the function to search
	return : return an iterator to the node containing the data
			or to the out of range node if not found in the range*/
dlist_iter_t DlistFind(dlist_iter_t first, dlist_iter_t last, predicator_t ptr_func,
				const void *data_cmp)
{
	dlist_iter_t iter = first;


	assert(first != 0);
	assert(last != 0);
	assert(ptr_func != 0);
	/* continue moving iter forward until we reach the end or we found a match */
	while ( (0 == DlistIsSameIter(iter, last)) &&
			(ptr_func(DlistGetData(iter), data_cmp) == 0) )
	{
		iter = DlistNext(iter);
	}

	return (iter);
}


/*	Check whether two iterators points to the same object.
	iter_a: first iterator.
	iter_b: second iterator.
	return: 1 - if they are the same.
	 		0 - if they are not the same */
int DlistIsSameIter(dlist_iter_t iter_a, dlist_iter_t iter_b)
{
	return(iter_a == iter_b);
}


/*	Splice two doubly linked list together.
 	location: an iterator to the node before the action begins.
	from: the begining range of the nodes to splice.
	to: the out of range node.
	return: an iterator to the last copied node*/
dlist_iter_t DlistSplice(dlist_iter_t location,
							dlist_iter_t from, dlist_iter_t to)
{
	dlist_iter_t tmp_next = to;
	dlist_iter_t tmp_prev = from->prev;

	assert(to != from);
	assert(location != 0);
	assert(from != 0);
	assert(to != 0);

	/* connecting two list together */
	DlistPrev(to)->next = location;
	DlistPrev(location)->next = from;
	from->prev = DlistPrev(location);
	location->prev = DlistPrev(to);
	to->prev = tmp_prev;
	tmp_prev->next = tmp_next;

	return (DlistPrev(location));

}


/*	traverse forward on the list.
	iter: an iterator to the location in the doubly list.
	return: an iterator to the next node  */
dlist_iter_t DlistNext(const dlist_iter_t iter)
{
	assert(iter != 0);

	return (iter->next);
}


/*	traverse backwards on the list.
	iter: an iterator to the location in the doubly list.
	return: an iterator to the previous node  */
dlist_iter_t DlistPrev(const dlist_iter_t iter)
{
	assert(iter != 0);
	assert(iter->prev != 0);

	return (iter->prev);
}
