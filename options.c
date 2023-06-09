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

static void show_help();
void set_opts( char *optstr, OptionsPtr opt_rec );


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

char **parse_opts(int *argc,char **argv, int* max_field, OptionsPtr global_opts )
/* global opts: parsed initially, until they aren't we need to know if they have
 * been, so that if field numbers are set, and the global options deviate from 
 * the defaults, those globall options are copied into the field options.
 */
{
    int fieldnr;
    char optstr[10]={0};
    int spec_fields=FALSE, spec_global=FALSE ;
    OptionsPtr cur_fld_opts=NULL;

   if ( *argc > 1 && ((strcmp(argv[1],"-h") == 0 ) || (strcmp(argv[1],"--help") == 0 ))) {
           show_help();
           exit(0);
   } else if (*argc > 1 ) {
       /* might be some options to process here */
       while (--*argc > 0 ) {
            ++argv;
           if (strcmp(*argv,"--")== 0 ) {
               --*argc;
               argv++;
               break ;
           } else if ((*argv)[0] == '-' ) {
               ++(*argv) ;
               /* looking for field numbers with options: */
               if (sscanf(*argv,"%3d%3[rfdnl]", &fieldnr,optstr) > 0) {
                   if (fieldnr == 0 ) 
                        /* we need field nr 0 to specify the whole line, aka no fields */
                        error("specified field outside of range: 1 ..255");
                   else if(fieldnr >= MAXF ) 
                        /* we need field nr 0 to specify the whole line, aka no fields */
                        error("specified field outside of range: 1 ..255");
                   else {

                       if (spec_global==TRUE) {
                           /* enque the stuff */
                           spec_global=FALSE;
                       }
                       cur_fld_opts = initFieldRec(); /* die if it doesn't work out. */
                       cur_fld_opts->fieldno = fieldnr; 
                       *max_field = max(*max_field,fieldnr);
                       /* we need to specify max field val */
                       if(spec_fields == FALSE )
                           spec_fields = TRUE ;

                    }
                    if (strlen(optstr) > 0 ) {
                       /* needs to check if there are any other stuff left */
                       set_opts( optstr, cur_fld_opts );
                    } else {
                        cur_fld_opts->method  = global_opts->method;
                        cur_fld_opts->folding = global_opts->folding;
                        cur_fld_opts->reverse = global_opts->reverse; 
                    }
                    /* this is the place we enque the current field record */

                    enqueue(&tailPtr,(void *)cur_fld_opts) ;

               } else if (sscanf(*argv,"%3[rfdnl]", optstr) > 0) {
                   /* or maybe we just got global options */
                    if ( spec_fields == TRUE )
                            error("once a field is specified subsequent arguments needs a specified field too.");
                    else {
                        spec_global = TRUE ;
                        set_opts( optstr, global_opts );
                    }
               
               } else {
                   error2(" Invalid options: didn't contain a valid argument optsr = %s\n",*argv);
               }
               /* end condition */
                if (*argc==1 )
                    argv++;
           }
           else 
               break;

           optstr[0]='\0';
           fieldnr = 0;
       }
       if (spec_global==TRUE) {
           /* enque the stuff */
       }
   } else {
       --*argc;
       argv++ ;
   }

    return argv;
}

/* set_opt: setting of sorting options */
void set_opts( char *optstr, OptionsPtr opt_rec )
/* We process one option by one, and check for any conflicts
 * against the previously set options in the options record
 * opt_rec.
 */
{
    while (*optstr != '\0') {
        /* printf("cur opt == %c\n",*optstr) ; */
        switch (*optstr) {
            case 'r' :
                opt_rec->reverse=1;
                break;
            case 'f' :
                if((opt_rec->method & NUM_METH) == NUM_METH )
                    error("We can't fold a numeric sort.");
                else
                    opt_rec->folding=1;
                break;
            case 'd' :
                if((opt_rec->method & NUM_METH) == NUM_METH )
                    error("We can't sort a numeric sort in dictionary order.");
                else
                    opt_rec->method=DICT_METH;
                break;
            case 'l' :
                if((opt_rec->method & NUM_METH) == NUM_METH )
                    error("We can't sort a numeric sort in lexiographic order.");
                else
                    opt_rec->method=LEX_METH;
                break;
            case 'n' :
                if (opt_rec->folding) 
                    error("We can't fold a numeric sort.");
                else if ((opt_rec->method & DICT_METH) == DICT_METH )
                    error("We can't sort a numeric sort in dictionary order.");
                else
                    opt_rec->method=NUM_METH;
                break;
            default:
                    error("Can't happen illegal argument in set_opt!");
        }
        ++optstr;
    }
}


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

/* error: print an error message and die! */
void error( char *s ) 
{
    fprintf(stderr,"\n\033[1msort\033[0m: error: %s\n\n",s);
    exit(2) ;
}    

/* error: print an error message  with some value and die! */
void error2( const char *s, char *t ) 
/* TODO: thinking the second pointer could be void *, and it would work
 * with any argument anyhow
 */
{
    char *u = NULL ;
    if ((u = (char *)malloc(256 )) == NULL )
        error("error2 couldn't malloc memory for err message!");
    else 
        sprintf(u,s,t);
    fprintf(stderr,"\n\033[1msort\033[0m: error: %s\n",u);
    exit(2) ;
}

/* show_help: print help and die */
static void show_help()
{
    printf("\n\033[1msort\033[0m: sort standard input or individual files to stdout\n"
            "\nSyntax\n"
            "\t\033[1msort\033[0m [-h | --help ] | [ [-r -n -d -f -l ] | [ -N[r,n,d,f,l] ... ] FILE\n"
            "Options\n"
            "\t-r : reverse\n"
            "\t-n : numeric\n"
            "\t-f : fold case\n"
            "\t-d : directory sort\n"
            "\t-l : lexiographic order (default)\n"
            "\t-N : field numbers between 1..255\n\n"
            "It is possible to specifiy global options first and then override on a per field basis\n"
            "\033[1msort\033[0m can only sort one file at a time, either from stdin or a file.\n\n"
          );
}

