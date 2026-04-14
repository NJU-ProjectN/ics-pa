/* unified sys/types.h: 
   start with sef's sysvi386 version.
   merge go32 version -- a few ifdefs.
   h8300hms, h8300xray, and sysvnecv70 disagree on the following types:

   typedef int gid_t;
   typedef int uid_t;
   typedef int dev_t;
   typedef int ino_t;
   typedef int mode_t;
   typedef int caddr_t;

   however, these aren't "reasonable" values, the sysvi386 ones make far 
   more sense, and should work sufficiently well (in particular, h8300 
   doesn't have a stat, and the necv70 doesn't matter.) -- eichin
 */

#ifndef	_SYS_TYPES_H
# define	_SYS_TYPES_H

#ifdef __i386__
#ifndef __unix__
#define __go32_types__
#endif
#endif

# include <stddef.h>	
# include <machine/types.h>

# ifndef	_POSIX_SOURCE

#  define	physadr		physadr_t
#  define	quad		quad_t

typedef	unsigned char	u_char;
typedef	unsigned short	u_short;
typedef	unsigned int	u_int;
typedef	unsigned long	u_long;
typedef	unsigned short	ushort;		/* System V compatibility */
typedef	unsigned int	uint;		/* System V compatibility */
# endif	/*!_POSIX_SOURCE */

#ifndef __time_t_defined
typedef _TIME_T_ time_t;
#define __time_t_defined
#endif

typedef	long	daddr_t;
typedef	char *	caddr_t;
#ifdef __go32_types__
typedef	unsigned long	ino_t;
#else
#ifdef __sparc__
typedef	unsigned long	ino_t;
#else
typedef	unsigned short	ino_t;
#endif
#endif
typedef	short	dev_t;
typedef	long	off_t;
typedef long  ssize_t;
typedef	unsigned short	uid_t;
typedef	unsigned short	gid_t;
typedef	long	key_t;
#ifdef __go32_types__
typedef	char *	addr_t;
typedef int mode_t;
#else
#ifdef __sparc__
typedef unsigned short mode_t;
#else
typedef unsigned mode_t;
#endif
#endif

# ifndef	_POSIX_SOURCE

#  define	NBBY	8		/* number of bits in a byte */
/*
 * Select uses bit masks of file descriptors in longs.
 * These macros manipulate such bit fields (the filesystem macros use chars).
 * FD_SETSIZE may be defined by the user, but the default here
 * should be >= NOFILE (param.h).
 */
#  ifndef	FD_SETSIZE
#	define	FD_SETSIZE	60
#  endif

typedef	long	fd_mask;
#  define	NFDBITS	(sizeof (fd_mask) * NBBY)	/* bits per mask */
#  ifndef	howmany
#	define	howmany(x,y)	(((x)+((y)-1))/(y))
#  endif

typedef	struct fd_set {
	fd_mask	fds_bits[howmany(FD_SETSIZE, NFDBITS)];
} fd_set;


#  define	FD_SET(n, p)	((p)->fds_bits[(n)/NFDBITS] |= (1L << ((n) % NFDBITS)))
#  define	FD_CLR(n, p)	((p)->fds_bits[(n)/NFDBITS] &= ~(1L << ((n) % NFDBITS)))
#  define	FD_ISSET(n, p)	((p)->fds_bits[(n)/NFDBITS] & (1L << ((n) % NFDBITS)))
#  define	FD_ZERO(p)	bzero((caddr_t)(p), sizeof (*(p)))


# endif	/* _POSIX_SOURCE */
#undef __go32_types__
#endif	/* _SYS_TYPES_H */
