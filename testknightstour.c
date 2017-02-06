#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define _COLOR_RESET  "\x1b[0m"
#define _UNDERLINE  "\x1b[4m"
#define C_RED(s) "\x1b[31m\x1b[1m" s _COLOR_RESET
#define C_GRN(s) "\x1b[32m\x1b[1m" s _COLOR_RESET
#define C_YEL(s) "\x1b[33m\x1b[1m" s _COLOR_RESET
#define C_BLU(s) "\x1b[34m\x1b[1m" s _COLOR_RESET

#define BOARD_SIZE 64
#define BOARD_ROW(i) ((i) / 8)
#define BOARD_COL(i) ((i) % 8)

#define ABS(x) (((x) >= 0) ? (x) : (-(x)))

#include "knightstour.h"

static int CheckKnightStep(int cur, int next);
static void PrintBoard(unsigned long bitmap, int highlight);
static int CheckKnightPath(int* path);

int main(int argc, char *argv[])
{

	int start = 5;
	int path[BOARD_SIZE];

	for (start = 0; start < BOARD_SIZE; ++start)
	{
		KnightsTour(start, path);

		printf("%2d) ", start);

		if (0 == CheckKnightPath(path))
		{
			puts(" INCORRECT PATH");

		}
		else
		{
			printf(" CORRECT PATH ");
			if (CheckKnightStep(path[0], path[BOARD_SIZE - 1]))
			{
				printf("and is a cycle");
			}
			putchar('\n');
		}
	}

	return(0);
}

static int CheckKnightPath(int* path)
{
	unsigned long bitmap = -1L;
	int i;

	bitmap &= ~(1UL << path[0]);

	for (i = 1; i < BOARD_SIZE; ++i)
	{
		if (0 == CheckKnightStep(path[i-1], path[i]))
		{
			return (0);
		}

		if ((bitmap & (1UL << path[i])) == 0)
		{
			return (0);
		}


		bitmap &= ~(1UL << path[i]);

		 /*PrintBoard(bitmap, path[i + 1]); */
	}

	/* printf("%16lX\n", bitmap); */

	return (bitmap == 0);
}

/* return 1 if correct step */
static int CheckKnightStep(int cur, int next)
{
	int step_row = BOARD_ROW(next) - BOARD_ROW(cur);
	int step_col = BOARD_COL(next) - BOARD_COL(cur);

	if (((ABS(step_row) == 2) && (ABS(step_col) == 1)) ||
		((ABS(step_row) == 1) && (ABS(step_col) == 2)))
	{
		return (1);
	}

	return (0);
}

static void PrintBoard(unsigned long bitmap, int highlight)
{
	int j;
	puts("~~~~~~~~~~~~~~~~~~~~");

	for (j = 0; j < 64; ++j)
	{
		if (j == highlight)
			printf(C_RED(" %c"), '0' + (int)((bitmap >> j) & 1LU));
		else
			printf(" %c", '0' + (int)((bitmap >> j) & 1LU));

		putchar((BOARD_COL(j) == 7) ? '\n' : ' ');
	}
	puts("~~~~~~~~~~~~~~~~~~~~");
}
