/* this is the parsing of options module. 
 * with the documentation of the syntax
 *
 * Organization of options-data:
 * We have one record with global options,
 * and we use an deque for options concerning individual fields.
 * We have a limit on 255 fields since field nr 0 is the global fields
 * however, since a field may appear more than once in our field list
 * we don't impose any limitiations on the number of *times* a field occur
 * in the list of field sorting options. So, for nothing else, a deque is dynamic
 * structure, without having to allocate unknown quantities of space up front,
 * which most probably is unnecessary.
 *
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "sort.h"
#include "dequeue.h"
#include "options.h"

#ifdef CAMBODIA
int fv[MAXF]={0} ;      /*  The fields we will sort the input on
                            It is mostly for communicating to readline
                            Maybe a need to parse input into fields would be
                            a better option.
                            
                        */

int parse_fields=0;
#endif

#define HANOI
#ifdef HANOI
char **parse_opts(int *argc,char **argv, int* max_field ) ;

int main(int argc, char **argv)
{
    int max_field=0;
    printf("Argc up front == %d\n",argc);
    char **ap = parse_opts(&argc, argv,&max_field) ;
	printf("Argc after  == %d, Argv == %s\n",argc, *ap);
	return 0;
}
#endif

void show_help();
void error( char *s ) ;

OptionsPtr initFieldRec(void) ;
/* not the first parsing of arguments, first we check for help
 * then we show help and we die without further ado in main.
 */

/* parse_opts: parses all command line options concerning sorting from the
 * command line, factored out from main to make for an easy main to read. 
 *
 * If fields were specified, we fill up a dequeue with options for the specific
 * fields.  We also set a max field, which serves the dual purpose of
 * signifying the number of fields we need to mark as fields when parsing
 * input, and also to signal that we indeed have data in our dequeue
 * (max_field * > 0 ).
 *
 * We pass back any remnants of the command line arguments, and the remaining
 * argc for further processing of file name arguments in main(). 
 */

char **parse_opts(int *argc,char **argv, int* max_field )
{
    int fieldnr;
    char optstr[10]={0};
    int spec_fields=FALSE, global_opts_pending=FALSE ;
    OptionsPtr curRec=NULL, globalRec=NULL;

    printf("spec_fields == %d\n",spec_fields );

   if ( *argc > 1 && ((strcmp(argv[1],"-h") == 0 ) || (strcmp(argv[1],"--help") == 0 ))) {
           show_help();
           exit(0);
   } else if (*argc > 1 ) {
       /* might be some options to process here */
       while (--*argc > 0 ) { /* *argv[0] */
            ++argv;
           if (strcmp(*argv,"--")== 0 ) {
               printf("end of options!\n");
               --*argc;
               argv++;
               break ;
           } else if ((*argv)[0] == '-' ) {
               ++(*argv) ;
               /* printf(" cur arg == %s\n",*argv); */
               if (sscanf(*argv,"%3d%3[rfdn]", &fieldnr,optstr) > 0) {
                   if (fieldnr == 0 ) 
                        /* we need field nr 0 to specify the whole line, aka no fields */
                        error("specified field outside of range: 1 ..255");
                   else if(fieldnr >= MAXF ) 
                        /* we need field nr 0 to specify the whole line, aka no fields */
                        error("specified field outside of range: 1 ..255");
                   else {

                       if (global_opts_pending==TRUE) {
                           /* enque the stuff */
                           global_opts_pending=FALSE;
                       }
                       curRec = initFieldRec();

                       printf("We got a field value: %d\n",fieldnr );
                       /* we need to specify max field val */
                       if(spec_fields == FALSE )
                           spec_fields = TRUE ;
                        /* printf("found a possibly valid fieldnr, value == %d\n",fieldnr) ; */

                    }
                    if (strlen(optstr) > 0 ) {
                       printf("options == %s\n", optstr) ;
                       /* needs to check if there are any other stuff left */
                    }
                    /* this is the place we enque the current field record */
               } else if (sscanf(*argv,"%3[rfdn]", optstr) > 0) {
                    if ( spec_fields == TRUE )
                            error("once a field is specified subsequent arguments needs one.");
                    else {
                        global_opts_pending = TRUE ;
                        if (curRec == NULL ) {
                            printf("curRec == NULL\n") ;
                            curRec = initFieldRec();
                        }
                        printf("we'll add to the first record\n");
                    }
               
               } else {
                   printf(" %s didn't contain a valid argument optsr = %s\n",*argv,optstr);
               }
                printf("End condition: argc == %d, argv = %s\n",*argc,*argv);
               /* end condition */
                if (*argc==1 )
                    argv++;
           }
           else 
               break;

           optstr[0]='\0';
           fieldnr = 0;
       }
       if (global_opts_pending==TRUE) {
           /* enque the stuff */
       }
   } else {
       --*argc;
       argv++ ;
       printf("here...argc == 1 \n"); 
   }
   printf("argc == %d\n",*argc );

    return argv;
}

#ifdef LAOS

/*
 * We only enqueue a record when:
 *
 *   - we have processed field,
 *   - we are out of options
 *   - we are out of arguments.
 *
 */
char **parse_opts(int *argc,char **argv, int* max_field )
{
    /* when are we done processing a field?
     * when a new field turns up to be processed
     * we know its a new field, because it starts with a number
     * after the hyphen, or just a a number starting with a hyphen is best.
     * every option needs to be in the group of the field?
     * all the parts for a  field is there.
     *
     * no number yet, 
     */

    OptionsPtr curRec = initFieldRec();

    int proc_opts = FALSE;
    int in_proc   = FALSE ;

	while ( *argc > 0  ) {
        if ((*argv)[0] == '-' ) {
            in_proc = TRUE ;

            printf("valid argument\n");
            /* break; */
        }
        else 
            printf("--> %c\n",(*argv)[0]);

		printf("!! argc == %d argv= %s\n",(*argc)--,*(argv++));
		/* printf("argc == %ds\n",(*argc)--); */
	}

    if (!proc_opts)
        enqueue(&tailPtr,(void *)curRec) ;


    return argv;
}
#endif

OptionsPtr initFieldRec(void)
{
    OptionsPtr fieldRec;
    if((fieldRec = (OptionsPtr) malloc(sizeof(Options)))!= NULL ) {
        /* fieldRec = (Options) { .method =LEX_METH, .folding=0, .reverse=0,  .fieldno = 0 } ; */
        memset(&(*fieldRec), 0, sizeof(Options));
    } else
        fprintf(stderr,"parse_opts: couldn't allocate mem for fieldRec\n") ;
    return fieldRec;
}

void printOptions( OptionsPtr aRec )
{
    printf("Field/Options record: \n" ) ;

    switch (aRec->method) {
        case DICT_METH:
            printf("Dictionary Order.\n");
            break;
        case NUM_METH:
            printf("Numerical order.\n");
            break;
        case LEX_METH:
            printf("Lexical order.\n");
            break;
        default:
            printf("BADABOOM, can't happen!");
            break;
    }

    if ( aRec->folding )
        printf("shall fold case\n");
    else
        printf("shan't fold case\n");

    if ( aRec->reverse )
        printf("shall sort reverse \n");
    else
        printf("shan't sort reverse\n");

    printf("current field number: %d\n", aRec->fieldno);
}

void error( char *s ) 
{
    fprintf(stderr,"\n\033[1msort\033[0m: error: %s\n",s);
    exit(2) ;
}    

/* show_help: print help and die */
void show_help()
{
    printf("\033[1msort\033[0m: sort standard input or files to stdout\n"
            "\n\nSyntax\n\n"
            "\033[1msort\033[0m [-r -n -d -f ] | [ -N[r,n,d,f] ... ] ...\n"
          );
}

