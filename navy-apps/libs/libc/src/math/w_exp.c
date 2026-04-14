
/* @(#)w_exp.c 5.1 93/09/24 */
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
	<<exp>>, <<expf>>---exponential
INDEX
	exp
INDEX
	expf

ANSI_SYNOPSIS
	#include <math.h>
	double exp(double <[x]>);
	float expf(float <[x]>);

TRAD_SYNOPSIS
	#include <math.h>
	double exp(<[x]>);
	double <[x]>;

	float expf(<[x]>);
	float <[x]>;

DESCRIPTION
	<<exp>> and <<expf>> calculate the exponential of <[x]>, that is, 
	@ifinfo
	e raised to the power <[x]> (where e
	@end ifinfo
	@tex
	$e^x$ (where $e$
	@end tex
	is the base of the natural system of logarithms, approximately 2.71828).

	You can use the (non-ANSI) function <<matherr>> to specify
	error handling for these functions.

RETURNS
	On success, <<exp>> and <<expf>> return the calculated value.
	If the result underflows, the returned value is <<0>>.  If the
	result overflows, the returned value is <<HUGE_VAL>>.  In
	either case, <<errno>> is set to <<ERANGE>>.

PORTABILITY
	<<exp>> is ANSI C.  <<expf>> is an extension.

*/

/* 
 * wrapper exp(x)
 */

#include "fdlibm.h"

#ifdef _LIBM_REENT
#define exp _exp_r
#endif

#if defined (_LIBM_REENT) || ! defined (_REENT_ONLY)

#ifdef __STDC__
static const double
#else
static double
#endif
o_threshold=  7.09782712893383973096e+02,  /* 0x40862E42, 0xFEFA39EF */
u_threshold= -7.45133219101941108420e+02;  /* 0xc0874910, 0xD52D3051 */

#ifdef __STDC__
	double exp(_R1 double x)		/* wrapper exp */
#else
	double exp(_R2 x)			/* wrapper exp */
	_R3 double x;
#endif
{
#ifndef _DOUBLE_IS_32BITS
#ifdef _IEEE_LIBM
	return __ieee754_exp(x);
#else
	double z;
	z = __ieee754_exp(x);
	if(_LIB_VERSION == _IEEE_) return z;
	if(finite(x)) {
	    if(x>o_threshold)
	        return __kernel_standard(_R4,x,x,6); /* exp overflow */
	    else if(x<u_threshold)
	        return __kernel_standard(_R4,x,x,7); /* exp underflow */
	} 
	return z;
#endif
#else /* defined (_DOUBLE_IS_32BITS) */
	return (double) _expf_r (_R4, (float) x);
#endif /* defined (_DOUBLE_IS_32BITS) */
}

#endif /* defined (_LIBM_REENT) || ! defined (_REENT_ONLY) */
