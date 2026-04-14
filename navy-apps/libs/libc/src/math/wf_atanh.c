/* wf_atanh.c -- float version of w_atanh.c.
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
 * wrapper atanhf(x)
 */

#include "fdlibm.h"

#ifdef _LIBM_REENT
#define atanhf _atanhf_r
#endif

#if defined (_LIBM_REENT) || ! defined (_REENT_ONLY)

#ifdef __STDC__
	float atanhf(_R1 float x)		/* wrapper atanhf */
#else
	float atanhf(_R2 x)			/* wrapper atanhf */
	_R3 float x;
#endif
{
#ifdef _IEEE_LIBM
	return __ieee754_atanhf(x);
#else
	float z,y;
	z = __ieee754_atanhf(x);
	if(_LIB_VERSION == _IEEE_ || isnanf(x)) return z;
	y = fabsf(x);
	if(y>=(float)1.0) {
	    if(y>(float)1.0)
	        /* atanhf(|x|>1) */
	        return (float)__kernel_standard(_R4,(double)x,(double)x,130);
	    else 
	        /* atanhf(|x|==1) */
	        return (float)__kernel_standard(_R4,(double)x,(double)x,131);
	} else
	    return z;
#endif
}

#endif /* defined (_LIBM_REENT) || ! defined (_REENT_ONLY) */
