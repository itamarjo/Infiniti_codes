/*
 * fib.c
 *
 *  Created on: Aug 30, 2016
 *      Author: pop
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

size_t FibItr(size_t n)
{
	size_t i = 0;
	size_t feb_0 = 0;
	size_t feb_1 = 1;
	size_t feb = 0;

	for (i = 1; i < n; ++i)
	{
		feb = feb_0 + feb_1;
		feb_0 = feb_1;
		feb_1 = feb;
	}

	return(feb);
}

size_t FibRec(size_t n)
{

	if (n <= 1)
	{
		return(n);
	}

	return (FibRec(n-1) + FibRec(n-2));
}

size_t strlenr(const char *p)
{
	if ('\0' == *p)
	{
		return(0);
	}
	return(strlenr(++p)+1);
}

int strcmpr(const char *s1, const char *s2)
{
	if ((*s1 != *s2) || (*s1 == '\0'))
	{
		return(*s1 - *s2);
	}

	return(strcmpr(++s1, ++s2));
}


char *strcpyr(char *dest, const char *src)
{
	*dest = *src;

	if (*src == '\0')
	{
		return(dest);
	}

	strcpyr(dest + 1, src + 1);
	return(dest);
}

char *strcatr(char *dest, const char *src)
{
	if (*src == '\0')
		{
			return(dest);
		}

	if (*dest == '\0')
	{
		*dest = *src;
		*(dest +1)  = '\0';
		strcatr(dest+1, src+1);

	}
	else
	{
		strcatr(dest + 1, src);
	}

	return(dest);
}

int strcmpr2(const char *s1, const char *s2)
{
	if (*s2 == '\0')
	{
			return (0);
	}

	if ((*s1 != *s2) )
	{
		return(*s1 - *s2);
	}


	return(strcmpr2(++s1, ++s2));
}

char *strstrr(char *haystack, const char *needle)
{

	if (*haystack =='\0')
	{
		return('\0');
	}
	if (0 == strcmpr2(haystack, needle))
	{
		return (haystack);
	}
	else
	{
		return (strstr(haystack + 1, needle));
	}

}
int main ()
{
	char str1[] = "Hello World"; /* string to use in tests */
	char str1c[] = "Hello World"; /* string to use in tests */
	char str2[] = "abcd";
	char str2c[] = "abcd";
	char str3c[] ="b";
	char str3[] = "ab";
	char str4[] = "XX";
	char str5[] = "hello world";
	char src[50], srcc[50], dest[10], destc[10];
	char del[] = ".? -";
	char str6[] = "hello??.world..!";
	char str6c[] = "hello...world..!";
	char string[] = "words separated by spaces -- and, punctuation!";
	char *line1;
	char *line2;
	char *new;
	int *a;
	int *b;
	int c = 32;
	int d = 2;
	char space[]="      trying to     see if it     delete spaces    ";
	a = &c;
	b = &d;
	printf("%lu ", FibRec(6));
	printf("%lu ", strlenr("hello"));
	printf("\n%d", strcmpr("aa","ab"));

	/*-------------------- testing strcmp-----------------------*/
	/* testing str1 vs str2 */
	printf("\nstrcmp(%s,%s) expected: %d", str1, str2, strcmp(str1, str2));
	printf(" result:  %d", strcmpr(str1, str2));
	/* testing "aaa" vs "aaa" */
	printf("\nstrcmp(%s,%s) expected: %d", "aaa", "aaa", strcmp("aaa","aaa"));
	printf(" result:  %d", strcmpr("aaa","aaa"));
	/* testing str2 vs str3 */
	printf("\nstrcmp(%s,%s) expected: %d", str2, str3, strcmp(str2,str3));
	printf(" result:  %d", strcmpr(str2,str3));
	/* testing str3 vs str2 */
	printf("\nstrcmp(%s,%s) expected:  %d", str3, str2, strcmp(str3,str2));
	printf(" result:  %d\n", strcmpr(str3,str2));

	/*-------------------- testing strcpy-----------------------*/
	/* testing str1 and str2 */
	printf("\nstrcpy (%s, %s) ", str1 , str2);
	printf(" expected: %s result: %s", strcpy (str1, str2) ,strcpyr(str1c, str2c));
	/* testing str3 and the new str1 */
	printf("\nstrcpy (%s, %s) ", str1 , str3);
	printf(" expected: %s result: %s\n", strcpy (str1, str3) ,strcpyr(str1c, str3));

	/*-------------------- testing strcat----------------------------*/
	/* initializing vars */
	strcpy (src, "Hello World");
	strcpy (srcc, "Hello World");
	strcpy (dest, "aaa");
	strcpy (destc, "aaa");
	printf("starcat src: %s, dest: %s", src, dest);
	printf(" expected: %s", strcat(dest,src));
	printf(" result: %s\n\n", strcatr(destc, srcc));

	/*-------------------- testing strstr----------------------------*/
	/* testing src vs dest */
	printf("strstr hay: %s, niddle: %s ", src, dest);
	printf(" expected: %s, result: %s\n", strstr(src,dest), strstrr(src,dest));
	/* testing str2 vs str3 */
	printf("strstr hay: %s, niddle: %s ", str2, str3);
	printf(" expected: %s, result: %s\n", strstr(str2,str3),strstrr(str2,str3));
	/* testing str2 vs "" */
	printf("strstr hay: %s, niddle: %s ", str2, "");
	printf(" expected: %s, result: %s\n", strstr(str2, ""), strstrr(str2, ""));
	/* testing str2 vs "" */
	printf("strstr hay: %s, niddle: %s ", str2, "c");
	printf(" expected: %s, result: %s\n", strstr(str2, "c"), strstrr(str2, "c"));

	printf("str \"ddd\"? \"%s\"\n", strstrr("ddd", "d"));
	printf("str \"(null)\"? \"%s\"\n", strstrr("ddd", "a"));
	printf("str \"a\"? \"%s\"\n", strstrr("ddda", "a"));
	printf("str \"ddd\"? \"%s\"\n", strstrr("ddd", ""));

	printf("str \"dcd\"? \"%s\"\n", strstrr("ddcd", "dc"));
	printf("str \"(null)\"? \"%s\"\n", strstrr("ddcd", "a"));
	printf("str \"a\"? \"%s\"\n", strstrr("ddcda", "a"));
	printf("str \"\"? \"%s\"\n", strstrr("", ""));

	puts("=====================================");

	printf("str \" Tree\"? \"%s\"\n", strstrr("Begin Tree", " "));
	printf("str \"in Tree\"? \"%s\"\n", strstrr("Begin Tree", "in"));
	printf("str \"egin Tree\"? \"%s\"\n", strstrr("Begin Tree", "e"));
	printf("str \"ee\"? \"%s\"\n", strstrr("Begin Tree", "ee"));
	printf("str \"(null)\"? \"%s\"\n", strstrr("Begin Tree", "eee"));
	printf("str \"(null)\"? \"%s\"\n", strstrr("Begin Tree", "eeee"));
	printf("str \"abc\"? \"%s\"\n", strstrr("aaababc", "abc"));


	return (0);
}
