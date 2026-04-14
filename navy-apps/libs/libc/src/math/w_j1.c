
/* @(#)w_j1.c 5.1 93/09/24 */
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
 * wrapper of j1,y1 
 */

#include "fdlibm.h"

#ifdef _LIBM_REENT
#define j1 _j1_r
#define y1 _y1_r
#endif

#if defined (_LIBM_REENT) || ! defined (_REENT_ONLY)

#ifdef __STDC__
	double j1(_R1 double x)		/* wrapper j1 */
#else
	double j1(_R2 x)			/* wrapper j1 */
	_R3 double x;
#endif
{
#ifndef _DOUBLE_IS_32BITS
#ifdef _IEEE_LIBM
	return __ieee754_j1(x);
#else
	double z;
	z = __ieee754_j1(x);
	if(_LIB_VERSION == _IEEE_ || isnan(x) ) return z;
	if(fabs(x)>X_TLOSS) {
	        return __kernel_standard(_R4,x,x,36); /* j1(|x|>X_TLOSS) */
	} else
	    return z;
#endif
#else /* defined (_DOUBLE_IS_32BITS) */
	return (double) _j1f_r (_R4, (float) x);
#endif /* defined (_DOUBLE_IS_32BITS) */
}

#ifdef __STDC__
	double y1(_R1 double x)		/* wrapper y1 */
#else
	double y1(_R2 x)			/* wrapper y1 */
	_R3 double x;
#endif
{
#ifndef _DOUBLE_IS_32BITS
#ifdef _IEEE_LIBM
	return __ieee754_y1(x);
#else
	double z;
	z = __ieee754_y1(x);
	if(_LIB_VERSION == _IEEE_ || isnan(x) ) return z;
        if(x <= 0.0){
                if(x==0.0)
                    /* d= -one/(x-x); */
                    return __kernel_standard(_R4,x,x,10);
                else
                    /* d = zero/(x-x); */
                    return __kernel_standard(_R4,x,x,11);
        }
	if(x>X_TLOSS) {
	        return __kernel_standard(_R4,x,x,37); /* y1(x>X_TLOSS) */
	} else
	    return z;
#endif
#else /* defined (_DOUBLE_IS_32BITS) */
	return (double) _y1f_r (_R4, (float) x);
#endif /* defined (_DOUBLE_IS_32BITS) */
}

#endif /* defined (_LIBM_REENT) || ! defined (_REENT_ONLY) */
