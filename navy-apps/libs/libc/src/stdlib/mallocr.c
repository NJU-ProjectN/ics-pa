/* doc in malloc.c */
/* @(#)nmalloc.c 1 (Caltech) 2/21/82
 *  This is a very fast storage allocator.  It allocates blocks of a small
 *  number of different sizes, and keeps free lists of each size.  Blocks that
 *  don't exactly fit are passed up to the next larger size.  In this
 *  implementation, the available sizes are (2^n)-4 (or -12) bytes long.
 *  This is designed for use in a program that uses vast quantities of memory,
 *  but bombs when it runs out.  To make it a little better, it warns the
 *  user when he starts to get near the end.
 *
 *  The smallest recorded block is 8 bytes.
 *
 * This code will work equally well in environments with 4 byte pointers
 * and 8 byte pointers.  Don't do anything to break this!
 *
 * This file contains only the reentrant versions of malloc().  All newlib
 * routines must call these routines directly, and not malloc().  Otherwise
 * an application might supply it's own malloc() and some newlib routines
 * would use it, while other routines would use the one below.
 */

#include <_ansi.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <reent.h>
#include <unistd.h>
#include "malloc.h"

#ifdef SMALL_MEMORY
#define ROUND_TO 0x3
#define CHUNK_POWER 9
#else
#define ROUND_TO 0x3ff
#define CHUNK_POWER 11
#endif



/* VxWorks provides its own version of malloc, and we can't use this
   one because VxWorks does not provide sbrk.  So we have a hook to
   not compile this code.  */

#ifdef MALLOC_PROVIDED

_PTR
_DEFUN (_malloc_r, (ptr, nbytes),
	struct _reent * ptr _AND
	size_t nbytes)		/* get a block */
{
  return malloc (nbytes);
}

void
_DEFUN (_free_r, (ptr, aptr),
	struct _reent *ptr _AND
	_PTR aptr)
{
  free (aptr);
}

_PTR
_DEFUN (_realloc_r, (ptr, ap, nbytes),
	struct _reent * ptr _AND
	_PTR ap _AND
	size_t nbytes)
{
  return realloc (ap, nbytes);
}

#else

#ifdef MSTATS
#include <stdio.h>
#endif	/* MSTATS */

#ifdef WARN_VLIMIT
#include <sys/vlimit.h>		/* warn the user when near the end */
#endif

extern char etext;		/* end of the program */


/*      The overhead on a block is a pointer, rounded up to meet alignment
 *  requirements.  When free, it will contain a pointer to the next free block,
 *  and the bottom bits must be zero.
 *  When in use, the first byte will be set to 0xFF, and the next
 *  byte will be the size index.  Any other bytes are only used for
 *  alignment.  If you are range checking, and the size of the block will fit
 *  into two bytes, then the top two bytes hold the size of the requested block
 *  plus the range checking words, and the header word MINUS ONE.
 */

static _VOID
_DEFUN (_morecore_r, (ptr, nu),	/* ask system for more memory */
	struct _reent *ptr _AND
	register int nu)	/* size index to get more of  */
{
  register mptr cp;
  register int rnu;	/* 2^rnu bytes will be requested */
  register int nblks;	/* that becomes nblks blocks of the desired size */
  register int siz;	/* size in ints, not bytes */
#ifdef WARN_VLIMIT
  static int warnlevel = 0;
#endif

  if (ptr->_nextf[nu] != NULL)
    return;

  cp = (mptr) _sbrk_r (ptr, 0);

#ifdef WARN_VLIMIT
  {
    register size_t used;
    used = cp - (mptr) & etext;
    siz = vlimit (LIM_DATA, -1);	/* find out how much we can get */
    switch (warnlevel)
      {
      case 0:
	if (used > (siz / 4) * 3)
	  {
	    write (2, "warning: past 75% of memory limit\7\n", 35);
	    warnlevel = 1;
	  }
	break;
      case 1:
	if (used > (siz / 20) * 17)
	  {
	    write (2, "warning: past 85% of memory limit\7\n", 35);
	    warnlevel = 2;
	  }
	break;
      case 2:
	if (used > (siz / 20) * 19)
	  {
	    write (2, "warning: past 95% of memory limit\7\n", 35);
	    warnlevel = 3;
	  }
	break;
      }
  }				/* end of warning switch */
#endif

  /* Round up */
  if ((((size_t) cp) & ROUND_TO) != 0)
    _sbrk_r (ptr, (ROUND_TO+1) - (((size_t) cp) & ROUND_TO));

  /* take 2k unless the block is bigger than that */
  rnu = (nu <= CHUNK_POWER - 3) ? CHUNK_POWER : nu + 3;

  nblks = 1 << (rnu - (nu + 3));/* how many blocks to get */
  if (rnu < nu)
    rnu = nu;
  /* FIXME: We assume sign extension here if sizeof(size_t) < sizeof(char *) */
  if ((cp = (mptr) _sbrk_r (ptr, 1 << rnu)) == (mptr) -1)
    return;			/* no more room! */

  /* Work out the rounding so that it will work whatever the size
     of the pointer, regardless of the size of size_t. */

  if ((siz = (int) cp & 7) != 0) 
    {
      cp = (mptr) ((char *) cp - siz);
      nblks--;
    }
  
  ptr->_nextf[nu] = cp;

  /* We chop the block up into nblk free blocks,
     go through the empty block filling in the free pointer stuff */

  siz = 1 << (nu + 3);		/* +3: blocks are multiples of 8 bytes */

  while (--nblks > 0)
    {
      *(mptr *) cp = (mptr) & ((char *) cp)[siz];
      cp = *(mptr *) cp;
    }

  *(mptr *) cp = NULL;	/* NULL terminate the list */
}


_PTR
_DEFUN (_malloc_r, (ptr, nbytes),
	struct _reent * ptr _AND
	size_t nbytes)		/* get a block */
{
  register unsigned char *p;
  register int nunits = 0;
  register unsigned shiftr;

  nbytes += OVERHEAD;
  nbytes = ALIGN (nbytes);
  shiftr = (nbytes - 1) >> 2;
  
  while ((shiftr >>= 1) != 0)	/* apart from this loop, this is O(1) */
    nunits++;
  if (ptr->_nextf[nunits] == NULL)	/* needed block, nunits is the size index */
    _morecore_r (ptr, nunits);
  if ((p = (unsigned char *) (ptr->_nextf[nunits])) == NULL)
    return (NULL);
  ptr->_nextf[nunits] = *(mptr *) (ptr->_nextf[nunits]);
  p[0] = 0xff;
  p[1] = nunits;

#ifdef MSTATS
  ptr->_nmalloc[nunits]++;
#endif	/* MSTATS */

#ifdef rcheck
  if (nbytes <= 0x10000)
    ((unsigned short *) p)[1] = (unsigned short) nbytes - 1;
  *((INT32 *) (p + 4)) = MAGIC;
  *((INT32 *) (p + nbytes - 4)) = MAGIC;
  return ((char *) (p + ALIGN (8)));
#else
  return ((char *) (p + ALIGN (4)));
#endif
}


void
_DEFUN (_free_r, (ptr, aptr),
	struct _reent *ptr _AND
	_PTR aptr)
{
  register int si;
  char *ap = aptr;

  if (ap == NULL)
    return;
#ifdef rcheck
  ap -= ALIGN (8);
  ASSERT (*(INT32 *) (ap + 4) == MAGIC);
#else
  ap -= ALIGN (4);
#endif

#ifdef debug
  ASSERT (ap[0] == (char) 0xff); /* make sure it was in use */
#else
  if (ap[0] != (char) 0xff)
    return;
#endif

#ifdef rcheck
  if (ap[1] <= 13)
    {
      si = ((unsigned short *) ap)[1] - 11;	/* get the size of the data */
      ASSERT (*((INT32 *) (ap + si + 8)) == MAGIC); /* check for overflow */
    }
#endif

  ASSERT (ap[1] <= 29);
  si = ap[1];
  *((mptr *) ap) = ptr->_nextf[si];
  ptr->_nextf[si] = (mptr) ap;

#ifdef MSTATS
  ptr->_nmalloc[si]--;
#endif	/* MSTATS */
}


_PTR
_DEFUN (_realloc_r, (ptr, ap, nbytes),
	struct _reent * ptr _AND
	_PTR ap _AND
	size_t nbytes)
{
  register char *res;
  register unsigned int onb;
  register char *p;

  if (ap == NULL)
    return (_malloc_r (ptr, nbytes));
#ifdef rcheck
  p = (char*)ap - ALIGN (8);
  if (p[1] < 13)
    onb = ((unsigned short *) p)[1] - (OVERHEAD - 1);	/* old number of data bytes only */
  else
    onb = (1 << (p[1] + 3)) - OVERHEAD;
#else
  p = (char*)ap - ALIGN (4);
  onb = (1 << (p[1] + 3)) - OVERHEAD;
#endif
  if ((res = _malloc_r (ptr, nbytes)) == NULL)
    return (NULL);
  memcpy (res, ap, (nbytes < onb) ? nbytes : onb);
  _free_r (ptr, ap);
  return (res);
}

#ifdef MSTATS

/* ****************************************************************
 * mstats - print out statistics about malloc
 *
 * Prints two lines of numbers, one showing the length of the free list
 * for each size category, the second showing the number of mallocs -
 * frees for each size category.
 */

void
_DEFUN (_mstats_r, (ptr, s),
	struct _reent *ptr _AND
	char *s)
{
  register int i, j;
  register mptr p;
  int totfree = 0, totused = 0;

  fprintf (_stderr_r (ptr), "Memory allocation statistics %s\nfree:\t", s);
  for (i = 0; i < _N_LISTS; i++)
    {
      p = ptr->_nextf[i];

      for (j = 0; p; j++)
	p = *(mptr *) p;
      fprintf (_stderr_r (ptr), " %d", j);
      totfree += j * (1 << (i + 3));
    }
  fprintf (_stderr_r (ptr), "\nused:\t");
  for (i = 0; i < _N_LISTS; i++)
    {
      fprintf (_stderr_r (ptr), " %d", ptr->_nmalloc[i]);
      totused += ptr->_nmalloc[i] * (1 << (i + 3));
    }
  fprintf (_stderr_r (ptr), "\n\tTotal in use: %d, total free: %d\n");
}

#endif /* ! defined (MSTATS) */

#endif /* ! defined (MALLOC_PROVIDED) */

