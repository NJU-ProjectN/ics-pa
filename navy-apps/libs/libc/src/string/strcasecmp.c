/*
FUNCTION
	<<strcasecmp>>---case insensitive character string compare
	
INDEX
	strcasecmp

ANSI_SYNOPSIS
	#include <string.h>
	int strcasecmp(const char *<[a]>, const char *<[b]>);

TRAD_SYNOPSIS
	#include <string.h>
	int strcasecmp(<[a]>, <[b]>)
	char *<[a]>;
	char *<[b]>;

DESCRIPTION
	<<strcasecmp>> compares the string at <[a]> to
	the string at <[b]> in a case-insensitive manner.

RETURNS 

	If <<*<[a]>>> sorts lexicographically after <<*<[b]>>> (after
	both are converted to upper case), <<strcasecmp>> returns a
	number greater than zero.  If the two strings match,
	<<strcasecmp>> returns zero.  If <<*<[a]>>> sorts
	lexicographically before <<*<[b]>>>, <<strcasecmp>> returns a
	number less than zero.

PORTABILITY
<<strcasecmp>> is in the Berkeley Software Distribution.

<<strcasecmp>> requires no supporting OS subroutines. It uses
toupper() from elsewhere in this library.

QUICKREF
	strcasecmp 
*/

#include <string.h>
#include <ctype.h>

int
_DEFUN (strcasecmp, (s1, s2),
	_CONST char *s1 _AND
	_CONST char *s2)
{
  while (*s1 != '\0' && toupper(*s1) == toupper(*s2))
    {
      s1++;
      s2++;
    }

  return toupper(*(unsigned char *) s1) - toupper(*(unsigned char *) s2);
}
