/*
FUNCTION
	<<strtoul>>---string to unsigned long

INDEX
	strtoul
INDEX
	_strtoul_r

ANSI_SYNOPSIS
	#include <stdlib.h>
        unsigned long strtoul(const char *<[s]>, char **<[ptr]>,
                              int <[base]>);

        unsigned long _strtoul_r(void *<[reent]>, const char *<[s]>,
                              char **<[ptr]>, int <[base]>);

TRAD_SYNOPSIS
	#include <stdlib.h>
        unsigned long strtoul(<[s]>, <[ptr]>, <[base]>)
        char *<[s]>;
        char **<[ptr]>;
        int <[base]>;

        unsigned long _strtoul_r(<[reent]>, <[s]>, <[ptr]>, <[base]>)
	char *<[reent]>;
        char *<[s]>;
        char **<[ptr]>;
        int <[base]>;

DESCRIPTION
The function <<strtoul>> converts the string <<*<[s]>>> to
an <<unsigned long>>. First, it breaks down the string into three parts:
leading whitespace, which is ignored; a subject string consisting
of the digits meaningful in the radix specified by <[base]>
(for example, <<0>> through <<7>> if the value of <[base]> is 8);
and a trailing portion consisting of one or more unparseable characters,
which always includes the terminating null character. Then, it attempts
to convert the subject string into an unsigned long integer, and returns the
result.

If the value of <[base]> is zero, the subject string is expected to look
like a normal C integer constant (save that no optional sign is permitted):
a possible <<0x>> indicating hexadecimal radix, and a number.
If <[base]> is between 2 and 36, the expected form of the subject is a
sequence of digits (which may include letters, depending on the
base) representing an integer in the radix specified by <[base]>.
The letters <<a>>--<<z>> (or <<A>>--<<Z>>) are used as digits valued from
10 to 35. If <[base]> is 16, a leading <<0x>> is permitted.

The subject sequence is the longest initial sequence of the input
string that has the expected form, starting with the first
non-whitespace character.  If the string is empty or consists entirely
of whitespace, or if the first non-whitespace character is not a
permissible digit, the subject string is empty.

If the subject string is acceptable, and the value of <[base]> is zero,
<<strtoul>> attempts to determine the radix from the input string. A
string with a leading <<0x>> is treated as a hexadecimal value; a string with
a leading <<0>> and no <<x>> is treated as octal; all other strings are
treated as decimal. If <[base]> is between 2 and 36, it is used as the
conversion radix, as described above. Finally, a pointer to the first
character past the converted subject string is stored in <[ptr]>, if
<[ptr]> is not <<NULL>>.

If the subject string is empty (that is, if <<*>><[s]> does not start
with a substring in acceptable form), no conversion
is performed and the value of <[s]> is stored in <[ptr]> (if <[ptr]> is
not <<NULL>>).

The alternate function <<_strtoul_r>> is a reentrant version.  The
extra argument <[reent]> is a pointer to a reentrancy structure.


RETURNS
<<strtoul>> returns the converted value, if any. If no conversion was
made, <<0>> is returned.

<<strtoul>> returns <<ULONG_MAX>> if the magnitude of the converted
value is too large, and sets <<errno>> to <<ERANGE>>.

PORTABILITY
<<strtoul>> is ANSI.

<<strtoul>> requires no supporting OS subroutines.
*/

/*
 * Andy Wilson, 2-Oct-89.
 */

#include <_ansi.h>
#include <reent.h>
#include "std.h"

unsigned long
_DEFUN (_strtoul_r, (rptr, s, ptr, base),
	struct _reent *rptr _AND
	_CONST char *s _AND
	char **ptr _AND
	int base)
{
  unsigned long total = 0;
  unsigned digit;
  int radix;
  _CONST char *start = s;
  int did_conversion = 0;
  int overflow = 0;
  int minus = 0;
  unsigned long maxdiv, maxrem;

  if (s == NULL)
    {
      rptr->_errno = ERANGE;
      if (!ptr)
	*ptr = (char *) start;
      return 0L;
    }

  while (Isspace (*s))
    s++;

  if (*s == '-')
    {
      s++;
      minus = 1;
    }
  else if (*s == '+')
    s++;

  radix = base;
  if (base == 0 || base == 16)
    {
      /*
       * try to infer radix from the string (assume decimal).
       * accept leading 0x or 0X for base 16.
       */
      if (*s == '0')
	{
	  did_conversion = 1;
	  if (base == 0)
	    radix = 8;		/* guess it's octal */
	  s++;			/* (but check for hex) */
	  if (*s == 'X' || *s == 'x')
	    {
	      did_conversion = 0;
	      s++;
	      radix = 16;
	    }
	}
    }
  if (radix == 0)
    radix = 10;

  maxdiv = ULONG_MAX / radix;
  maxrem = ULONG_MAX % radix;

  while ((digit = *s) != 0)
    {
      if (digit >= '0' && digit <= '9' && digit < ('0' + radix))
	digit -= '0';
      else if (radix > 10)
	{
	  if (digit >= 'a' && digit < ('a' + radix - 10))
	    digit = digit - 'a' + 10;
	  else if (digit >= 'A' && digit < ('A' + radix - 10))
	    digit = digit - 'A' + 10;
	  else
	    break;
	}
      else
	break;
      did_conversion = 1;
      if (total > maxdiv
	  || (total == maxdiv && digit > maxrem))
	overflow = 1;
      total = (total * radix) + digit;
      s++;
    }
  if (overflow)
    {
      rptr->_errno = ERANGE;
      if (ptr != NULL)
	*ptr = (char *) s;
      return ULONG_MAX;
    }
  if (ptr != NULL)
    *ptr = (char *) ((did_conversion) ? (char *) s : start);
  return minus ? - total : total;
}

#ifndef _REENT_ONLY

unsigned long
_DEFUN (strtoul, (s, ptr, base),
	_CONST char *s _AND
	char **ptr _AND
	int base)
{
  return _strtoul_r (_REENT, s, ptr, base);
}

#endif
