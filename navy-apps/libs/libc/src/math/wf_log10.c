/* wf_log10.c -- float version of w_log10.c.
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
 * wrapper log10f(X)
 */

#include "fdlibm.h"

#ifdef _LIBM_REENT
#define log10f _log10f_r
#endif

#if defined (_LIBM_REENT) || ! defined (_REENT_ONLY)

#ifdef __STDC__
	float log10f(_R1 float x)		/* wrapper log10f */
#else
	float log10f(_R2 x)			/* wrapper log10f */
	_R3 float x;
#endif
{
#ifdef _IEEE_LIBM
	return __ieee754_log10f(x);
#else
	float z;
	z = __ieee754_log10f(x);
	if(_LIB_VERSION == _IEEE_ || isnanf(x)) return z;
	if(x<=(float)0.0) {
	    if(x==(float)0.0)
	        /* log10(0) */
	        return (float)__kernel_standard(_R4,(double)x,(double)x,118);
	    else 
	        /* log10(x<0) */
	        return (float)__kernel_standard(_R4,(double)x,(double)x,119);
	} else
	    return z;
#endif
}

#endif /* defined (_LIBM_REENT) || ! defined (_REENT_ONLY) */
