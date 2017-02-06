#ifndef __S_LIST_H__
#define __S_LIST_H__
#include <stddef.h>

typedef struct slist_node slist_node_t;
typedef int (*predicator_t) ( void *node, void *seek);
typedef int (*action_t) (void *data, void *param);

struct slist_node
{
	void *data;
	slist_node_t *next;
};




/* Create a new instance of a node
	data: pointer to the data
	return: pointer to the node upon success
			0 - upon failure
*/
slist_node_t *SListCreateAndInitNode(void *data, slist_node_t *next);


/* Insert a new node before the 'where' node
	where : existing node
	new_node: a new node to Insert
	return : void
	note: this function invalidate exsisting pointers to nodes
*/
void SListInsert(slist_node_t *where, slist_node_t *new_node);


/* Insert a new node after the where node
	where : existing node
	new_node: a new node to Insert
	return : void
*/
void SListInsertAfter(slist_node_t *where, slist_node_t *new_node);


/* Remove a node
	node : node to remove
	return : the removed node
	note: this function invalidate pointers
		  it is not possible to remove last node in the list
		  the data received is not the data removed
*/
slist_node_t *SListRemove(slist_node_t *node);


/* Remove the node after the node pointed
	node : node before the node to remove
	return : the removed node
	note: it is not possible to remove the node before the last node in the list
		  the data received is not the data removed
*/
slist_node_t *SListRemoveAfter(slist_node_t *node);


/* Free the list starting with the pointed node
	node : first node to free
	return : void
*/
void SListFreeList(slist_node_t *node);


/* Count the number of nodes starting with the pointed node
	node : first node to count
	return : number of nodes
	note : cannot receive a list with a loop
*/
size_t SListCount(const slist_node_t *node);


/* Flip the list starting with the pointed node
	node : the node to start flip from
	return : pointer to the new first node
	note : cannot receive a list with a loop
*/
slist_node_t *SListFlip(slist_node_t *node);


/* Check if a node in the list is pointed by more then one node
	node : first node in the list
	return : 0 if no loop , 1 if loop
*/
int SListIsLoop(const slist_node_t *node);


/* Check if two lists share a node
	node_a : node of first list
	node_b : node of second list
	return : 0 if no intersect , 1 if intersect
	note : cannot receive a list with a loop
*/
int SListIsIntersect(const slist_node_t *node_a, const slist_node_t *node_b);


/* Find a node acording to criteria
	node : the start node
	ptr_func : pointer to the comparison function (see typedef)
	data_cmp : the data to search in all nodes
	return : return pointer to the node containing the data
			or NULL if not found
	note : cannot receive a list with a loop
*/
 slist_node_t *SListFind(const slist_node_t *node, predicator_t ptr_func,
 						 const void *data_cmp);


 /* Applining func (with the param) to each node in the list
 	node : the start node
 	ptr_func : pointer to the function
 	param : parameter to be used by the function
 	return : return pointer to the last node used in the function,
			if function failed it will not continue,
			0 if the function wass applied on all nodes in the list
	note : cannot receive a list with a loop
 */
  slist_node_t *SListForEach(slist_node_t *node, action_t ptr_func, void *param);


#endif /*__S_LIST_H__*/
