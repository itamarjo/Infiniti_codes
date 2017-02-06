/*
 * heap.c
 *
 *  Created on: Sep 8, 2016
 *      Author: pop
 */

#include <assert.h>
#include <string.h>
#include "heap.h"


struct heap
{
	dyn_vec_t *vec;
	is_before_t is_before;
	size_t elm_sz;
	void *param;
};

static int swap(void *data_a, void * data_b, size_t elm_sz)
{
	void *buffer = malloc(elm_sz);
	if (0 == buffer)
	{
		return(1);

	}
	memcpy(buffer, data_a, elm_sz);
	memcpy(data_a, data_b, elm_sz);
	memcpy(data_b, buffer, elm_sz);

	return(0);
}

heap_t *HeapCreate(is_before_t is_before, size_t element_size,  void *param)
{
	heap_t *heap = 0;

	assert(is_before != 0);
	assert(element_size != 0);

	heap = (heap_t *)malloc(sizeof(heap_t));
	if (0 == heap)
	{
		return(0);
	}
	heap->vec = DynVecCreate(element_size);
	if (0 == heap->vec)
	{
		return(0);
	}
	heap->is_before = is_before;
	heap->elm_sz = element_size;
	heap->param = param;

	return(heap);

}

void HeapDestroy(heap_t *heap)
{
	assert(heap != 0);

	DynVecDestroy(heap->vec);

	free(heap);
}

static int PushUp(heap_t *heap, size_t leaf_location)
{
	void *data = DynVecGetItemAddress(heap->vec, leaf_location);
	void *parent = DynVecGetItemAddress(heap->vec, (leaf_location - 1)/2);
	int parent_location = (leaf_location - 1 ) / 2;

	while (( parent_location >= 0) && (heap->is_before(data, parent, heap->param)))
	{
		if (1 == swap(data, parent, heap->elm_sz))
		{
			return(1);
		}
		leaf_location = parent_location;
		parent_location = (leaf_location -1) / 2;
		data = parent;

		parent = DynVecGetItemAddress(heap->vec, parent_location);
	}

	return(0);
}

int HeapPush(heap_t *heap, void *data)
{
	assert(heap != 0);
	assert(data != 0);

	if (1 == DynVecPushBack(heap->vec, data))
	{
		return(1);
	}

	 if (1 == PushUp(heap, DynVecSize(heap->vec) - 1))
	 {
	 return(1);
	 }

	return(0);
}



static int Heapify(heap_t *heap, size_t idx)
{
	if(DynVecSize(heap->vec)< ((2 * idx) + 1))
	{
		return (0);
	}

	/*if ((0 == (DynVecSize(heap->vec) % 2) && (((2 * idx) + 1) == (DynVecSize(heap->vec) -1)) && (heap->is_before(DynVecGetItemAddress(heap->vec, (2 * idx) + 1), DynVecGetItemAddress (heap->vec, idx)))))*/
	if (((2 * idx) + 1) == (DynVecSize(heap->vec) -1))
	{
		if (heap->is_before(DynVecGetItemAddress(heap->vec, (2 * idx) + 1), DynVecGetItemAddress (heap->vec, idx), heap->param))
		{
			if (1 == swap(DynVecGetItemAddress(heap->vec, (2 * idx) + 1) ,DynVecGetItemAddress(heap->vec, idx), heap->elm_sz))
			{
				return(1);
			}
		}
		return(0);
	}

	if ((heap->is_before(DynVecGetItemAddress(heap->vec, (2 * idx) + 1), DynVecGetItemAddress (heap->vec, idx), heap->param)) &&
			(heap->is_before(DynVecGetItemAddress(heap->vec, (2 * idx) + 1), DynVecGetItemAddress(heap->vec, (2 * idx) + 2), heap->param)))
	{
		if (1 == swap(DynVecGetItemAddress(heap->vec, (2 * idx) + 1) ,DynVecGetItemAddress(heap->vec, idx), heap->elm_sz))
		{
			return(1);
		}
		return(Heapify(heap, (2 * idx) + 1));
	}
	else if ((heap->is_before(DynVecGetItemAddress(heap->vec, (2 * idx) + 2) ,DynVecGetItemAddress(heap->vec, idx), heap->param)) &&
			(heap->is_before(DynVecGetItemAddress(heap->vec, (2 * idx) + 2) ,DynVecGetItemAddress(heap->vec,(2 * idx) + 1),heap->param)))
	{
		if (1 == swap(DynVecGetItemAddress(heap->vec, (2 * idx) + 2) ,DynVecGetItemAddress(heap->vec, idx), heap->elm_sz))
		{
			return(1);
		}
		return(Heapify(heap, (2 * idx) + 2));
	}

	return(0);
}

/* remove the root - the max number in the heap */
int HeapPop(heap_t *heap)
{
	assert(heap != 0);

	if(DynVecSize(heap->vec) == 0)
	{
		return(1);
	}

	/* swap the top to the heap with the last item in the array ; */
	if (1 == swap(DynVecGetItemAddress(heap->vec, 0), DynVecGetItemAddress(heap->vec, DynVecSize(heap->vec) -1), heap->elm_sz))
	{
		return(1);
	}

	DynVecPopBack(heap->vec);

	if (1 == Heapify(heap, 0))
	{
		return(1);
	}

	return(0);
}


int HeapRemove(heap_t *heap, is_match_t is_match, const void *data, void *result)
{
	int i = 0;
	while (is_match(DynVecGetItemAddress(heap->vec, i), data) != 1)
	{
		++i;
	}

	memcpy(result, DynVecGetItemAddress(heap->vec, i), heap->elm_sz);

	if (1 == swap(DynVecGetItemAddress(heap->vec, i), DynVecGetItemAddress(heap->vec, DynVecSize(heap->vec) -1), heap->elm_sz))
	{
		return(1);
	}

	DynVecPopBack(heap->vec);

	if ((1 == Heapify(heap, i) || (1 == PushUp(heap, i))))
	{
		return(1);
	}

	return(0);
}

void *HeapPeek(heap_t *heap)
{
	assert(heap != 0);

	return(DynVecGetItemAddress(heap->vec, 0));
}

size_t HeapSize(heap_t *heap)
{
	assert(heap != 0);

	return(DynVecSize(heap->vec));
}

int HeapIsEmpty(heap_t *heap)
{
	assert(heap != 0);

	return(DynVecSize(heap->vec) == 0);
}


int HeapForEach(heap_t *heap, callback_t callback, void *params)
{
	size_t size = 0;
	size_t i = 0;

	assert(heap);

	size = DynVecSize(heap->vec);

	for(i = 0; i < size; ++i)
	{
		if (1 == callback(DynVecGetItemAddress(heap->vec, i) ,params))
		{
			return(1);
		}
	}

	return(0);
}
