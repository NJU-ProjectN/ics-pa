/*
FUNCTION
	<<strcpy>>---copy string

INDEX
	strcpy

ANSI_SYNOPSIS
	#include <string.h>
	char *strcpy(char *<[dst]>, const char *<[src]>);

TRAD_SYNOPSIS
	#include <string.h>
	char *strcpy(<[dst]>, <[src]>)
	char *<[dst]>;
	char *<[src]>;

DESCRIPTION
	<<strcpy>> copies the string pointed to by <[src]>
	(including the terminating null character) to the array
	pointed to by <[dst]>.

RETURNS
	This function returns the initial value of <[dst]>.

PORTABILITY
<<strcpy>> is ANSI C.

<<strcpy>> requires no supporting OS subroutines.

QUICKREF
	strcpy ansi pure
*/

#include <string.h>

/*SUPPRESS 560*/
/*SUPPRESS 530*/

char *
_DEFUN (strcpy, (s1, s2),
	char *s1 _AND
	_CONST char *s2)
{
  char *s = s1;

  while (*s1++ = *s2++)
    ;

  return s;
}
