/*
	assert.h
*/

#ifdef __cplusplus
extern "C" {
#endif

#include "_ansi.h"

#undef assert

#ifdef NDEBUG           /* required by ANSI standard */
#define assert(p)  	((void)0)
#else

#if defined(__STDC__) || defined (__cplusplus)
#define assert(p)   ((p) ? (void)0 : (void) __assertfail( \
        			(char *)"Assertion failed: %s, file %s, line %d\n", \
    	    		(char *)#p, (char *)__FILE__, __LINE__ ) )
#else /* -traditional */
#define assert(p)   ((p) ? (void)0 : (void) __assertfail( \
        			(char *)"Assertion failed: %s, file %s, line %d\n", \
    	    		(char *)"p", (char *)__FILE__, __LINE__ ) )
#endif /* -traditional */
int _EXFUN(__assertfail,(char *__msg, char *__cond, char *__file, int __line));

#endif /* NDEBUG */

#ifdef __cplusplus
}
#endif
