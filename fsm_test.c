#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fsm.h"

int main(int argc, char *argv[])
{
	void *mem = 0;
	fsm_t *memt;
	printf("\n %lu" ,FSMSuggestSize(1,10));
	mem = malloc(FSMSuggestSize(1,10));
	if (mem == 0)
	{
		printf("falied allocation");
	}
	else
	{
		memt = FSMInit(mem,1, 10);
		mem = FSMAlloc(memt);
		*(char *)mem= '1'; 
	}

	return(0);
}
