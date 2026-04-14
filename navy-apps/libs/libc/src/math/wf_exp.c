/* wf_exp.c -- float version of w_exp.c.
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
 * wrapper expf(x)
 */

#include "fdlibm.h"

#ifdef _LIBM_REENT
#define expf _expf_r
#endif

#if defined (_LIBM_REENT) || ! defined (_REENT_ONLY)

#ifdef __STDC__
static const float
#else
static float
#endif
o_threshold=  8.8721679688e+01,  /* 0x42b17180 */
u_threshold= -1.0397208405e+02;  /* 0xc2cff1b5 */

#ifdef __STDC__
	float expf(_R1 float x)		/* wrapper expf */
#else
	float expf(_R2 x)			/* wrapper expf */
	_R3 float x;
#endif
{
#ifdef _IEEE_LIBM
	return __ieee754_expf(x);
#else
	float z;
	z = __ieee754_expf(x);
	if(_LIB_VERSION == _IEEE_) return z;
	if(finitef(x)) {
	    if(x>o_threshold)
	        /* exp overflow */
	        return (float)__kernel_standard(_R4,(double)x,(double)x,106);
	    else if(x<u_threshold)
	        /* exp underflow */
	        return (float)__kernel_standard(_R4,(double)x,(double)x,107);
	} 
	return z;
#endif
}

#endif /* defined (_LIBM_REENT) || ! defined (_REENT_ONLY) */
