/* wf_j1.c -- float version of w_j1.c.
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
 * wrapper of j1f,y1f 
 */

#include "fdlibm.h"

#ifdef _LIBM_REENT
#define j1f _j1f_r
#define y1f _y1f_r
#endif

#if defined (_LIBM_REENT) || ! defined (_REENT_ONLY)
#ifdef __STDC__
	float j1f(_R1 float x)		/* wrapper j1f */
#else
	float j1f(_R2 x)			/* wrapper j1f */
	_R3 float x;
#endif
{
#ifdef _IEEE_LIBM
	return __ieee754_j1f(x);
#else
	float z;
	z = __ieee754_j1f(x);
	if(_LIB_VERSION == _IEEE_ || isnanf(x) ) return z;
	if(fabsf(x)>(float)X_TLOSS) {
		/* j1(|x|>X_TLOSS) */
	        return (float)__kernel_standard(_R4,(double)x,(double)x,136);
	} else
	    return z;
#endif
}

#ifdef __STDC__
	float y1f(_R1 float x)		/* wrapper y1f */
#else
	float y1f(_R2 x)			/* wrapper y1f */
	_R3 float x;
#endif
{
#ifdef _IEEE_LIBM
	return __ieee754_y1f(x);
#else
	float z;
	z = __ieee754_y1f(x);
	if(_LIB_VERSION == _IEEE_ || isnanf(x) ) return z;
        if(x <= (float)0.0){
                if(x==(float)0.0)
                    /* d= -one/(x-x); */
                    return (float)__kernel_standard(_R4,(double)x,(double)x,110);
                else
                    /* d = zero/(x-x); */
                    return (float)__kernel_standard(_R4,(double)x,(double)x,111);
        }
	if(x>(float)X_TLOSS) {
		/* y1(x>X_TLOSS) */
	        return (float)__kernel_standard(_R4,(double)x,(double)x,137);
	} else
	    return z;
#endif
}

#endif /* defined (_LIBM_REENT) || ! defined (_REENT_ONLY) */
