#include <stdio.h>
#include "bin_tree.h"

int comp(const void *data, const void *datab)
{
	return(*(int*)data - *(int*)datab);  
}

int main()
{
	bin_tree_t *tree =BinTCreate(comp); 
	int ar[] = {14,2,6,28,5,3,32,61,74,62,35};
	int i = 0; 
	bt_iter_t find; 
	
	for (i = 0; i < 11; ++i)
	{
		BinTInsert(tree, &ar[i]);
	}
	TreePrint(tree); 
	/*BinTDestroy(tree); 
	printf("count: %lu Is Empty: %d ", BinTCount(tree), BinTIsEmpty(tree));*/
	for (i = 0; i < 11; ++i)
	{
		find = BinTFind(tree, &ar[i]);
		BinTRemove(find);
		printf("\n"); 
		printf("count: %lu Is Empty: %d ", BinTCount(tree), BinTIsEmpty(tree)); 
		TreePrint(tree);	
	}

	return(0); 
	 
}