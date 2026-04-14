/* wf_acos.c -- float version of w_acos.c.
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
 * wrap_acosf(x)
 */

#include "fdlibm.h"

#ifdef _LIBM_REENT
#define acosf _acosf_r
#endif

#if defined (_LIBM_REENT) || ! defined (_REENT_ONLY)

#ifdef __STDC__
	float acosf(_R1 float x)		/* wrapper acosf */
#else
	float acosf(_R2 x)			/* wrapper acosf */
	_R3 float x;
#endif
{
#ifdef _IEEE_LIBM
	return __ieee754_acosf(x);
#else
	float z;
	z = __ieee754_acosf(x);
	if(_LIB_VERSION == _IEEE_ || isnanf(x)) return z;
	if(fabsf(x)>(float)1.0) {
	        /* acosf(|x|>1) */
	        return (float)__kernel_standard(_R4,(double)x,(double)x,101);
	} else
	    return z;
#endif
}

#endif /* defined (_LIBM_REENT) || ! defined (_REENT_ONLY) */
