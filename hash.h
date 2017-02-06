#ifndef __HASH_H__
#define __HASH_H__

#include <stddef.h>

typedef struct hash_table hash_table_t;

/*
 * hash function reads key
 *
 */

typedef size_t (*hash_func_t) (void *data);

typedef int (*is_match_t) (const void *data1, const void *data2);
typedef int (*callback_t) (void *data, void *param);


hash_table_t *HashCreate (hash_func_t hash_func,
							size_t table_size,
							is_match_t is_match);

void HashDestroy (hash_table_t *hash);

/* returns 0 on success, 1 on failure */
int HashInsert (hash_table_t *hash, void *data);

void HashRemove (hash_table_t *hash, void *data);

size_t HashSize (hash_table_t *hash);

int HashIsEmpty (hash_table_t *hash);

/* returns data */
void *HashFind (hash_table_t *hash, void *data);

/* returns 0 on success, 1 on failure */
int HashForEach (hash_table_t *hash, callback_t callback, void *params);

#endif /*__HASH_H__*/
