/*
 * nan () returns a nan.
 * Added by Cygnus Support.
 */

/*
FUNCTION
	<<nan>>, <<nanf>>---representation of infinity

INDEX
	nan
INDEX
	nanf

ANSI_SYNOPSIS
	#include <math.h>
	double nan(void);
	float nanf(void);

TRAD_SYNOPSIS
	#include <math.h>
	double nan();
	float nanf();


DESCRIPTION
	<<nan>> and <<nanf>> return an IEEE NaN (Not a Number) in
	double and single precision arithmetic respectivly.

QUICKREF
	nan - pure

*/

#include "fdlibm.h"

	double nan()
{
#ifndef _DOUBLE_IS_32BITS
	double x;

	INSERT_WORDS(x,0x7ff80000,0);
	return x;
#else /* defined (_DOUBLE_IS_32BITS) */
	return nanf ();
#endif /* defined (_DOUBLE_IS_32BITS) */
}
