/* wf_scalb.c -- float version of w_scalb.c.
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
 * wrapper scalbf(float x, float fn) is provide for
 * passing various standard test suite. One 
 * should use scalbn() instead.
 */

#include "fdlibm.h"
#include <errno.h>

#ifdef _LIBM_REENT
#define scalbf _scalbf_r
#endif

#if defined (_LIBM_REENT) || ! defined (_REENT_ONLY)

#ifdef __STDC__
#ifdef _SCALB_INT
	float scalbf(_R1 float x, int fn)		/* wrapper scalbf */
#else
	float scalbf(_R1 float x, float fn)		/* wrapper scalbf */
#endif
#else
	float scalbf(_R2 x,fn)			/* wrapper scalbf */
#ifdef _SCALB_INT
	_R3 float x; int fn;
#else
	_R3 float x,fn;
#endif
#endif
{
#ifdef _IEEE_LIBM
	return __ieee754_scalbf(x,fn);
#else
	float z;
	z = __ieee754_scalbf(x,fn);
	if(_LIB_VERSION == _IEEE_) return z;
	if(!(finitef(z)||isnanf(z))&&finitef(x)) {
	    /* scalbf overflow */
	    return (float)__kernel_standard(_R4,(double)x,(double)fn,132);
	}
	if(z==(float)0.0&&z!=x) {
	    /* scalbf underflow */
	    return (float)__kernel_standard(_R4,(double)x,(double)fn,133);
	} 
#ifndef _SCALB_INT
	if(!finitef(fn)) _R4->_errno = ERANGE;
#endif
	return z;
#endif 
}

#endif /* defined (_LIBM_REENT) || ! defined (_REENT_ONLY) */
