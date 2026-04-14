
/* @(#)w_scalb.c 5.1 93/09/24 */
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
 * wrapper scalb(double x, double fn) is provide for
 * passing various standard test suite. One 
 * should use scalbn() instead.
 */

#include "fdlibm.h"
#include <errno.h>

#ifdef _LIBM_REENT
#define scalb _scalb_r
#endif

#if defined (_LIBM_REENT) || ! defined (_REENT_ONLY)

#ifdef __STDC__
#ifdef _SCALB_INT
	double scalb(_R1 double x, int fn)		/* wrapper scalb */
#else
	double scalb(_R1 double x, double fn)	/* wrapper scalb */
#endif
#else
	double scalb(_R2 x,fn)			/* wrapper scalb */
#ifdef _SCALB_INT
	_R3 double x; int fn;
#else
	_R3 double x,fn;
#endif
#endif
{
#ifndef _DOUBLE_IS_32BITS
#ifdef _IEEE_LIBM
	return __ieee754_scalb(x,fn);
#else
	double z;
	z = __ieee754_scalb(x,fn);
	if(_LIB_VERSION == _IEEE_) return z;
	if(!(finite(z)||isnan(z))&&finite(x)) {
	    return __kernel_standard(_R4,x,(double)fn,32); /* scalb overflow */
	}
	if(z==0.0&&z!=x) {
	    return __kernel_standard(_R4,x,(double)fn,33); /* scalb underflow */
	} 
#ifndef _SCALB_INT
	if(!finite(fn)) _R4->_errno = ERANGE;
#endif
	return z;
#endif 
#else /* defined (_DOUBLE_IS_32BITS) */
	return (double) _scalbf_r (_R4, (float) x, fn);
#endif /* defined (_DOUBLE_IS_32BITS) */
}

#endif /* defined (_LIBM_REENT) || ! defined (_REENT_ONLY) */
