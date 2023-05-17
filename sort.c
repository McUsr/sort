#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "sort.h"
#include "io.h"
#include "options.h"
#include "dequeue.h"
/* sort.c 
 * K&R exercises 5.14 - 5.17
 *
 * I have done all at once: not starting
 * before I had a plan for exercise 5.17
 */
#define MAXLINES 5000
char *lineptr[MAXLINES];
static char t1[MAXLEN], t2[MAXLEN] ;

int numcmp0(char *s1, char *s2);
int r_numcmp0(char *s1, char *s2);
int r_strcmp0(char *s1, char *s2 );
int cmpfold0( char *s1, char *s2 );
int r_cmpfold0( char *s1, char *s2 );

/* qsort: a reverse engineered one! */
void qsort0(void *v[], int left,
        int right, int (*comp)(void *, void *));

int main(int argc, char **argv)
{
    FILE *in=stdin ;
    int processed=0; 
    int max_field=0;
    int nlines;
    OptionsPtr global_opts=initFieldRec();

    char **ap = parse_opts(&argc, argv,&max_field, global_opts) ;
    printf("Max fields set: %d\n",max_field );
    printf("Global options set!");
    printOptions( global_opts) ;

    if (max_field > 0 ) {
        while (!isEmpty(*headPtr)) {
            printf("\nthe queue isn't empty!\n");
            QueueNodePtr nextNode = dequeue(headPtr) ;
            OptionsPtr  nextField = (OptionsPtr) nextNode->data ;
            int curfno = nextField->fieldno ;
            printf(" options for field nr %d\n",curfno) ;
            printOptions(nextField) ;
            free(nextNode) ;
        }
        printf("the queue is empty!\n");
    }

    /* setting up the processing of the input...for sorting! */
        

        if ( argc > 1 ) {
            /* current convention is, is that ap holds the current arg
             * after exiting parse_opts */
            if ( (in = fopen(*ap,"r") ) == NULL ) {
                fprintf(stderr,"fileargs1: Can't open %s\n",*argv );
                exit(1) ;
            }
        }

        if (max_field == 0 ) {
            /* reads in the file in a simple fashion and sorts */
            if ((nlines = readlines0(lineptr, MAXLINES)) >= 0) {
                /* readlines0: here? */
                /* TODO: pick correct cmp0 routine here */

                int (*cmpfunc0)(void*, void*) = NULL ;
                
                switch (global_opts->method ) {
                    case LEX_METH: 
                        if( global_opts->reverse && global_opts->folding )
                            /* TODO */
                            cmpfunc0 =  (int (*)(void*, void*))r_cmpfold0 ;
                        else if (global_opts->reverse )
                            cmpfunc0 =  (int (*)(void*, void*))r_strcmp0; 
                        else if (global_opts->folding )
                            /* TODO */
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
                            /* TODO */
                            ;
                        else if (global_opts->reverse )
                            /* TODO */ 
                            ;
                        else if (global_opts->folding )
                            /* TODO */
                            ;
                        else {
                            /* TODO */
                            ;
                        }
                        break;
                        ;
                }
                        
                qsort0((void **) lineptr, 0, nlines-1,
                    (int (*)(void*, void*))cmpfunc0);

                writelines0(lineptr, nlines);
                return 0;
            } else {
                printf("input too big to sort nlines == %d\n",nlines);
                return 1;
            }
        } else {
            
            /* reads in the file in a field wise fashion and sorts */
            while (!isEmpty(*headPtr)) {

            }
            /* post processes the sorted file and prints the file to stdout.*/
        }

	return 0;
}

/* TODO: Rework, to just select the correct sort function up front,
 * without anything else going on inside qsort.
 */


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

int numcmp0(char *s1, char *s2)
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

int r_numcmp0(char *s1, char *s2)
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

int r_strcmp0(char *s1, char *s2 )
{
    return (strcmp(s1,s2) * -1 ) ;
}

/* this function is made, so that 
 * the input is just sorted in a folded
 * fashion, but otherwise displayed as normal
 *
 * We need the MAXLEN line constant!
 */
int cmpfold0( char *s1, char *s2 )
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

int r_cmpfold0( char *s1, char *s2 )
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

