/* wf_acosh.c -- float version of w_acosh.c.
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

/* 
 * wrapper acoshf(x)
 */

#include "fdlibm.h"

#ifdef _LIBM_REENT
#define acoshf _acoshf_r
#endif

#if defined (_LIBM_REENT) || ! defined (_REENT_ONLY)

#ifdef __STDC__
	float acoshf(_R1 float x)		/* wrapper acoshf */
#else
	float acoshf(_R2 x)			/* wrapper acoshf */
	_R3 float x;
#endif
{
#ifdef _IEEE_LIBM
	return __ieee754_acoshf(x);
#else
	float z;
	z = __ieee754_acoshf(x);
	if(_LIB_VERSION == _IEEE_ || isnanf(x)) return z;
	if(x<(float)1.0) {
		/* acosh(x<1) */
	        return (float)__kernel_standard(_R4,(double)x,(double)x,129);
	} else
	    return z;
#endif
}

#endif /* defined (_LIBM_REENT) || ! defined (_REENT_ONLY) */
