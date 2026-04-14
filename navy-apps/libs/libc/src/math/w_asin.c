
/* @(#)w_asin.c 5.1 93/09/24 */
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
        <<asin>>, <<asinf>>---arc sine

INDEX
   asin
INDEX
   asinf

ANSI_SYNOPSIS
        #include <math.h>
        double asin(double <[x]>);
        float asinf(float <[x]>);

TRAD_SYNOPSIS
        #include <math.h>
        double asin(<[x]>)
        double <[x]>;

        float asinf(<[x]>)
        float <[x]>;


DESCRIPTION

<<asin>> computes the inverse sine (arc sine) of the argument <[x]>.
Arguments to <<asin>> must be in the range @minus{}1 to 1.

<<asinf>> is identical to <<asin>>, other than taking and
returning floats.

You can modify error handling for these routines using <<matherr>>. 

RETURNS
@ifinfo
<<asin>> returns values in radians, in the range of -pi/2 to pi/2.
@end ifinfo
@tex
<<asin>> returns values in radians, in the range of $-\pi/2$ to $\pi/2$.
@end tex

If <[x]> is not in the range @minus{}1 to 1, <<asin>> and <<asinf>>
return NaN (not a number), set the global variable <<errno>> to
<<EDOM>>, and issue a <<DOMAIN error>> message.

You can change this error treatment using <<matherr>>.

QUICKREF ANSI SVID POSIX RENTRANT
 asin	 y,y,y,m
 asinf   n,n,n,m

MATHREF  
 asin,  -1<=arg<=1, asin(arg),,,
 asin,  NAN,  arg,EDOM, DOMAIN

MATHREF  
 asinf,  -1<=arg<=1, asin(arg),,,
 asinf,  NAN,  arg,EDOM, DOMAIN 


*/

/* 
 * wrapper asin(x)
 */


#include "fdlibm.h"

#ifdef _LIBM_REENT
#define asin _asin_r
#endif

#if defined (_LIBM_REENT) || ! defined (_REENT_ONLY)

#ifdef __STDC__
	double asin(_R1 double x)		/* wrapper asin */
#else
	double asin(_R2 x)			/* wrapper asin */
	_R3 double x;
#endif
{
#ifndef _DOUBLE_IS_32BITS
#ifdef _IEEE_LIBM
	return __ieee754_asin(x);
#else
	double z;
	z = __ieee754_asin(x);
	if(_LIB_VERSION == _IEEE_ || isnan(x)) return z;
	if(fabs(x)>1.0) {
	        return __kernel_standard(_R4,x,x,2); /* asin(|x|>1) */
	} else
	    return z;
#endif
#else /* defined (_DOUBLE_IS_32BITS) */
	return (double) _asinf_r (_R4, (float) x);
#endif /* defined (_DOUBLE_IS_32BITS) */
}

#endif /* defined (_LIBM_REENT) || ! defined (_REENT_ONLY) */
