#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#include "calculator.h"
#include "stack.h"

#define EVENTS_NUM 256

/* a generic operation
 * add, subtraction etc...
 */
typedef double (*operation_t)(double a, double b);

typedef struct operator
{
	operation_t op;
	int priority;
} operator_t;

/* STATES_NUM: Number of states */
enum 
{
	GET_NUM,
	GET_OP,
	STATES_NUM
}state;

/* FState Machine action prototype
 * return 0 on success */
typedef char *(*action_t) (char *expr);

/* operators LUT */
static operator_t g_lut_operators[EVENTS_NUM];

/* FSM table */
static action_t g_calculator_fsm[STATES_NUM][EVENTS_NUM];

/* stacks (malloc based) */
static stack_t *num_stack;
static stack_t *op_stack;


double CalcEval(char *expr);
static void CalcInit(char *expr);
static char *ActionNum(char *expr);
static char *ActionPOpen(char *expr);
static char *ActionPClose(char *expr);
static char *ActionOp(char *expr);
static char *ActionError(char *expr);
static double Addition(double a, double b);
static double Substraction(double a, double b);
static double Multiplication(double a, double b);
static double Division(double a, double b);
static double Power(double a, double b);
static double ResolveAll(void);
static int CheckPriority(char exp_op);
static char *Resolve(void);


/*calculate an expression*/
double CalcEval(char *expr)
{
	double num = 0;
	/*state = GET_NUM;*/
	CalcInit(expr);

	while(*expr != '\0')
	{
		expr = g_calculator_fsm[state][*(unsigned char *)expr](expr);

		if(expr == 0)
		{
			return(2);
		}
	}
	if(0 != StackSize(op_stack))
	{
		num = ResolveAll();
	}

	else
	{
		num = *(double *)StackPeek(num_stack);
	}

	StackDestroy(op_stack);
	StackDestroy(num_stack);

	return (num);
}


/* Fill LUT, Fill FSM, create stack  */
static void CalcInit(char *expr)
{
	size_t expr_sz = strlen(expr);
	int i = 0;

	num_stack = StackCreate(sizeof(double), expr_sz);
	if(0 == num_stack)
	{
		ActionError(expr);

		return;
	}
	op_stack = StackCreate(sizeof(char)*expr_sz, expr_sz);
	if(0 == op_stack)
	{
		StackDestroy(num_stack);
		ActionError(expr);

		return;
	}
	state = GET_NUM;

	/*fill the LUT */
	g_lut_operators['+'].priority = 1;
	g_lut_operators['+'].op = Addition;
	g_lut_operators['-'].priority = 1;
	g_lut_operators['-'].op = Substraction;
	g_lut_operators['*'].priority = 2;
	g_lut_operators['*'].op = Multiplication;
	g_lut_operators['/'].priority = 3;
	g_lut_operators['/'].op = Division;
	g_lut_operators['^'].priority = 4;
	g_lut_operators['^'].op = Power;
	g_lut_operators[')'].priority = 5;/**/
	g_lut_operators['('].priority = 0; /**/

	/*fill fsm table*/
	for(i = 0; i < EVENTS_NUM; ++i)
	{

		if(i >= '0' && i <= '9')
		{
			g_calculator_fsm[GET_NUM][i] = ActionNum;
		}
		else
		{
			g_calculator_fsm[GET_NUM][i] = ActionError;
		}

		g_calculator_fsm[GET_OP][i] = ActionError;

	}
	g_calculator_fsm[GET_NUM]['('] = ActionPOpen;
	g_calculator_fsm[GET_OP][')'] = ActionPClose;
	g_calculator_fsm[GET_OP]['*'] = ActionOp;
	g_calculator_fsm[GET_OP]['+'] = ActionOp;
	g_calculator_fsm[GET_OP]['-'] = ActionOp;
	g_calculator_fsm[GET_OP]['/'] = ActionOp;
	g_calculator_fsm[GET_OP]['^'] = ActionOp;


}




/** sate/event action functions prototypes **/
/*
 * 1) get 'operator' via LUT 'g_lut_operators' for the current symbol (i.e. **expr)
 *		priority of current symbol
 * 2) stack_symbol = peek op_stack
 * 3) get 'stack_operator' via LUT 'g_lut_operators'
 * 4) IF operator.priority > stack_operator.priority
 * 		push current symbol, *expr += 1,  state = GET_NUM
 * 	ELSE
 * 		a = pop num, b = pop num, pop op, push(stack_operator.op(b, a))
 * */
static char *ActionOp(char *expr)
{
	/*char *last_op = 0;
	double num_a = 0;
	double num_b = 0;*/

	if (state != GET_OP)
	{
		return(ActionError(expr));
	}
	/*int StackPop(stack_t *stack)*/


	/*		StackPop(op_stack);
	if (1 == last_op)
	{
		return (ActionError);
	}*/
	if(1 == CheckPriority(*expr))
	{
		StackPush(op_stack,(const void *)expr);
		state = GET_NUM;

		return (expr + 1); /*check where advance*/
	}

	Resolve();


	return(expr);
}




/* 1) num = atoi(*expr)
 * 2) push num to num_stack
 * 3) *expr += num of chars
 * 4) state = GET_OP
 * */
static char *ActionNum(char *expr)
{
	/*double num = (double)atoi((const char *)expr);*/
	double num = 0;

	num = (double)strtol(expr, &expr, 10);


	/* Add a new elemente in the 'stack'.
	 *  return 0 - upon success, 1 upon failure
	 */

	if(1 == StackPush(num_stack, (const void *)&num))
	{
		return(0);
	}
	state = GET_OP;

	return(expr);

}




/*
 * '('
 * 1) push op
 */
static char *ActionPOpen(char *expr)
{
	StackPush(op_stack, expr);


	return(expr + 1);
}
/*
 * 1) while pop.op != '('
 * 		a = pop num, b = pop num, pop op, push(op(b, a))
 * 2) *expr += 1
 * */
static char *ActionPClose(char *expr)
{

	/*double num_a= 0;
	double num_b= 0;*/
	/*char *operator = (char *)StackPeek(op_stack);

	StackPop(op_stack);*/

	while(*Resolve() != '(')
	{
		/*++expr;*/

	}
	StackPop(op_stack);

	return(expr + 1);
}

static char *Resolve(void)
{
	char *last_op = 0;
	double *num_a = 0;
	double *num_b = 0;
	double lut_op = 0;

	last_op = (char *)StackPeek(op_stack);
	StackPop(op_stack);
	num_a = (double *)StackPeek(num_stack);
	StackPop(num_stack);
	num_b = (double *)StackPeek(num_stack);
	StackPop(num_stack);

	/*printf("\n %lf %c %lf\n", *num_b, *last_op, *num_a);*/

	lut_op = g_lut_operators[*(unsigned char *)last_op].op(*num_b, *num_a);

	StackPush(num_stack,(const void *)&lut_op);
	/*printf("%lg",*(double *)StackPeek(num_stack));*/


	return((char *)StackPeek(op_stack));
}

static double ResolveAll(void)
{
	while(StackSize(op_stack) != 0 )
	{
		Resolve();
	}

	return(*(double *)StackPeek(num_stack));
}

/*
 * fprintf(stderr, "error");
 * return (0);
 * */
static char *ActionError(char *expr)
{
	fprintf(stderr, "error");

	return(0);
}



/*Check Priority of operator*/
static int CheckPriority(char exp_op)
{

	char *last_op = 0;

	last_op = (char *)StackPeek(op_stack);
	if (0 == last_op)
	{
		return(1);
	}


	return(g_lut_operators[(unsigned char)exp_op].priority > g_lut_operators[*(unsigned char *)last_op].priority);
}




/* LUT operations */

static double Addition(double a, double b)
{
	return(a + b);
}

static double Substraction(double a, double b)
{
	return(a - b);
}

static double Multiplication(double a, double b)
{
	return(a * b);
}

static double Division(double a, double b)
{
	return(a / b);
}

static double Power(double a, double b)
{
	return(pow(a,b));
}





