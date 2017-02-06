#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stack.h"

const char* str_a =
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
	tst_t *tst_big_type_r = 0;
	stack_t *stk1 = StackCreate(sizeof(char), 120);
	const char *p_str = 0;
	size_t i;
	int tst;

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
	else
	{
		puts(" ------- ");
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


	putchar('\n');


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

	for (i = 222; i > 0; --i)
	{
		tst_big_type_r = StackPeek(stk1);

		if ( tst_big_type_r->value != i )
		{
			puts("wrong value was stored");
			break;
		}

		if (StackSize(stk1) != i)
		{
			puts("(3) wrong size");
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


/*#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <strings.h>
#include <stddef.h>

#include "stack.h"



struct stack
{
	capacity of the stack
	char *stack_end;
	 pointer to the last element of the stack 
	char *last_elem;	
	 element's size 
	size_t elem_sz;
	 an array to the beginning of a stack
	char data[1];
};

int main(int argc, char *argv[])
{ 
 	
 	stack_t *stack_int = StackCreate(sizeof(int), 5);
 	if (stack_int)
 	{
 		printf("new stack created: elem_sz = %lu\nstack_end at = %lu (size) ",
 		stack_int -> elem_sz, (((stack_int -> stack_end) - (stack_int -> data)) / (stack_int -> elem_sz));
 	}
	return(0);
} */
