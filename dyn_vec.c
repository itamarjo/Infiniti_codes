#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <strings.h>
#include <stddef.h>
#include "dyn_vec.h"

struct dyn_vec
{
	/* a pointer to the beginning of an array */
	void *start;
	/* element's size */
	size_t elem_sz;
	/* number of elements in a vector */
	size_t size;
	/* capacity of a vector
		a currently reserved amount of elements */
	size_t capacity;
};



/* Create a new instance of dynamic vector
	elem_size : size of elements in a vector
	return: pointer to the new vector upon success
			0 - upon failure
*/
dyn_vec_t* DynVecCreate(size_t elem_sz)
{
	/* dynamically allocate the structure itself */
	dyn_vec_t *dyn_ptr = malloc ( sizeof (dyn_vec_t) );
	if (dyn_ptr != 0) /* check for failure in the allocation */
	{
		/* if the allocation was successfull allocate the start */
		dyn_ptr->start = malloc (2 * elem_sz);
		/* check for failure in the allocation */
		if (0 == dyn_ptr->start)
		{
			free(dyn_ptr);
			return (0); /*exit if failed */
		}
		/* initialize the size, elem_sz and capacity */
		dyn_ptr-> elem_sz = elem_sz;
		dyn_ptr-> size = 0;
		dyn_ptr-> capacity = 2;
	}

	return (dyn_ptr);
}

/* Destroy an instance of the vector 'vec'
	vec : the vector
*/
void DynVecDestroy(dyn_vec_t *vec)
{
	/* release both the dynamic allocation, pay attention to the order */
	free (vec->start);
	free (vec);
}


/*	Add a new elemented at the end of 'vec'
	and contents of 'elem' is copied into it
	vec : the vector
	elem : the new element to add
	return : 0 - upon success, 1 - upon failure
*/
int DynVecPushBack(dyn_vec_t *vec, const void *elem)
{
	/* check if there is enough capacity to hold the new value */
	if ( (vec->size) + 1 > vec->capacity )
	{
		/* if not use the reserve function to allocate double the size */
		/* check for failure */
		if (1 == DynVecReserve(vec, vec-> capacity * 2))
		{
			return (1);
		}
	}
	/* copy the new value with memcpy */
	memcpy(((char*) (vec->start) + (vec->elem_sz * vec->size)), elem, vec->elem_sz);
	/* update the size */
	++(vec->size);
	return (0);

}


/* Delete last element in the vector 'vec'
	vec : the vector
*/
void DynVecPopBack(dyn_vec_t *vec)
{
	/* no need to hard delete, just to decrease the size */
	--(vec->size);
}


/* Give access to element of 'vec' at index 'idx'
	vec : the vector
	idx : index of element
	return: pointer to the element at index 'idx'
*/
void* DynVecGetItemAddress(dyn_vec_t *vec, size_t idx)
{
	/* cast to char * so we will be able to do move forward to the end of the data*/
	return ((char*)vec->start + (idx * vec->elem_sz));
}


/* Size of the vector 'vec'
	vec : the vector
	return: number of elements in 'vec'
*/
size_t DynVecSize(dyn_vec_t *vec)
{
	return (vec->size);
}


/* Capacity of the vector 'vec'
	vec : the vector
	return: capacity of the vector 'vec'
*/
size_t DynVecCapacity(dyn_vec_t *vec)
{
	return (vec->capacity);
}


/* Reserve storage cpacity for the vector 'vec'
	vec : the vector
	cap : desired capacity
	return : 0 - upon success, 1 - upon failure
*/
int DynVecReserve(dyn_vec_t *vec, size_t cap)
{
	/* check if there is a need to allocate */
	if (cap > vec->capacity)
	{
		/* realloc and use another pointer to check the result */
		void *help = realloc (vec->start, cap * vec->elem_sz);
		/* check for failure */
		if (0 == help)
		{
			return (1);
		}
		/* update the start pointer and the value of capacity */
		vec->start = help;
		vec->capacity = cap ;
	}
	return (0);
}
