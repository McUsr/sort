/* io.h io for full lines */

#define MAXLEN 4096
#define IFS '\t'
#define OFS '\t'

int readlinesN(char *lineptr[], char * *fparr[], int maxlines, int lastfield) ;
void writelinesN(char *lineptr[], char * *fparr[], int nlines, int lastfield ) ;

int numcmpN(const char *s1, const char *s2);
int r_numcmpN(const char *s1, const char *s2);
int r_strcmpN(const char *s1, const char *s2 );
int cmpfoldN( const char *s1, const char *s2 );
int r_cmpfoldN( const char *s1, const char *s2 );
int cmpdirN(const char *s1, const char *s2);
int r_cmpdirN(const char *s1, const char *s2);
int cmpdir_foldN(const char *s1, const char *s2);
int r_cmpdir_foldN(const char *s1, const char *s2);

/* qsort: a reverse engineered one! */
void qsortN(void *v[], int left,
        int right, int (*comp)(void *, void *));

int (*get_cmpN(OptionsPtr global_opts))(void*, void*); 
