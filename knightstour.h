/*
 * knightstour.h
 *
 *  Created on: Sep 14, 2016
 *      Author: pizza
 */

#define LS 8 /* Line Size */
#define BS LS * LS /*Board size */
#define MN 8 /* move number - how many jups can the knight do */


static void BuildLot(int LUT[][MN]);

static int CountMove(int LUT[][MN], int location, unsigned long board);

void KnightsTour (int start, int *path);


