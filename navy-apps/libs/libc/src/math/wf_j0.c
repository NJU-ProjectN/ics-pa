/* wf_j0.c -- float version of w_j0.c.
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
 * wrapper j0f(float x), y0f(float x)
 */

#include "fdlibm.h"

#ifdef _LIBM_REENT
#define j0f _j0f_r
#define y0f _y0f_r
#endif

#if defined (_LIBM_REENT) || ! defined (_REENT_ONLY)
#ifdef __STDC__
	float j0f(_R1 float x)		/* wrapper j0f */
#else
	float j0f(_R2 x)			/* wrapper j0f */
	_R3 float x;
#endif
{
#ifdef _IEEE_LIBM
	return __ieee754_j0f(x);
#else
	float z = __ieee754_j0f(x);
	if(_LIB_VERSION == _IEEE_ || isnanf(x)) return z;
	if(fabsf(x)>(float)X_TLOSS) {
		/* j0f(|x|>X_TLOSS) */
	        return (float)__kernel_standard(_R4,(double)x,(double)x,134);
	} else
	    return z;
#endif
}

#ifdef __STDC__
	float y0f(_R1 float x)		/* wrapper y0f */
#else
	float y0f(_R2 x)			/* wrapper y0f */
	_R3 float x;
#endif
{
#ifdef _IEEE_LIBM
	return __ieee754_y0f(x);
#else
	float z;
	z = __ieee754_y0f(x);
	if(_LIB_VERSION == _IEEE_ || isnanf(x) ) return z;
        if(x <= (float)0.0){
                if(x==(float)0.0)
                    /* d= -one/(x-x); */
                    return (float)__kernel_standard(_R4,(double)x,(double)x,108);
                else
                    /* d = zero/(x-x); */
                    return (float)__kernel_standard(_R4,(double)x,(double)x,109);
        }
	if(x>(float)X_TLOSS) {
		/* y0(x>X_TLOSS) */
	        return (float)__kernel_standard(_R4,(double)x,(double)x,135);
	} else
	    return z;
#endif
}

#endif /* defined (_LIBM_REENT) || ! defined (_REENT_ONLY) */
