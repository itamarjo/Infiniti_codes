#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <strings.h>
#include <stddef.h>
#include <math.h>
#include "barr.h"

#define WORD_SZ 8

/*
set all the bits in a bit array to on
	receive: void
	return: a bit array with all bits on
*/
long BarrAllOn(void)
{
		long barr = 0;
		int i = 0;

		for (; i < (WORD_SZ * 8); ++i)
		{
			barr <<= 1;
			barr |= 1;
		}

		return (barr);
}


/*
set all the bit in a bit array to off
	receive: void
	return: a bit array with all bits off
*/
long BarrAllOff(void)
{
		return (0);
}

/*
set the bit on index 'index' to on
	barr: an array of bits
	index: the location of the bit in the bit array
	return: a bit array with the index bit on
*/
long BarrSetOn (long barr, unsigned int index)
{
	barr |= (unsigned long)1 << index;

	return (barr);
}

/*
set the bit on index 'index' to off
	barr: an array of bits
	index: the location of the bit in the bit array
	return: a bit array with the index bit off
*/
long BarrSetOff (long barr, unsigned int index)
{
	barr &= ~( (unsigned long)1 << index );

	return (barr);
}

/*
change the bit on index 'index' to set
	barr: an array of bits
	index: the location of the bit in the bit array
	set: 1 for on 0 to off
	return: a bit array with the index bit set according to set
*/
long BarrSetBit (long barr, unsigned int index, unsigned int set)
{
	if (set)
	{
		barr |= ( unsigned long)1 << index;
	}
	else
	{
		barr &= ~( (unsigned long)1 << index );
	}

	return (barr);
}

/*
check if the bit on index 'index' is on
	barr: an array of bits
	index: the location of the bit in the bit array
	return: 1 for on 0 for off
*/
unsigned int BarrIsOn (long barr, unsigned int index)
{
	return ( (barr & ((unsigned long)1 << index) ) != 0);
}

/*
check if the bit on index 'index' is off
	barr: an array of bits
	index: the location of the bit in the bit array
	return: 1 for off 0 for on
*/
unsigned int BarrIsOff (long barr, unsigned int index)
{
	return ((barr & ((unsigned long)1 << index) ) == 0);
}

/*
toggle the bit on index 'index'
	barr: an array of bits
	index: the location of the bit in the bit array
	return: the bit array flipped
*/
long BarrFlip (long barr, unsigned int index)
{
	return (barr ^= (unsigned long)1 << index);
}

/*
push n bits right and add n bits to the left side
	barr: an array of bits
	rounds: number of times to "push" to the right
	return: the rotated bit array
*/
long BarrRotateRight (unsigned long barr, unsigned int rounds)
{
	rounds %= 64;
	barr =  ( barr << ((WORD_SZ * 8) - rounds) ) | (barr >> rounds);

	return (barr);
}

/*
push n bits left and add n bits to the right side
	barr: an array of bits
	rounds: number of times to "push" to the left
	return: the rotated bit array
*/
long BarrRotateLeft (unsigned long barr, unsigned int rounds)
{
		rounds %= 64;

		barr =  ( barr >> ((WORD_SZ * 8) - rounds) ) | (barr << rounds);

		return ( barr );
}

/*
make a mirror image of the bit array
	barr: an array of bits
	return: the mirror image of the bit array
*/
long BarrMirror (unsigned long barr)
{
	int i = 0;
	unsigned long mask_r = 1;
	unsigned long mask_l = (unsigned long) 1 << ( (WORD_SZ * 8) - 1 );
	unsigned long mirror = 0;

	for (i = 0; i < ( (WORD_SZ * 8) / 2 ) ; ++ i)
	{
		mirror |= (barr & mask_r) << ( (WORD_SZ * 8) - (2 * i) -1 )
					| (barr & mask_l) >> ( (WORD_SZ * 8) - (2 * i) - 1);
		mask_r <<= 1;
		mask_l >>= 1;
	}

	return (mirror);
}
/* without loop:
x = ((x & 0x55555555) << 1) | ((x & 0xAAAAAAAA) >> 1);
    x = ((x & 0x33333333) << 2) | ((x & 0xCCCCCCCC) >> 2);
    x = ((x & 0x0F0F0F0F) << 4) | ((x & 0xF0F0F0F0) >> 4);
    x = ((x & 0x00FF00FF) << 8) | ((x & 0xFF00FF00) >> 8);
    x = ((x & 0x0000FFFF) << 16) | ((x & 0xFFFF0000) >> 16);
    return x >> (32 - bits); */


/*
count the bits set to on in a bit array
	barr: an array of bits
	return: the number of bits that are on
*/
unsigned int BarrCountOn (long barr)
{
	int counter = 0;
	int i = 0;

	for (; i < (WORD_SZ * 8); ++i)
	{
		counter += barr & 1;
		barr >>= 1;
	}

	return (counter);
}

/*
count the bits set to off in a bit array
	barr: an array of bits
	return: the number of bits that are off
*/
unsigned int BarrCountOff (long barr)
{
	int counter = 0;
	int i = 0;

	for (; i < (WORD_SZ * 8); ++i)
	{
		counter += barr & 1;
		barr >>= 1;
	}

	return (64 - counter);
}
/*
make a mirror image of the bits using LUT
	barr: an array of bits
	return: the number of bits that are on
*/
long BarrLutMirror (unsigned long barr)
{
	unsigned char lut_mirror[256] = {0};
	int i = 0;
	unsigned long mask = 0x00FF;
	unsigned long mirror = 0;
	/*lut creation*/
	for (; i < 256; ++i)
	{
		lut_mirror [i] = (i & 0xF0) >> 4 | (i & 0x0F) << 4;
		lut_mirror [i] = (lut_mirror [i] & 0xCC) >> 2 |(lut_mirror [i] & 0x33) << 2;
		lut_mirror [i] = (lut_mirror [i] & 0xAA) >> 1 | (lut_mirror [i] & 0x55) << 1;
  	}

	for (i = 0; i < 8; ++i)
	{
		mirror |= ((unsigned long) lut_mirror[(barr & mask) >> (i * 8)] ) << ((8 - i -1) * WORD_SZ ) ;
		mask <<= 8;
	}

	return (mirror);
}
/*
count the bits set to on in a bit array using LUT
	barr: array of bits
	return: the number of bits that are on
*/
unsigned int BarrLutCountOn (long barr)
{
	size_t counter = 0;
	size_t word_counter = 0;
	int i = 0;
	unsigned char lut_mirror[256] = {0};
	unsigned long mask = 0x00FF;

	for (i = 0 ; i < 256; ++i)
	{
		char help;
		while (help != 0)
     {
         help &= help - 1;
         counter++;
     }
		 lut_mirror[i] = counter;
	}

	for (i = 0; i < 8; ++i)
	{
		word_counter +=  lut_mirror[ (barr & mask) >> (i* 8) ];
	}

	return (word_counter);
}

void BarrPrint (long barr)
{
	int i = 0;
	unsigned long mask = (long)1 << ((WORD_SZ * 8) -1);
		for (; i < ( WORD_SZ *8 ); ++i)
	{
		printf("%d", ((barr & mask) != 0));
		mask >>=1;
	}
	printf ("\n");
}
