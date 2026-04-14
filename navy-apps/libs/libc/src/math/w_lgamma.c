
/* @(#)w_lgamma.c 5.1 93/09/24 */
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

/* double lgamma(double x)
 * Return the logarithm of the Gamma function of x.
 *
 * Method: call __ieee754_lgamma_r
 */

#include "fdlibm.h"

#ifdef _LIBM_REENT
#define lgamma _lgamma_r
#endif

#if defined (_LIBM_REENT) || ! defined (_REENT_ONLY)

#ifdef __STDC__
	double lgamma(_R1 double x)
#else
	double lgamma(_R2 x)
	_R3 double x;
#endif
{
#ifndef _DOUBLE_IS_32BITS
#ifdef _IEEE_LIBM
	return __ieee754_lgamma_r(x,&_R4->_signgam);
#else
        double y;
        y = __ieee754_lgamma_r(x,&_R4->_signgam);
        if(_LIB_VERSION == _IEEE_) return y;
        if(!finite(y)&&finite(x)) {
            if(floor(x)==x&&x<=0.0)
                return __kernel_standard(_R4,x,x,15); /* lgamma pole */
            else
                return __kernel_standard(_R4,x,x,14); /* lgamma overflow */
        } else
            return y;
#endif
#else /* defined (_DOUBLE_IS_32BITS) */
	return (double) _lgammaf_r (_R4, (float) x);
#endif /* defined (_DOUBLE_IS_32BITS) */
}             

#endif /* defined (_LIBM_REENT) || ! defined (_REENT_ONLY) */
