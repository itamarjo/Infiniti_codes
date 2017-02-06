#include <stdio.h> /*for printf*/
#include <assert.h>


#include "fsm.h"

#define WORD_SZ sizeof(void *)

#ifdef _DEBUG
#define TEST(x) x
#else
#define TEST(x)
#endif


struct fsmem {
	size_t block_sz;
	size_t block_qnt;
	size_t offset;
};

/*	return the size required for the pool
	Complexity O(n)*/
size_t FSMSuggestSize(size_t block_size, size_t num_blocks)
{
	return(sizeof(fsm_t) +
			(((block_size + (WORD_SZ - 1)) & ~(WORD_SZ - 1)) * num_blocks));
}

/*	return pointer to the pool
	Complexity O(n)*/
fsm_t *FSMInit(void *memory, size_t block_size, size_t num_blocks)
{
	int i = 0;
	char *int_ptr = (char *)memory;
	fsm_t *fsm_ptr = (fsm_t *)memory;

	assert(memory != 0);
	assert(block_size != 0);
	assert(num_blocks != 0);

	/* initialize the structure with the relevant info on the buffer */
	fsm_ptr->block_sz = ((block_size + (WORD_SZ - 1)) & ~(WORD_SZ - 1));
	fsm_ptr->block_qnt = num_blocks;
	fsm_ptr->offset =sizeof(fsm_t);

	/* move the pointer to the first available block */
	int_ptr += sizeof(fsm_t);

	/* write in each block the distance in bytes between the start of the memory
		area to the next available block */
	for (i = 1; i < num_blocks; ++i)
	{
		/* cast to size t and write the distance in bytes */
		*(size_t *)int_ptr = sizeof(fsm_t) + (fsm_ptr->block_sz * i);
		/* progress the pointer to the next available block */
		int_ptr += fsm_ptr->block_sz;
	}
	/* write 0 in the last block to mark that this is the last available block */
	*(size_t *)int_ptr = 0;

	return((fsm_t *)memory);
}

/*	return pointer to the block
	Complexity O(1)*/
void *FSMAlloc(fsm_t *fsm)
{
	void *ret = 0;

	assert(fsm != 0);
	/* if there is no element to allocate return 0 */
	if (0 == fsm->offset)
	{
		return(0);
	}
	/* return will go to the next available element */
	ret = (char *)fsm + fsm->offset;
	/* update the struct to the next available element */
	fsm->offset = *(size_t *)ret;

	return(ret);
}

/* check if an addressed was freed before */
TEST(static int IsFree(fsm_t *fsm, void *block)
{
	size_t distance = fsm->offset;
	/* if distance is 0 we checked all the elements */

	while (distance != 0)
	{
		/* compare the address to a free element*/
		if ((char *)block == ((char *)fsm + distance))
		{
			return(1);
		}
		/*update the distance to the next element */
		distance = *((char *)fsm + distance);
	}

	return(0);
})
/*	Complexity O(1) */
void FSMFree(fsm_t *fsm, void *block)
{
	assert(fsm != 0);
	assert(block != 0);

	TEST (
		if( ((char *)block - (char *)fsm - sizeof(fsm_t)) % fsm->block_sz != 0) /* check if the address is on a boudry */
		{
			printf("address are not on a boundry");
			return;
		}
		/*check if the address is in range */
		if (((char *)block < (char *)fsm + sizeof(fsm_t)) ||
			((char *)block > (char *)fsm + sizeof(fsm_t) + (fsm->block_sz * fsm->block_qnt)))
		{
			printf("addres are not in range");
			return;
		}
		/*check if the address was freed before */
		if (IsFree(fsm, block) == 1)
		{
			printf("block is free already");
			return;
		}

		)
	/* write in the block the offset to the next available free element */
	*(size_t *)block = fsm->offset;
	/*update the offset in the struct to reflect the new element */
	fsm->offset = (char *)block - (char *)fsm;
}

/*	return number of free blocks
	Complexity O(n)*/
size_t FSMCountFree(fsm_t *fsm)
{
	size_t distance = 0;
	size_t counter = 0;

	assert(fsm != 0);

	/*first take the value from the offset */
	distance = fsm->offset;
	/*go through each one of the offset values */
	while (distance != 0)
	{
		++counter; /*advance counter */
		distance = *((char *)fsm + distance); /*distance get the next value */
	}

	return(counter);
}
