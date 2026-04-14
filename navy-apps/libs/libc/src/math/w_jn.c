
/* @(#)w_jn.c 5.1 93/09/24 */
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
 * wrapper jn(int n, double x), yn(int n, double x)
 * floating point Bessel's function of the 1st and 2nd kind
 * of order n
 *          
 * Special cases:
 *	y0(0)=y1(0)=yn(n,0) = -inf with division by zero signal;
 *	y0(-ve)=y1(-ve)=yn(n,-ve) are NaN with invalid signal.
 * Note 2. About jn(n,x), yn(n,x)
 *	For n=0, j0(x) is called,
 *	for n=1, j1(x) is called,
 *	for n<x, forward recursion us used starting
 *	from values of j0(x) and j1(x).
 *	for n>x, a continued fraction approximation to
 *	j(n,x)/j(n-1,x) is evaluated and then backward
 *	recursion is used starting from a supposed value
 *	for j(n,x). The resulting value of j(0,x) is
 *	compared with the actual value to correct the
 *	supposed value of j(n,x).
 *
 *	yn(n,x) is similar in all respects, except
 *	that forward recursion is used for all
 *	values of n>1.
 *	
 */

#include "fdlibm.h"

#ifdef _LIBM_REENT
#define jn _jn_r
#define yn _yn_r
#endif

#if defined (_LIBM_REENT) || ! defined (_REENT_ONLY)

#ifdef __STDC__
	double jn(_R1 int n, double x)	/* wrapper jn */
#else
	double jn(_R2 n,x)			/* wrapper jn */
	_R3 double x; int n;
#endif
{
#ifndef _DOUBLE_IS_32BITS
#ifdef _IEEE_LIBM
	return __ieee754_jn(n,x);
#else
	double z;
	z = __ieee754_jn(n,x);
	if(_LIB_VERSION == _IEEE_ || isnan(x) ) return z;
	if(fabs(x)>X_TLOSS) {
	    return __kernel_standard(_R4,(double)n,x,38); /* jn(|x|>X_TLOSS,n) */
	} else
	    return z;
#endif
#else /* defined (_DOUBLE_IS_32BITS) */
	return (double) _jnf_r (_R4, n, (float) x);
#endif /* defined (_DOUBLE_IS_32BITS) */
}

#ifdef __STDC__
	double yn(_R1 int n, double x)	/* wrapper yn */
#else
	double yn(_R2 n,x)			/* wrapper yn */
	_R3 double x; int n;
#endif
{
#ifndef _DOUBLE_IS_32BITS
#ifdef _IEEE_LIBM
	return __ieee754_yn(n,x);
#else
	double z;
	z = __ieee754_yn(n,x);
	if(_LIB_VERSION == _IEEE_ || isnan(x) ) return z;
        if(x <= 0.0){
                if(x==0.0)
                    /* d= -one/(x-x); */
                    return __kernel_standard(_R4,(double)n,x,12);
                else
                    /* d = zero/(x-x); */
                    return __kernel_standard(_R4,(double)n,x,13);
        }
	if(x>X_TLOSS) {
	    return __kernel_standard(_R4,(double)n,x,39); /* yn(x>X_TLOSS,n) */
	} else
	    return z;
#endif
#else /* defined (_DOUBLE_IS_32BITS) */
	return (double) _ynf_r (_R4, n, (float) x);
#endif /* defined (_DOUBLE_IS_32BITS) */
}

#endif /* defined (_LIBM_REENT) || ! defined (_REENT_ONLY) */
