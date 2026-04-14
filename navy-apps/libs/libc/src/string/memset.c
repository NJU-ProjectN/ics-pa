/*
FUNCTION
	<<memset>>---set an area of memory

INDEX
	memset

ANSI_SYNOPSIS
	#include <string.h>
	void *memset(const void *<[dst]>, int <[c]>, size_t <[length]>);

TRAD_SYNOPSIS
	#include <string.h>
	void *memset(<[dst]>, <[c]>, <[length]>)
	void *<[dst]>;
	int <[c]>;
	size_t <[length]>;

DESCRIPTION
	This function converts the argument <[c]> into an unsigned
	char and fills the first <[length]> characters of the array
	pointed to by <[dst]> to the value.

RETURNS
	<<memset>> returns the value of <[m]>.

PORTABILITY
<<memset>> is ANSI C.

    <<memset>> requires no supporting OS subroutines.

QUICKREF
	memset ansi pure
*/

#include <string.h>

#define STRIDE int

_PTR 
_DEFUN (memset, (m, c, n),
	_PTR m _AND
	int c _AND
	size_t n)
{
  char *s = (char *) m;
  int count;
  STRIDE *ip;

  if (c == 0)
    {
      /* Special case when storing zero onto an aligned boundary */
      count = (((int) s) & (sizeof (STRIDE) - 1));
      while (n != 0 && count > 0 && count != sizeof (STRIDE))
	{
	  *s++ = 0;
	  count++;
	  n--;
	}
      ip = (STRIDE *) s;
      while (n >= sizeof (STRIDE))
	{
	  *ip++ = 0;
	  n -= sizeof (STRIDE);
	}
      s = (char *) ip;
    }
  while (n-- != 0)
    {
      *s++ = (char) c;
    }

  return m;
}
