#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sort.h"
#include "options.h"
#include "io0.h"
#include "dequeue.h"
/* sort.c 
 * K&R exercises 5.14 - 5.17
 *
 * I have done all at once: not starting
 * before I had a plan for exercise 5.17
 */
#define MAXLINES 5000
char *lineptr[MAXLINES];


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

                int (*cmpfunc0)(void*, void*) = get_cmp0( global_opts) ;
               /* 
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
                */
                        
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
