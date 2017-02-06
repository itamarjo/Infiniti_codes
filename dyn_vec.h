#ifndef __DYN_VEC_H__
#define __DYN_VEC_H__

typedef struct dyn_vec dyn_vec_t;


/* Create a new instance of dynamic vector
	elem_size : size of elements in a vector
	return: pointer to the new vector upon success
			0 - upon failure
*/
dyn_vec_t* DynVecCreate(size_t elem_sz);


/* Destroy an instance of the vector 'vec'
	vec : the vector
*/
void DynVecDestroy(dyn_vec_t *vec);


/*	Add a new elemented at the end of 'vec'
	and contents of 'elem' is copied into it
	vec : the vector
	elem : the new element to add
	return : 0 - upon success, 1 - upon failure
*/
int DynVecPushBack(dyn_vec_t *vec, const void *elem);


/* Delete last element in the vector 'vec'
	vec : the vector
*/
void DynVecPopBack(dyn_vec_t *vec);


/* Give access to element of 'vec' at index 'idx'
	vec : the vector
	idx : index of element
	return: pointer to the element at index 'idx'
*/
void* DynVecGetItemAddress(dyn_vec_t *vec, size_t idx);


/* Size of the vector 'vec'
	vec : the vector
	return: number of elements in 'vec'
*/
size_t DynVecSize(dyn_vec_t *vec);


/* Capacity of the vector 'vec'
	vec : the vector
	return: capacity of the vector 'vec'
*/
size_t DynVecCapacity(dyn_vec_t *vec);


/* Reserve storage cpacity for the vector 'vec'
	vec : the vector
	cap : desired capacity
	return : 0 - upon success, 1 - upon failure
*/
int DynVecReserve(dyn_vec_t *vec, size_t cap);





#endif /*__DYN_VEC_H__*/
