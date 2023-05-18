/* io.h io for full lines */

#define MAXLEN 10000
int readlines0(char *lineptr[], int maxlines);
void writelines0(char *lineptr[], int nlines);

int numcmp0(const char *s1, const char *s2);
int r_numcmp0(const char *s1, const char *s2);
int r_strcmp0(const char *s1, const char *s2 );
int cmpfold0( const char *s1, const char *s2 );
int r_cmpfold0( const char *s1, const char *s2 );
int cmpdir0(const char *s1, const char *s2);
int r_cmpdir0(const char *s1, const char *s2);
int cmpdir_fold0(const char *s1, const char *s2);
int r_cmpdir_fold0(const char *s1, const char *s2);

/* qsort: a reverse engineered one! */
void qsort0(void *v[], int left,
        int right, int (*comp)(void *, void *));

int (*get_cmp0(OptionsPtr global_opts))(void*, void*); 
