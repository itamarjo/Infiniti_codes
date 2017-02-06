#ifndef __BARR_H__
#define __BARR_H__

#include <stdint.h>

/*
set all the bits in a bit array to on
	receive: void
	return: a bit array with all bits on
*/
long BarrAllOn(void);

/*
set all the bit in a bit array to off
	receive: void
	return: a bit array with all bits off
*/
long BarrAllOff (void);

/*
set the bit on index 'index' to on
	barr: an array of bits
	index: the location of the bit in the bit array
	return: a bit array with the index bit on
*/
long BarrSetOn (long barr, unsigned int index);

/*
set the bit on index 'index' to off
	barr: an array of bits
	index: the location of the bit in the bit array
	return: a bit array with the index bit off
*/
long BarrSetOff (long barr, unsigned int index);

/*
change the bit on index 'index' to set
	barr: an array of bits
	index: the location of the bit in the bit array
	set: 1 for on 0 to off
	return: a bit array with the index bit set according to set
*/
long BarrSetBit (long barr, unsigned int index, unsigned int set);

/*
check if the bit on index 'index' is on
	barr: an array of bits
	index: the location of the bit in the bit array
	return: 1 for on 0 for off
*/
unsigned int BarrIsOn (long barr, unsigned int index);

/*
check if the bit on index 'index' is off
	barr: an array of bits
	index: the location of the bit in the bit array
	return: 1 for off 0 for on
*/
unsigned int BarrIsOff (long barr, unsigned int index);

/*
toggle the bit on index 'index'
	barr: an array of bits
	index: the location of the bit in the bit array
	return: the bit array flipped
*/
long BarrFlip (long barr, unsigned int index);

/*
push n bits right and add n bits to the left side
	barr: an array of bits
	rounds: number of times to "push" to the right
	return: the rotated bit array
*/
long BarrRotateRight (unsigned long barr, unsigned int rounds);

/*
push n bits left and add n bits to the right side
	barr: an array of bits
	rounds: number of times to "push" to the left
	return: the rotated bit array
*/
long BarrRotateLeft (unsigned long barr, unsigned int rounds);

/*
make a mirror image of the bit array
	barr: an array of bits
	return: the mirror image of the bit array
*/
long BarrMirror (unsigned long barr);

/*
count the bits set to on in a bit array
	barr: an array of bits
	return: the number of bits that are on
*/
unsigned int BarrCountOn (long barr);

/*
count the bits set to off in a bit array
	barr: an array of bits
	return: the number of bits that are off
*/
unsigned int BarrCountOff (long barr);
/*
make a mirror image of the bits using LUT
	barr: an array of bits
	return: the number of bits that are on
*/
long BarrLutMirror (unsigned long barr);
/*
count the bits set to on in a bit array using LUT
	barr: array of bits
	return: the number of bits that are on
*/
unsigned int BarrLutCountOn (long barr);

void BarrPrint (long barr);

#endif /*__BARR_H__*/
