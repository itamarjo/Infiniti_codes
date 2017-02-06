/* Sun Jun 26 17:42:21 IDT 2016 */
/* Name: Danielle Shkop */
/* Program: Hash Table */

#include <stdio.h>
#include <stdlib.h>

#include "hash.h"
#include "dlist.h"
#include <assert.h>

struct hash_table
{
	dlist_t **	table_base;
	hash_func_t hash_func;
	is_match_t 	is_match;
	size_t 		table_size;
};
size_t	table_size = 20;

struct info
{
	int		num;
	size_t	table_size;
};

void PrintHashtable (hash_table_t *hash);
int IsFound(size_t arr[], size_t size, size_t data);

int CallBack(void *data, void *param)
{
	data = (size_t)data + 1;
	return (0);
}

int IsMatch(const void *data1, const void *data2)
{
	return ((size_t)data1 != (size_t)data2);
}

size_t HashFunc(void *data)
{
	return ((size_t)data % table_size);
}

int main (int argc, char *argv[], char **envp)
{
	hash_table_t *table = 0;
	size_t arr[table_size];
	size_t arr_rmv[6] = {21, 77, 26, 86, 35};
	size_t i = 0;
	size_t size_table = 0;
	size_t size = 0;
	size_t number = 0;
	void *res = 0;
	int ret = 0;

	printf("1) ");
	puts("/*****HashCreate*****/");
	table = HashCreate(HashFunc, table_size, IsMatch);
	if (0 == table)
	{
		printf ("HashCreate failed\n");
	}
	else
	{
		puts(" OK");
	}
	puts("/*****HashIsEmpty*****/");
	if (!HashIsEmpty(table))
	{
		printf ("HashIsEmpty failed after create\n");
	}
	else
	{
		puts(" OK");
	}

	size_table = HashSize(table);
	printf("/*****HashSize:*****/\nsize (0?) = %lu\n", size_table);
	puts("/*****HashDestroy*****/");
	HashDestroy(table);
	puts(" OK");
	/****************/
	puts("");
	printf("2) ");
	puts("/*****HashCreate:*****/");
	table = HashCreate(HashFunc, table_size, IsMatch);
	if (0 == table)
	{
		printf ("HashCreate failed\n");
	}
	puts(" OK");
	puts("/*****HashInsert (Index Hash Table)*****/");
	for(i = 0; i < table_size; ++i)
	{
		arr[i] = rand() % 100;
		printf("%lu (%lu)\t", arr[i], HashFunc((void *)arr[i]));
		if(i % 5 == 0)
	   {
		   printf("\n");
	   }
		HashInsert(table, (void *)arr[i]);
	}
	puts("");
	size_table = HashSize(table);
	puts("Map of HashTable:");
	PrintHashtable(table);
	printf("/*****HashSize:*****/\nsize (%lu?) = %lu\n",table_size, size_table);
	puts("");

	puts("/*****HashFind:*****/");
	for(i = 0 ; i < 5 ; ++i)
	{
		number = arr[rand() % 10];
		printf("Find? [%lu] = %lu\n",number,
				(size_t)HashFind (table, (void *)number));
	}
	puts("");
	for(i = 0 ; i < 5 ; ++i)
	{
		number = rand() % 100;
		ret = IsFound(arr, table_size, number);
		res = HashFind (table, (void *)number);
		printf("Find? number: %lu [%d?] = %lu\n", number, ret, (size_t)res);
	}
	size_table = HashSize(table);
	printf("/*****HashSize:*****/\nsize (%lu?) = %lu\n",table_size, size_table);

	puts("/*****HashRemove*****/:");
	size = table_size;
	for(i = 1 ; i < 6 ; ++i)
	{
		number = arr_rmv[i - 1];
		printf("number to remove: %lu\t", number);
		HashRemove (table, (void *)number);
		--size;
		printf("HashSize: size (%lu?) = %lu\n", size, HashSize(table));

	}
	PrintHashtable(table);
	puts("/*****HashForEach*****/:");
	HashForEach(table, CallBack, 0);
	puts("");
	puts("HashDestroy:");
	HashDestroy(table);
	puts(" OK");
	return (0);
}

int IsFound(size_t arr[], size_t size, size_t data)
{
	size_t i = 0;
	for(; i < size; ++i)
	{
		if(data == arr[i])
		{
			return (1);
		}
	}
	return (0);
}
void PrintHashtable (hash_table_t *hash)
{
	size_t i = 0;
	size_t res = 0;
	dlist_iter_t node = 0;
	size_t data = 0;
	assert(0 != hash);

	printf("index\tnmemb\tdata \n");
	for (; i < table_size; ++i)
	{
		if (0 != hash->table_base[i])
		{
			node = DlistBegin(hash->table_base[i]);
			res = DlistSize(hash->table_base[i]);
			printf("(%lu)\t(%lu)\t", i, res);
			while (DlistEnd(hash->table_base[i]) != node)
			{
				data = (size_t )DlistGetData(node);
				node = DlistNext(node);
				printf("%lu => ", data);
			}
			puts("NULL");
		}
	}
}
