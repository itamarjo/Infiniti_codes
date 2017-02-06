#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "barr.h"

#define arr_sizeof(x) (sizeof(x) / sizeof((x)[0]))

static void PrintBarr(unsigned long barr);

int main(int argc, char *argv[])
{
	const unsigned long ref[] =
	{
		0xC801020304050607,
		0xAAAAAAAAAAAAAAAA,
		0x8000000000000000,
		0x0101010101010101
	};
	const unsigned long ref_mir[] =
	{
		0xE060A020C0408013,
		0x5555555555555555,
	    0x0000000000000001,
		0x8080808080808080
	};

	const unsigned long ref_rot_r[][4] =
	{
		{
			0xE400810182028303,
			0x3E40081018202830,
			0x28303E4008101820,
			0x28303E4008101820
		}, {
			0x5555555555555555,
			0x5555555555555555,
			0x5555555555555555,
			0x5555555555555555
		}, {
			0x4000000000000000,
			0x0400000000000000,
			0x0000040000000000,
			0x0000040000000000
		}, {
			0x8080808080808080,
			0x0808080808080808,
			0x0808080808080808,
			0x0808080808080808
		}
	};

	const unsigned long ref_rot_l[][4] =
	{
		{
			0x90020406080A0C0F,
			0x0020406080A0C0F9,
			0x406080A0C0F90020,
			0x406080A0C0F90020
		}, {
			0x5555555555555555,
			0x5555555555555555,
			0x5555555555555555,
			0x5555555555555555
		}, {
			0x0000000000000001,
			0x0000000000000010,
			0x0000000000100000,
			0x0000000000100000
		}, {
			0x0202020202020202,
			0x2020202020202020,
			0x2020202020202020,
			0x2020202020202020
		}
	};

	unsigned long tst = 1;
	size_t i, j, k;

	tst = BarrAllOn();
	if (tst != 0xFFFFFFFFFFFFFFFF)
	{
		printf("fail: BarrAllOn() : %lu", BarrAllOn());
	}

	puts("Mirror: ");

	for (i = 0; i < arr_sizeof(ref); ++i)
	{
		tst = BarrMirror(ref[i]);

		if (tst != ref_mir[i])
		{
			printf("BarrMirror failed: ");
			PrintBarr(tst);
			printf("expected: ");
			PrintBarr(ref_mir[i]);
		}
	}

	puts("MirrorLut: ");

	for (i = 0; i < arr_sizeof(ref); ++i)
	{
		for (j = 1; j < 300; j += 3 + j*j)
		{
			tst = BarrMirror(BarrLutMirror(ref[i]*j));

			if (tst != (ref[i]*j))
			{
				printf("MirrorLut failed at: ");
				PrintBarr(ref[i]*j);
			}
		}
	}

	puts("RotateRight: ");

	for (i = 0; i < arr_sizeof(ref); ++i)
	{
		for (j = 1, k = 0; j < 300; j += 3*j + 1, ++k)
		{
			tst = BarrRotateRight(ref[i], j);
			if (tst != ref_rot_r[i][k])
			{
				printf("RotateRight failed: ");
				PrintBarr(tst);
				printf("expected: ");
				PrintBarr(ref_rot_r[i][k]);
			}
		}
	}

	puts("RotateLeft: ");

	for (i = 0; i < arr_sizeof(ref); ++i)
	{
		for (j = 1, k = 0; j < 300; j += 3*j + 1, ++k)
		{
			tst = BarrRotateLeft(ref[i], j);
			if (tst != ref_rot_l[i][k])
			{
				printf("RotateRight failed: ");
				PrintBarr(tst);
				printf("expected: ");
				PrintBarr(ref_rot_l[i][k]);
			}
		}
	}

	return(0);
}

static void PrintBarr(unsigned long barr)
{
	unsigned long mask = 1UL << ((8 * sizeof(long))-1);

	putchar('\n');
	while (0 != mask)
	{
		putchar( '0' + !!(mask & barr));
		mask >>= 1;
	}
	putchar('\n');
}
