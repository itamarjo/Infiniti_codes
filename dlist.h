  #ifndef __DLIST_H__
#define __DLIST_H__
#include <stddef.h>

/*version num. RD20 DS dlist.h v1.00 */

typedef struct dlist_node node_t;
typedef struct dlist dlist_t;
typedef node_t *dlist_iter_t;

typedef int (*predicator_t) (const void *data, const void *seek);
typedef int (*callback_t) (void *data, void *param);



/* 	Create an new empty instance of a doubly list
	return: pointer to a pointer the list upon success
			0 - upon failure */
dlist_t *DlistCreate(void);


/*	Destroy the dlist 'list' and free all memory associated with it.
	list: pointer to a dlist.
	return: void */
void DlistDestroy(dlist_t *list);


/*	Insert a node before the node pointed to by the iterator 'location'.
	location: the exsisting node, the new data will be inserted before it.
	data: a pointer to the content that the new node will hold.
	return: an iterator to the new node
	 		list 'end' - upon failure */
dlist_iter_t DlistInsert(dlist_t *list, dlist_iter_t location, void *data);


/*	Remove a node 'location'.
	location: the requested node to remove.
	return: an iterator to the next node */
dlist_iter_t DlistRemove(dlist_iter_t location);


/*	Push a new node containing 'data' to the end of the dlist 'list'
 	list: a doubly linked list.
	data: the content of the new node.
	return: an iterator to the inserted node
				list 'end' if failed */
dlist_iter_t DlistPushBack(dlist_t *list, void *data);


/*	Push a new node containing 'data' to the begining of the dlist 'list'.
 	list: a doubly linked list.
	data: the content of the new node to be added to the begining of the list.
	return: an iterator to the last node
				list 'end' if failed */
dlist_iter_t DlistPushFront(dlist_t *list, void *data);


/*	Remove the last node from the doubly list 'list'.
	list: a doubly linked list.
	return: a pointer to the data that was contained in the last item */
void *DlistPopBack(dlist_t *list);


/*	Remove the first node from the doubly linked list 'list'.
	list: a doubly linked list.
	return: a pointer to the data that was contained in the first item */
void *DlistPopFront(dlist_t *list);


/*	Counts the number of nodes in doubly linked list 'list'
	list: doubly linked list.
	return: the number of nodes in doubly linked list 'list'*/
size_t DlistSize(const dlist_t *list);


/*	Check if the doubly linked list 'list' is empty
   	list: a doubly linked list.
	return: 1 - is empty. 0 is not empty */
int DlistIsEmpty(const dlist_t *list);


/*	Returns a data from a node.
	iter: an iterator to the node.
	return: a pointer to the data contained in the node pointed by 'iter' */
void *DlistGetData(const dlist_iter_t iter);


/*	Applying function 'callback' (with 'param') to each node in a range,
		or until falied
 	first: an iterator pointing to the first node to perform the function.
	last:  an iterator pointing to the out of range node.
	callback : pointer to the function.
 	param : parameter to be used by the function.
 	return : return exist status.
 			0 - success or returns the error number with param
 			will point to the  location of the failed node  */
int DlistForEach(dlist_iter_t first, const dlist_iter_t last, callback_t callback, void *param);


/*	Return the first node in the 'list'.
	list: a doubly linked list.
	return: an iterator to the first node in the list */
dlist_iter_t DlistBegin(const dlist_t *list);


/*	Return the out of range node in the 'list'.
	list: a doubly linked list.
	return: an iterator to the end node in the list */
dlist_iter_t DlistEnd(const dlist_t *list);


/* Find a node within a range according to criteria
	first : the start node to preform the search on.
	last: the out of range node.
	ptr_func : pointer to the comparison function (see typedef)
	data_cmp : the data sent to the function to search
	return : return an iterator to the node containing the data
			or to the out of range node if not found in the range*/
dlist_iter_t DlistFind(dlist_iter_t first, dlist_iter_t last, predicator_t ptr_func,
				const void *data_cmp);


/*	Check whether two iterators points to the same object.
	iter_a: first iterator.
	iter_b: second iterator.
	return: 1 - if they are the same.
	 		0 - if they are not the same */
int DlistIsSameIter(dlist_iter_t iter_a, dlist_iter_t iter_b);


/*	Splice two doubly linked list together.
 	location: an iterator to the node before the action begins.
	from: the begining range of the nodes to splice.
	to: the out of range node.
	return: an iterator to the last copied node*/
dlist_iter_t DlistSplice(dlist_iter_t location, dlist_iter_t from, dlist_iter_t to);


/*	traverse forward on the list.
	iter: an iterator to the location in the doubly list.
	return: an iterator to the next node  */
dlist_iter_t DlistNext(const dlist_iter_t iter);


/*	traverse backwards on the list.
	iter: an iterator to the location in the doubly list.
	return: an iterator to the previous node  */
dlist_iter_t DlistPrev(const dlist_iter_t iter);

#endif /*__DLIST_H__*/
