/*
 * hash.c
 *
 *  Created on: Sep 7, 2016
 *      Author: pop
 */

#include <stddef.h>
#include <assert.h>
#include <stdlib.h>
#include "hash.h"
#include "dlist.h"

struct hash_table
{
	dlist_t **table_base;
	hash_func_t hash_func;
	is_match_t is_match;
	size_t table_size;
};

typedef size_t (*hash_func_t) (void *data);

typedef int (*is_match_t) (const void *data1, const void *data2);
typedef int (*callback_t) (void *data, void *param);


hash_table_t *HashCreate (hash_func_t hash_func,
							size_t table_size,
							is_match_t is_match)
{
	hash_table_t *hash = (hash_table_t *)malloc(sizeof(hash_table_t));
	if (0 == hash)
	{
		return(0);
	}

	hash->table_base = (dlist_t **)calloc(table_size, sizeof(dlist_t *));
	if (0 == hash->table_base)
	{
		free(hash);
		return(0);
	}

	hash->table_size = table_size;
	hash->hash_func = hash_func;
	hash->is_match = is_match;

	return(hash);
}

void HashDestroy (hash_table_t *hash)
{
	dlist_t **dlist_scn = hash->table_base;
	size_t i = 0;
	for (i =0; i < hash->table_size; ++i)
	{
		if (*dlist_scn != 0)
		{
			DlistDestroy(*dlist_scn);
		}
		++dlist_scn;
	}
	free(hash);

}

/* returns 0 on success, 1 on failure */
int HashInsert (hash_table_t *hash, void *data)
{
	assert(hash != 0);
	assert(data != 0);

	if (0 == hash->table_base[hash->hash_func(data)])
	{
		hash->table_base[hash->hash_func(data)] = DlistCreate();
		if (0 == hash->table_base[hash->hash_func(data)])
		{
			return(1);
		}
	}
	if (DlistIsSameIter(DlistPushBack(hash->table_base[hash->hash_func(data)], data),
			DlistEnd(hash->table_base[hash->hash_func(data)])))
	{
		return(1);
	}

	return(0);
}

void HashRemove (hash_table_t *hash, void *data)
{
	dlist_t *dlist = 0;
	dlist_iter_t find = 0;
	assert(hash != 0);
	assert(data != 0);

	dlist = hash->table_base[hash->hash_func(data)];
	if (dlist != 0)
	{
		find = DlistFind(DlistBegin(dlist), DlistEnd(dlist), hash->is_match, data);
		if (DlistIsSameIter(DlistEnd(dlist), find) != 1)
		{
			DlistRemove(find);
		}
	}
}

size_t HashSize (hash_table_t *hash)
{
	dlist_t **dlist_scn = 0;
	size_t i = 0;
	size_t total = 0;

	assert(hash != 0);

	dlist_scn = hash->table_base;

	for (i =0; i < hash->table_size; ++i)
	{
		if (*dlist_scn != 0)
		{
			total += DlistSize(*dlist_scn);
		}
		++dlist_scn;
	}

	return(total);
}

int HashIsEmpty (hash_table_t *hash)
{
	dlist_t **dlist_scn = 0;
	size_t i = 0;

	assert(hash != 0);

	dlist_scn = hash->table_base;

	for (i =0; i < hash->table_size; ++i)
	{
		if (*dlist_scn != 0)
		{
			if(DlistIsEmpty(*dlist_scn) != 1)
			{
				return(0);
			}
		}
		++dlist_scn;
	}

	return(1);
}

/* returns data */
void *HashFind (hash_table_t *hash, void *data)
{
	dlist_t *dlist = 0;

	assert(hash != 0);
	assert(data != 0);

	dlist = hash->table_base[hash->hash_func(data)];
	if (0 == dlist)
	{
		return(0);
	}
	return(DlistGetData(DlistFind(DlistBegin(dlist), DlistEnd(dlist), hash->is_match, data)));
}

/* returns 0 on success, 1 on failure */
int HashForEach (hash_table_t *hash, callback_t callback, void *params)
{
	dlist_t **dlist_scn = 0;
	size_t i = 0;

	assert(hash != 0);

	dlist_scn = hash->table_base;

	for (i =0; i < hash->table_size; ++i)
	{
		if (*dlist_scn != 0)
		{
			if (0 != DlistForEach(DlistBegin(*dlist_scn), DlistEnd(*dlist_scn), callback, params))
			{
				return(1);
			}
		}
		++dlist_scn;
	}

	return(0);
}
