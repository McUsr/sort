/* The datastructure that we use to communicate the
 * sorting of a file by fields with.
 */

#define LEX_METH    0X00
#define NUM_METH    0X01
#define DICT_METH   0X02


#define MAXF 256

extern int parse_fields;
extern int fv[MAXF] ;

/*anyways, however how I do it, I need to replace any newlines when field nr 0 is set
 * anyway, so that the cmp routines can work in a uniform fashion.
 */

struct fieldCommand {
    unsigned method : 2 ;
    unsigned folding : 1 ;
    unsigned reverse : 1 ;
    unsigned : 0 ;
    unsigned fieldno : 8 ;
};

typedef struct fieldCommand Options ;

typedef Options *OptionsPtr;



OptionsPtr initFieldRec(void) ;
/* not the first parsing of arguments, first we check for help
 * then we show help and we die without further ado in main.
 */

char **parse_opts(int *argc,char **argv, int* max_field, OptionsPtr global_opts ) ;

void printOptions( OptionsPtr aRec ) ;

void error( char *s ) ;
void error2( const char *s, char *t ) ;

/* The field options are sent behind the scenes inside the dequeue */
