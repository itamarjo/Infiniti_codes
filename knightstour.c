/*
 * knightstour.c
 *
 *  Created on: Sep 14, 2016
 *      Author: pizza
 */
#include <assert.h>
#include <stdio.h>
#include "knightstour.h"


static void BuildLot(int LUT[][MN])
{
	int i = 0, j = 0, column = 0;
	int jump[] = {-17, -15, -10, -6, 6, 10, 15, 17};

	for(i = 0; i < BS; ++i)
	{

		for(j = 0; j < MN; ++j)
		{
			column = i % LS;
			if((i + jump[j] > 63) || (i + jump[j] < 0) ||
					( ( (column - ((i + jump[j]) % MN)) * (1 - (2 * (j %2))) ) < 0 ))
			{
				LUT[i][j] = 64;
			}
			else
			{
				LUT[i][j] = i + jump[j];
			}

		}

	}
}

static int CountMove(int LUT[][MN], int location, unsigned long board)
{
	int ret = 0, i =0;

	for(i = 0; i < MN; ++i)
	{
		if ((LUT[location][i] != 64) && ((board | ~(1LU << LUT[location][i])) == 0xffffffffffffffff))
		{
			++ret;
		}
	}

	return(ret);
}


void KnightsTour (int start, int *path)
{
	unsigned long board = 0;

	int location = start, min = 1, next_location = 0;

	int LUT[BS][MN] = {0};

	int i = 0;

	assert((start < BS) && (start >= 0));


	BuildLot(LUT);

	board = 0xffffffffffffffff;

	board &= ~(1L << start);

	*path = location;
	++path;

	while (min != 0)
	{
		min = 9;
		for( i = 0; i < MN; ++i)
		{
			if ((LUT[location][i] != 64) && ((board | ~(1LU << LUT[location][i])) == 0xffffffffffffffff))
			{
				if (CountMove(LUT, LUT[location][i], board) < min)
				{
					min = CountMove(LUT, LUT[location][i], board);
					next_location = LUT[location][i];
				}
			}
		}
		board &= ~(1L << next_location);
		location = next_location;
		*path = location;
		++path;
	}

}


