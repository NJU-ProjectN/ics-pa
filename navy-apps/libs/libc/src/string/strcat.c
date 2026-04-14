/*
FUNCTION
	<<strcat>>---concatenate strings

INDEX
	strcat

ANSI_SYNOPSIS
	#include <string.h>
	char *strcat(char *<[dst]>, const char *<[src]>);

TRAD_SYNOPSIS
	#include <string.h>
	char *strcat(<[dst]>, <[src]>)
	char *<[dst]>;
	char *<[src]>;

DESCRIPTION
	<<strcat>> appends a copy of the string pointed to by <[src]>
	(including the terminating null character) to the end of the
	string pointed to by <[dst]>.  The initial character of
	<[src]> overwrites the null character at the end of <[dst]>.

RETURNS
	This function returns the initial value of <[dst]>

PORTABILITY
<<strcat>> is ANSI C.

<<strcat>> requires no supporting OS subroutines.

QUICKREF
	strcat ansi pure
*/

#include <string.h>

/*SUPPRESS 560*/
/*SUPPRESS 530*/

char *
_DEFUN (strcat, (s1, s2),
	char *s1 _AND
	_CONST char *s2)
{
  char *s = s1;

  while (*s1)
    s1++;

  while (*s1++ = *s2++)
    ;
  return s;
}
