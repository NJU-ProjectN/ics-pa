/* wf_cosh.c -- float version of w_cosh.c.
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
 * wrapper coshf(x)
 */

#include "fdlibm.h"

#ifdef _LIBM_REENT
#define coshf _coshf_r
#endif

#if defined (_LIBM_REENT) || ! defined (_REENT_ONLY)
#ifdef __STDC__
	float coshf(_R1 float x)		/* wrapper coshf */
#else
	float coshf(_R2 x)			/* wrapper coshf */
	_R3 float x;
#endif
{
#ifdef _IEEE_LIBM
	return __ieee754_coshf(x);
#else
	float z;
	z = __ieee754_coshf(x);
	if(_LIB_VERSION == _IEEE_ || isnanf(x)) return z;
	if(fabsf(x)>(float)8.9415985107e+01) {	
		/* cosh overflow */
	        return (float)__kernel_standard(_R4,(double)x,(double)x,105);
	} else
	    return z;
#endif
}

#endif /* defined (_LIBM_REENT) || ! defined (_REENT_ONLY) */
