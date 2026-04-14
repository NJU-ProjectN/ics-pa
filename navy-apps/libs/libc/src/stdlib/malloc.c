/* VxWorks provides its own version of malloc, and we can't use this
   one because VxWorks does not provide sbrk.  So we have a hook to
   not compile this code.  */

/* The routines here are simple cover fns to the routines that do the real
   work (the reentrant versions).  */
/* FIXME: Does the warning below (see WARNINGS) about non-reentrancy still
   apply?  A first guess would be "no", but how about reentrancy in the *same*
   thread?  */

#ifdef MALLOC_PROVIDED

int _dummy_malloc = 1;

#else

/*
FUNCTION
<<malloc>>, <<realloc>>, <<free>>---manage memory

INDEX
	malloc
INDEX
	realloc
INDEX
	free
INDEX
	_malloc_r
INDEX
	_realloc_r
INDEX
	_free_r

ANSI_SYNOPSIS
	#include <stdlib.h>
	void *malloc(size_t <[nbytes]>);
	void *realloc(void *<[aptr]>, size_t <[nbytes]>);
	void free(void *<[aptr]>);

	void *_malloc_r(void *<[reent]>, size_t <[nbytes]>);
	void *_realloc_r(void *<[reent]>, 
                         void *<[aptr]>, size_t <[nbytes]>);
	void _free_r(void *<[reent]>, void *<[aptr]>);

TRAD_SYNOPSIS
	#include <stdlib.h>
	char *malloc(<[nbytes]>)
	size_t <[nbytes]>;

	char *realloc(<[aptr]>, <[nbytes]>)
	char *<[aptr]>;
	size_t <[nbytes]>;

	void free(<[aptr]>)
	char *<[aptr]>;

	char *_malloc_r(<[reent]>,<[nbytes]>)
	char *<[reent]>;
	size_t <[nbytes]>;

	char *_realloc_r(<[reent]>, <[aptr]>, <[nbytes]>)
	char *<[reent]>;
	char *<[aptr]>;
	size_t <[nbytes]>;

	void _free_r(<[reent]>, <[aptr]>)
	char *<[reent]>;
	char *<[aptr]>;

DESCRIPTION
These functions manage a pool of system memory.

Use <<malloc>> to request allocation of an object with at least
<[nbytes]> bytes of storage available.  If the space is available,
<<malloc>> returns a pointer to a newly allocated block as its result.

If you already have a block of storage allocated by <<malloc>>, but
you no longer need all the space allocated to it, you can make it
smaller by calling <<realloc>> with both the object pointer and the
new desired size as arguments.  <<realloc>> guarantees that the
contents of the smaller object match the beginning of the original object.

Similarly, if you need more space for an object, use <<realloc>> to
request the larger size; again, <<realloc>> guarantees that the
beginning of the new, larger object matches the contents of the
original object.

When you no longer need an object originally allocated by <<malloc>>
or <<realloc>> (or the related function <<calloc>>), return it to the
memory storage pool by calling <<free>> with the address of the object
as the argument.  You can also use <<realloc>> for this purpose by
calling it with <<0>> as the <[nbytes]> argument.

The alternate functions <<_malloc_r>>, <<_realloc_r>>, and <<_free_r>>
are reentrant versions.  The extra argument <[reent]> is a pointer to
a reentrancy structure.

RETURNS
<<malloc>> returns a pointer to the newly allocated space, if
successful; otherwise it returns <<NULL>>.  If your application needs
to generate empty objects, you may use <<malloc(0)>> for this purpose.

<<realloc>> returns a pointer to the new block of memory, or <<NULL>>
if a new block could not be allocated.  <<NULL>> is also the result
when you use `<<realloc(<[aptr]>,0)>>' (which has the same effect as
`<<free(<[aptr]>)>>').  You should always check the result of
<<realloc>>; successful reallocation is not guaranteed even when
you request a smaller object.

<<free>> does not return a result.



PORTABILITY
<<malloc>>, <<realloc>>, and <<free>> are specified by the ANSI C
standard, but other conforming implementations of <<malloc>> may
behave differently when <[nbytes]> is zero.

Supporting OS subroutines required: <<sbrk>>, <<write>> (if WARN_VLIMIT).
*/

#include <_ansi.h>
#include <reent.h>
#include <stdlib.h>
#include "malloc.h"

#ifndef _REENT_ONLY

_PTR
_DEFUN (malloc, (nbytes),
	size_t nbytes)		/* get a block */
{
  return _malloc_r (_REENT, nbytes);
}

void
_DEFUN (free, (aptr),
	_PTR aptr)
{
  _free_r (_REENT, aptr);
}

_PTR
_DEFUN (realloc, (ap, nbytes),
	_PTR ap _AND
	size_t nbytes)
{
  return _realloc_r (_REENT, ap, nbytes);
}

#endif

#endif /* ! defined (MALLOC_PROVIDED) */
