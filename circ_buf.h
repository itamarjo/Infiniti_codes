#ifndef __CIRC_BUF_H__
#define __CIRC_BUF_H__
#include <stddef.h>

typedef struct circ_buf cbuff_t;

/* Create a new instance of Circular Buff
	num_bytes: number of bytes in the Circular Buff
	return: pointer to the new  Circular Buffer upon success
			0 - upon failure
*/
cbuff_t *CBuffCreate(size_t num_bytes);


/* Destroy an instance of the Circular Buffer
	cbuff : the CBuff
*/
void CBuffDestroy(cbuff_t *cbuff);


/* Read bytes in the 'CBuff'
	cbuff : the CBuff
	target: where to read bytes
	amount: how many bytes to read
	return : the number of bytes read is returned (zero indicates
	end of buffer), -1 - on error,
*/
size_t CBuffRead(cbuff_t *cbuff, void *target, size_t amount);


/* Write bytes of element in the CBuff 'CBuff'
	cbuff : the CBuff
	elem : the new element to add
	amount: how many bytes to write from new element
	return : the  number  of bytes written is returned  (zero indicates
	nothing was written), -1 - on error,
*/
size_t CBuffWrite(cbuff_t *cbuff, const void *elem, size_t amount);


/* Check free space on the Circular Buffer
	cbuff : the CBuff
	return: number of spaces bytes free in 'CBuff'
*/
size_t CBuffFreeSpace(const cbuff_t *cbuff);


/* Check if the Circular Buffer is empty
	cbuff : the CBuff
	return: 1 if empty and 0 if not
*/
int CBuffIsEmpty (const cbuff_t *cbuff);


/* Give bytes's Capacity of the Circular Buffer
	cbuff : the CBuff
	return: bytes's capacity of the 'CBuff'
*/
size_t CBuffCapacity(const cbuff_t *cbuff);

#endif /*__CIRC_BUF_H__*/
