#include "options.h"
#include <stdio.h>
#include <stdlib.h>
#include "sort.h"
#include "dequeue.h"
/* sort.c 
 * K&R exercises 5.14 - 5.17
 *
 * I have done all at once: not starting
 * before I had a plan for exercise 5.17
 */


int main(int argc, char **argv)
{
    int max_field=0;
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
    do {

    } while (argc > 0) ;
        

	return 0;
}
