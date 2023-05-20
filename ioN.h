/* ioN.h io for selected fields */


void creat_fparr(char * *fparr[], int rows, int lastfield) ;
int readlinesN(char *lineptr[], char * *fparr[], int maxlines, int lastfield) ;
void writelinesN(char *lineptr[], char * *fparr[], int nlines, int lastfield ) ;
char * retf(  int fieldno, int lastfield, char* *fparr[], int row, int maxline );

void qsortN2(void *v[], char* *fp[], int left, int right,
			int fn, int (*comp)(void *, void *));

int (*get_cmpN(OptionsPtr global_opts))(void*, void*); 
