/*
 * search.c
 *
 *  Created on: Sep 13, 2016
 *      Author: pop
 */
#include <stdlib.h>
#include <math.h>


void *BinarySearch(const void *base, size_t nmemb, size_t elem_size,
		const void *req_data, int (*compar)(const void *a, const void *b))
{
	char *mid = (char *) base + ((nmemb/ 2 ) * elem_size );
	char *left = (char *) base;
	char *right = (char *)base + ((nmemb - 1) *elem_size) ;
	int res = 0;

	while (left < right)
	{
		res = (compar((void *)mid, req_data));
		if (0 == res)
		   {
			   return((void *)mid);
		   }
		   else if (res > 0)
		   {
			   right = mid - elem_size;
		   }
		   else
		   {
			   left = mid + elem_size;
		   }
		mid = left + ((((right - left) / elem_size)/ 2 ) * elem_size );
	}

	res = (compar((void *)mid, req_data));
	if (0 == res)
    {
	   return((void *)mid);
    }

	return(0);
}

void *RecBinarySearch(const void *base, size_t nmemb, size_t elem_size,
		const void *req_data, int (*compar)(const void *a, const void *b))
{
	char *mid = (char *) base + ((nmemb/ 2) * elem_size);
	char *left = (char *) base;
	char *right = (char *)base + ((nmemb - 1) *elem_size) ;
	int res = 0;


	res = (compar((void *)mid, req_data));
	if (0 == res)
	   {
		   return((void *)mid);
	   }
   else if (res > 0)
	   {
		   return(RecBinarySearch(base, (size_t)((mid - left)/ elem_size), elem_size, req_data, compar));
	   }
   else
	   {
		   return(RecBinarySearch(mid + elem_size, (right - mid ) / elem_size, elem_size, req_data, compar));
	   }

	return(0);
}

void *JumpSearch(const void *base, size_t nmemb, size_t elem_size,
		const void *req_data, int (*compar)(const void *a, const void *b))
{
	size_t index = sqrt(nmemb) - 1;
	int res = compar((char *)base + (index * elem_size), req_data);

	if (0 == res)
	{
		return((char *)base + (index * elem_size));
	}

	if(res > 0)
	{
		return(JumpSearch(base, index, elem_size, req_data, compar));
	}
	else
	{
		return(JumpSearch((char *)base + ((index + 1) * elem_size), nmemb - index -1, elem_size, req_data, compar));
	}

	return(0);


	return(base);
}



