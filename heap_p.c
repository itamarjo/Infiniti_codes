#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

#include "heap.h"
#include "dyn_vec.h"

/*index of parent, left child and right child*/
#define LEFT(i) 2*(i) + 1
#define RIGHT(i) 2*(i) + 2
#define PARENT(i) ((i) - 1) / 2

struct heap
{
	dyn_vec_t *vec;
	is_before_t is_before;
	size_t elem_size;
	void *params;
};


static int Swap(void *a, void *b, size_t elem_size);
static int HpifyDown(heap_t *heap, void *data_root,int index);
static int HpifyUp(heap_t *heap, void *data, int index);

heap_t *HeapCreate(is_before_t is_before, size_t element_size, void *params)
{
	heap_t *heap = (heap_t *)malloc(sizeof(heap_t));
	if(heap == 0)
	{
		return (0);
	}
	heap->vec = DynVecCreate(element_size);
	if(heap->vec == 0)
	{
		free(heap);
		return(0);
	}

	heap->is_before = is_before;
	heap->elem_size = element_size;
	heap->params = params;
	return(heap);
}

void HeapDestroy(heap_t *heap)
{
	assert(heap != 0);

	DynVecDestroy(heap->vec);

	free(heap);
}

int HeapPush(heap_t *heap, void *data)
{

	int insert = DynVecPushBack(heap->vec, data);
	/*void *new_elem = DynVecGetItemAddress(heap->vec, HeapSize(heap)-1);*/

	if (insert == 1)
	{
		return (1);
	}
	return(HpifyUp(heap, data, 0));

	
}

static int HpifyUp(heap_t *heap, void *data, int index)
{
	int i = index;
	void *new_elem = DynVecGetItemAddress(heap->vec, HeapSize(heap)-1);
	for(i = HeapSize(heap) - 1; i  > 0 ; i = PARENT(i))
	{
		/* i/2 => parent */
		if(heap->is_before(
						DynVecGetItemAddress(heap->vec, PARENT(i)), data, heap->params) == 1 )
		{
			break;
		}
	
		if(1 == Swap(new_elem,
				DynVecGetItemAddress(heap->vec, PARENT(i)), heap->elem_size))
		{
			return(1);
		}
		new_elem = DynVecGetItemAddress(heap->vec, PARENT(i));
	}
	return(0);
}


/*swap between two elements*/
static int Swap(void *a, void *b, size_t elem_size)
{
	void *temp = malloc(elem_size);
	assert(a != 0);
	assert(b != 0);

	if (0 == temp)
	{
		return (1);
	}

	memcpy(temp, b, elem_size);
	memcpy(b, a, elem_size);
	memcpy(a, temp, elem_size);

	free(temp);

	return (0);
}


/* remove the root - the max number in the heap */
int HeapPop(heap_t *heap)
{
	void *last_elem= 0;
	void *data_root = HeapPeek(heap);
	int index = 0;

	last_elem = DynVecGetItemAddress(heap->vec, HeapSize(heap) - 1);

	if( 1 == Swap(last_elem, data_root,  heap->elem_size))
	{
		return(1);
	}


	DynVecPopBack(heap->vec);
	/*data_root = last_elem;*/

	if(1 == HpifyDown(heap, data_root, index))
	{
		return(1);
	}

	return(0);
}

static int HpifyDown(heap_t *heap, void *data_root, int i)
{
	size_t size;
	void *elem = 0;

	assert(heap != 0);

	size = HeapSize(heap);

	while( LEFT(i) <= size)
	{
		/*check if there is right child*/
		if(RIGHT(i) >= size)
		{
			elem = DynVecGetItemAddress(heap->vec, LEFT(i));
			i = LEFT(i);

		}
		else
		{
			/*check who child have the most value "before*/
			if(heap->is_before(DynVecGetItemAddress(heap->vec, LEFT(i)),
									DynVecGetItemAddress(heap->vec, RIGHT(i)), heap->params))
			{
				elem = DynVecGetItemAddress(heap->vec, LEFT(i));
				i = LEFT(i);
			}
			else
			{
				elem = DynVecGetItemAddress(heap->vec, RIGHT(i));
				i = RIGHT(i);
			}
		}
		/*check if needed to swap*/
		if (heap->is_before(elem, data_root, heap->params))
		{
			if(1 == Swap(data_root, elem, heap->elem_size))
			{
				return(1);
			}
			data_root = elem;
		}

	}
	return(0);
}


int HeapRemove(heap_t *heap, is_match_t is_match, const void *data, void *res)
{
	void *last_elem= 0;
	void *heap_data = 0;
	int i = 0;
	size_t size = 0;

	assert(heap != 0);
	assert(is_match != 0);
	assert(data != 0);

	size = DynVecSize(heap->vec);

	last_elem = DynVecGetItemAddress(heap->vec,  HeapSize(heap) - 1);

	for(i = 0; i < size ; ++i)
	{
		heap_data = DynVecGetItemAddress(heap->vec, i);
		/*search the index of the elem to remove*/
		if(is_match(heap_data, data) == 1)
		{
			/*copy the data to the out param*/
			memcpy(res, heap_data, heap->elem_size);
			if (1 == Swap(heap_data, last_elem, heap->elem_size))
			{
				return(1);
			}
			DynVecPopBack(heap->vec);
			break;
		}
	}


	if(1 == HpifyDown(heap, heap_data, i ))
	{
		return(0);
	}

	res = (void *)data;

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

void *HeapFind(heap_t *heap, void *data)
{
	size_t size =  0;
	int i = 0;
	void *heap_data = 0;

	assert(heap != 0);

	size =  HeapSize(heap);
	for(i = 0; i < size ; ++i)
	{
		heap_data = DynVecGetItemAddress(heap->vec, i);
		if(heap->is_before(data, heap_data, heap->params) == 0 &&
				(heap->is_before(heap_data, data, heap->params) == 0))
		{
			return(heap_data);
		}
	}

	return(0);
}
int HeapForEach(heap_t *heap, callback_t callback, void *params)
{
	size_t size =  0;
	int i = 0;
	int job = 0;

	assert(heap != 0);
	assert(callback != 0);

	size =  HeapSize(heap);
	for(i = 0; i < size ; ++i)
	{
		job = callback(DynVecGetItemAddress(heap->vec, i), params);
		if(job != 0)
		{
			return(job);
		}
	}
	return(job);
}
