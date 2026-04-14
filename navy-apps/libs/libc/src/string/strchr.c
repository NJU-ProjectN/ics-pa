/*
FUNCTION
	<<strchr>>---search for character in string

INDEX
	strchr

ANSI_SYNOPSIS
	#include <string.h>
	char * strchr(const char *<[string]>, int <[c]>);

TRAD_SYNOPSIS
	#include <string.h>
	char * strchr(<[string]>, <[c]>);
	char *<[string]>;
	int *<[c]>;

DESCRIPTION
	This function finds the first occurence of <[c]> (converted to
	a char) in the string pointed to by <[string]> (including the
	terminating null character).

RETURNS
	Returns a pointer to the located character, or a null pointer
	if <[c]> does not occur in <[string]>.

PORTABILITY
<<strchr>> is ANSI C.

<<strchr>> requires no supporting OS subroutines.

QUICKREF
	strchr ansi pure
*/

#include <string.h>

char *
_DEFUN (strchr, (s, i),
	_CONST char *s _AND
	int i)
{
  char c = i;

  while (*s && *s != c)
    {
      s++;
    }

  if (*s != c)
    {
      s = NULL;
    }

  return (char *) s;
}
