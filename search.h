#ifndef __SEARCH_H__
#define __SEARCH_H__
#include <stdint.h>

void *BinarySearch(const void *base, size_t nmemb, size_t elem_size,
		const void *req_data, int (*compar)(const void *a, const void *b));

void *RecBinarySearch(const void *base, size_t nmemb, size_t elem_size,
		const void *req_data, int (*compar)(const void *a, const void *b));

void *JumpSearch(const void *base, size_t nmemb, size_t elem_size,
		const void *req_data, int (*compar)(const void *a, const void *b));





#endif /*__SEARCH_H__*/
