/* io.h io for full lines */

#define MAXLEN 8192 
#define MAXLINES 5000
#define IFS '\t'
#define OFS '\t'
int readlines0(char *lineptr[], int maxlines);
void writelines0(char *lineptr[], int nlines);

int numcmp(const char *s1, const char *s2);
int r_numcmp(const char *s1, const char *s2);
int r_strcmp(const char *s1, const char *s2 );
int cmpfold( const char *s1, const char *s2 );
int r_cmpfold( const char *s1, const char *s2 );
int cmpdir(const char *s1, const char *s2);
int r_cmpdir(const char *s1, const char *s2);
int cmpdir_fold(const char *s1, const char *s2);
int r_cmpdir_fold(const char *s1, const char *s2);

/* qsort: a reverse engineered one! */
void qsort0(void *v[], int left,
        int right, int (*comp)(void *, void *));

int (*get_cmp(OptionsPtr global_opts))(void*, void*); 
char *alloc(int n); 
void afree( char *p );
