#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stack.h"

char* str_a =
"A stack is a basic data structure that can be logically thought as linear structure represented by a real physical stack or pile, a structure where insertion and deletion of items takes place at one end called top of the stack.";

typedef struct tst
{
	char key;
	size_t value;
	char meta;
	void* data[2];
	int id[50];
} tst_t;


int main(int argc, char *argv[])
{
 	tst_t tst_big_type = {1, 22, 1};
	stack_t *stk1 = StackCreate(sizeof(char), 120);
	char *p_str = 0;
	int tst;
	int i;
	tst_t *tst_big_type_r;

	if (0 == stk1)
	{
		return (0);
	}

	for (p_str = str_a; *p_str; ++p_str)
	{
		if (0 != StackPush(stk1, p_str))
		{
			break;
		}

		if (((size_t)(p_str - str_a) % 5) == 0)
		{
			StackPop(stk1);
		}

	}

	if (*p_str == '\0')
	{
		puts(" StackOverflow ");
	}

	do
	{
		p_str = StackPeek(stk1);

		if (0 != p_str)
		{
			putchar(*p_str);
		}

		tst = StackPop(stk1);

		if ((0 != tst) || (0 == p_str))
		{
			/* Pop - failed */
			/* Thus Peek must return null*/
			if ((0 != p_str) || (0 == tst))
			{
				puts(" Cognitive dissonance ");
			}
		}
	}
	while ( 0 == tst );


	StackDestroy(stk1);  stk1 = 0;


	stk1 = StackCreate(sizeof(tst_t), 222);


	if (0 == stk1)
	{
		puts("'Create' failed");
		return (0);
	}

	for (i = 1; i <= 222; ++i)
	{
		tst_big_type.value = i;

		if (0 != StackPush(stk1, &tst_big_type) )
		{
			puts("(3) 'Push' failed");
		}
	}

	if (0 == StackPush(stk1, &tst_big_type) )
	{
		puts("(3) 'Push' failed to fail");
	}


	for (i = 1; i <= 222; ++i)
	{
		tst_big_type_r = StackPeek(stk1);

		if ( tst_big_type_r->value != i )
		{
			puts("wrong value was stored");
			break;
		}

		if (0 != StackPop(stk1))
		{
			puts("(3) 'Pop' failed");
		}
	}

	if (0 == StackPop(stk1))
	{
		puts("(3) 'Pop' failed to fail");
	}


	if (StackSize(stk1) != 0)
	{
		puts("(3) wrong size");
	}

	StackDestroy(stk1);

	return(0);
}
