/*
FUNCTION
	<<memcpy>>---copy memory regions

ANSI_SYNOPSIS
	#include <string.h>
	void* memcpy(void *<[out]>, const void *<[in]>, size_t <[n]>);

TRAD_SYNOPSIS
	void *memcpy(<[out]>, <[in]>, <[n]>
	void *<[out]>;
	void *<[in]>;
	size_t <[n]>;

DESCRIPTION
	This function copies <[n]> bytes from the memory region
	pointed to by <[in]> to the memory region pointed to by
	<[out]>.

	If the regions overlap, the behavior is undefined.

RETURNS
	<<memcpy>> returns a pointer to the first byte of the <[out]>
	region.

PORTABILITY
<<memcpy>> is ANSI C.

<<memcpy>> requires no supporting OS subroutines.

QUICKREF
	memcpy ansi pure
*/

#include <_ansi.h>
#include <stddef.h>

_PTR
_DEFUN (memcpy, (out, in, length),
	_PTR out _AND
	_CONST _PTR in _AND
	size_t length)
{
  char *dst = (char *) out;
  char *src = (char *) in;

  _PTR save = out;

  while (length--)
    {
      *dst++ = *src++;
    }

  return save;
}
