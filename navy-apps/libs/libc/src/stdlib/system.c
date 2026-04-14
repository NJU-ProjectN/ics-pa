/*
FUNCTION
<<system>>---execute command string

INDEX
	system
INDEX
	_system_r

ANSI_SYNOPSIS
	#include <stdlib.h>
	int system(char *<[s]>);

	int _system_r(void *<[reent]>, char *<[s]>);

TRAD_SYNOPSIS
	#include <stdlib.h>
	int system(<[s]>)
	char *<[s]>;

	int _system_r(<[reent]>, <[s]>)
	char *<[reent]>;
	char *<[s]>;

DESCRIPTION

Use <<system>> to pass a command string <<*<[s]>>> to <</bin/sh>> on
your system, and wait for it to finish executing.

Use `<<system(NULL)>>' to test whether your system has <</bin/sh>>
available.

The alternate function <<_system_r>> is a reentrant version.  The
extra argument <[reent]> is a pointer to a reentrancy structure.

RETURNS
<<system(NULL)>> returns a non-zero value if <</bin/sh>> is available, and
<<0>> if it is not.

With a command argument, the result of <<system>> is the exit status
returned by <</bin/sh>>.

PORTABILITY
ANSI C requires <<system>>, but leaves the nature and effects of a
command processor undefined.  ANSI C does, however, specify that
<<system(NULL)>> return zero or nonzero to report on the existence of
a command processor.

POSIX.2 requires <<system>>, and requires that it invoke <</bin/sh>>.

Supporting OS subroutines required: <<_exit>>, <<execve>>, <<fork>>,
<<wait>>.
*/

#include <stddef.h>
#include <stdlib.h>
#include <_syslist.h>

#ifndef NO_EXEC
extern int execve ();
extern int fork ();
extern int wait ();

extern char *environ[];
#endif

int
_system_r (ptr, s)
     struct _reent *ptr;
     _CONST char *s;
{
  char *argv[4];
  int pid, status;

#ifdef NO_EXEC
  return 0;
#else
  argv[0] = "sh";
  argv[1] = "-c";
  if (s == NULL)
    argv[2] = (char *) "exit 0";
  else
    argv[2] = (char *) s;
  argv[3] = NULL;

  if ((pid = _fork_r (ptr)) == 0)
    {
      _execve ("/bin/sh", argv, environ);
      exit (100);
    }
  else if (pid == -1)
    return s == NULL ? 0 : -1;
  else
    {
      _wait_r (ptr, &status);
      status = (status >> 8) & 0xff;
      return s == NULL ? status == 0 : status;
    }
#endif
}

#ifndef _REENT_ONLY

int
system (s)
     _CONST char *s;
{
  return _system_r (_REENT, s);
}

#endif
