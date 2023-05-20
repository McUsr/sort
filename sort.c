#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sort.h"
#include "options.h"
#include "io0.h"
#include "ioN.h"
#include "dequeue.h"
/* sort.c 
 * K&R exercises 5.14 - 5.17
 *
 * I have done all at once: not starting
 * before I had a plan for exercise 5.17
 */
char *lineptr[MAXLINES];
char* *fp[MAXLINES];


int main(int argc, char **argv)
{
    FILE *in=stdin ;
    int processed=0; 
    int max_field=0;
    int nlines;
    OptionsPtr global_opts=initFieldRec();

    char **ap = parse_opts(&argc, argv,&max_field, global_opts) ;
    /* setting up the processing of the input...for sorting! */
        

        if ( argc > 1 ) {
            /* current convention is, is that ap holds the current arg
             * after exiting parse_opts */
            if ( (in = fopen(*ap,"r") ) == NULL ) {
                fprintf(stderr,"sort: fileargs1: Can't open %s\n",*argv );
                exit(1) ;
            }
        }

        if (max_field == 0 ) {
            /* reads in the file in a simple fashion and sorts */
            if ((nlines = readlines0(lineptr, MAXLINES)) >= 0) {

                int (*cmpfunc)(void*, void*) = get_cmp( global_opts) ;
                        
                qsort0((void **) lineptr, 0, nlines-1,
                    (int (*)(void*, void*))cmpfunc);

                writelines0(lineptr, nlines);
                return 0;
            } else {
                fprintf(stderr,"sort: error: input too big to sort nlines == %d\n",
                        nlines);
                return 1;
            }
        } else {
                creat_fparr(fp, MAXLINES, (max_field+1));
                /* makes an array of pointers to char (field pointers)
                 * in the corresponding linebuffer lineptr[nlines] */
                if ((nlines = readlinesN(lineptr,fp, MAXLINES, max_field)) >= 0) {
                /* reads in the file in a field wise fashion and sorts */

                    while (!isEmpty(*headPtr)) {
                        QueueNodePtr nextNode = dequeue(headPtr) ;
                        OptionsPtr  nextField = (OptionsPtr) nextNode->data ;
                        int curfno = nextField->fieldno - 1;
#if 1 == 0
                        printOptions(nextField) ;
#endif 
                        int (*cmpfunc)(void*, void*) = get_cmp( nextField) ;
                        free(nextField) ;
                        free(nextNode) ;

                        qsortN2((void **) lineptr, fp , 0, nlines-1,
                                curfno, (int (*)(void *, void *))cmpfunc);
                    }
                    /* post processes the sorted file and prints the file to stdout.*/
                    writelinesN(lineptr,fp, nlines,max_field);

                } else {
                    fprintf(stderr,"sort:  error: input too big to sort nlines == %d\n",
                            nlines);
                    return 1;
                }
        }

	return 0;
}
