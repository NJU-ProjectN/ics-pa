
/* @(#)w_atan2.c 5.1 93/09/24 */
/*
 * ====================================================
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunPro, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice 
 * is preserved.
 * ====================================================
 *
 */

/*
FUNCTION
        <<atan2>>, <<atan2f>>---arc tangent of y/x

INDEX
   atan2
INDEX
   atan2f

ANSI_SYNOPSIS
        #include <math.h>
        double atan2(double <[y]>,double <[x]>);
        float atan2f(float <[y]>,float <[x]>);

TRAD_SYNOPSIS
        #include <math.h>
        double atan2(<[y]>,<[x]>);
        double <[y]>;
        double <[x]>;

        float atan2f(<[y]>,<[x]>);
        float <[y]>;
        float <[x]>;

DESCRIPTION

<<atan2>> computes the inverse tangent (arc tangent) of <[y]>/<[x]>. 
<<atan2>> produces the correct result even for angles near 
@ifinfo
pi/2 or -pi/2 
@end ifinfo
@tex
$\pi/2$ or $-\pi/2$
@end tex
(that is, when <[x]> is near 0). 

<<atan2f>> is identical to <<atan2>>, save that it takes and returns
<<float>>. 

RETURNS
<<atan2>> and <<atan2f>> return a value in radians, in the range of 
@ifinfo
-pi to pi.
@end ifinfo
@tex
$-\pi$ to $\pi$.
@end tex

If both <[x]> and <[y]> are 0.0, <<atan2>> causes a <<DOMAIN>> error.

You can modify error handling for these functions using <<matherr>>.

PORTABILITY
<<atan2>> is ANSI C.  <<atan2f>> is an extension.


*/

/* 
 * wrapper atan2(y,x)
 */

#include "fdlibm.h"

#ifdef _LIBM_REENT
#define atan2 _atan2_r
#endif

#if defined (_LIBM_REENT) || ! defined (_REENT_ONLY)

#ifdef __STDC__
	double atan2(_R1 double y, double x)	/* wrapper atan2 */
#else
	double atan2(_R2 y,x)			/* wrapper atan2 */
	_R3 double y,x;
#endif
{
#ifndef _DOUBLE_IS_32BITS
#ifdef _IEEE_LIBM
	return __ieee754_atan2(y,x);
#else
	double z;
	z = __ieee754_atan2(y,x);
	if(_LIB_VERSION == _IEEE_||isnan(x)||isnan(y)) return z;
	if(x==0.0&&y==0.0) {
	        return __kernel_standard(_R4,y,x,3); /* atan2(+-0,+-0) */
	} else
	    return z;
#endif
#else /* defined (_DOUBLE_IS_32BITS) */
	return (double) _atan2f_r (_R4, (float) x, (float) y);
#endif /* defined (_DOUBLE_IS_32BITS) */
}

#endif /* defined (_LIBM_REENT) || ! defined (_REENT_ONLY) */
