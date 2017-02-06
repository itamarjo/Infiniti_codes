#ifndef __SORTED_LIST_H__
#define __SORTED_LIST_H__

#include <stddef.h> /* for size_t*/
typedef struct sl sl_t;
typedef struct
{
	struct sl_info *info;
}sl_iter_t;

/* 	return: Null if allocation memory fails.
	complexity: O(1) */
sl_t *SLCreate(int (*is_before) (const void *data_a, const void *data_b, void *params),
void *params);

/*  complexity: O(n) */
void SLDestroy(sl_t *list);

/*	return: an iterator to the new item,
	list 'end' - if allocation memory fails.
	complexity: O(n) */
sl_iter_t SLInsert(sl_t *list, void *data);

/*	return: an iterator to the next item.
	IMPORTANT: passing 'end' as a parameter will result in undefined behaviour
	complexity: O(1) */
sl_iter_t SLRemove(sl_iter_t location);

/*	return: a pointer to the data that was contained in the last item 
	IMPORTANT: using on empty list result in undefined behaviour.
 	complexity: O(1)*/
void *SLPopTail(sl_t *list);


/*	return: a pointer to the data that was contained in the first item
	IMPORTANT: using on empty list result in undefined behaviour.
	complexity: O(1) */
void *SLPopHead(sl_t *list);

/*	return: an iterator to the first item in the list, end on an empy list
	complexity: O(1)*/
sl_iter_t SLBegin(sl_t *list);


/*	return: an "ilegal" iterator that should only be used with "is_same_iter"
	complexity: O(1)*/
sl_iter_t SLEnd(sl_t *list);

/*	return: an iterator to the next item 
	IMPORTANT: using on 'end' will result in undefined behaviour. 
	complexity: O(1)*/
sl_iter_t SLNext(sl_iter_t iter);


/*	return: an iterator to the previous item 
	IMPORTANT: using on 'head' will result in undefined behaviour. 
	complexity: O(1) */
sl_iter_t SLPrev(sl_iter_t iter);

/*  return : an iterator to the item containing the data
	or to the out of range item if not found in the range
	complexity: O(n)*/
sl_iter_t SLFind(sl_t *list, const void *data_cmp);

/*  return : an iterator to the item containing the data
	or to the out of range item if not found in the range
	complexity: O(n) */
sl_iter_t SLFindIf(sl_t *list, 
				int (*is_match) (const void *data, void *params), void *params);
				
/*  return : 0 if callback was well implemented on all elements.
	if callback returns non zero, the operation will stop and that value will return.
 	complexity: O(n)*/
int SLForEach(sl_iter_t from, sl_iter_t to,
  int (*callback) (const void *data, void *params), void *params);
				
/*	return: a pointer to the data contained in the item pointed by 'iter' 
	IMPORTANT: using on 'end' will result in undefined behaviour. 
	complexity: O(1)*/
void *SLGetData(sl_iter_t iter);		

/*	return: 1 - if they are the same. 0 - if they are not the same. 
	complexity: O(1)*/
int SLIsSameIter(sl_iter_t iter_a, sl_iter_t iter_b);	

/*	return: 1 - is empty. 0 is not empty 
	complexity: O(1)*/
int SLIsEmpty(const sl_t *list);

/*	return: the number of items in sorted doubly linked list 'list'
	complexity: O(n)*/
size_t SLSize(const sl_t *list);

/*	merges two linked lists with identical comparison functions. 
	undefined behaviour if they are not identical.
	note: src will be an empty list after the merging.
	complexity: O(n) */
void SLMerge(sl_t *dest, sl_t *src);




#endif /*__SORTED_LIST_H__*/
