/*
 * sort.c
 *
 *  Created on: Aug 17, 2016
 *      Author: radio
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "stack.h"
#include "sort.h"

int insert_sort (void *base,
				size_t num_of_elem,
				size_t element_size,
				int (*comp) (const void *a, const void *b))
{
	int i = 0;

	void *current = (char *)base + element_size;
	void *scanptr = base;
	void *tmp = (void *)malloc(element_size);

	if (0 == tmp)
	{
		return(1);
	}


	for (i = 1; i < num_of_elem; ++i)
	{

		/* if scanptr > current and the search is not at the beginning
		 * notice that comp need to be before the check if we reach the base
		 *  * int (*comp) (const void *a, const void *b) - a comparison function
 * 									which returns: < 0 when  a < b
 * 												    0  when  a == b
 * 												   > 0 when  a > b */
		while ((comp(scanptr, current) > 0) && (scanptr != base))
		{
			scanptr = (char *)scanptr - element_size;
		}
		if ((comp(scanptr, current)) < 0)
		{
			scanptr = (char *)scanptr + element_size;
		}
		memcpy(tmp, current, element_size);
		memmove((char *)scanptr + element_size, scanptr, (char *)current - (char *)scanptr);
		memcpy(scanptr,tmp, element_size);

		scanptr = current;
		current = (char *)current + element_size;

	}
	free(tmp);

	return(0);
}

void swap(void *a, void *b, void *tmp,  size_t element_size)
{
	memcpy(tmp, a, element_size);
	memcpy(a, b, element_size);
	memcpy(b, tmp, element_size);
}

int select_sort (void *base,
				size_t num_of_elem,
				size_t element_size,
				int (*comp) (const void *a, const void *b))
{
	void *min = base;
	void *current = (char *)(base + element_size);
	void *sorted_ptr = base;
	void * tmp = malloc(element_size);
		if (0 == tmp)
		{
			return (1);
		}

	while (sorted_ptr != (char *)(base + (element_size * num_of_elem)) )
	{
		while (current != (char *)(base + (element_size * num_of_elem)))
		{
			if(comp(min, current) > 0)
			{
				min = current;
			}
			current = (char *)(current + element_size);
		}
		swap(min, sorted_ptr, tmp, element_size);
		sorted_ptr = (char *)(sorted_ptr + element_size);
		current = sorted_ptr;
		min = sorted_ptr;
	}

	free(tmp);

	return(0);
}



int bubble_sort (void *base,
				size_t num_of_elem,
				size_t element_size,
				int (*comp) (const void *a, const void *b))
{
	int is_sorted = 0;
	void *scn_l = base;
	void *scn_r = (char *)(base + element_size);
	void * tmp = malloc(element_size);
	if (0 == tmp)
	{
		return (1);
	}

	while (is_sorted == 0)
	{
		is_sorted = 1;
		while (scn_r != (char *)(base + (element_size * num_of_elem)))
		{

			if (comp(scn_l, scn_r) > 0)
			{
				is_sorted = 0;
				swap(scn_l, scn_r, tmp, element_size);

			}
			scn_l = scn_r;
			scn_r = (char *)(scn_r + element_size);
		}
		scn_l = base;
		scn_r = (char *)(base + element_size);
	}

	free(tmp);

	return (0);
}

int counting_sort(void *base, size_t element_size, size_t num_of_element,
					size_t(*key_to_num)(const void *data, const void *args), const void *args, size_t range)
{
	long i = 0;

	void *sorted = 0;
	size_t *count = (size_t *)calloc(range , sizeof(size_t));

	if (0 == count)
	{
		return(1);
	}

	sorted = malloc(num_of_element * element_size);

	if (0 == sorted)
	{
		free(count);
		return(1);
	}


	for (i = 0; i < num_of_element; ++i)
	{
		++count[key_to_num((char *)base + (i * element_size), args)];
	}

	for (i = 1; i < range; ++i)
	{
		count[i] += count[i - 1];

	}

	for (i = (num_of_element - 1); i >= 0 ; --i)
	{
		--count[key_to_num((char *)base + (i * element_size), args)];
		memcpy((char *)(sorted + (count[key_to_num((char *)base + (i * element_size), args)] * element_size)) ,
					  (char *)base + (i * element_size), element_size);
	}

	memcpy(base, sorted, element_size * num_of_element);

	free(count);
	free(sorted);

	return(0);
}

int RadixSort(void *base,
				size_t num_of_numbers,
				size_t element_size,
				size_t(*key_to_num)(const void *data, const void *arge),
				void *args,
				size_t num_of_bytes)
{
	int i = 0;

	for (i = 0; i < (num_of_bytes * 2); ++i)
	{
		if (counting_sort(base, element_size, num_of_numbers, key_to_num, (void *)&i, 16) == 1)
		{
			return(1);
		}
	}

	return(0);
}

int InsertStack(stack_t *stack, const void *data, comp_func_t comp_func)
{
	void *tmp;

	if ( (StackPeek(stack) == NULL) || (comp_func(data, *(void **)StackPeek(stack)) >= 0 ) )
	{
		return(StackPush(stack, &data) != 0);

	}
	else
	{
		tmp = *(void **)StackPeek(stack);
		StackPop(stack);
		InsertStack(stack,data, comp_func);
		StackPush(stack, &tmp);
	}

	return(0);
}

int SortStack(stack_t *stack, comp_func_t comp_func)
{
	void *tmp;

	if(StackSize(stack) > 0)
	{
		tmp = *(void **)(StackPeek(stack));
		StackPop(stack);
		SortStack(stack, comp_func);
		return(InsertStack(stack, tmp, comp_func));
	}

	return(0);
}

int RecMerge(void *list1, void *buffer, size_t nmemb, size_t size, int(*compar)(const void *a, const void *b) )
{
	void *list2 = 0;

	void *endlist2 = 0;

	if ((nmemb == 1) || (nmemb == 0))
	{
		return(0);
	}

	list2 = (char *)list1 + ((nmemb - (nmemb / 2)) * size);
	RecMerge(list1, buffer, nmemb - (nmemb / 2), size, compar);
	RecMerge(list2, buffer, nmemb/2, size, compar);

	endlist2 = (char *)list1 + (nmemb * size);


	while ((list2 < endlist2) && (list1 < list2))
	{
		if (compar(list1, list2) > 0 )
		{
			memcpy(buffer, list2, size);
			memmove((char *)list1 + size , list1, (char *) list2 -(char *) list1);
			memcpy(list1, buffer, size);
			list2 = (char*)list2 + size;
		}
		list1 = (char *)list1 +size;

	}

	return(0);
}


int MergeSort(void *base, size_t nmemb, size_t size, int(*compar)(const void *a, const void *b))
{
	void *buffer = malloc(size);
	if ((0 == buffer) || RecMerge(base, buffer, nmemb, size, compar))
	{
		return(1);
	}
	free(buffer);

	return(0);
}


void RecQuick (	  void *base,
			  	  size_t nmemb,
				  size_t element_size,
				  int (*comp) (const void *a, const void *b),
				  void *buffer)    /*(int arr[], int left, int right) */
{
	char *left = (char *)base;
	char *right = (char*)base + ((nmemb-1) * element_size) ;
	char *pivot = left + ((nmemb / 2) * element_size);
	while (left < right)
	{
		while(comp(left, pivot) < 0) /*&& ((left + element_size) != pivot))*/
		{
			left += element_size;
		}
		while(comp(right, pivot) > 0) /*&& ((right - element_size) != pivot)) */
		{
			right -= element_size;
		}
		if (left < right)
		{
			memcpy(buffer, left, element_size);
			memcpy(left, right, element_size);
			memcpy(right, buffer, element_size);
			if (pivot == right)
			{
				pivot = left;
				right -= element_size;
			}
			else if (pivot == left)
			{
				pivot = right;
				left += element_size;
			}
			else
			{
				left += element_size;
				right -= element_size;
			}
		}
	}
	if ((char *)base < right)
	{
		RecQuick(base, (pivot - (char *)base)/ element_size , element_size, comp, buffer);
	}
	if (left < (char *)base + ((nmemb - 1) * element_size))
	{
		RecQuick(pivot + element_size, (((char *)base + ((nmemb - 1) * element_size) - pivot))/ element_size, element_size, comp, buffer);
	}
}


int QuickSort (void *base,
			size_t nmemb,
			size_t element_size,
			int (*comp) (const void *a, const void *b))
{
	void *buffer = malloc(element_size);
	if (0 == buffer)
	{
		return(1);
	}

	RecQuick(base, nmemb, element_size, comp, buffer);

	return(0);

}

void quickSort(int arr[], int left, int right) {
      int i = left, j = right;
      int tmp;
      int pivot = arr[(left + right) / 2];
      /* partition */
      while (i <= j) {
            while (arr[i] < pivot)
                  i++;
            while (arr[j] > pivot)
                  j--;
            if (i <= j) {
                  tmp = arr[i];
                  arr[i] = arr[j];
                  arr[j] = tmp;
                  i++;
                  j--;
            }
      };
      /* recursion */
      if (left < j)
            quickSort(arr, left, j);
      if (i < right)
            quickSort(arr, i, right);
}

void Heapify(char *base, size_t i, size_t elem_size, comp_func_t compar, size_t nmemb, void *buffer)
{
	if(((2 * i + 1) ) >= nmemb)
	{
		return;
	}

	if((2 * i + 1) == (nmemb-1))
	{
		if (compar(base + (i * elem_size), base + ((2 * i + 1) * elem_size)) < 0 )
		{
			swap(base + (i * elem_size), base + ((2 * i + 1) * elem_size), buffer, elem_size);
		}
		return;
	}

	if ((compar(base + (i * elem_size), base + ((2 * i + 1) * elem_size)) < 0 ) &&
			(compar(base + (((2 * i) + 2) * elem_size), base + ((2 * i + 1) * elem_size)) < 0))
	{
		swap(base + (i * elem_size), base +(((2 * i + 1) * elem_size)), buffer, elem_size);
		Heapify(base, (2 * i) + 1, elem_size, compar, nmemb, buffer);
	}

	if ((compar(base +(i * elem_size), base + (((2 * i + 2) * elem_size) )) < 0 ) &&
					(compar(base+((2 * i + 1) * elem_size), base + ((2 * i + 2) * elem_size)) < 0))
	{
		swap(base + (i * elem_size), base +(((2 * i + 2) * elem_size)), buffer, elem_size);
		Heapify(base, (2 * i) + 2, elem_size, compar, nmemb, buffer);
	}

}


int HeapSort(void *base, size_t nmemb, size_t elem_size, int (*compar)(const void *a, const void *b))
{

	int parent = (nmemb / 2) -1  ;
	int i = 0;
	void *buffer = malloc(elem_size);
	if (0 == buffer)
	{
		return(1);
	}

	for (i = parent; i >= 0; --i)
	{
		Heapify(base, i, elem_size, compar, nmemb, buffer);
	}

	for (i = (nmemb -1); i > 0; --i)
	{
		swap(base, (char *)base + (i * elem_size), buffer, elem_size);
		Heapify(base, 0, elem_size, compar, nmemb, buffer);
	}
	free(buffer);

	return(0);
}

