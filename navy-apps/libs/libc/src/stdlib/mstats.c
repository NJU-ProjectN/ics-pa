/* VxWorks provides its own version of malloc, and we can't use this
   one because VxWorks does not provide sbrk.  So we have a hook to
   not compile this code.  */

#ifdef MALLOC_PROVIDED

int _dummy_mstats = 1;

#else

#include <_ansi.h>
#include <reent.h>
#include <stdlib.h>
#include "malloc.h"

#ifdef MSTATS
#ifndef _REENT_ONLY

void
_DEFUN (mstats, (s),
	char *s)
{
  _mstats_r (_REENT, s);
}

#endif
#else

mstats ()
{				/* dummy to keep people happy */
}

#endif

#endif /* ! defined (MALLOC_PROVIDED) */
