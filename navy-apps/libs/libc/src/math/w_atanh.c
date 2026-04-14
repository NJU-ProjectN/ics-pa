
/* @(#)w_atanh.c 5.1 93/09/24 */
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
	<<atanh>>, <<atanhf>>---inverse hyperbolic tangent 

INDEX
	atanh
INDEX
	atanhf

ANSI_SYNOPSIS
	#include <math.h>
	double atanh(double <[x]>);
	float atanhf(float <[x]>);

TRAD_SYNOPSIS
	#include <math.h>
	double atanh(<[x]>)
	double <[x]>;

	float atanhf(<[x]>)
	float <[x]>;

DESCRIPTION
	<<atanh>> calculates the inverse hyperbolic tangent of <[x]>.

	<<atanhf>> is identical, other than taking and returning
	<<float>> values.

RETURNS
	<<atanh>> and <<atanhf>> return the calculated value.

	If 
	@ifinfo
	|<[x]>|
	@end ifinfo
	@tex
	$|x|$
	@end tex
	is greater than 1, the global <<errno>> is set to <<EDOM>> and
	the result is a NaN.  A <<DOMAIN error>> is reported.

	If 
	@ifinfo
	|<[x]>|
	@end ifinfo
	@tex
	$|x|$
	@end tex
	is 1, the global <<errno>> is set to <<EDOM>>; and the result is 
	infinity with the same sign as <<x>>.  A <<SING error>> is reported.

	You can modify the error handling for these routines using
	<<matherr>>.

PORTABILITY
	Neither <<atanh>> nor <<atanhf>> are ANSI C.

QUICKREF
	atanh - pure
	atanhf - pure


*/

/* 
 * wrapper atanh(x)
 */

#include "fdlibm.h"

#ifdef _LIBM_REENT
#define atanh _atanh_r
#endif

#if defined (_LIBM_REENT) || ! defined (_REENT_ONLY)

#ifdef __STDC__
	double atanh(_R1 double x)		/* wrapper atanh */
#else
	double atanh(_R2 x)			/* wrapper atanh */
	_R3 double x;
#endif
{
#ifndef _DOUBLE_IS_32BITS
#ifdef _IEEE_LIBM
	return __ieee754_atanh(x);
#else
	double z,y;
	z = __ieee754_atanh(x);
	if(_LIB_VERSION == _IEEE_ || isnan(x)) return z;
	y = fabs(x);
	if(y>=1.0) {
	    if(y>1.0)
	        return __kernel_standard(_R4,x,x,30); /* atanh(|x|>1) */
	    else 
	        return __kernel_standard(_R4,x,x,31); /* atanh(|x|==1) */
	} else
	    return z;
#endif
#else /* defined (_DOUBLE_IS_32BITS) */
	return (double) _atanhf_r (_R4, (float) x);
#endif /* defined (_DOUBLE_IS_32BITS) */
}

#endif /* defined (_LIBM_REENT) || ! defined (_REENT_ONLY) */
