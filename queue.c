#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "slist.h"
#include "queue.h"

struct queue
{
	slist_node_t *head;
	slist_node_t *tail;
};


/* return NULL on fail */
queue_t *QCreate(void)
{
	/*create a new queue list */
	queue_t *new = (queue_t *)malloc(sizeof(queue_t));
	/*check allocation */
	if ( new != 0 )
	{
		/* create a new node that have 0 in Data and point to NULL */
		new->head = SListCreateAndInitNode(0,0);
		/* check allocation */
		if (new->head == 0)
		{
			/*release the managment structure */
			free(new);
			return(0);
		}
		/* both head and tail point to the dammy element */
		new->tail = new->head;
	}

	return(new);
}

void QDestroy(queue_t *queue)
{
	assert (queue != 0);

	/* free all members in the list */
	SListFreeList(queue->head);
	/* free the managment structure */
	free(queue);
}

/* return 0 on succses or 1 if fail*/
int QEnqueue(queue_t *queue, void *data)
{
	slist_node_t *new_node = 0;

	assert(queue != 0);

	/*create new node with the value Data*/
	new_node = SListCreateAndInitNode(data, 0);
	/*check for failure in the allocation */
	if (0  == new_node)
	{
		return (1);
	}
	/*insert the new node before tail */
	SListInsert(queue->tail, new_node);
	/*change location of tail to reflect the last node */
	queue->tail = queue->tail->next;

	return (0);
}

/* return the data from the removed item or NULL if the list is empty */
void *QDequeue(queue_t *queue)
{

	void *ret = 0;

	assert(queue != 0);

	ret = queue->head->data;

	/* check if the queue is empty before preforming the Dequeue */
	if(QIsEmpty(queue) == 1)
	{
		return(0);
	}
	free(SListRemove(queue->head)); /*remove the node */
	/* check if we remove the last member, if so update the tail pointer*/
	if(QIsEmpty(queue) == 1)
	{
		/*update the tail pointer */
		queue->tail = queue->head;
	}

	/*return the value of the deleted member */
	return(ret);
}

size_t QSize(const queue_t *queue)
{
	assert(queue != 0);
	assert (queue->head != 0);
	/* substracting 1 since we have a node reserve as an anchor */
	return(SListCount(queue->head) - 1);
}

/* return 1 if empty or 0 if full */
int QIsEmpty(const queue_t *queue)
{
	assert (queue != 0);
	assert (queue->head != 0);

	return(0 == queue->head->next);
}

void *QPeek(const queue_t *queue)
{
	assert(queue != 0);

	return(queue->head->data);
}

void QAppend(queue_t *dest, queue_t *source)
{
	assert(source != 0);
	assert(dest != 0);
	assert(dest != source);

	/*copy the head of source to the end of dest */
	dest->tail->data = source->head->data;
	dest->tail->next = source->head->next;
	dest->tail = source->tail;
	/*zero the values of source head and copy source tail to it */
	source->head->data = 0;
	source->head->next = 0;
	source->tail = source->head;

/*
	moving each element (complexity O(n))
	while(QIsEmpty(source) == 0)
	{
		QEnqueue(dest, QDequeue(source));
	} */
}
