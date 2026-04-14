/*
FUNCTION
<<div>>---divide two integers

INDEX
	div

ANSI_SYNOPSIS
	#include <stdlib.h>
	div_t div(int <[n]>, int <[d]>);

TRAD_SYNOPSIS
	#include <stdlib.h>
	div_t div(<[n]>, <[d]>)
	int <[n]>, <[d]>;

DESCRIPTION
Divide
@tex
$n/d$,
@end tex
@ifinfo
<[n]>/<[d]>,
@end ifinfo
returning quotient and remainder as two integers in a structure <<div_t>>.

RETURNS
The result is represented with the structure

. typedef struct
. {
.  int quot;
.  int rem;
. } div_t;

	where the <<quot>> field represents the quotient, and <<rem>> the
	remainder.  For nonzero <[d]>, if `<<<[r]> = div(<[n]>,<[d]>);>>' then
	<[n]> equals `<<<[r]>.rem + <[d]>*<[r]>.quot>>'.

	When <[d]> is zero, the <<quot>> member of the result has the same
	sign as <[n]> and the largest representable magnitude.

	To divide <<long>> rather than <<int>> values, use the similar
	function <<ldiv>>.

PORTABILITY
	<<div>> is ANSI, but the behavior for zero <[d]> is not specified by
	the standard.

No supporting OS subroutines are required.
*/

#include <limits.h>
#include <stdlib.h>

div_t
_DEFUN (div, (n, d),
	int n _AND
	int d)
{
  div_t res;

  if (d)
    {
      res.quot = abs (n) / abs (d);
      res.rem = abs (n) % abs (d);

      if ((n < 0 && d > 0) || (n >= 0 && d < 0))
	res.quot = -res.quot;
      if (n < 0)
	res.rem = -res.rem;
    }
  else
    {
      if (n < 0)
	res.quot = INT_MIN;
      else
	res.quot = INT_MAX;

      res.rem = 0;
    }

  return res;
}
