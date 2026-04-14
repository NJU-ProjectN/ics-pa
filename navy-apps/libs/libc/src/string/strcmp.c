/*
FUNCTION
	<<strcmp>>---character string compare
	
INDEX
	strcmp

ANSI_SYNOPSIS
	#include <string.h>
	int strcmp(const char *<[a]>, const char *<[b]>);

TRAD_SYNOPSIS
	#include <string.h>
	int strcmp(<[a]>, <[b]>)
	char *<[a]>;
	char *<[b]>;

DESCRIPTION
	<<strcmp>> compares the string at <[a]> to
	the string at <[b]>.

RETURNS
	If <<*<[a]>>> sorts lexicographically after <<*<[b]>>>,
	<<strcmp>> returns a number greater than zero.  If the two
	strings match, <<strcmp>> returns zero.  If <<*<[a]>>>
	sorts lexicographically before <<*<[b]>>>, <<strcmp>> returns a
	number less than zero.

PORTABILITY
<<strcmp>> is ANSI C.

<<strcmp>> requires no supporting OS subroutines.

QUICKREF
	strcmp ansi pure
*/

#include <string.h>

int
_DEFUN (strcmp, (s1, s2),
	_CONST char *s1 _AND
	_CONST char *s2)
{
  while (*s1 != '\0' && *s1 == *s2)
    {
      s1++;
      s2++;
    }

  return (*(unsigned char *) s1) - (*(unsigned char *) s2);
}
