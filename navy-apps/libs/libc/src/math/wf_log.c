/* wf_log.c -- float version of w_log.c.
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
 * wrapper logf(x)
 */

#include "fdlibm.h"

#ifdef _LIBM_REENT
#define logf _logf_r
#endif

#if defined (_LIBM_REENT) || ! defined (_REENT_ONLY)

#ifdef __STDC__
	float logf(_R1 float x)		/* wrapper logf */
#else
	float logf(_R2 x)			/* wrapper logf */
	_R3 float x;
#endif
{
#ifdef _IEEE_LIBM
	return __ieee754_logf(x);
#else
	float z;
	z = __ieee754_logf(x);
	if(_LIB_VERSION == _IEEE_ || isnanf(x) || x > (float)0.0) return z;
	if(x==(float)0.0)
	    /* logf(0) */
	    return (float)__kernel_standard(_R4,(double)x,(double)x,116);
	else 
	    /* logf(x<0) */
	    return (float)__kernel_standard(_R4,(double)x,(double)x,117);
#endif
}

#endif /* defined (_LIBM_REENT) || ! defined (_REENT_ONLY) */
