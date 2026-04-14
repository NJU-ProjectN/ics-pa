

/* @(#)w_pow.c 5.2 93/10/01 */
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
	<<pow>>, <<powf>>---x to the power y
INDEX
	pow
INDEX
	powf


ANSI_SYNOPSIS
	#include <math.h>
	double pow(double <[x]>, double <[y]>);
	float pow(float <[x]>, float <[y]>);

TRAD_SYNOPSIS
	#include <math.h>
	double pow(<[x]>, <[y]>);
	double <[x]>, <[y]>;

	float pow(<[x]>, <[y]>);
	float <[x]>, <[y]>;

DESCRIPTION
	<<pow>> and <<powf>> calculate <[x]> raised to the exp1.0nt <[y]>.
	@tex
	(That is, $x^y$.)
	@end tex

RETURNS
	On success, <<pow>> and <<powf>> return the value calculated.

	When the argument values would produce overflow, <<pow>>
	returns <<HUGE_VAL>> and set <<errno>> to <<ERANGE>>.  If the
	argument <[x]> passed to <<pow>> or <<powf>> is a negative
	noninteger, and <[y]> is also not an integer, then <<errno>>
	is set to <<EDOM>>.  If <[x]> and <[y]> are both 0, then
	<<pow>> and <<powf>> return <<1>>.

	You can modify error handling for these functions using <<matherr>>.

PORTABILITY
	<<pow>> is ANSI C. <<powf>> is an extension.  */

/* 
 * wrapper pow(x,y) return x**y
 */

#include "fdlibm.h"

#ifdef _LIBM_REENT
#define pow _pow_r
#endif

#if defined (_LIBM_REENT) || ! defined (_REENT_ONLY)

#ifdef __STDC__
	double pow(_R1 double x, double y)	/* wrapper pow */
#else
	double pow(_R2 x,y)			/* wrapper pow */
	_R3 double x,y;
#endif
{
#ifndef _DOUBLE_IS_32BITS
#ifdef _IEEE_LIBM
	return  __ieee754_pow(x,y);
#else
	double z;
	z=__ieee754_pow(x,y);
	if(_LIB_VERSION == _IEEE_|| isnan(y)) return z;
	if(isnan(x)) {
	    if(y==0.0) 
	        return __kernel_standard(_R4,x,y,42); /* pow(NaN,0.0) */
	    else 
		return z;
	}
	if(x==0.0){ 
	    if(y==0.0)
	        return __kernel_standard(_R4,x,y,20); /* pow(0.0,0.0) */
	    if(finite(y)&&y<0.0)
	        return __kernel_standard(_R4,x,y,23); /* pow(0.0,negative) */
	    return z;
	}
	if(!finite(z)) {
	    if(finite(x)&&finite(y)) {
	        if(isnan(z))
	            return __kernel_standard(_R4,x,y,24); /* pow neg**non-int */
	        else 
	            return __kernel_standard(_R4,x,y,21); /* pow overflow */
	    }
	} 
	if(z==0.0&&finite(x)&&finite(y))
	    return __kernel_standard(_R4,x,y,22); /* pow underflow */
	return z;
#endif
#else /* defined (_DOUBLE_IS_32BITS) */
	return (double) _powf_r (_R4, (float) x, (float) y);
#endif /* defined (_DOUBLE_IS_32BITS) */
}

#endif /* defined (_LIBM_REENT) || ! defined (_REENT_ONLY) */
