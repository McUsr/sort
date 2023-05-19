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
#include "ioN.h"
/*
 * STRATEGIES:
 * 
 * Reunderstand the code, and build a way it can work properly.
 *
 * I'm thinking of having a preallocated array outside of the read line function, much like the
 * the ineptr array, the thing is, that I'd rather not declare any too big size, what I could do, was to declare
 * a block, -> an array with calloc? then I could treat it as I wanted?
 *
 */
static char t1[MAXLEN], t2[MAXLEN] ;
/* Linebuffers for altering contents before comparision of strings*/


char *alloc(int);

#define MAXLINES 5000

char* *fp[MAXLINES];
char *lineptr[MAXLINES];

static int mygetline(char *s, int lim) ;
void creat_fparr(char * *fparr[], int rows, int lastfield) ;


#define HANOI 1
#ifdef HANOI
int fv[MAXF] ;      /* ??? really? numerical equivalent of arguments. */
/* old way, which we have chipped in our queue for */
int main(int argc, char **argv)
{
    extern char* *fp[MAXLINES];
    extern char *lineptr[MAXLINES];

    extern int fv[MAXF] ;      /* ??? really? numerical equivalent of arguments. */

    FILE *in=stdin ;
    int nlines ;
    creat_fparr(fp, MAXLINES, 6);

    /* next test is to check out some fields instead of writelines */
    if ((nlines = readlinesN(lineptr,fp, MAXLINES,5)) >= 0) {
        /* readlines0: here? */
        /* TODO: pick correct cmp0 routine here */
#if 1 == 0
        for(int k=0;k<4;k++)
            printf("%d:%s%s",k,fp[0][k],(k<4)? " " : "\n"); 
            printf("how about this: %s",fp[0][4]); 
        return 0;
        --nlines ;
        for(int k=0;k<nlines;k++)
            printf("%s\n",retf( 1 , 5, fp, k, nlines) );

#endif
        writelinesN(lineptr,fp, nlines,5);


    } else {
        printf("input too big to sort nlines == %d\n",nlines);
        return 1;
    }


}
#endif

void creat_fparr(char * *fparr[], int rows, int lastfield)
{
    register int i;
    for(i=0;i<rows;i++) {
       fparr[i] = (char **) calloc(lastfield, sizeof(char *) );
       if( fparr[i] == NULL ) {
          fprintf(stderr,"create_fparr: error, couldn't calloc mem.");
          exit(1) ;
       } 
    }
}

/* ref: returns a field, from a line
 * It is more of a proof of concept, than anything else, as I think we-re going 
 * to do it directly in the comparisionn functions, maybe by an array for further
 * indirection.
 */

char * retf(  int fieldno, int lastfield, char* *fparr[], int row, int maxline )
{
    if(fieldno <1  ||  fieldno > lastfield ){
        fprintf(stderr,"retf: fieldno out of range: 1..%d was: %d\n",
                lastfield,fieldno); 
        exit(2);
    } else if(row <0  ||  row >maxline ){
        fprintf(stderr,"retf: row out of range: 1..%d was: %d\n",
                maxline,row); 
        exit(2);
    } else {
        return fparr[row][--fieldno];
    }
}

#if 1 == 0
/* output field n from the current line */
void putf( int n, int lastfield) 
{
    register char *cp = fp[n] ;             /* we get a pointer into the field pointer array for this field. */
    register char c;                        /* the pointer returned points into the line buf. */
    if(n<0 || n >=lastfield) return ;
    while(c = *cp++) putchar(c) ;
} 
#endif

int readlinesN(char *lineptr[], char * *fparr[], int maxlines, int lastfield)
{
    /* lastfield, so we don't overdo it */
	int mygetline(char *, int);

	int len, nlines;
	char *L, line[MAXLEN];

    register char *cp;
    register char **ap;
    register int c;
    register int fc;
	nlines = 0;
	while ((len = mygetline(line, MAXLEN)) > 0)
		if (nlines >= maxlines || (L = alloc(len)) == NULL)
			return -1; /* out of mem on both accounts! */
		else {
            L = strdup(line);

			lineptr[nlines] = L; /* REMOVED postincrement here! */
            /* fill fparr with pointers to first char of field */
        
            cp = L ;                /* address of L[0] */
            ap = fparr[nlines] ;    /* address of fld_ptr[0] on current line ! */
            *ap++= cp ;             /* fparr[nlines][0] == &L[0] */

            fc=1;                   /* just registered the first field */

            while(1) {
                c = *cp; 
                if(c=='\n' || c==EOF ) {            /* end of current line! */
                    if(cp==L && c==EOF )
                        break;     /* empty last line. cp == L == &L[0] */
                    else if (c==EOF)
                        --fc ;
                    *cp++ = '\0';                      /* We put a NULL into the current pos of L */                  
                    ++nlines ;
                    break ;            /* end of file, but we did put out the last field first! */
                }
                else if(c == IFS) {
                    *cp++ = 0 ;                     /* we put NULL into the Linebuf */
                    *ap++ = cp;                     /* we put the pointer to the linbuf into the field pointer array and points
                                                     * to the new slot. */
                    if(++fc > (lastfield-1)) break ;
                } else ++cp ;
            }
            *ap=0;
            if (c == EOF ) break ;
            ++nlines;
		}
	return nlines;
}

void writelinesN(char *lineptr[], char * *fparr[], int nlines, int lastfield )
{
    register int i;
    i=0;

	while (--nlines > 0) {
        /* rebuild the line with OFS where we put 0's */
        register int j;
        for(j=1;j<=(lastfield-1);j++)
            if((fparr[i][j]) != NULL)
                *(fparr[i][j]-1) = OFS ;
        ++i;
		printf("%s", *lineptr++);
    } 
}

static int mygetline(char *s, int lim)
{
	int c;
	char *t = s;
    /* printf("inside getline\n"); */
	while (--lim > 0 && (c = getchar()) != EOF && c != '\n')
		*t++ = c;

	if ((c == '\n') || (c==EOF))
		*t++ = c;

	*t = '\0';
    /* the new line gets stored for sure */
	return t - s;
}

int (*get_cmpN(OptionsPtr global_opts))(void*, void*) {

    int (*cmpfuncN)(void*, void*) = NULL ;
                
    switch (global_opts->method ) {
        case LEX_METH: 
            if( global_opts->reverse && global_opts->folding )
                cmpfuncN =  (int (*)(void*, void*))r_cmpfoldN ;
            else if (global_opts->reverse )
                cmpfuncN =  (int (*)(void*, void*))r_strcmpN; 
            else if (global_opts->folding )
                cmpfuncN =  (int (*)(void*, void*)) cmpfoldN;
            else
                cmpfuncN = (int (*)(void*, void*))strcmp ;
            break;
        case NUM_METH:
            if( global_opts->reverse )
                cmpfuncN = (int (*)(void*, void*))r_numcmpN ;
            else
                cmpfuncN = (int (*)(void*, void*))numcmpN; 
            break;
        case DICT_METH:
            if( global_opts->reverse && global_opts->folding )
                cmpfuncN = (int (*)(void*, void*)) r_cmpdir_foldN;
            else if (global_opts->reverse )
                cmpfuncN = (int (*)(void*, void*))r_cmpdirN; 
            else if (global_opts->folding )
                cmpfuncN = (int (*)(void*, void*)) cmpdir_foldN ;
            else 
                cmpfuncN = (int (*)(void*, void*))cmpdirN; 
            break;
        default:
            error("Bad constant for sort method!");
    }

    return cmpfuncN ;
}

void qsortN(void *v[], int left, int right,
			int (*comp)(void *, void *))
{
	int i, last;
	/* void swap(void *v[], int, int); */
    void swapN(void *v[], int i, int j) ;

	if (left >= right)
		return;
	swapN(v, left, (left + right)/2);
	last = left;
	for (i = left+1; i <= right; i++)
		if ((*comp)(v[i], v[left]) < 0)
			swapN(v, ++last, i);
	swapN(v, left, last);
	qsortN(v, left, last-1, comp);
	qsortN(v, last+1, right, comp);
}

void swapN(void *v[], int i, int j)
{
	void *temp;
	temp = v[i];
	v[i] = v[j];
	v[j] = temp;
}

int numcmpN(const char *s1, const char *s2)
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

int r_numcmpN(const char *s1, const char *s2)
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

int r_strcmpN(const char *s1, const char *s2 )
{
    return (strcmp(s1,s2) * -1 ) ;
}

/* this function is made, so that 
 * the input is just sorted in a folded
 * fashion, but otherwise displayed as normal
 *
 * We need the MAXLEN line constant!
 */
int cmpfoldN( const char *s1, const char *s2 )
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

int r_cmpfoldN( const char *s1, const char *s2 )
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

/* cmpdirN: ONLY compares alnum and blanks in the strings */
int cmpdirN(const char *s1, const char *s2)
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
/* r_cmpdirN: ONLY compares alnum and blanks, reverse order */
int r_cmpdirN(const char *s1, const char *s2)
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


int cmpdir_foldN(const char *s1, const char *s2)
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

int r_cmpdir_foldN(const char *s1, const char *s2)
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

            
