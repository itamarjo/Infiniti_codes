#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>

#include "hash.h"

#define SIZE_TABLE 6

 static size_t HashFunction(void *data);
 static int IsMatch (const void *data1, const void *data2);
 static int PrintHelp(void *data, void *param);
 static void Print(hash_table_t *hash,void *params);
 static size_t HashFunction(void *data);
 /*static int IsMatch2 (const void *data1, const void *data2);*/

 int main(int argc, char *argv[])
{
	 int arr[] = {5,8,3,56,7,6};
	 int arr1[] = {8,9,5,98,2,6};
	 int i = 0;

	 hash_table_t *hash = HashCreate (HashFunction, SIZE_TABLE, IsMatch);
	 hash_table_t *hash1 = HashCreate (HashFunction, SIZE_TABLE, IsMatch);
	 int *ret= 0;

	 printf("count? (0) : %lu\n", HashSize(hash1));


	 printf("isempty ? (1) : %d\n",HashIsEmpty(hash));

	 for(i = 0; i<SIZE_TABLE; ++i)
	 {
		 if(1 == HashInsert(hash, &arr[i]))
		 {
			 printf("Insert FAILED");

			 return(0);
		 }
		 else
		 {
			 printf("HiHa-");
		 }
	 }
	 printf("\n");
	 printf("isempty ? (0) : %d\n",HashIsEmpty(hash));

	 printf("count? (6) : %lu\n", HashSize(hash));

	 Print(hash,0);
	 printf("\n now trying to remove: %d", arr[2]);

	 HashRemove(hash,&arr[2] );
	 printf("after 1 remove count ? (5) : %lu\n", HashSize(hash));

	Print(hash,0);

	printf("\nfind %d : ", arr[4]);
	ret =  (int *)HashFind(hash,&arr[4]);
	if(ret != 0)
	{
		printf("%d is found\n", *ret);
	}
	else
	{
		printf("%d is NOT found\n", arr[4]);
	}
	ret =  (int *)HashFind(hash,&arr1[4]);

	printf("\nfind %d", arr1[4]);
	if(ret != 0)
	{
		printf("%d is found\n", *ret);
	}
	else
	{
		printf("%d is NOT found\n", arr1[4]);
	}

	 /*second hash*/
	 for(i = 0;i<SIZE_TABLE; ++i)
	 {
		 if(1 == HashInsert(hash1, &arr1[i]))
		 {
			 printf("Insert FAILED");

			 return(0);
		 }
		 else
		 {
			 printf("HiHa-");
		 }
	 }
	 printf("\n");
	 printf("isempty ? (0) : %d\n",HashIsEmpty(hash1));
	 for(i = 0;i<SIZE_TABLE; ++i)
	 {
		 HashRemove(hash1,&arr1[i]);

	 }
	 printf("count? (0) : %lu\n", HashSize(hash1));
	 printf("isempty ? (1) : %d\n",HashIsEmpty(hash1));


	 printf("-------------send to review-------------");

	 return(0);
}

static void Print(hash_table_t *hash,void *params)
 {
	 HashForEach(hash, PrintHelp, params);
 }

 static int PrintHelp(void *data, void *param)
{
	 printf("%d-",*(int *)data);

	 return(0);
}


 static size_t HashFunction(void *data)
 {

	 return(*(int *)data % SIZE_TABLE);
 }


 /*static int IsMatch (const void *data1, const void *data2)
 {
	 return(*(int *)data1 != *(int *)data2);
 }*/
 static int IsMatch (const void *data1, const void *data2)
  {
 	 return(*(int *)data1 == *(int *)data2);
  }

