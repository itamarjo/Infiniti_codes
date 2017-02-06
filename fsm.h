#ifndef __FSM_H__
#define __FSM_H__

#include <stddef.h> /* size_t */

typedef struct fsmem fsm_t;

/*	return the size required for the pool
	Complexity O(n)*/
size_t FSMSuggestSize(size_t block_size, size_t num_blocks);

/*	return pointer to the pool
	Complexity O(n)*/
fsm_t *FSMInit(void *memory, size_t block_size, size_t bnum_blocks);

/*	return pointer to the block
	Complexity O(1)*/
void *FSMAlloc(fsm_t *fsm);

/*	Complexity O(1) */
void FSMFree(fsm_t *fsm, void *block);

/*	return number of free blocks
	Complexity O(n)*/
size_t FSMCountFree(fsm_t *fsm);

#endif /*__FSM_H__*/
