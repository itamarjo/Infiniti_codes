/*================================================================================
    Author: Dudi Kahana.
    Creation date:      24.12.14
    Last modified date: 21.7.16
    Description: State-machine implementation, to evaluates arithmetic expression.
==================================================================================*/
#include <stdio.h>
#include <math.h>

#include "stack_calculator.h"

typedef struct exp_test_case
{
    char*   expression;
    double  result;
}exp_test_case_t;


int main(int argc, char* argv[])
{

	int     i, num_of_errors=0;
	double  res;

	exp_test_case_t  test_case[100] = {{"1", 1},             /* #1: Single operand. */
                                      {"2345", 2345},        /* #2: Multi-digit operand. */
                                      {"(123)", 123 },       /* #3: Parenthesis on a single operand. */
                                      {"1+1", 2},            /* #4: Single operator - ADD */
                                      {"33-12", 21},         /* #5: Single operator - SUB */
                                      {"333*3", 999},        /* #6: Single operator - MUL */
                                      {"1/2", 0.5},          /* #7: Single operator - DIV */
                                      {"3^2", 9},            /* #8: Single operator - POW */
                                      {"(1+1)", 2},          /* #9: Parenthesis on a single expression. */
                                      {"(5-3)-2", 0},        /* #10: Associative law - חק הקיבוץ*/
                                      {"5-(3-2)", 4},        /* #11: Associative law - חק הקיבוץ*/
                                      {"5-3-2", 0},          /* #12: Associative law - חק הקיבוץ*/
                                      {"(320/8)/4", 10},     /* #13: Associative law - חק הקיבוץ*/
                                      {"320/(8/4)", 160},    /* #14: Associative law - חק הקיבוץ*/
                                      {"320/8/4", 10},       /* #15: Associative law - חק הקיבוץ */
                                      {"1+2+3+4", 10},       /* #16: Order of operation: left to right.*/
                                      {"1-2-3-4", -8},       /* #17: Order of operation: left to right.*/
                                      {"1*2*3*4", 24},       /* #18: Order of operation: left to right.*/
                                      {"2^3^2", 512},        /* #19: Order of operation: right to left.*/
                                      {"12-3+4",  13},       /* #20: Order of multi-operation: left to right.*/
                                      {"12+3-4",  11},       /* #21: Order of multi-operation: left to right.*/
                                      {"8/4*2",  4},         /* #22: Order of multi-operation: left to right.*/
                                      {"8*4/2",  16},        /* #23: Order of multi-operation: left to right.*/
                                      {"12+3*5",  27},       /* #24: Order of multi-operation: priority. */
                                      {"12*3+5",  41},       /* #25: Order of multi-operation: priority. */
                                      {"2*4^2-1", 31},       /* #26: Order of multi-operation: priority. */
                                      {"2*(4+1)-3", 7},      /* #27: Order of multi-operation: priority. */
                                      {"2^(3+5)*2", 512},    /* #28: Order of multi-operation: priority. */
                                      {"6+4^2*2+1", 39},     /* #29: Order of multi-operation: priority.  */
                                      {"((2+1)*3)", 9},      /* #30: Nested parenthesis. */
                                      {"(2*(2-3))", -2},     /* #31: Nested parenthesis. */
                                      {"(3*(3+3)/3)", 6},    /* #32: Nested parenthesis. */
                                      {"8/(16*2-30)", 4},    /* #33: Parenthesis with multi operations. */
                                      {"(1+2+3+4+5)*2", 30}, /* #34: Parenthesis with multi operations. */
                                      {"(2^3^2)*2", 1024},   /* #35: Parenthesis with multi operations. */
                                      {"3^(0-2)", 0.11},     /* #36: negative power. */
                                      {"1/3*6", 2},          /* #37: MUL of a non-int. */
                                      {"1/5/2", 0.10},       /* #38: DIV of a non-int. */
                                      {"1/(0-3)", -0.33},    /* #39: DIV by negative number. */
                                      {"2^(1/3)", 1.25} ,    /* #40: SQRT. */
                                      {"(1/3)^2", 0.11},     /* #41: POW of non-int. */
                                      {"((2-4)^3/((2*2)^2*2-30))^2", 16},     /* #42:  final exercise. */
                                      {NULL, 0}};


  printf("\nCalculator testing:\n");
  printf("===================\n");
  for (i=0; test_case[i].expression; ++i)
    {
        res = Evaluate(test_case[i].expression);
        res = trunc(res * 100.0) / 100.0;
        printf("Expression# %d: %s = %.2f\n",i+1, test_case[i].expression, res);
        if (res != test_case[i].result)
        {
            ++num_of_errors;
            printf("                --->Error! expected result: %.2f\n", test_case[i].result);
        }
  }
  printf("\nTotal test-cases: %d.\nYou have %d errors.\n", i ,num_of_errors);

    return 0;
}
