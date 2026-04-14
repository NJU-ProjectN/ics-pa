/* wrf_lgamma.c -- float version of wr_lgamma.c.
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
 */

/* 
 * wrapper float lgammaf_r(float x, int *signgamp)
 */

#include "fdlibm.h"

#ifdef _LIBM_REENT
#define lgammaf_r _lgammaf_r_r
#endif

#if defined (_LIBM_REENT) || ! defined (_REENT_ONLY)

#ifdef __STDC__
	float lgammaf_r(_R1 float x, int *signgamp) /* wrapper lgammaf_r */
#else
	float lgammaf_r(_R2 x,signgamp)              /* wrapper lgammaf_r */
	_R3 float x; int *signgamp;
#endif
{
#ifdef _IEEE_LIBM
	return __ieee754_lgammaf_r(x,signgamp);
#else
        float y;
        y = __ieee754_lgammaf_r(x,signgamp);
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
