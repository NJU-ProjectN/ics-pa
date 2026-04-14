/*
FUNCTION
	<<reent>>---definition of impure data.
	
INDEX
	reent

DESCRIPTION
	This module defines the impure data area used by the
	non-rentrant functions, such as strtok.
*/

#include <reent.h>

static struct _reent inpure_data = _REENT_INIT (inpure_data);
struct _reent *_impure_ptr = &inpure_data;

/* Interim cleanup code */

void
cleanup_glue (ptr, glue)
     struct _reent *ptr;
     struct _glue *glue;
{
  /* Have to reclaim these in reverse order: */
  if (glue->_next)
    cleanup_glue (ptr, glue->_next);

  _free_r (ptr, glue);
}

void
_reclaim_reent (ptr)
     struct _reent *ptr;
{
  if (ptr != _impure_ptr)
    {
      int k;

      /* used by mprec routines. */
      if (ptr->_freelist)
	{
	  struct _Bigint *nextone = ptr->_freelist[15]; /*  15 is _Kmax  */

	  for (;nextone;)
	    {
	      struct _Bigint *thisone = nextone;
	      nextone = nextone->_reclaim;
	      _free_r (ptr, thisone);
	    } 

	  _free_r (ptr, ptr->_freelist);
	}

      /* atexit stuff */
      if ((ptr->_atexit) && (ptr->_atexit != &ptr->_atexit0))
	{
	  struct _atexit *p, *q;
	  for (p = ptr->_atexit; p != &ptr->_atexit0;)
	    {
	      q = p;
	      p = p->_next;
	      _free_r (ptr, q);
	    }
	}

      if (ptr->_cvtbuf)
	_free_r (ptr, ptr->_cvtbuf);

      if (ptr->__sdidinit)
	{
	  /* cleanup won't reclaim memory 'coz usually it's run
	     before the program exits, and who wants to wait for that? */
	  _cleanup_r (ptr);

	  if (ptr->__sglue._next)
	    cleanup_glue (ptr, ptr->__sglue._next);
	}

      /* Malloc memory not reclaimed; no good way to return memory anyway. */

    }
}

