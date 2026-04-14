
/* @(#)w_sinh.c 5.1 93/09/24 */
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
        <<sinh>>, <<sinhf>>---hyperbolic sine

INDEX
	sinh
INDEX
	sinhf

ANSI_SYNOPSIS
        #include <math.h>
        double sinh(double <[x]>);
        float  sinhf(float <[x]>);

TRAD_SYNOPSIS
        #include <math.h>
        double sinh(<[x]>)
        double <[x]>;

        float  sinhf(<[x]>)
        float <[x]>;

DESCRIPTION
	<<sinh>> computes the hyperbolic sine of the argument <[x]>.
	Angles are specified in radians.   <<sinh>>(<[x]>) is defined as 
	@ifinfo
	. (exp(<[x]>) - exp(-<[x]>))/2
	@end ifinfo
	@tex
	$${e^x - e^{-x}}\over 2$$
	@end tex

	<<sinhf>> is identical, save that it takes and returns <<float>> values.

RETURNS
	The hyperbolic sine of <[x]> is returned.  

	When the correct result is too large to be representable (an
	overflow),  <<sinh>> returns <<HUGE_VAL>> with the
	appropriate sign, and sets the global value <<errno>> to
	<<ERANGE>>. 

	You can modify error handling for these functions with <<matherr>>.

PORTABILITY
	<<sinh>> is ANSI C.  
	<<sinhf>> is an extension.

QUICKREF
	sinh ansi pure
	sinhf - pure
*/

/* 
 * wrapper sinh(x)
 */

#include "fdlibm.h"

#ifdef _LIBM_REENT
#define sinh _sinh_r
#endif

#if defined (_LIBM_REENT) || ! defined (_REENT_ONLY)

#ifdef __STDC__
	double sinh(_R1 double x)		/* wrapper sinh */
#else
	double sinh(_R2 x)			/* wrapper sinh */
	_R3 double x;
#endif
{
#ifndef _DOUBLE_IS_32BITS
#ifdef _IEEE_LIBM
	return __ieee754_sinh(x);
#else
	double z; 
	z = __ieee754_sinh(x);
	if(_LIB_VERSION == _IEEE_) return z;
	if(!finite(z)&&finite(x)) {
	    return __kernel_standard(_R4,x,x,25); /* sinh overflow */
	} else
	    return z;
#endif
#else /* defined (_DOUBLE_IS_32BITS) */
	return (double) _sinhf_r (_R4, (float) x);
#endif /* defined (_DOUBLE_IS_32BITS) */
}

#endif /* defined (_LIBM_REENT) || ! defined (_REENT_ONLY) */
