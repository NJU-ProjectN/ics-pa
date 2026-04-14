/* wf_fmod.c -- float version of w_fmod.c.
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
 * wrapper fmodf(x,y)
 */

#include "fdlibm.h"

#ifdef _LIBM_REENT
#define fmodf _fmodf_r
#endif

#if defined (_LIBM_REENT) || ! defined (_REENT_ONLY)

#ifdef __STDC__
	float fmodf(_R1 float x, float y)	/* wrapper fmodf */
#else
	float fmodf(_R2 x,y)		/* wrapper fmodf */
	_R3 float x,y;
#endif
{
#ifdef _IEEE_LIBM
	return __ieee754_fmodf(x,y);
#else
	float z;
	z = __ieee754_fmodf(x,y);
	if(_LIB_VERSION == _IEEE_ ||isnanf(y)||isnanf(x)) return z;
	if(y==(float)0.0) {
		/* fmodf(x,0) */
	        return (float)__kernel_standard(_R4,(double)x,(double)y,127);
	} else
	    return z;
#endif
}

#endif /* defined (_LIBM_REENT) || ! defined (_REENT_ONLY) */
