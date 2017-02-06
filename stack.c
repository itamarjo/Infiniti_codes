#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "stack.h"


struct stack
{
	/* pointer to the end of the stack */
	char *stack_end;
	/* element's size */
	size_t elem_sz;
	/* pointer to the last element of the stack */
	char *last_elem;
	/* an array to the beginning of a stack*/
	char data[1];
};

/* Create a new instance of stack
	elem_sz: element's size
	stack_sz: number of elements in the stack
	return: pointer to the new  stack upon success
			0 - upon failure
*/
stack_t* StackCreate(size_t elem_sz, size_t stack_sz)
{
		/* allocate the size of stack t and the number of bytes for the elements -1 */
	stack_t *stk_ptr = (stack_t*)malloc (sizeof(stack_t) + (elem_sz * stack_sz) -1);

	assert (elem_sz * stack_sz != 0); /* checking if elements is not empty */

	if (0 == stk_ptr) /* check for failure in the allocation */
	{
		return (0);
	}
	/* filling the structure with info on it */
	stk_ptr->stack_end = stk_ptr->data + (elem_sz * stack_sz);
	stk_ptr->last_elem = stk_ptr->data;
	stk_ptr->elem_sz = elem_sz;

	return (stk_ptr);
}


/* Destroy an instance of the stack
	stack : the stackt
*/
void StackDestroy(stack_t *stack)
{
	free(stack);
}


/* Add a new elemented in the 'stack'
	stack : the stack
	elem : the new element to add
	return : 0 - upon success, 1 - upon failure
*/
int StackPush(stack_t *stack, const void *elem)
{
	assert(stack != NULL);/*checking if stack is not defined  */

	/*checking that the stucture is not empty */
	if (stack->last_elem >= stack->stack_end )
	{
		return (1);
	}
	/* copy the elemnt using the pointer last_elem */
	memcpy (stack->last_elem, elem, stack->elem_sz);
	/* moving pointer last_elem */
	stack->last_elem += stack->elem_sz;

	/*return 0 for sucess */
	return (0);
}


/* Delete the last element in the stack 'stack'
	stack : the stack
	return : 0 - upon success, 1 - upon failure
*/
int StackPop(stack_t *stack)
{
	assert(stack != NULL);/*checking if stack is not defined  */
	/*checking that the stucture is not empty */
	if (stack->last_elem == stack->data)
	{
		return (1);
	}
	/*no need for delete. just moving last_elem one step backwards */
	stack->last_elem -= stack->elem_sz;

	/*return 0 for sucess */
	return (0);
}


/* Give access to last element of 'stack'
	stack : the stack
	return: pointer to the last element
*/
void *StackPeek(const stack_t *stack)
{
	assert(stack != NULL);/*checking if stack is not defined  */
	/*checking that the stucture is not empty */
	if (stack->last_elem == stack->data)
	{
		return (NULL);
	}

	/* position of last_elem */
	return ((void*) (stack->last_elem - stack->elem_sz));
}


/* Size of the stack 'stack'
	stack : the stack
	return: number of elements in 'stack'
*/
size_t StackSize(const stack_t *stack)
{
	assert(stack != NULL);/*checking if stack is not defined  */
	/*calculating the size of the stack using last_elem and data */

	return ((stack->last_elem - stack->data) / stack->elem_sz);
}
