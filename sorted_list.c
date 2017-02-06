#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "dlist.h"
#include "sorted_list.h"

struct sl
{
	dlist_t *dlist;
	int (*is_before) (const void *data_a, const void *data_b, void *params);
	void *params;
};

struct pack
{
	void *seek;
	sl_t *sl_list;
};

/* 	return: Null if allocation memory fails.
	complexity: O(1) */
sl_t *SLCreate(int (*is_before) (const void *data_a, const void *data_b, void *params),
		void *params)
{
	sl_t *new = 0;

	assert(is_before != 0);

	new = (sl_t *)malloc(sizeof(sl_t));
	/*checking if the malloc failed */
	if (new != 0)
	{
		new->dlist = DlistCreate();
		/*checking if the malloc for dlist falied */
		if (new->dlist != 0)
		{
			new->is_before = is_before; /*populating the managment struct */
			new->params = params; /*populating the managment struct */
		}
		else
		{
			free(new);
			new = 0; /* if allocation falied */
		}
	}
	return (new);
}

/*  complexity: O(n) */
void SLDestroy(sl_t *list)
{
	assert(list != 0);

	DlistDestroy(list->dlist);

	free(list);
}

/*	return: an iterator to the new item,
	list 'end' - if allocation memory fails.
	complexity: O(n) */
sl_iter_t SLInsert(sl_t *list, void *data)
{
	sl_iter_t location;

	void *params = list->params;

	assert(list != 0);
	/*first we positioning the iterator location on the begining of the list */
	location.info = (struct sl_info *)DlistBegin(list->dlist);

	/*we first checking if we didn't get to the end and then if the data of
		the new element is before the location */
	while  (( DlistIsSameIter((dlist_iter_t)location.info,
				DlistEnd(list->dlist)) == 0 )
				&&
				list->is_before( data,
				DlistGetData((dlist_iter_t)location.info), params) == 0 )
	{
		/* moving the location forward */
		location.info = (struct sl_info *)DlistNext((dlist_iter_t)location.info);
	}
	/*inserting the new element in the suitable location */
	location.info = (struct sl_info *)DlistInsert(list->dlist,
					(dlist_iter_t)location.info, data);

	return (location);
}

/*	return: an iterator to the next item.
	IMPORTANT: passing 'end' as a parameter will result in undefined behaviour
	complexity: O(1) */
sl_iter_t SLRemove(sl_iter_t location)
{
	assert (location.info != 0);
	assert(SLNext(location).info != 0);
	assert(SLPrev(location).info != 0);
	/* using the Dlistremove to remove the element */
	location.info = (struct sl_info *)DlistRemove((dlist_iter_t)location.info);


	return (location);
}



/*	return: a pointer to the data that was contained in the last item
	IMPORTANT: using on empty list result in undefined behaviour.
 	complexity: O(1)*/
void *SLPopTail(sl_t *list)
{
	assert(list != 0);
	assert(SLIsEmpty(list) != 1);
	/*using the DlistPopBack to remove the element */
	return(DlistPopBack(list->dlist));

}


/*	return: a pointer to the data that was contained in the first item
	IMPORTANT: using on empty list result in undefined behaviour.
	complexity: O(1) */
void *SLPopHead(sl_t *list)
{
	assert(list != 0);
	assert(SLIsEmpty(list) != 1);
	/* using the dlistpopfront to remove the element */
	return(DlistPopFront(list->dlist));
}

/*	return: an iterator to the first item in the list, end on an empy list
	complexity: O(1)*/
sl_iter_t SLBegin(sl_t *list)
{
	sl_iter_t begin;

	assert(list != 0);

	begin.info = (struct sl_info *)DlistBegin(list->dlist);

	return(begin);
}


/*	return: an "ilegal" iterator that should only be used with "is_same_iter"
	complexity: O(1)*/
sl_iter_t SLEnd(sl_t *list)
{
	sl_iter_t end;

	assert(list != 0);

	end.info = (struct sl_info *)DlistEnd(list->dlist);

	return(end);
}

/*	return: an iterator to the next item
	IMPORTANT: using on 'end' will result in undefined behaviour.
	complexity: O(1)*/
sl_iter_t SLNext(sl_iter_t iter)
{
	sl_iter_t next;

	assert(iter.info != 0);

	next.info = (struct sl_info *)DlistNext((dlist_iter_t)iter.info);

	return(next);
}


/*	return: an iterator to the previous item
	IMPORTANT: using on 'head' will result in undefined behaviour.
	complexity: O(1) */
sl_iter_t SLPrev(sl_iter_t iter)
{
	sl_iter_t prev;

	assert(iter.info != 0);

	prev.info = (struct sl_info *)DlistPrev((dlist_iter_t)iter.info);

	return(prev);
}

static int SLUnPack(const void *data, const void *package)
{
	const struct pack *package_ptr = (const struct pack *)package;

	return(  !(*package_ptr).sl_list->is_before(data,
						package_ptr->seek, package_ptr->sl_list->params));
}



/*  return : an iterator to the item containing the data
	or to the out of range item if not found in the range
	complexity: O(n)*/
sl_iter_t SLFind(sl_t *list, const void *data_cmp)
{
	sl_iter_t ret = {0};
	struct pack packed_data;

	assert(list != 0);

	packed_data.seek = (void *) data_cmp;
	packed_data.sl_list = list;

	ret.info = (struct sl_info *)DlistFind(DlistBegin(list->dlist),
	 			DlistEnd(list->dlist), (predicator_t)SLUnPack, &packed_data);
	if  ( (SLIsSameIter(ret, SLEnd(list))) ||
			 (list->is_before(data_cmp, SLGetData(ret), list->params)) )
	{
		return(SLEnd(list));
	}

	return(ret);
}

/*  return : an iterator to the item containing the data
	or to the out of range item if not found in the range
	complexity: O(n) */
sl_iter_t SLFindIf(sl_t *list,
				int (*is_match) (const void *data, void *params), void *params)
{
	sl_iter_t ret;

	assert(list != 0);

	ret.info = (struct sl_info *)DlistFind(DlistBegin(list->dlist),
				DlistEnd(list->dlist), (predicator_t)is_match, (const void *)params);
	return(ret);
}

/*  return : 0 if callback was well implemented on all elements.
	if callback returns non zero, the operation will stop and that value will return.
 	complexity: O(n)*/
int SLForEach(sl_iter_t from, sl_iter_t to,
  int (*callback) (const void *data, void *params), void *params)
 {
	 assert(from.info != 0);
	 assert(to.info != 0);
	 assert(callback != 0);

	 return(DlistForEach((dlist_iter_t)from.info,
	 		(dlist_iter_t)to.info, (callback_t)callback, params));
 }

/*	return: a pointer to the data contained in the item pointed by 'iter'
	IMPORTANT: using on 'end' will result in undefined behaviour.
	complexity: O(1)*/
void *SLGetData(sl_iter_t iter)
{
	assert(iter.info != 0);
	assert(SLNext(iter).info != 0);
	assert(SLPrev(iter).info != 0);

	return(DlistGetData((dlist_iter_t)iter.info));
}

/*	return: 1 - if they are the same. 0 - if they are not the same.
	complexity: O(1)*/
int SLIsSameIter(sl_iter_t iter_a, sl_iter_t iter_b)
{
	assert(iter_a.info != 0);
	assert(iter_b.info != 0);


	return(iter_a.info == iter_b.info);
}

/*	return: 1 - is empty. 0 is not empty
	complexity: O(1)*/
int SLIsEmpty(const sl_t *list)
{
	assert(list != 0);

	return(DlistIsSameIter(DlistBegin(list->dlist), DlistEnd(list->dlist)));
}

/*	return: the number of items in sorted doubly linked list 'list'
	complexity: O(n)*/
size_t SLSize(const sl_t *list)
{
	assert (list != 0);

	return(DlistSize(list->dlist));
}

/*	merges two linked lists with identical comparison functions.
	undefined behaviour if they are not identical.
	note: src will be an empty list after the merging.
	complexity: O(n) */
void SLMerge(sl_t *dest, sl_t *src)
{
	dlist_iter_t from = 0;
	dlist_iter_t to = 0;
	dlist_iter_t location =0;

	assert(dest != 0);
	assert(src != 0);
	assert(dest->is_before == src->is_before);
	assert(dest->params == src->params);

	location = DlistBegin(dest->dlist);

	while (0 == DlistIsEmpty(src->dlist) )
	{
		from = DlistBegin(src->dlist);

		while ((DlistIsSameIter(location, DlistEnd(dest->dlist)) == 0) &&
		 		(src->is_before(DlistGetData(from), DlistGetData(location),
				dest->params) == 0))
		{
			location = DlistNext(location);
		}



		if (location == DlistEnd(dest->dlist))
		{
			DlistSplice(location, from, DlistEnd(src->dlist));
		}
		else
		{
			to = DlistNext(from);
			while (( DlistIsSameIter(to, DlistEnd(src->dlist)) == 0 ) &&
					(src->is_before(DlistGetData(to), DlistGetData(location),
					dest->params) != 0))
			{
				to = DlistNext(to);
			}

			location = DlistNext(DlistSplice(location, from, to));
		}
	}
}
