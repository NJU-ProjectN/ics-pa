/*
 * infinityf () returns the representation of infinity.
 * Added by Cygnus Support.
 */

#include "fdlibm.h"

	float infinityf()
{
	float x;

	SET_FLOAT_WORD(x,0x7f800000);
	return x;
}
