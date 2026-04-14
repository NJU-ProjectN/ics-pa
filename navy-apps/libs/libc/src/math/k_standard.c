
/* @(#)k_standard.c 5.1 93/09/24 */
/*
 * ====================================================
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunPro, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice 
 * is preserved.
 * ====================================================
 *
 */

#include "fdlibm.h"
#include <errno.h>

#ifndef _USE_WRITE
#include <stdio.h>			/* fputs(), stderr */
#define	WRITE2(u,v)	fputs(u, _stderr_r(p))
#else	/* !defined(_USE_WRITE) */
#include <unistd.h>			/* write */
#define	WRITE2(u,v)	_write_r(p, 2, u, v)
#undef fflush
#endif	/* !defined(_USE_WRITE) */

#ifdef __STDC__
static const double zero = 0.0;	/* used as const */
#else
static double zero = 0.0;	/* used as const */
#endif

/* 
 * Standard conformance (non-IEEE) on exception cases.
 * Mapping:
 *	1 -- acos(|x|>1)
 *	2 -- asin(|x|>1)
 *	3 -- atan2(+-0,+-0)
 *	4 -- hypot overflow
 *	5 -- cosh overflow
 *	6 -- exp overflow
 *	7 -- exp underflow
 *	8 -- y0(0)
 *	9 -- y0(-ve)
 *	10-- y1(0)
 *	11-- y1(-ve)
 *	12-- yn(0)
 *	13-- yn(-ve)
 *	14-- lgamma(finite) overflow
 *	15-- lgamma(-integer)
 *	16-- log(0)
 *	17-- log(x<0)
 *	18-- log10(0)
 *	19-- log10(x<0)
 *	20-- pow(0.0,0.0)
 *	21-- pow(x,y) overflow
 *	22-- pow(x,y) underflow
 *	23-- pow(0,negative) 
 *	24-- pow(neg,non-integral)
 *	25-- sinh(finite) overflow
 *	26-- sqrt(negative)
 *      27-- fmod(x,0)
 *      28-- remainder(x,0)
 *	29-- acosh(x<1)
 *	30-- atanh(|x|>1)
 *	31-- atanh(|x|=1)
 *	32-- scalb overflow
 *	33-- scalb underflow
 *	34-- j0(|x|>X_TLOSS)
 *	35-- y0(x>X_TLOSS)
 *	36-- j1(|x|>X_TLOSS)
 *	37-- y1(x>X_TLOSS)
 *	38-- jn(|x|>X_TLOSS, n)
 *	39-- yn(x>X_TLOSS, n)
 *	40-- gamma(finite) overflow
 *	41-- gamma(-integer)
 *	42-- pow(NaN,0.0)
 */

#include <assert.h>

#ifdef __STDC__
	double __kernel_standard(struct _reent *p, double x, double y,
				 int type) 
#else
	double __kernel_standard(p,x,y,type) 
	struct _reent *p; double x,y; int type;
#endif
{
  assert(0);
}


