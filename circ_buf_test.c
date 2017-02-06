#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "circ_buf.h"

/* All test are destructive
*/

/* A basic test which creates a new buffer
	and then destroys it
*/
int TestCBuffCreateDestroy(void);

/* read and write to circular buffer using uniform steps
	Sequential read and write of randomly generated data
	All data is verified
*/
int TestCBuffUniform(size_t sz, size_t stp_sz);

/*
*/
int TestCBuffSizeCapacity(size_t sz);


int main(int argc, char *argv[])
{
	TestCBuffCreateDestroy();

	/* TestCBuffUniform(1, 1); */
	TestCBuffUniform(1, 7);
	TestCBuffUniform(1024, 1);
	TestCBuffUniform(1024, 4);
	TestCBuffUniform(1024, 7);

	TestCBuffSizeCapacity(23);
	TestCBuffSizeCapacity(223);
	TestCBuffSizeCapacity(4096);

	system("cowsay \"Congratulations!\"");

	return(0);
}

/* A basic test which creates a new buffer
	and then destroys it
*/
int TestCBuffCreateDestroy(void)
{
	cbuff_t *new_buf = CBuffCreate(1);
	assert(0 != new_buf);
	CBuffDestroy(new_buf);

	new_buf = CBuffCreate(sizeof(void*));
	assert(0 != new_buf);
	CBuffDestroy(new_buf);

	new_buf = CBuffCreate(10000 * sizeof(void*));
	assert(0 != new_buf);
	CBuffDestroy(new_buf);

	return (0);
}

/* uses uniform size to read and write
	Sequential read and write of randomly generated data
	All data is verified
*/
int TestCBuffUniform(size_t sz, size_t stp_sz)
{
	cbuff_t *cbuf = 0;
	size_t tot_sz = sz * stp_sz;
	size_t num = 0;
	size_t i;

	char *buf_w = (char*) calloc(sz, stp_sz);
	char *buf_r = (char*) calloc(sz, stp_sz);
	char *p_buf = 0;

	/* fill 'buf_w' with random data */
	srand(242);
	for (p_buf = buf_w, i = 0; i < tot_sz; i += 4, p_buf += 4)
	{
		*p_buf = rand();
	}

	/* create new circular buffer */
	cbuf = CBuffCreate(tot_sz);
	assert(0 != cbuf);

	num = CBuffRead(cbuf, buf_r, 2);
	assert(0 == num);

	/* write whole buffer, each time 'stp_sz' bytes */
	for (p_buf = buf_w, i = 0; i < tot_sz; i += stp_sz, p_buf += stp_sz)
	{
		num = CBuffWrite(cbuf, p_buf, stp_sz);
		if (num < stp_sz)
		{
			assert((tot_sz - i) == num);
		}
		else
		{
			assert(stp_sz == num);
		}
	}

	/* check if last write fails */
	num = CBuffWrite(cbuf, p_buf, stp_sz);
	assert(0 == num);


	/* read whole buffer, each time 'stp_sz' bytes */
	for (p_buf = buf_r, i = 0; i < tot_sz; i += stp_sz, p_buf += stp_sz)
	{
		num = CBuffRead(cbuf, p_buf, stp_sz);
		if (num < stp_sz)
		{
			assert((tot_sz - i) == num);
		}
		else
		{
			assert(stp_sz == num);
		}
	}

	/* check if last read fails */
	num = CBuffRead(cbuf, p_buf, stp_sz);
	assert(0 == num);

	CBuffDestroy(cbuf);

	/* finally verify rhat the two buffers are equal */
	assert(memcmp(buf_r, buf_w, tot_sz) == 0);

	free(buf_r);
	free(buf_w);

	return (0);
}

/*
*/
int TestCBuffSizeCapacity(size_t tot_sz)
{
	cbuff_t *cbuf = 0;
	size_t stp_sz = 19;
	size_t num = 0;
	size_t i;

	char *buf[32] = {0};

	/* create new circular buffer */
	cbuf = CBuffCreate(tot_sz);
	assert(0 != cbuf);

	/* write each time 'stp_sz' bytes and read 'stp_sz'-1 bytes */
	for (i = 0; i < (tot_sz - stp_sz); ++i)
	{
		num = CBuffWrite(cbuf, buf, stp_sz);
		assert(num == stp_sz);

		num = CBuffRead(cbuf, buf, stp_sz - 1);
		assert(num == (stp_sz - 1));

		assert(0 == CBuffIsEmpty(cbuf));
		assert((tot_sz - i - 1) == CBuffFreeSpace(cbuf));
	}

	num = CBuffWrite(cbuf, buf, stp_sz);
	assert(stp_sz == num);

	assert(0 == CBuffIsEmpty(cbuf));
	assert(0 == CBuffFreeSpace(cbuf));

	num = CBuffRead(cbuf, buf, 1);
	assert(1 == num);
	assert(0 == CBuffIsEmpty(cbuf));
	assert(1 == CBuffFreeSpace(cbuf));
	printf("%lu ", num);
	CBuffDestroy(cbuf);

	return (0);
}
