#ifndef __QUEUE_H__
#define __QUEUE_H__

#include <stddef.h> /* size_t */

typedef struct queue queue_t;



/* return NULL on fail */
queue_t *QCreate(void);

void QDestroy(queue_t *queue);

/* return 0 on succses or 1 if fail*/
int QEnqueue(queue_t *queue, void *data);

/* return the data from the removed item or NULL if the list is empty */
void *QDequeue(queue_t *queue);

size_t QSize(const queue_t *queue);

/* return 1 if empty or 0 if full */
int QIsEmpty(const queue_t *queue);

void *QPeek(const queue_t *queue);

void QAppend(queue_t *source, queue_t *dest);


#endif /*__QUEUE_H__*/
