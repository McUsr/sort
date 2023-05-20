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

    if(strcmp(fp[2][1],fp[3][1]) > 0) 
        printf("needs to swap! %s and %s\n",fp[2][1],fp[3][1]) ;

        /* printf("numlines = %d\n",nlines); */
        /* return 0; */

    return 0 ;
#endif
    /* observe: fieldnr decremented up front. */
    qsortN2((void **) lineptr, fp , 0, nlines-1,
			4, (int (*)(void *, void *))strcmp);
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

/* retf: returns a field, from a line
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
            ap = fparr[nlines++] ;    /* address of fld_ptr[0] on current line ! */
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
                    break ;            /* end of file, but we did put out the last field first! */
                }
                else if(c == IFS) {
                    *cp++ = 0 ;                     /* we put NULL into the Linebuf */
                    *ap++ = cp;                     /* we put the pointer to the linbuf into the field pointer array and points
                                                     * to the new slot. */
                    if(++fc > (lastfield-1)) break ;
                } else ++cp ;
            }
            if (c == EOF ) break ;

            if (fc < (lastfield-1) ) {
             /* we need to reassemble the line, print it with an errormessage
             * and reset the linenumber to over-write it.
             */
                fprintf(stderr,"sort: input error too few fields:, line nr %d rejected: ",--nlines);
                ap=fparr[nlines] ;
                while (*ap) {
                    fprintf(stderr,"%s%s",*ap, (*(ap+1)) ? "\t" : "\n");
                    ap++;
                }
            } 
            *ap=0;
		}
	return --nlines;
}

void writelinesN(char *lineptr[], char * *fparr[], int nlines, int lastfield )
{
    register int i;
    i=0;

	while (nlines-- > 0) {
        /* rebuild the line with OFS where we put 0's */
        register int j;
        for(j=1;j<=(lastfield-1);j++)
            if((fparr[i][j]) != NULL)
                *(fparr[i][j]-1) = OFS ;
            else {
                /* inserting a newline after last field
                 * when a line has too few fields
                 */
                char *tp = fparr[i][(j-1)];
                while (*tp != '\0')
                    tp++;
                *tp++ = '\n' ;
                *tp = '\0' ;
                break;
            }
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

void qsortN2(void *v[], char* *fp[], int left, int right,
			int fn, int (*comp)(void *, void *))
{
	int i, last;
    void swapN2(void *v[], char* *fp[], int i, int j) ;
	if (left >= right)
		return;
	swapN2(v,fp, left, (left + right)/2);
	last = left;
	for (i = left+1; i <= right; i++)
		if ((*comp)((void *)fp[i][fn], (void *)fp[left][fn]) < 0) 
			swapN2(v,fp, ++last, i);
	swapN2(v,fp, left, last);
	qsortN2(v,fp, left, last-1,fn, comp);
	qsortN2(v,fp, last+1, right,fn, comp);
}

void swapN2(void *v[], char* *fp[], int i, int j)
{
	void *temp;
	temp  = v[i];
	v[i]  = v[j];
	v[j]  = temp;
    temp  = fp[i];
    fp[i] = fp[j];
    fp[j] = temp ;
}
