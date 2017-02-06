#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <strings.h>
#include <stddef.h>
#include "dyn_vec.h"

#define arr_sizeof(x) (sizeof(x) / sizeof((x)[0]))

const char* str_a[] =
{
	"In computer science, an array type is a data type that is meant to ",
	"describe a collection of elements (values or variables), each selectedby ",
	"one or more indices (identifying keys) that can be computed at run time ",
	"by the program. Such a collection is usually called an array variable, ",
	"array value, or simply array. By analogy with the mathematical concepts ",
	"of vector and matrix, array types with one and two indices are often ",
	"called vector type and matrix type, respectively.",
	0
};

typedef struct tst
{
	char key;
	size_t value;
	char meta;
	void* data;
	int id[50];
} tst_t;



int main(int argc, char *argv[], char *envp[])
{
	tst_t tst_big_type = {1, 22, 1};
		tst_t *tst_big_type_r = 0;
		dyn_vec_t *vec1 = DynVecCreate(sizeof(char));
		dyn_vec_t *vec2 = DynVecCreate(sizeof(char*));
		size_t i;
		const char *p_str = 0;
		const char **p_arr_str = 0;

		if ((0 == vec1) || (0 == vec2))
		{
			puts("'Create' failed");
			return (0);
		}

		for (p_arr_str = str_a; *p_arr_str; ++p_arr_str)
		{
			if (0 != DynVecPushBack(vec2, p_arr_str) )
			{
				puts("(2) 'PushBack' failed");
			}

			if (DynVecSize(vec2) > DynVecCapacity(vec2))
			{
				puts("(2) size > capacity");
			}

			for (p_str = *p_arr_str; *p_str; ++p_str)
			{
				if (0 != DynVecPushBack(vec1, p_str) )
				{
					puts("(1) 'PushBack' failed");
				}

				if (DynVecSize(vec1) > DynVecCapacity(vec1))
				{
					puts("(1) size > capacity");
				}
			}

			if (0 != DynVecPushBack(vec1, p_str) )
			{
				puts("(1) 'PushBack' failed");
			}

			DynVecPopBack(vec2);
		}

		if (DynVecSize(vec2) != 0)
		{
			puts("(2) wrong size");
		}

		puts(DynVecGetItemAddress(vec1, 230));
		puts(" Such a collection is usually called an array variable, ");

		/* should generate Valgrind error */
		puts(DynVecGetItemAddress(vec2, 1));


		DynVecDestroy(vec1);  vec1 = 0;
		DynVecDestroy(vec2);  vec2 = 0;

		vec1 = DynVecCreate(sizeof(tst_t));

		if ((0 == vec1))
		{
			puts("'Create' failed");
			return (0);
		}

		for (i = 1; i < 1000; ++i)
		{
			tst_big_type.value = i;

			if (0 != DynVecPushBack(vec1, &tst_big_type) )
			{
				puts("(3) 'PushBack' failed");
			}

			if (DynVecSize(vec1) > DynVecCapacity(vec1))
			{
				puts("(3) size > capacity");
			}
		}


		if (DynVecCapacity(vec1) < 1010)
		{
			puts("(3) capacity too small ");
		}

		if (0 != DynVecReserve(vec1, 1500))
		{
			puts("(3) Reserve failed");
			return (0);
		}



		for (i = 1; i < 1000; ++i)
		{

			tst_big_type_r = DynVecGetItemAddress(vec1, i-1);

			if ( tst_big_type_r->value != i )
			{
				puts("wrong value stored");
				break;
			}

			DynVecPopBack(vec1);
		}

		if (DynVecSize(vec1) != 0)
		{
			puts("(3) wrong size");
		}

		if (0 != DynVecReserve(vec1, 500))
		{
			puts("(3) Reserve failed CRITICAL!");
			return (0);
		}

		if (DynVecCapacity(vec1) != 1500)
		{
			puts("(3) wrong capacity ");
		}

		DynVecDestroy(vec1);

/*
	char *elemtest1 = "2df";
	char *elemtest2 = "3dd";
	char *elemtest3 = "fff";
	dyn_vec_t *test = DynVecCreate(sizeof("444"));
	DynVecPushBack (test, &elemtest1);
	DynVecPushBack (test, &elemtest2);
	DynVecPushBack (test, &elemtest3);
	DynVecPushBack (test, &elemtest1);
	DynVecPopBack (test);
	DynVecPushBack (test, &elemtest2);

	printf ("%s\n", *(char**)DynVecGetItemAddress (test, 3));
	DynVecDestroy (test); */
	return(0);
}
