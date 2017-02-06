#ifndef __PQ_H__
#define __PQ_H__

#include <stddef.h> /* size_t */

typedef struct pqueue pqueue_t;

/* return NULL on fail */
pqueue_t *PQCreate(int (*is_before) (const void *data1, const void *data2, void *param),
					void *param);

void PQDestroy(pqueue_t *queue);

/* return 0 on succses or 1 if fail */
int PQEnqueue(pqueue_t *queue, void *data);

/* return 0 on succses or 1 if fail (i.e. queue is empty ) */
int PQDequeue(pqueue_t *queue);

/* Remove the data according to different priority criteria
return the data from the removed item or NULL if not found */
void *PQRemove(pqueue_t *queue,
				int (*is_match) (const void *data, void *param),
				void *param);

/* make the queue empty */
void PQClear(pqueue_t *queue);

size_t PQSize(const pqueue_t *queue);

/* return 1 if empty or 0 if full */
int PQIsEmpty(const pqueue_t *queue);

void *PQPeek(const pqueue_t *queue);

#endif /*__PQ_H__*/
