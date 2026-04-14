/* wf_sqrt.c -- float version of w_sqrt.c.
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
 * wrapper sqrtf(x)
 */

#include "fdlibm.h"

#ifdef _LIBM_REENT
#define sqrtf _sqrtf_r
#endif

#if defined (_LIBM_REENT) || ! defined (_REENT_ONLY)

#ifdef __STDC__
	float sqrtf(_R1 float x)		/* wrapper sqrtf */
#else
	float sqrt(_R2 x)			/* wrapper sqrtf */
	_R3 float x;
#endif
{
#ifdef _IEEE_LIBM
	return __ieee754_sqrtf(x);
#else
	float z;
	z = __ieee754_sqrtf(x);
	if(_LIB_VERSION == _IEEE_ || isnanf(x)) return z;
	if(x<(float)0.0) {
	    /* sqrtf(negative) */
	    return (float)__kernel_standard(_R4,(double)x,(double)x,126);
	} else
	    return z;
#endif
}

#endif /* defined (_LIBM_REENT) || ! defined (_REENT_ONLY) */
