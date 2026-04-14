/*
FUNCTION
	<<strncmp>>---character string compare
	
INDEX
	strncmp

ANSI_SYNOPSIS
	#include <string.h>
	int strncmp(const char *<[a]>, const char * <[b]>, size_t <[length]>);

TRAD_SYNOPSIS
	#include <string.h>
	int strncmp(<[a]>, <[b]>, <[length]>)
	char *<[a]>;
	char *<[b]>;
	size_t <[length]>

DESCRIPTION
	<<strncmp>> compares up to <[length]> characters
	from the string at <[a]> to the string at <[b]>.

RETURNS
	If <<*<[a]>>> sorts lexicographically after <<*<[b]>>>,
	<<strncmp>> returns a number greater than zero.  If the two
	strings are equivalent, <<strncmp>> returns zero.  If <<*<[a]>>>
	sorts lexicographically before <<*<[b]>>>, <<strncmp>> returns a
	number less than zero.

PORTABILITY
<<strncmp>> is ANSI C.

<<strncmp>> requires no supporting OS subroutines.

QUICKREF
	strncmp ansi pure
*/

#include <string.h>

int 
_DEFUN (strncmp, (s1, s2, n),
	_CONST char *s1 _AND
	_CONST char *s2 _AND
	size_t n)
{
  if (n == 0)
    return 0;

  while (n-- != 0 && *s1 == *s2)
    {
      if (n == 0 || *s1 == '\0' || *s2 == '\0')
	break;
      s1++;
      s2++;
    }

  return (*(unsigned char *) s1) - (*(unsigned char *) s2);
}
