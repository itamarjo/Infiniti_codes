#ifdef _DEBUG
	#include <stdlib.h>
	#include <stdio.h>
#endif
#include <assert.h>
#include "vsm.h"

#define WORD_SZ sizeof(void *)

#ifdef _DEBUG
#define AT_DEBUG(x) x
#else
#define AT_DEBUG(x)
#endif

struct vsmem
{
	size_t header;
};

AT_DEBUG(static void printpool(vsm_t *vsm)
{
	size_t count = 0;
	long *scan = (long *)((char *)vsm + sizeof(long));
	long *vsm_end = (long *)((char *)vsm + vsm->header);

	printf("VSM Address: %p Header Value: %lu VSM end: %p\n",
				(void *)vsm, vsm->header, (void *)vsm_end);
	while(scan < vsm_end)
	{
		++count;
		printf("BLOCK %lu HEADER IS %ld ADDRESS %p\n",
					count, *scan, (void *)scan);
		scan = (long *)((char *)scan + labs(*scan));
	}
})


/* return pointer to the pool */
vsm_t *VSMInit(void *memory, size_t sz)
{
	vsm_t *vsm = (vsm_t *)memory;

	assert(memory != 0);

	if (sz < (2 * (sizeof(size_t))))
	{
		return(0);
	}

	vsm->header = sz;
	*(long *)((char *)memory + sizeof(vsm_t)) = sz - sizeof(long);
	AT_DEBUG(printf("\nInitizlization\n");
		printpool(vsm);)
	return((vsm_t *)memory);
}

/* return 0 if failed, pointer to allocated block on success*/
void *VSMAlloc(vsm_t *vsm, size_t sz_block)
{
	char *search = 0;
	char *vsm_end = 0;
	char *next_block = 0;
	long rd_size = (sz_block + (WORD_SZ -1)) & (~(WORD_SZ - 1));

	AT_DEBUG(
	printf("\nAllocation \nrequested size is: %lu, rounded size is: %lu\n",
				sz_block, rd_size);)

	assert(vsm != 0);
	assert(sz_block != 0);

	vsm_end = (char *)vsm + vsm->header;
	/* check if the block is bigger than the size of the whole pool*/
	if (sz_block > vsm->header - (2L * sizeof(long)))
	{
		AT_DEBUG(printpool(vsm);)
		return(0);
	}

	/*use a search pointer to scan the pool for available block */
	search = (char *)vsm + sizeof(vsm_t);
	AT_DEBUG(printf("\nSearch:%ld\n", *(long *)search);)

	while (search < vsm_end)
	{
		/* check if the amount of free bytes in the block is enough to hold
		 	the asked size */
		if( rd_size <= (*(long *)search - (long)sizeof(long)))
		{
			/*check if the amount of available bytes is enough to hold
				the round size and the 2 headers
				i.e. in case we need to split the block into two blocks */
			if (rd_size <= *(long *)search - (long)(2 * sizeof(long)))
			{
				/* write the new header */
				*(long *)(search + sizeof(long) + rd_size) = *(long *)search
												- (rd_size + sizeof(long));
				/* update the exsiting header to reflect the change */
				*(long *)search = -(rd_size + sizeof(long));
				AT_DEBUG(printf("\nSplit\nSearch:%ld rd_size:%ld\n",
						*(long *)search, rd_size);)
			}
			else
			{
				/* giving the user the whole block, we need to mark the block as
					occupied */
				*(long *)search = -(*(long *)search);
				AT_DEBUG(printf("\n\nNo Split\nSearch:%ld\n", *(long *)search);)
			}
			AT_DEBUG(printpool(vsm);)

			/* return to the client a pointer to the new allocate block */
			return ((void *)(search + (long)sizeof(long)));
		}

		/* check if the value in seach is negative.
			i.e. the block is occupied */
		if(*(long *)search < 0)
		{
			/* move the search to next block */
			search -= *(long *)search; /* two minus is plus */
			AT_DEBUG(printf("\nNegative header skiping to next address: %p"
							, (void *)search);)
		}
		/* if the header is positive but not enough to hold the ask size
			we need to check for a possible merger */
		else
		{
						/* scan the blocks while search is in the range of the pool */
			next_block = (char *)search + *(long *)search;
			/* if the next block is outside the range, there is no merge to be
				done*/
			if (next_block >= vsm_end)
			{
				break;
			}
			if (*(long *)next_block < 0)
			{
				AT_DEBUG(printf("\n(CAN NOT MERGE)\n"); printpool(vsm);)
				/* move to the block after the next block */
				/* twice minus is a plus */
				search = next_block - *(long *)next_block;
			}
			AT_DEBUG(printf("search: %ld, nextblock: %ld, rd_size: %ld",
						*(long *)search, *(long *)next_block, rd_size);)
			/*check if the next block is within the pool and it's free as well
				and we still don't have enough space to make the allocation*/
			while ((next_block < vsm_end) && (*(long *)next_block > 0)
					&& (*(long *)search < (rd_size + (long)sizeof(long))))
			{
				AT_DEBUG(printf("\n(MERGE)\n"); printpool(vsm);)
				/*merge the two blocks */
				*(long *)search = *(long *)search + *(long *)next_block;
				next_block = (char *)search + *(long *)search;
			}

		}

	}
	AT_DEBUG(printf("\n(NO SPACE))\n"); printpool(vsm);)
	return(0);
}

void VSMFree(void *block)
{
	if (block != 0)
 	{
	AT_DEBUG(printf("\n(FREE)\n"));
	*(long *)((char *)block - sizeof(long)) *= -1L;

	}
}
