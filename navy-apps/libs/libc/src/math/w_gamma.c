
/* @(#)w_gamma.c 5.1 93/09/24 */
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

/*
FUNCTION
        <<gamma>>, <<gammaf>>, <<lgamma>>, <<lgammaf>>, <<gamma_r>>,
	<<gammaf_r>>, <<lgamma_r>>, <<lgammaf_r>>---logarithmic gamma
	function
INDEX
gamma
INDEX
gammaf
INDEX
lgamma
INDEX
lgammaf
INDEX
gamma_r
INDEX
gammaf_r
INDEX
lgamma_r
INDEX
lgammaf_r

ANSI_SYNOPSIS
#include <math.h>
double gamma(double <[x]>);
float gammaf(float <[x]>);
double lgamma(double <[x]>);
float lgammaf(float <[x]>);
double gamma_r(double <[x]>, int *<[signgamp]>);
float gammaf_r(float <[x]>, int *<[signgamp]>);
double lgamma_r(double <[x]>, int *<[signgamp]>);
float lgammaf_r(float <[x]>, int *<[signgamp]>);

TRAD_SYNOPSIS
#include <math.h>
double gamma(<[x]>)
double <[x]>;
float gammaf(<[x]>)
float <[x]>;
double lgamma(<[x]>)
double <[x]>;
float lgammaf(<[x]>)
float <[x]>;
double gamma_r(<[x]>, <[signgamp]>)
double <[x]>;
int <[signgamp]>;
float gammaf_r(<[x]>, <[signgamp]>)
float <[x]>;
int <[signgamp]>;
double lgamma_r(<[x]>, <[signgamp]>)
double <[x]>;
int <[signgamp]>;
float lgammaf_r(<[x]>, <[signgamp]>)
float <[x]>;
int <[signgamp]>;

DESCRIPTION
<<gamma>> calculates 
@tex
$\mit ln\bigl(\Gamma(x)\bigr)$, 
@end tex
the natural logarithm of the gamma function of <[x]>.  The gamma function
(<<exp(gamma(<[x]>))>>) is a generalization of factorial, and retains
the property that  
@ifinfo
<<exp(gamma(N))>> is equivalent to <<N*exp(gamma(N-1))>>.
@end ifinfo
@tex
$\mit \Gamma(N)\equiv N\times\Gamma(N-1)$.
@end tex
Accordingly, the results of the gamma function itself grow very
quickly.  <<gamma>> is defined as 
@tex
$\mit ln\bigl(\Gamma(x)\bigr)$ rather than simply $\mit \Gamma(x)$
@end tex
@ifinfo
the natural log of the gamma function, rather than the gamma function
itself, 
@end ifinfo
to extend the useful range of results representable.

The sign of the result is returned in the global variable <<signgam>>,
which is declared in math.h.

<<gammaf>> performs the same calculation as <<gamma>>, but uses and
returns <<float>> values.

<<lgamma>> and <<lgammaf>> are alternate names for <<gamma>> and
<<gammaf>>.  The use of <<lgamma>> instead of <<gamma>> is a reminder
that these functions compute the log of the gamma function, rather
than the gamma function itself.

The functions <<gamma_r>>, <<gammaf_r>>, <<lgamma_r>>, and
<<lgammaf_r>> are just like <<gamma>>, <<gammaf>>, <<lgamma>>, and
<<lgammaf>>, respectively, but take an additional argument.  This
additional argument is a pointer to an integer.  This additional
argument is used to return the sign of the result, and the global
variable <<signgam>> is not used.  These functions may be used for
reentrant calls (but they will still set the global variable <<errno>>
if an error occurs).

Do not confuse the function <<gamma_r>>, which takes an additional
argument which is a pointer to an integer, with the function
<<_gamma_r>>, which takes an additional argument which is a pointer to
a reentrancy structure.

RETURNS
Normally, the computed result is returned.  

When <[x]> is a nonpositive integer, <<gamma>> returns <<HUGE_VAL>>
and <<errno>> is set to <<EDOM>>.  If the result overflows, <<gamma>>
returns <<HUGE_VAL>> and <<errno>> is set to <<ERANGE>>.

You can modify this error treatment using <<matherr>>.

PORTABILITY
Neither <<gamma>> nor <<gammaf>> is ANSI C.  */

/* double gamma(double x)
 * Return the logarithm of the Gamma function of x.
 *
 * Method: call gamma_r
 */

#include "fdlibm.h"

#ifdef _LIBM_REENT
#define gamma _gamma_r
#endif

#if defined (_LIBM_REENT) || ! defined (_REENT_ONLY)

#ifdef __STDC__
	double gamma(_R1 double x)
#else
	double gamma(_R2 x)
	_R3 double x;
#endif
{
#ifndef _DOUBLE_IS_32BITS
#ifdef _IEEE_LIBM
	return __ieee754_gamma_r(x,&_R4->_signgam);
#else
        double y;
        y = __ieee754_gamma_r(x,&_R4->_signgam);
        if(_LIB_VERSION == _IEEE_) return y;
        if(!finite(y)&&finite(x)) {
            if(floor(x)==x&&x<=0.0)
                return __kernel_standard(_R4,x,x,41); /* gamma pole */
            else
                return __kernel_standard(_R4,x,x,40); /* gamma overflow */
        } else
            return y;
#endif
#else /* defined (_DOUBLE_IS_32BITS) */
	return (double) _gammaf_r (_R4, (float) x);
#endif /* defined (_DOUBLE_IS_32BITS) */
}             

#endif /* defined (_LIBM_REENT) || ! defined (_REENT_ONLY) */
