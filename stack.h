#ifndef __STACK_H__
#define __STACK_H__
#include <stdint.h>

typedef struct stack stack_t;


/* Create a new instance of stack
	elem_sz: element's size
	stack_sz: number of elements in the stack
	return: pointer to the new  stack upon success
			0 - upon failure
*/
stack_t* StackCreate(size_t elem_sz, size_t stack_sz);


/* Destroy an instance of the stack
	stack : the stackt
*/
void StackDestroy(stack_t *stack);


/* Add a new elemented in the 'stack'
	stack : the stack
	elem : the new element to add
	return : 0 - upon success, 1 - upon failure
*/
int StackPush(stack_t *stack, const void *elem);


/* Delete the last element in the stack 'stack'
	stack : the stack
	return : 0 - upon success, 1 - upon failure
*/
int StackPop(stack_t *stack);


/* Give access to last element of 'stack'
	stack : the stack
	return: pointer to the last element
*/
void* StackPeek(const stack_t *stack);


/* Size of the stack 'stack'
	stack : the stack
	return: number of elements in 'stack'
*/
size_t StackSize(const stack_t *stack);




#endif /*__STACK_H__*/
