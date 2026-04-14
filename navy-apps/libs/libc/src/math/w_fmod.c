
/* @(#)w_fmod.c 5.1 93/09/24 */
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
<<fmod>>, <<fmodf>>---floating-point remainder (modulo)

INDEX
fmod
INDEX
fmodf

ANSI_SYNOPSIS
#include <math.h>
double fmod(double <[x]>, double <[y]>)
float fmodf(float <[x]>, float <[y]>)

TRAD_SYNOPSIS
#include <math.h>
double fmod(<[x]>, <[y]>)
double (<[x]>, <[y]>);

float fmodf(<[x]>, <[y]>)
float (<[x]>, <[y]>);

DESCRIPTION
The <<fmod>> and <<fmodf>> functions compute the floating-point
remainder of <[x]>/<[y]> (<[x]> modulo <[y]>).

RETURNS
The <<fmod>> function returns the value 
@ifinfo
<[x]>-<[i]>*<[y]>, 
@end ifinfo
@tex
$x-i\times y$,
@end tex
for the largest integer <[i]> such that, if <[y]> is nonzero, the
result has the same sign as <[x]> and magnitude less than the
magnitude of <[y]>. 

<<fmod(<[x]>,0)>> returns NaN, and sets <<errno>> to <<EDOM>>.

You can modify error treatment for these functions using <<matherr>>.

PORTABILITY
<<fmod>> is ANSI C. <<fmodf>> is an extension.
*/

/* 
 * wrapper fmod(x,y)
 */

#include "fdlibm.h"

#ifdef _LIBM_REENT
#define fmod _fmod_r
#endif

#if defined (_LIBM_REENT) || ! defined (_REENT_ONLY)

#ifdef __STDC__
	double fmod(_R1 double x, double y)	/* wrapper fmod */
#else
	double fmod(_R2 x,y)		/* wrapper fmod */
	_R3 double x,y;
#endif
{
#ifndef _DOUBLE_IS_32BITS
#ifdef _IEEE_LIBM
	return __ieee754_fmod(x,y);
#else
	double z;
	z = __ieee754_fmod(x,y);
	if(_LIB_VERSION == _IEEE_ ||isnan(y)||isnan(x)) return z;
	if(y==0.0) {
	        return __kernel_standard(_R4,x,y,27); /* fmod(x,0) */
	} else
	    return z;
#endif
#else /* defined (_DOUBLE_IS_32BITS) */
	return (double) _fmodf_r (_R4, (float) x, (float) y);
#endif /* defined (_DOUBLE_IS_32BITS) */
}

#endif /* defined (_LIBM_REENT) || ! defined (_REENT_ONLY) */
