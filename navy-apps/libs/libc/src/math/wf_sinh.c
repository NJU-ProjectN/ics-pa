/* wf_sinh.c -- float version of w_sinh.c.
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
 * wrapper sinhf(x)
 */

#include "fdlibm.h"

#ifdef _LIBM_REENT
#define sinhf _sinhf_r
#endif

#if defined (_LIBM_REENT) || ! defined (_REENT_ONLY)

#ifdef __STDC__
	float sinhf(_R1 float x)		/* wrapper sinhf */
#else
	float sinhf(_R2 x)			/* wrapper sinhf */
	_R3 float x;
#endif
{
#ifdef _IEEE_LIBM
	return __ieee754_sinhf(x);
#else
	float z; 
	z = __ieee754_sinhf(x);
	if(_LIB_VERSION == _IEEE_) return z;
	if(!finitef(z)&&finitef(x)) {
	    /* sinhf overflow */
	    return (float)__kernel_standard(_R4,(double)x,(double)x,125);
	} else
	    return z;
#endif
}

#endif /* defined (_LIBM_REENT) || ! defined (_REENT_ONLY) */
