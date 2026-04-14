/*
FUNCTION
<<signal>>---specify handler subroutine for a signal

INDEX
	signal
INDEX
	_signal_r
INDEX
	raise
INDEX
	_raise_r

ANSI_SYNOPSIS
	#include <signal.h>
	void ( * signal(int <[sig]>, void(*<[func]>)(int)) )(int);

	void ( * _signal_r(void *<[reent]>, 
                           int <[sig]>, void(*<[func]>)(int)) )(int);

	int raise (int <[sig]>);

	int _raise_r (void *<[reent]>, int <[sig]>);

TRAD_SYNOPSIS
	#include <signal.h>
	char ( * signal(<[sig]>, <[func]>) )()
	int <[sig]>;
	char ( * <[func]> )();

	char ( * _signal_r(<[reent]>, <[sig]>, <[func]>) )()
	char *<[reent]>;
	int <[sig]>;
	char ( * <[func]> )();

	int raise (<[sig]>)()
	int <[sig]>;

	int _raise_r (<[reent]>, <[sig]>)()
	char *<[reent]>;
	int <[sig]>;

DESCRIPTION
<<signal, raise>> provide a simple signal/raise implementation for embedded
targets.

<<signal>> allows you to request changed treatment for a particular
signal <[sig]>.  You can use one of the predefined macros <<SIG_DFL>>
(select system default handling) or <<SIG_IGN>> (ignore this signal)
as the value of <[func]>; otherwise, <[func]> is a function pointer
that identifies a subroutine in your program as the handler for this signal.

Some of the execution environment for signal handlers is
unpredictable; notably, the only library function required to work
correctly from within a signal handler is @code{signal} itself, and
only when used to redefine the handler for the current signal value.

Static storage is likewise unreliable for signal handlers, with one
exception: if you declare a static storage location as `<<volatile
sig_atomic_t>>', then you may use that location in a signal handler to
store signal values.

If your signal handler terminates using <<return>> (or implicit
return), your program's execution continues at the point
where it was when the signal was raised (whether by your program
itself, or by an external event).  Signal handlers can also
use functions such as <<exit>> and <<abort>> to avoid returning.

<<raise>> sends the signal sig to the executing program.  It returns zero if
successful, non-zero if unsuccessful.

The alternate functions <<_signal_r, _raise_r>> are the reentrant versions.
The extra argument <[reent]> is a pointer to a reentrancy structure.


@c FIXME: do we have setjmp.h and assoc fns?

RETURNS
If your request for a signal handler cannot be honored, the result is
<<SIG_ERR>>; a specific error number is also recorded in <<errno>>.

Otherwise, the result is the previous handler (a function pointer or
one of the predefined macros).

PORTABILITY
ANSI C requires <<raise>>, <<signal>>.

No supporting OS subroutines are required to link with <<signal>>, but
it will not have any useful effects, except for software generated signals,
without an operating system that can actually raise exceptions.
*/

/*
 * signal.c
 * Original Author:	G. Haley
 *
 * signal associates the function pointed to by func with the signal sig. When
 * a signal occurs, the value of func determines the action taken as follows:
 * if func is SIG_DFL, the default handling for that signal will occur; if func
 * is SIG_IGN, the signal will be ignored; otherwise, the default handling for
 * the signal is restored (SIG_DFL), and the function func is called with sig
 * as its argument. Returns the value of func for the previous call to signal
 * for the signal sig, or SIG_ERR if the request fails.
 */

/* _init_signal initialises the signal handlers for each signal. This function
   is called by crt0 at program startup.  */

#ifndef SIMULATED_SIGNALS

int _dummy_simulated_signal;

#else

#include <errno.h>
#include <signal.h>
#include <stddef.h>
#include <stdlib.h>
#include <reent.h>
#include <_syslist.h>

_sig_func_ptr
_DEFUN (_signal_r, (ptr, sig, func),
	struct _reent *ptr _AND
	int sig _AND
	_sig_func_ptr func)
{
  _sig_func_ptr old_func, *temp;

  switch (sig)
    {
    case SIGABRT:
    case SIGFPE:
    case SIGILL:
    case SIGINT:
    case SIGSEGV:
    case SIGTERM:

      if (ptr->sig_func == NULL)
	{
	   ptr->sig_func = _malloc_r (ptr, sizeof (void (*(_sig_func[_MAX_SIGNALS]))()));
	   if (ptr->sig_func == NULL)
	     {
	       return SIG_ERR;
	     }
	}

      old_func = ptr->sig_func[sig];
      ptr->sig_func[sig] = func;
      break;
    default:
      old_func = SIG_ERR;
      ptr->_errno = EINVAL;
      break;
    }

  return old_func;
}

int 
_raise_r (ptr, sig)
     struct _reent *ptr;
     int sig;
{
  _sig_func_ptr func;
  int result = 0;

  /* No signals to raise! */
  if (ptr->sig_func == NULL) return 1;
  
  switch (sig)
    {
    case SIGABRT:
    case SIGFPE:
    case SIGILL:
    case SIGINT:
    case SIGSEGV:
    case SIGTERM:
      switch ((int) ptr->sig_func[sig])
	{
	case SIG_DFL:
	case SIG_IGN:
	  break;
	case SIG_ERR:
	  result = 1;
	  break;
	default:
	  func = ptr->sig_func[sig];
	  ptr->sig_func[sig] = SIG_DFL;
	  func (sig);
	  break;
	}
      break;
    default:
      result = 1;
      break;
    }

  return result;
}

#ifndef _REENT_ONLY

int 
raise (sig)
     int sig;
{
  return _raise_r (_REENT, sig);
}

_sig_func_ptr
_DEFUN (signal, (sig, func),
	int sig _AND
	_sig_func_ptr func)
{
  return _signal_r (_REENT, sig, func);
}

void 
_init_signal ()
{
  _signal_r (_REENT, SIGABRT, SIG_DFL);
  _signal_r (_REENT, SIGFPE, SIG_DFL);
  _signal_r (_REENT, SIGILL, SIG_DFL);
  _signal_r (_REENT, SIGINT, SIG_DFL);
  _signal_r (_REENT, SIGSEGV, SIG_DFL);
  _signal_r (_REENT, SIGTERM, SIG_DFL);
}

#endif

#endif /* SIMULATED_SIGNALS */
