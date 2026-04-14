/* wf_atan2.c -- float version of w_atan2.c.
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
 * wrapper atan2f(y,x)
 */

#include "fdlibm.h"

#ifdef _LIBM_REENT
#define atan2f _atan2f_r
#endif

#if defined (_LIBM_REENT) || ! defined (_REENT_ONLY)

#ifdef __STDC__
	float atan2f(_R1 float y, float x)		/* wrapper atan2f */
#else
	float atan2f(_R2 y,x)			/* wrapper atan2 */
	_R3 float y,x;
#endif
{
#ifdef _IEEE_LIBM
	return __ieee754_atan2f(y,x);
#else
	float z;
	z = __ieee754_atan2f(y,x);
	if(_LIB_VERSION == _IEEE_||isnanf(x)||isnanf(y)) return z;
	if(x==(float)0.0&&y==(float)0.0) {
		/* atan2f(+-0,+-0) */
	        return (float)__kernel_standard(_R4,(double)y,(double)x,103);
	} else
	    return z;
#endif
}

#endif /* defined (_LIBM_REENT) || ! defined (_REENT_ONLY) */
