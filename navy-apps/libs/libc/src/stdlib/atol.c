/*
FUNCTION
   <<atol>>---convert string to long

ANSI_SYNOPSIS
      long atol(const char *s);

TRAD_SYNOPSIS
      long atol(s)
      char *s;


DESCRIPTION
   <<atol>> converts the initial portion of a string to an <<long>>.

   <<atol(s)>> is implemented as <<strtol(s,NULL,10)>>.

PORTABILITY
<<atol>> is ANSI.

No supporting OS subroutines are required.
*/

/*
 * Andy Wilson, 2-Oct-89.
 */

#include <stdlib.h>
#include <_ansi.h>

long
_DEFUN (atol, (s),
  _CONST char *s)
{
  return strtol (s, NULL, 10);
}
