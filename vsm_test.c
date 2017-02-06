#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vsm.h"

int main(int argc, char *argv[])
{
	void *memory = malloc(100);
	vsm_t *vsm = VSMInit(memory, 100);
	void *used1;
	void *used2;
	void *used3;
	void *used4;

	void *used =VSMAlloc(vsm, 5);
	*(char *)used = '1';
	used1 =VSMAlloc(vsm, 5);
	used2 =VSMAlloc(vsm, 12);
	used3 =VSMAlloc(vsm, 5);
	used4 =VSMAlloc(vsm, 5);
	VSMFree(used1);
	VSMFree(used2);
	VSMFree(used3);
	used =VSMAlloc(vsm, 12);

	VSMFree(used4);
	VSMFree(used);

	return(0);
}
