#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "circ_buf.h"

#define MIN(a,b) (((a)<(b))?(a):(b))

struct circ_buf
{
	/* buffer's lenght */
	size_t capacity;
	/* pointer to the byte of the buffer busy to read */
	char *read;
	/* pointer to the byte of the buffer empty to write */
	char *write;
	/* buffer circular */
	char data[1];
};

/* Create a new instance of Circular Buff
	num_bytes: number of bytes in the Circular Buff
	return: pointer to the new  Circular Buffer upon success
			0 - upon failure
*/
cbuff_t *CBuffCreate(size_t num_bytes)
{
	cbuff_t *cbuff;

	assert(num_bytes != 0);
	/* allocate memory for the structure itself */
	assert(cbuff != NULL);

	cbuff = (cbuff_t*)malloc(sizeof(cbuff_t) + num_bytes);

	/* checking for allocation sucesses */
	if (cbuff == NULL)
	{
		return (0);
	}
	/* initializing the structure data fields */
	cbuff->capacity = num_bytes;
	cbuff->read = cbuff->data;
	/* we will always keep a distacnce of one byte between read and write
		unless it is full*/
	cbuff->write = cbuff->data + 1;

	return(cbuff);
}


/* Destroy an instance of the Circular Buffer
	cbuff : the CBuff
*/
void CBuffDestroy(cbuff_t *cbuff)
{
	free (cbuff);
}


/* Read bytes in the 'CBuff'
	cbuff : the CBuff
	target: where to read bytes
	amount: how many bytes to read
	return : the number of bytes read is returned (zero indicates
	end of buffer), -1 - on error,
*/
size_t CBuffRead(cbuff_t *cbuff, void *target, size_t amount)
{
	/* preparing vars to help with the calculation */
	/* writtend -
		written bytes will hold the amount of written bytes in the buffer  */
	size_t writtenb = (((cbuff->write - cbuff->read) + (cbuff->capacity) )
						% (cbuff->capacity +1))  ;
	/* bytes to read will hold the actual amount of bytes we can read */
	size_t btoread = MIN (writtenb, amount);
	/* Byte to end will hold the amounts of bytes until the end of the buffer */
	size_t btoend = cbuff->data + cbuff->capacity - (cbuff->read);

	assert (target != NULL);
	/* checking if there any byets to read*/
	assert(cbuff != NULL);

	if (0 == btoread)
	{
		return (0);
	}
	/* checking if we need to use one memcpy or two */
	if (btoread > btoend)
	{
		/* copy first until the end of the buffer ,and then from the start */
		memcpy (target, cbuff->read + 1 ,btoend);
		memcpy ((char*)target + btoend, cbuff->data, btoread - btoend);
	}
	else
	{	/* copy all btoread in one command */
		memcpy (target, cbuff->read + 1, btoread);
	}
	/* move the read pointer to it's next location */
	cbuff->read = cbuff->data + (((cbuff->read - cbuff->data) + btoread)
	 				% (cbuff->capacity + 1));

	/*return the amount that we read */
	return (btoread);
}


/* Write bytes of element in the CBuff 'CBuff'
	cbuff : the CBuff
	elem : the new element to add
	amount: how many bytes to write from new element
	return : the  number  of bytes written is returned  (zero indicates
	nothing was written), -1 - on error,
*/
size_t CBuffWrite(cbuff_t *cbuff, const void *elem, size_t amount)
{
		/* preparing vars to help with the calculation */
		/* free Bytes holds the number of bytes that are free in the buffer */
		size_t free_b = (((cbuff->read - cbuff->write) + (cbuff->capacity + 1))
		 				% ( cbuff->capacity +1))  ;
		/*bytes to write calculate the actual amount to write */
		size_t btowrite = MIN (free_b, amount);
		/*  bytes to end calculate the amount of bytes to the end of the buffer */
		size_t btoend = cbuff->data + cbuff->capacity + 1 - cbuff->write ;

		assert (elem != 0);

		assert(cbuff != NULL);

		/* check if there is any bytes to write */
		if (0 == btowrite)
		{
			return (0);
		}
		/* check if there is a need to do two memcpy or one  */
		if (btowrite > btoend)
		{
			/* use memcopy: one to write until the end, the other from the start */
			memcpy (cbuff->write, elem, btoend);
			memcpy (cbuff->data,(char*) elem + btoend, btowrite - btoend);
		}
		else
		{
			/* write all the btowrite in one memcpy */
			memcpy (cbuff->write, elem, btowrite);
		}
		/* move the pointer to the next point */
		cbuff->write = cbuff->data + (((cbuff->write - cbuff->data) + btowrite)
		 				% (cbuff->capacity+1));

		return (btowrite);
}


/* Check free space on the Circular Buffer
	cbuff : the CBuff
	return: number of spaces bytes free in 'CBuff'
*/
size_t CBuffFreeSpace(const cbuff_t *cbuff)
{
	assert(cbuff != NULL);

	return (((cbuff->read - cbuff->write) + cbuff->capacity )
			% cbuff->capacity +1 );
}


/* Check if the Circular Buffer is empty
	cbuff : the CBuff
	return: 1 if empty and 0 if not
*/
int CBuffIsEmpty (const cbuff_t *cbuff)
{
	assert(cbuff != NULL);

	return (((cbuff->write - cbuff->read) + cbuff->capacity)
				% cbuff->capacity == 1);
}


/* Give bytes's Capacity of the Circular Buffer
	cbuff : the CBuff
	return: bytes's capacity of the 'CBuff'
*/
size_t CBuffCapacity(const cbuff_t *cbuff)
{
	assert(cbuff != NULL);

	return (cbuff->capacity);
}
