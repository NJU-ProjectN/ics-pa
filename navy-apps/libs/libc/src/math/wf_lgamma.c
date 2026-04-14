/* wf_lgamma.c -- float version of w_lgamma.c.
 * Conversion to float by Ian Lance Taylor, Cygnus Support, ian@cygnus.com.
 */

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

#include "fdlibm.h"

#ifdef _LIBM_REENT
#define lgammaf _lgammaf_r
#endif

#if defined (_LIBM_REENT) || ! defined (_REENT_ONLY)

#ifdef __STDC__
	float lgammaf(_R1 float x)
#else
	float lgammaf(_R2 x)
	_R3 float x;
#endif
{
#ifdef _IEEE_LIBM
	return __ieee754_lgammaf_r(x,&_R4->_signgam);
#else
        float y;
        y = __ieee754_lgammaf_r(x,&_R4->_signgam);
        if(_LIB_VERSION == _IEEE_) return y;
        if(!finitef(y)&&finitef(x)) {
            if(floorf(x)==x&&x<=(float)0.0)
	        /* lgamma pole */
                return (float)__kernel_standard(_R4,(double)x,(double)x,115);
            else
	        /* lgamma overflow */
                return (float)__kernel_standard(_R4,(double)x,(double)x,114);
        } else
            return y;
#endif
}             

#endif /* defined (_LIBM_REENT) || ! defined (_REENT_ONLY) */
