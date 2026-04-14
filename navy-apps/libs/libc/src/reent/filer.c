/* Reentrant versions of file handling system calls.  These
   implementations just call the usual system calls.  */

#include <reent.h>
#include <unistd.h>
#include <_syslist.h>

/* Some targets provides their own versions of these functions.  Those
   targets should define REENTRANT_SYSCALLS_PROVIDED in TARGET_CFLAGS.  */

#ifdef _REENT_ONLY
#ifndef REENTRANT_SYSCALLS_PROVIDED
#define REENTRANT_SYSCALLS_PROVIDED
#endif
#endif

#ifdef REENTRANT_SYSCALLS_PROVIDED

int _dummy_file_syscalls = 1;

#else

/* We use the errno variable used by the system dependent layer.  */
#undef errno
int errno;

/*
FUNCTION
	<<_open_r>>---Reentrant version of open
	
INDEX
	_open_r

ANSI_SYNOPSIS
	#include <reent.h>
	int _open_r(struct _reent *<[ptr]>,
		    const char *<[file]>, int <[flags]>, int <[mode]>);

TRAD_SYNOPSIS
	#include <reent.h>
	int _open_r(<[ptr]>, <[file]>, <[flags]>, <[mode]>)
	struct _reent *<[ptr]>;
	char *<[file]>;
	int <[flags]>;
	int <[mode]>;

DESCRIPTION
	This is a reentrant version of <<open>>.  It
	takes a pointer to the global data block, which holds
	<<errno>>.
*/

int
_open_r (ptr, file, flags, mode)
     struct _reent *ptr;
     _CONST char *file;
     int flags;
     int mode;
{
  int ret;

  errno = 0;
  ret = _open (file, flags, mode);
  if (errno != 0)
    ptr->_errno = errno;
  return ret;
}

/*
FUNCTION
	<<_close_r>>---Reentrant version of close
	
INDEX
	_close_r

ANSI_SYNOPSIS
	#include <reent.h>
	int _close_r(struct _reent *<[ptr]>, int <[fd]>);

TRAD_SYNOPSIS
	#include <reent.h>
	int _close_r(<[ptr]>, <[fd]>)
	struct _reent *<[ptr]>;
	int <[fd]>;

DESCRIPTION
	This is a reentrant version of <<close>>.  It
	takes a pointer to the global data block, which holds
	<<errno>>.
*/

int
_close_r (ptr, fd)
     struct _reent *ptr;
     int fd;
{
  int ret;

  errno = 0;
  ret = _close (fd);
  if (errno != 0)
    ptr->_errno = errno;
  return ret;
}

/*
FUNCTION
	<<_lseek_r>>---Reentrant version of lseek
	
INDEX
	_lseek_r

ANSI_SYNOPSIS
	#include <reent.h>
	off_t _lseek_r(struct _reent *<[ptr]>,
		       int <[fd]>, off_t <[pos]>, int <[whence]>);

TRAD_SYNOPSIS
	#include <reent.h>
	off_t _lseek_r(<[ptr]>, <[fd]>, <[pos]>, <[whence]>)
	struct _reent *<[ptr]>;
	int <[fd]>;
	off_t <[pos]>;
	int <[whence]>;

DESCRIPTION
	This is a reentrant version of <<lseek>>.  It
	takes a pointer to the global data block, which holds
	<<errno>>.
*/

off_t
_lseek_r (ptr, fd, pos, whence)
     struct _reent *ptr;
     int fd;
     off_t pos;
     int whence;
{
  off_t ret;

  errno = 0;
  ret = _lseek (fd, pos, whence);
  if (errno != 0)
    ptr->_errno = errno;
  return ret;
}

/*
FUNCTION
	<<_read_r>>---Reentrant version of read
	
INDEX
	_read_r

ANSI_SYNOPSIS
	#include <reent.h>
	long _read_r(struct _reent *<[ptr]>,
		     int <[fd]>, void *<[buf]>, size_t <[cnt]>);

TRAD_SYNOPSIS
	#include <reent.h>
	long _read_r(<[ptr]>, <[fd]>, <[buf]>, <[cnt]>)
	struct _reent *<[ptr]>;
	int <[fd]>;
	char *<[buf]>;
	size_t <[cnt]>;

DESCRIPTION
	This is a reentrant version of <<read>>.  It
	takes a pointer to the global data block, which holds
	<<errno>>.
*/

long
_read_r (ptr, fd, buf, cnt)
     struct _reent *ptr;
     int fd;
     _PTR buf;
     size_t cnt;
{
  long ret;

  errno = 0;
  ret = _read (fd, buf, cnt);
  if (errno != 0)
    ptr->_errno = errno;
  return ret;
}

/*
FUNCTION
	<<_write_r>>---Reentrant version of write
	
INDEX
	_write_r

ANSI_SYNOPSIS
	#include <reent.h>
	long _write_r(struct _reent *<[ptr]>,
		      int <[fd]>, const void *<[buf]>, size_t <[cnt]>);

TRAD_SYNOPSIS
	#include <reent.h>
	long _write_r(<[ptr]>, <[fd]>, <[buf]>, <[cnt]>)
	struct _reent *<[ptr]>;
	int <[fd]>;
	char *<[buf]>;
	size_t <[cnt]>;

DESCRIPTION
	This is a reentrant version of <<write>>.  It
	takes a pointer to the global data block, which holds
	<<errno>>.
*/

long
_write_r (ptr, fd, buf, cnt)
     struct _reent *ptr;
     int fd;
     _CONST _PTR buf;
     size_t cnt;
{
  long ret;

  errno = 0;
  ret = _write (fd, buf, cnt);
  if (errno != 0)
    ptr->_errno = errno;
  return ret;
}

#endif /* ! defined (REENTRANT_SYSCALLS_PROVIDED) */
