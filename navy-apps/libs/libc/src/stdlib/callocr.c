/*
Doc in calloc.c

Split out of calloc.c so that calloc can be redefined without causing link errors when
using other library routines which call calloc_r.
*/ 

#include <string.h>
#include <stdlib.h>
_PTR
_DEFUN (_calloc_r, (ptr, n, size),
	struct _reent * ptr _AND
	size_t n _AND
	size_t size)
{
  register char *cp;

  n *= size;
  cp = _malloc_r (ptr, n);
  if (cp == 0)
    return 0;
  memset (cp, '\0', n);
  return cp;
}
