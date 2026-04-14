/***
**** CAUTION!!! KEEP DOC CONSISTENT---if you change text of a message
****            here, change two places:
****            1) the leading doc section (alphabetized by macro)
****            2) the real text inside switch(errnum)
***/

/*
FUNCTION
	<<strerror>>---convert error number to string

INDEX
	strerror

ANSI_SYNOPSIS
	#include <string.h>
	char *strerror(int <[errnum]>);

TRAD_SYNOPSIS
	#include <string.h>
	char *strerror(<[errnum]>)
	int <[errnum]>;

DESCRIPTION
<<strerror>> converts the error number <[errnum]> into a
string.  The value of <[errnum]> is usually a copy of <<errno>>.
If <<errnum>> is not a known error number, the result points to an
empty string.

This implementation of <<strerror>> prints out the following strings
for each of the values defined in `<<errno.h>>':

o+
o E2BIG
Arg list too long

o EACCES
Permission denied

o EADV
Advertise error

o EAGAIN
No more processes

o EBADF
Bad file number

o EBADMSG
Bad message

o EBUSY
Device or resource busy

o ECHILD
No children

o ECOMM
Communication error

o EDEADLK
Deadlock

o EEXIST
File exists

o EDOM
Math argument

o EFAULT
Bad address

o EFBIG
File too large

o EIDRM
Identifier removed

o EINTR
Interrupted system call

o EINVAL
Invalid argument

o EIO
I/O error

o EISDIR
Is a directory

o ELIBACC
Cannot access a needed shared library

o ELIBBAD
Accessing a corrupted shared library

o ELIBEXEC
Cannot exec a shared library directly

o ELIBMAX
Attempting to link in more shared libraries than system limit

o ELIBSCN
<<.lib>> section in a.out corrupted

o EMFILE
Too many open files

o EMLINK
Too many links

o EMULTIHOP
Multihop attempted

o ENFILE
File table overflow

o ENODEV
No such device

o ENOENT
No such file or directory

o ENOEXEC
Exec format error

o ENOLCK
No lock

o ENOLINK
Virtual circuit is gone

o ENOMEM
Not enough space

o ENOMSG
No message of desired type

o ENONET
Machine is not on the network

o ENOPKG
No package

o ENOSPC
No space left on device

o ENOSR
No stream resources

o ENOSTR
Not a stream

o ENOTBLK
Block device required

o ENOTDIR
Not a directory

o ENOTTY
Not a character device

o ENXIO
No such device or address

o EPERM
Not owner

o EPIPE
Broken pipe

o EPROTO
Protocol error

o ERANGE
Result too large

o EREMOTE
Resource is remote

o EROFS
Read-only file system

o ESPIPE
Illegal seek

o ESRCH
No such process

o ESRMNT
Srmount error

o ETIME
Stream ioctl timeout

o ETXTBSY
Text file busy

o EXDEV
Cross-device link

o-

RETURNS
This function returns a pointer to a string.  Your application must
not modify that string.

PORTABILITY
ANSI C requires <<strerror>>, but does not specify the strings used
for each error number.

Although this implementation of <<strerror>> is reentrant, ANSI C
declares that subsequent calls to <<strerror>> may overwrite the
result string; therefore portable code cannot depend on the reentrancy
of this subroutine.

<<strerror>> requires no supporting OS subroutines.

QUICKREF
	strerror ansi pure
*/

#include <errno.h>
#include <string.h>

char *
_DEFUN (strerror, (errnum),
	int errnum)
{
  char *error;

  switch (errnum)
    {
    case EPERM:
      error = "Not owner";
      break;
    case ENOENT:
      error = "No such file or directory";
      break;
    case ESRCH:
      error = "No such process";
      break;
    case EINTR:
      error = "Interrupted system call";
      break;
    case EIO:
      error = "I/O error";
      break;
    case ENXIO:
      error = "No such device or address";
      break;
    case E2BIG:
      error = "Arg list too long";
      break;
    case ENOEXEC:
      error = "Exec format error";
      break;
    case EBADF:
      error = "Bad file number";
      break;
    case ECHILD:
      error = "No children";
      break;
    case EAGAIN:
      error = "No more processes";
      break;
    case ENOMEM:
      error = "Not enough space";
      break;
    case EACCES:
      error = "Permission denied";
      break;
    case EFAULT:
      error = "Bad address";
      break;
    case ENOTBLK:
      error = "Block device required";
      break;
    case EBUSY:
      error = "Device or resource busy";
      break;
    case EEXIST:
      error = "File exists";
      break;
    case EXDEV:
      error = "Cross-device link";
      break;
    case ENODEV:
      error = "No such device";
      break;
    case ENOTDIR:
      error = "Not a directory";
      break;
    case EISDIR:
      error = "Is a directory";
      break;
    case EINVAL:
      error = "Invalid argument";
      break;
    case ENFILE:
      error = "File table overflow";
      break;
    case EMFILE:
      error = "Too many open files";
      break;
    case ENOTTY:
      error = "Not a character device";
      break;
    case ETXTBSY:
      error = "Text file busy";
      break;
    case EFBIG:
      error = "File too large";
      break;
    case ENOSPC:
      error = "No space left on device";
      break;
    case ESPIPE:
      error = "Illegal seek";
      break;
    case EROFS:
      error = "Read-only file system";
      break;
    case EMLINK:
      error = "Too many links";
      break;
    case EPIPE:
      error = "Broken pipe";
      break;
    case EDOM:
      error = "Math argument";
      break;
    case ERANGE:
      error = "Result too large";
      break;
    case ENOMSG:
      error = "No message of desired type";
      break;
    case EIDRM:
      error = "Identifier removed";
      break;
    case EDEADLK:
      error = "Deadlock";
      break;
    case ENOLCK:
      error = "No lock";
      break;
    case ENOSTR:
      error = "Not a stream";
      break;
    case ETIME:
      error = "Stream ioctl timeout";
      break;
    case ENOSR:
      error = "No stream resources";
      break;
    case ENONET:
      error = "Machine is not on the network";
      break;
    case ENOPKG:
      error = "No package";
      break;
    case EREMOTE:
      error = "Resource is remote";
      break;
    case ENOLINK:
      error = "Virtual circuit is gone";
      break;
    case EADV:
      error = "Advertise error";
      break;
    case ESRMNT:
      error = "Srmount error";
      break;
    case ECOMM:
      error = "Communication error";
      break;
    case EPROTO:
      error = "Protocol error";
      break;
    case EMULTIHOP:
      error = "Multihop attempted";
      break;
    case EBADMSG:
      error = "Bad message";
      break;
    case ELIBACC:
      error = "Cannot access a needed shared library";
      break;
    case ELIBBAD:
      error = "Accessing a corrupted shared library";
      break;
    case ELIBSCN:
      error = ".lib section in a.out corrupted";
      break;
    case ELIBMAX:
      error = "Attempting to link in more shared libraries than system limit";
      break;
    case ELIBEXEC:
      error = "Cannot exec a shared library directly";
      break;
    default:
      error = "";
      break;
    }

  return error;
}
