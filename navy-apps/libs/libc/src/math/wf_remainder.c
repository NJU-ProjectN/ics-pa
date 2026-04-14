/* wf_remainder.c -- float version of w_remainder.c.
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
 * wrapper remainderf(x,p)
 */

#include "fdlibm.h"

#ifdef _LIBM_REENT
#define remainderf _remainderf_r
#endif

#if defined (_LIBM_REENT) || ! defined (_REENT_ONLY)
#ifdef __STDC__
	float remainderf(_R1 float x, float y)	/* wrapper remainder */
#else
	float remainderf(_R2 x,y)			/* wrapper remainder */
	_R3 float x,y;
#endif
{
#ifdef _IEEE_LIBM
	return __ieee754_remainderf(x,y);
#else
	float z;
	z = __ieee754_remainderf(x,y);
	if(_LIB_VERSION == _IEEE_ || isnanf(y)) return z;
	if(y==(float)0.0) 
	    /* remainder(x,0) */
	    return (float)__kernel_standard(_R4,(double)x,(double)y,128);
	else
	    return z;
#endif
}

#endif /* defined (_LIBM_REENT) || ! defined (_REENT_ONLY) */
