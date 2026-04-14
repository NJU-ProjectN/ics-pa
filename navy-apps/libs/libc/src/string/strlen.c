/* 
FUNCTION
	<<strlen>>---character string length
	
INDEX
	strlen

ANSI_SYNOPSIS
	#include <string.h>
	size_t strlen(const char *<[str]>);

TRAD_SYNOPSIS
	#include <string.h>
	size_t strlen(<[str]>)
	char *<[src]>;

DESCRIPTION
	The <<strlen>> function works out the length of the string
	starting at <<*<[str]>>> by counting chararacters until it
	reaches a <<NULL>> character.

RETURNS
	<<strlen>> returns the character count.

PORTABILITY
<<strlen>> is ANSI C.

<<strlen>> requires no supporting OS subroutines.

QUICKREF
	strlen ansi pure
*/

#include <_ansi.h>
#include <string.h>

size_t
_DEFUN (strlen, (str),
	_CONST char *str)
{
  _CONST char *start = str;

  while (*str)
    str++;

  return str - start;
}
