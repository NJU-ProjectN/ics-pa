
/* @(#)w_log10.c 5.1 93/09/24 */
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
	<<log10>>, <<log10f>>---base 10 logarithms

INDEX
log10
INDEX
log10f

ANSI_SYNOPSIS
	#include <math.h>
	double log10(double <[x]>);
	float log10f(float <[x]>);

TRAD_SYNOPSIS
	#include <math.h>
	double log10(<[x]>)
	double <[x]>;

	float log10f(<[x]>)
	float <[x]>;

DESCRIPTION
<<log10>> returns the base 10 logarithm of <[x]>.
It is implemented as <<log(<[x]>) / log(10)>>.

<<log10f>> is identical, save that it takes and returns <<float>> values.

RETURNS
<<log10>> and <<log10f>> return the calculated value. 

See the description of <<log>> for information on errors.

PORTABILITY
<<log10>> is ANSI C.  <<log10f>> is an extension.

 */

/* 
 * wrapper log10(X)
 */

#include "fdlibm.h"

#ifdef _LIBM_REENT
#define log10 _log10_r
#endif

#if defined (_LIBM_REENT) || ! defined (_REENT_ONLY)

#ifdef __STDC__
	double log10(_R1 double x)		/* wrapper log10 */
#else
	double log10(_R2 x)			/* wrapper log10 */
	_R3 double x;
#endif
{
#ifndef _DOUBLE_IS_32BITS
#ifdef _IEEE_LIBM
	return __ieee754_log10(x);
#else
	double z;
	z = __ieee754_log10(x);
	if(_LIB_VERSION == _IEEE_ || isnan(x)) return z;
	if(x<=0.0) {
	    if(x==0.0)
	        return __kernel_standard(_R4,x,x,18); /* log10(0) */
	    else 
	        return __kernel_standard(_R4,x,x,19); /* log10(x<0) */
	} else
	    return z;
#endif
#else /* defined (_DOUBLE_IS_32BITS) */
	return (double) _log10f_r (_R4, (float) x);
#endif /* defined (_DOUBLE_IS_32BITS) */
}

#endif /* defined (_LIBM_REENT) || ! defined (_REENT_ONLY) */
