/*
 * ctime.c
 * Original Author:	G. Haley
 */

/*
FUNCTION
<<ctime>>---convert time to local and format as string

INDEX
	ctime

ANSI_SYNOPSIS
	#include <time.h>
	char *ctime(time_t <[timp]>);

TRAD_SYNOPSIS
	#include <time.h>
	char *ctime(<[timp]>)
	time_t <[timp]>;

DESCRIPTION
Convert the time value at <[timp]> to local time (like <<localtime>>)
and format it into a string of the form
. Wed Jun 15 11:38:07 1988\n\0
(like <<asctime>>).

RETURNS
A pointer to the string containing a formatted timestamp.

PORTABILITY
ANSI C requires <<ctime>>.

<<ctime>> requires no supporting OS subroutines.
*/

#include <time.h>

char *
_DEFUN (ctime, (tim_p),
	_CONST time_t * tim_p)
{
  return asctime (localtime (tim_p));
}
