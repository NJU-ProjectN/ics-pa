
/* @(#)w_log.c 5.1 93/09/24 */
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
       <<log>>, <<logf>>---natural logarithms

INDEX
    log
INDEX
    logf

ANSI_SYNOPSIS
       #include <math.h>
       double log(double <[x]>);
       float logf(float <[x]>);

TRAD_SYNOPSIS
       #include <math.h>
       double log(<[x]>);
       double <[x]>;

       float logf(<[x]>);
       float <[x]>;

DESCRIPTION
Return the natural logarithm of <[x]>, that is, its logarithm base e
(where e is the base of the natural system of logarithms, 2.71828@dots{}).
<<log>> and <<logf>> are identical save for the return and argument types.

You can use the (non-ANSI) function <<matherr>> to specify error
handling for these functions. 

RETURNS
Normally, returns the calculated value.  When <[x]> is zero, the
returned value is <<-HUGE_VAL>> and <<errno>> is set to <<ERANGE>>.
When <[x]> is negative, the returned value is <<-HUGE_VAL>> and
<<errno>> is set to <<EDOM>>.  You can control the error behavior via
<<matherr>>.

PORTABILITY
<<log>> is ANSI, <<logf>> is an extension.
*/

/*
 * wrapper log(x)
 */

#include "fdlibm.h"

#ifdef _LIBM_REENT
#define log _log_r
#endif

#if defined (_LIBM_REENT) || ! defined (_REENT_ONLY)

#ifdef __STDC__
	double log(_R1 double x)		/* wrapper log */
#else
	double log(_R2 x)			/* wrapper log */
	_R3 double x;
#endif
{
#ifndef _DOUBLE_IS_32BITS
#ifdef _IEEE_LIBM
	return __ieee754_log(x);
#else
	double z;
	z = __ieee754_log(x);
	if(_LIB_VERSION == _IEEE_ || isnan(x) || x > 0.0) return z;
	if(x==0.0)
	    return __kernel_standard(_R4,x,x,16); /* log(0) */
	else 
	    return __kernel_standard(_R4,x,x,17); /* log(x<0) */
#endif
#else /* defined (_DOUBLE_IS_32BITS) */
	return (double) _logf_r (_R4, (float) x);
#endif /* defined (_DOUBLE_IS_32BITS) */
}

#endif /* defined (_LIBM_REENT) || ! defined (_REENT_ONLY) */
