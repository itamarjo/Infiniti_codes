/*
 * sort.h
 *
 *  Created on: Aug 17, 2016
 *      Author: radio
 */
#ifndef __SORT_H__
#define __SORT_H__

#include <stddef.h> /* size_t */
#include "stack.h"

typedef  int (*comp_func_t)(const void *a, const void*b);


/* sorting functions input params:
 * base - an array to sort
 * num_of_elem - number of elements
 * element_size - size of each element (in bytes)
 * int (*comp) (const void *a, const void *b) - a comparison function
 * 									which returns: < 0 when  a < b
 * 												    0  when  a == b
 * 												   > 0 when  a > b
 */

int insert_sort (void *base,
				size_t num_of_elem,
				size_t element_size,
				int (*comp) (const void *a, const void *b));

int select_sort (void *base,
				size_t num_of_elem,
				size_t element_size,
				int (*comp) (const void *a, const void *b));

int bubble_sort (void *base,
				size_t num_of_elem,
				size_t element_size,
				int (*comp) (const void *a, const void *b));

int counting_sort (void *base,
					size_t elem_size,
					size_t num_of_elements,
					size_t(*key_to_num)(const void *data, const void *arge),
					const void *arge,
					size_t range);

int RadixSort(void *base,
		size_t num_of_numbers,
		size_t element_size,
		size_t(*key_to_num)(const void *data, const void *arge),
		void *args,
		size_t num_of_bytes);

int SortStack(stack_t *stack,comp_func_t comp_func);

int InsertStack(stack_t *stack, const void *data, comp_func_t comp_func);

int MergeSort(void *base, size_t nmemb, size_t size, int(*compar)(const void *a, const void *b));

int QuickSort(	void *base,
				size_t nmemb,
				size_t element_size,
				int (*comp) (const void *a, const void *b));


#endif /*__SORT_H__*/



