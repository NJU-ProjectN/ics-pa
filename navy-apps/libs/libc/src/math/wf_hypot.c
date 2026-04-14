/* wf_hypot.c -- float version of w_hypot.c.
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
 * wrapper hypotf(x,y)
 */

#include "fdlibm.h"

#ifdef _LIBM_REENT
#define hypotf _hypotf_r
#endif

#if defined (_LIBM_REENT) || ! defined (_REENT_ONLY)

#ifdef __STDC__
	float hypotf(_R1 float x, float y)	/* wrapper hypotf */
#else
	float hypotf(_R2 x,y)		/* wrapper hypotf */
	_R3 float x,y;
#endif
{
#ifdef _IEEE_LIBM
	return __ieee754_hypotf(x,y);
#else
	float z;
	z = __ieee754_hypotf(x,y);
	if(_LIB_VERSION == _IEEE_) return z;
	if((!finitef(z))&&finitef(x)&&finitef(y))
	    /* hypot overflow */
	    return (float)__kernel_standard(_R4,(double)x,(double)y,104);
	else
	    return z;
#endif
}

#endif /* defined (_LIBM_REENT) || ! defined (_REENT_ONLY) */
