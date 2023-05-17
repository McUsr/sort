#include <stdio.h>
#include <string.h>
#include "io.h"

/* This is the module for reading in a file from stdin
 * and prepare pointers for each line of input to make
 * it easy to sort by fields, when any fields are specified
 * in sort's arguments.
 *
 * the whole concept is stolen from Stephen R. Bourns book:
 * "The Unix(tm) System V Environment" pp. 226.
 * ISBN: 0 201 18484 2
 *
 *
 */

char *alloc(int);

int readlines0(char *lineptr[], int maxlines)
{
	int mygetline(char *, int);

	int len, nlines;
	char *p, line[MAXLEN];
	nlines = 0;
	while ((len = mygetline(line, MAXLEN)) > 0)
		if (nlines >= maxlines || (p = alloc(len)) == NULL)
			return -1;
		else {
			line[len-1] = '\0';
			strcpy(p, line);
			lineptr[nlines++] = p;
		}

	return nlines;
}

void writelines0(char *lineptr[], int nlines)
{
	while (nlines-- > 0)
		printf("%s\n", *lineptr++);
}

int mygetline(char *s, int lim)
{
	int c;
	char *t = s;
	while (--lim > 0 && (c = getchar()) != EOF && c != '\n')
		*t++ = c;

	if (c == '\n')
		*t++ = c;

	*t = '\0';
	return t - s;
}



/* K&R p. 101-102 */
/* pointer arithmetic demo */
#define ALLOCSIZE 100000 /* size of available space */

static char allocbuf[ALLOCSIZE] ; /* Storage for alloc */
static char *allocp = allocbuf ;

char *alloc(int n) /* return pointer to n characters */
{
    if (allocbuf + ALLOCSIZE - allocp >= n ) { /* it fits */
        allocp +=n ;
        return allocp -n ; /* old p */
    } else /* not enough room */
        return 0;
}

void afree( char *p ) /* free storage pointed to by p */
{
    if (p >= allocbuf && p < allocbuf + ALLOCSIZE)
       allocp = p ;
} 

            
