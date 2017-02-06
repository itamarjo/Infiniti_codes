/*
 * heap.c
 *
 *  Created on: Sep 8, 2016
 *      Author: pop
 */

#ifndef HEAP_H
#define HEAP_H

#include <stdlib.h>
#include "dyn_vec.h"

typedef struct heap heap_t;
typedef int (*is_before_t)(const void *data_a, const void *data_b, void *param);
typedef int (*callback_t) (void *data, void *params);
typedef int (*is_match_t) (const void *data_a,const void *data_b);




heap_t *HeapCreate(is_before_t is_before, size_t element_size, void *param);

void HeapDestroy(heap_t *heap);

int HeapPush(heap_t *heap, void *data);

int HeapPop(heap_t *heap);

/* remove the root - the max number in the heap */
int HeapRemove(heap_t *heap, is_match_t is_match, const void *data, void *result);

void *HeapPeek(heap_t *heap);

size_t HeapSize(heap_t *heap);

int HeapIsEmpty(heap_t *heap);

void *HeapFind(heap_t *heap, void *data);

int HeapForEach(heap_t *heap, callback_t callback, void *params);



#endif /* HEAP_H */
