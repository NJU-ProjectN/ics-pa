
/* @(#)s_ldexp.c 5.1 93/09/24 */
/*
 * ====================================================
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunPro, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice 
 * is preserved.
 * ====================================================
 */

/*
FUNCTION
       <<ldexp>>, <<ldexpf>>---load exponent

INDEX
	ldexp
INDEX
	ldexpf

ANSI_SYNOPSIS
       #include <math.h>
       double ldexp(double <[val]>, int <[exp]>);
       float ldexpf(float <[val]>, int <[exp]>);

TRAD_SYNOPSIS
       #include <math.h>

       double ldexp(<[val]>, <[exp]>)
              double <[val]>;
              int <[exp]>;

       float ldexpf(<[val]>, <[exp]>)
              float <[val]>;
              int <[exp]>;


DESCRIPTION
<<ldexp>> calculates the value 
@ifinfo
<[val]> times 2 to the power <[exp]>.
@end ifinfo
@tex
$val\times 2^{exp}$.
@end tex
<<ldexpf>> is identical, save that it takes and returns <<float>>
rather than <<double>> values.

RETURNS
<<ldexp>> returns the calculated value.

Underflow and overflow both set <<errno>> to <<ERANGE>>.
On underflow, <<ldexp>> and <<ldexpf>> return 0.0.
On overflow, <<ldexp>> returns plus or minus <<HUGE_VAL>>.

PORTABILITY
<<ldexp>> is ANSI, <<ldexpf>> is an extension.
              
*/   

#include "fdlibm.h"
#include <errno.h>

#ifdef _LIBM_REENT
#define ldexp _ldexp_r
#endif

#if defined (_LIBM_REENT) || ! defined (_REENT_ONLY)

#ifdef __STDC__
	double ldexp(_R1 double value, int exp)
#else
	double ldexp(_R2 value, exp)
	_R3 double value; int exp;
#endif
{
#ifndef _DOUBLE_IS_32BITS
	if(!finite(value)||value==0.0) return value;
	value = scalbn(value,exp);
	if(!finite(value)||value==0.0) _R4->_errno = ERANGE;
	return value;
#else /* defined (_DOUBLE_IS_32BITS) */
	return (double) _ldexpf_r (_R4, (float) value, exp);
#endif /* defined (_DOUBLE_IS_32BITS) */
}

#endif /* defined (_LIBM_REENT) || ! defined (_REENT_ONLY) */
