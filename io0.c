

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

int (*get_cmp0(OptionsPtr global_opts))(void*, void*) {

    int (*cmpfunc0)(void*, void*) = NULL ;
                
    switch (global_opts->method ) {
        case LEX_METH: 
            if( global_opts->reverse && global_opts->folding )
                cmpfunc0 =  (int (*)(void*, void*))r_cmpfold0 ;
            else if (global_opts->reverse )
                cmpfunc0 =  (int (*)(void*, void*))r_strcmp0; 
            else if (global_opts->folding )
                cmpfunc0 =  (int (*)(void*, void*)) cmpfold0;
            else
                cmpfunc0 = (int (*)(void*, void*))strcmp ;
            break;
        case NUM_METH:
            if( global_opts->reverse )
                cmpfunc0 = (int (*)(void*, void*))r_numcmp0 ;
            else
                cmpfunc0 = (int (*)(void*, void*))numcmp0; 
            break;
        case DICT_METH:
            if( global_opts->reverse && global_opts->folding )
                cmpfunc0 = (int (*)(void*, void*)) r_cmpdir_fold0;
            else if (global_opts->reverse )
                cmpfunc0 = (int (*)(void*, void*))r_cmpdir0; 
            else if (global_opts->folding )
                cmpfunc0 = (int (*)(void*, void*)) cmpdir_fold0 ;
            else 
                cmpfunc0 = (int (*)(void*, void*))cmpdir0; 
            break;
        default:
            error("Bad constant for sort method!");
    }

    return cmpfunc0 ;
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

int numcmp0(const char *s1, const char *s2)
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

int r_numcmp0(const char *s1, const char *s2)
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

int r_strcmp0(const char *s1, const char *s2 )
{
    return (strcmp(s1,s2) * -1 ) ;
}

/* this function is made, so that 
 * the input is just sorted in a folded
 * fashion, but otherwise displayed as normal
 *
 * We need the MAXLEN line constant!
 */
int cmpfold0( const char *s1, const char *s2 )
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

int r_cmpfold0( const char *s1, const char *s2 )
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

/* cmpdir0: ONLY compares alnum and blanks in the strings */
int cmpdir0(const char *s1, const char *s2)
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
/* r_cmpdir0: ONLY compares alnum and blanks, reverse order */
int r_cmpdir0(const char *s1, const char *s2)
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


int cmpdir_fold0(const char *s1, const char *s2)
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

int r_cmpdir_fold0(const char *s1, const char *s2)
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

            
