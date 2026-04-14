/*
FUNCTION
<<calloc>>---allocate space for arrays

INDEX
	calloc

INDEX
	_calloc_r

ANSI_SYNOPSIS
	#include <stdlib.h>
	void *calloc(size_t <[n]>, size_t <[s]>);
	void *calloc_r(void *<[reent]>, size_t <n>, <size_t> <[s]>);
	
TRAD_SYNOPSIS
	#include <stdlib.h>
	char *calloc(<[n]>, <[s]>)
	size_t <[n]>, <[s]>;

	char *_calloc_r(<[reent]>, <[n]>, <[s]>)
	char *<[reent]>;
	size_t <[n]>;
	size_t <[s]>;



DESCRIPTION
Use <<calloc>> to request a block of memory sufficient to hold an
array of <[n]> elements, each of which has size <[s]>.

The memory allocated by <<calloc>> comes out of the same memory pool
used by <<malloc>>, but the memory block is initialized to all zero
bytes.  (To avoid the overhead of initializing the space, use
<<malloc>> instead.)

The alternate functios <<_calloc_r>> is reentrant.
The extra argument <[reent]> is a pointer to a reentrancy structure.

RETURNS
If successful, a pointer to the newly allocated space.

If unsuccessful, <<NULL>>.

PORTABILITY
<<calloc>> is ANSI.

Supporting OS subroutines required: <<close>>, <<fstat>>, <<isatty>>,
<<lseek>>, <<read>>, <<sbrk>>, <<write>>.
*/

#include <string.h>
#include <stdlib.h>

#ifndef _REENT_ONLY

_PTR
_DEFUN (calloc, (n, size),
	size_t n _AND
	size_t size)
{
  register char *cp;

  n *= size;

  /* While it's tempting to have this call _malloc_r() instead, don't do it.
     The application may have provided it's own malloc() and we want free()
     (which will also be replaced) to properly free the buffer we allocate.  */

  cp = malloc (n);
  if (cp == 0)
    return 0;
  memset (cp, '\0', n);
  return cp;
}

#endif

