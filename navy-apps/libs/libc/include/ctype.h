#ifndef _CTYPE_H_
#ifdef __cplusplus
extern "C" {
#endif
#define _CTYPE_H_

#include "_ansi.h"

int _EXFUN(isalnum, (int c));
int _EXFUN(isalpha, (int c));
int _EXFUN(iscntrl, (int c));
int _EXFUN(isdigit, (int c));
int _EXFUN(isgraph, (int c));
int _EXFUN(islower, (int c));
int _EXFUN(isprint, (int c));
int _EXFUN(ispunct, (int c));
int _EXFUN(isspace, (int c));
int _EXFUN(isupper, (int c));
int _EXFUN(isxdigit,(int c));
int _EXFUN(tolower, (int c));
int _EXFUN(toupper, (int c));
int _EXFUN(_tolower, (int c));
int _EXFUN(_toupper, (int c));
#ifndef _STRICT_ANSI
int _EXFUN(isascii, (int c));
int _EXFUN(toascii, (int c));
#endif

#define	_U	01
#define	_L	02
#define	_N	04
#define	_S	010
#define _P	020
#define _C	040
#define _X	0100
#define	_B	0200

extern	_CONST char	_ctype_[];

#define	isalpha(c)	((_ctype_+1)[c]&(_U|_L))
#define	isupper(c)	((_ctype_+1)[c]&_U)
#define	islower(c)	((_ctype_+1)[c]&_L)
#define	isdigit(c)	((_ctype_+1)[c]&_N)
#define	isxdigit(c)	((_ctype_+1)[c]&(_X|_N))
#define	isspace(c)	((_ctype_+1)[c]&_S)
#define ispunct(c)	((_ctype_+1)[c]&_P)
#define isalnum(c)	((_ctype_+1)[c]&(_U|_L|_N))
#define isprint(c)	((_ctype_+1)[c]&(_P|_U|_L|_N|_B))
#define	isgraph(c)	((_ctype_+1)[c]&(_P|_U|_L|_N))
#define iscntrl(c)	((_ctype_+1)[c]&_C)
#define toupper(c)	(islower(c) ? ((c)-'a'+'A') : (c))
#define tolower(c)	(isupper(c) ? ((c)-'A'+'a') : (c))
#ifndef _STRICT_ANSI
#define isascii(c)	((unsigned)(c)<=0177)
#define toascii(c)	((c)&0177)
#endif

#ifdef __cplusplus
}
#endif
#endif /* _CTYPE_H_ */
