#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

int main(int argc, char *argv[])
{
	char * st = "Averyun"; /* conventional view of Anakin's path.*/
	
	queue_t *queue_pt = QCreate();
	queue_t *queue_pt2 = QCreate();

	while (*st != 0)
	{
		QEnqueue(queue_pt, (void*)st);
		QEnqueue(queue_pt2, (void*)st);
		st++;
	}

	printf("\n");

	printf("%c", *(char *)QDequeue(queue_pt));
	printf("%c", *(char *)QDequeue(queue_pt));
	printf("%c", *(char *)QPeek(queue_pt));

	QAppend(queue_pt,queue_pt2);

	while (QIsEmpty(queue_pt2) == 0)
	{
		printf("%c", *(char *)QDequeue(queue_pt2));
	}

	printf("\n");

	return(0);
}
