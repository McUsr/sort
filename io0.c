/* This is the module for reading in a file from stdin when no fields are
 * specified, aka awk's $0 concept for the whole record, which in this case is
 * a line.  The whole thing is per today pretty much exactly as the examples
 * are in K&R, with the exercises 5.14, 5.15 and 5.16 done.
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "options.h"
#include "io0.h"

static char t1[MAXLEN], t2[MAXLEN] ;
/* Linebuffers for altering contents before comparision of strings*/

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

int (*get_cmp(OptionsPtr global_opts))(void*, void*) {

    int (*cmpfunc)(void*, void*) = NULL ;
                
    switch (global_opts->method ) {
        case LEX_METH: 
            if( global_opts->reverse && global_opts->folding )
                cmpfunc =  (int (*)(void*, void*))r_cmpfold ;
            else if (global_opts->reverse )
                cmpfunc =  (int (*)(void*, void*))r_strcmp; 
            else if (global_opts->folding )
                cmpfunc =  (int (*)(void*, void*)) cmpfold;
            else
                cmpfunc = (int (*)(void*, void*))strcmp ;
            break;
        case NUM_METH:
            if( global_opts->reverse )
                cmpfunc = (int (*)(void*, void*))r_numcmp ;
            else
                cmpfunc = (int (*)(void*, void*))numcmp; 
            break;
        case DICT_METH:
            if( global_opts->reverse && global_opts->folding )
                cmpfunc = (int (*)(void*, void*)) r_cmpdir_fold;
            else if (global_opts->reverse )
                cmpfunc = (int (*)(void*, void*))r_cmpdir; 
            else if (global_opts->folding )
                cmpfunc = (int (*)(void*, void*)) cmpdir_fold ;
            else 
                cmpfunc = (int (*)(void*, void*))cmpdir; 
            break;
        default:
            error("Bad constant for sort method!");
    }

    return cmpfunc ;
}

void qsort0(void *v[], int left, int right,
			int (*comp)(void *, void *))
{
	int i, last;
	/* void swap(void *v[], int, int); */
    void swap0(void *v[], int i, int j) ;

	if (left >= right)
		return;
	swap0(v, left, (left + right)/2);
	last = left;
	for (i = left+1; i <= right; i++)
		if ((*comp)(v[i], v[left]) < 0)
			swap0(v, ++last, i);
	swap0(v, left, last);
	qsort0(v, left, last-1, comp);
	qsort0(v, last+1, right, comp);
}

void swap0(void *v[], int i, int j)
{
	void *temp;
	temp = v[i];
	v[i] = v[j];
	v[j] = temp;
}

int numcmp(const char *s1, const char *s2)
{
	double v1, v2;

	v1 = atof(s1);
	v2 = atof(s2);
	if (v1 < v2)
		return -1;
	else if (v1 > v2)
		return 1;
	else
		return 0;
}

int r_numcmp(const char *s1, const char *s2)
{
	double v1, v2;

	v1 = atof(s1);
	v2 = atof(s2);
	if (v1 < v2)
		return 1;
	else if (v1 > v2)
		return -1;
	else
		return 0;
}

int r_strcmp(const char *s1, const char *s2 )
{
    return (strcmp(s1,s2) * -1 ) ;
}

/* this function is made, so that 
 * the input is just sorted in a folded
 * fashion, but otherwise displayed as normal
 *
 * We need the MAXLEN line constant!
 */
int cmpfold( const char *s1, const char *s2 )
{
    extern char t1[MAXLEN], t2[MAXLEN] ;
    char *u1, *u2;
    u1=t1;
    u2=t2;

    while ((*u1++ = toupper(*s1++)))
        ;

    while ((*u2++ = toupper(*s2++)))
        ;
    u1=t1;
    u2=t2;
    return(strcmp(u1,u2));
}

int r_cmpfold( const char *s1, const char *s2 )
{
    extern  char t1[MAXLEN], t2[MAXLEN] ;
    char *u1, *u2;
    u1=t1;
    u2=t2;

    while ((*u1++ = toupper(*s1++)))
        ;

    while ((*u2++ = toupper(*s2++)))
        ;
    u1=t1;
    u2=t2;
    return((strcmp(u1,u2) * -1));
}

/* cmpdir: ONLY compares alnum and blanks in the strings */
int cmpdir(const char *s1, const char *s2)
{
    extern  char t1[MAXLEN], t2[MAXLEN] ;
    char *u1, *u2;
    u1=t1;
    u2=t2;

    for (;*s1;s1++) 
        if( isalnum(*s1) || isblank(*s1) )
            *u1++ = *s1;
    *u1=*s1;

    for (;*s2;s2++) 
        if( isalnum(*s2) || isblank(*s2) )
            *u2++ = *s2;
    *u2=*s2;
   
    u1=t1;
    u2=t2;

    return (strcmp(u1,u2));
}
/* r_cmpdir: ONLY compares alnum and blanks, reverse order */
int r_cmpdir(const char *s1, const char *s2)
{
    extern  char t1[MAXLEN], t2[MAXLEN] ;
    char *u1, *u2;
    u1=t1;
    u2=t2;

    for (;*s1;s1++) 
        if( isalnum(*s1) || isblank(*s1) )
            *u1++ = *s1;
    *u1=*s1;

    for (;*s2;s2++) 
        if( isalnum(*s2) || isblank(*s2) )
            *u2++ = *s2;
    *u2=*s2;
   
    u1=t1;
    u2=t2;

    return (strcmp(u1,u2)*-1);
}


int cmpdir_fold(const char *s1, const char *s2)
{
    extern  char t1[MAXLEN], t2[MAXLEN] ;
    char *u1, *u2;
    u1=t1;
    u2=t2;

    for (;*s1;s1++) 
        if( isalnum(*s1) || isblank(*s1) )
            *u1++ = toupper(*s1);
    *u1=*s1;

    for (;*s2;s2++) 
        if( isalnum(*s2) || isblank(*s2) )
            *u2++ = toupper(*s2);
    *u2=*s2;
   
    u1=t1;
    u2=t2;

    return (strcmp(u1,u2));

}

int r_cmpdir_fold(const char *s1, const char *s2)
{
    extern  char t1[MAXLEN], t2[MAXLEN] ;
    char *u1, *u2;
    u1=t1;
    u2=t2;

    for (;*s1;s1++) 
        if( isalnum(*s1) || isblank(*s1) )
            *u1++ = toupper(*s1);
    *u1=*s1;

    for (;*s2;s2++) 
        if( isalnum(*s2) || isblank(*s2) )
            *u2++ = toupper(*s2);
    *u2=*s2;
   
    u1=t1;
    u2=t2;

    return (strcmp(u1,u2)*-1);
}

/* K&R p. 101-102 */
/* pointer arithmetic demo */
#define ALLOCSIZE 65536 /* size of available space */

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

            
