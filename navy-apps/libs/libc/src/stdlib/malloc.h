#ifndef _MALLOC_H_
#define _MALLOC_H_

typedef _PTR mptr;		/* a generic pointer, eg: (void *) */

/* #define debug */
/* #define rcheck */
#define MSTATS

/*
 * INT32 may be ugly, but we need to specify exactly this, and INT32 gets
 * the point across real quick.
 * FIXME: More work may be needed in this area.
 */

#ifndef INT32
#define INT32	int
#endif

#ifdef debug
#include <assert.h>
#define ASSERT(p)	assert(p)
#else
#define ASSERT(p)
#endif

#ifndef NULL
#define NULL 0
#endif

#ifdef rcheck
#define MAGIC 0x55555555
#endif

#ifndef MALLOC_ALIGNMENT
#define MALLOC_ALIGNMENT	4
#endif
#define ALIGN(x)	(((x) + MALLOC_ALIGNMENT - 1) & ~(MALLOC_ALIGNMENT - 1))

#ifdef rcheck
#define OVERHEAD	ALIGN(12)	/* make sure the range checkers will fit */
#else
#define OVERHEAD	ALIGN(4)
#endif

#endif /* _MALLOC_H_ */
