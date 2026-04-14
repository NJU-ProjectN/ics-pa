/*
FUNCTION
<<vprintf>>, <<vfprintf>>, <<vsprintf>>---format argument list

INDEX
	vprintf
INDEX
	vfprintf
INDEX
	vsprintf

ANSI_SYNOPSIS
	#include <stdio.h>
	#include <stdarg.h>
	int vprintf(const char *<[fmt]>, va_list <[list]>);
	int vfprintf(FILE *<[fp]>, const char *<[fmt]>, va_list <[list]>);
	int vsprintf(char *<[str]>, const char *<[fmt]>, va_list <[list]>);

	int _vprintf_r(void *<[reent]>, const char *<[fmt]>,
                        va_list <[list]>);
	int _vfprintf_r(void *<[reent]>, FILE *<[fp]>, const char *<[fmt]>,
                        va_list <[list]>);
	int _vsprintf_r(void *<[reent]>, char *<[str]>, const char *<[fmt]>,
                        va_list <[list]>);

TRAD_SYNOPSIS
	#include <stdio.h>
	#include <varargs.h>
	int vprintf( <[fmt]>, <[list]>)
	char *<[fmt]>;
	va_list <[list]>;

	int vfprintf(<[fp]>, <[fmt]>, <[list]>)
	FILE *<[fp]>;
	char *<[fmt]>;
	va_list <[list]>;

	int vsprintf(<[str]>, <[fmt]>, <[list]>)
	char *<[str]>;
	char *<[fmt]>;
	va_list <[list]>;

	int _vprintf_r(<[reent]>, <[fmt]>, <[list]>)
	char *<[reent]>;
	char *<[fmt]>;
	va_list <[list]>;

	int _vfprintf_r(<[reent]>, <[fp]>, <[fmt]>, <[list]>)
	char *<[reent]>;
	FILE *<[fp]>;
	char *<[fmt]>;
	va_list <[list]>;

	int _vsprintf_r(<[reent]>, <[str]>, <[fmt]>, <[list]>)
	char *<[reent]>;
	char *<[str]>;
	char *<[fmt]>;
	va_list <[list]>;

DESCRIPTION
<<vprintf>>, <<vfprintf>>, and <<vsprintf>> are (respectively)
variants of <<printf>>, <<fprintf>>, and <<sprintf>>.  They differ
only in allowing their caller to pass the variable argument list as a
<<va_list>> object (initialized by <<va_start>>) rather than directly
accepting a variable number of arguments.

RETURNS
The return values are consistent with the corresponding functions:
<<vsprintf>> returns the number of bytes in the output string,
save that the concluding <<NULL>> is not counted.
<<vprintf>> and <<vfprintf>> return the number of characters transmitted.
If an error occurs, <<vprintf>> and <<vfprintf>> return <<EOF>>. No
error returns occur for <<vsprintf>>.

PORTABILITY
ANSI C requires all three functions.

Supporting OS subroutines required: <<close>>, <<fstat>>, <<isatty>>,
<<lseek>>, <<read>>, <<sbrk>>, <<write>>.
*/

#define	to_char(n)	((n) + '0')
#define	to_digit(c)	((c) - '0')

/*
 * doprint.c
 * Original Author:	G. Haley
 */

#ifdef INTEGER_ONLY
#define VFPRINTF vfiprintf
#define _VFPRINTF_R _vfiprintf_r
#else
#define VFPRINTF vfprintf
#define _VFPRINTF_R _vfprintf_r
#endif

#include <_ansi.h>
#include <stdio.h>
#include <string.h>
#include <reent.h>

#ifdef _HAVE_STDC
#include <stdarg.h>
#else
#include <varargs.h>
#endif

#include <stddef.h>
#include "local.h"

#define UNSET -1

#define FPERCENT 0x01
#define FMINUS   0x02
#define FPLUS    0x04
#define FSPACE   0x08
#define FHASH    0x10
#define FDOT     0x20
#define FZERO    0x40
#define FSHORT   0x80
#define FLONG    0x100
#define FDLONG   0x200

static int 
out_string (s, n_chars, flags, n_zero, n_pad, prefix, stream)
     char *s;
     int n_chars, flags, n_zero, n_pad;
     char *prefix;
     FILE *stream;
{
  int i;

  if (n_pad && !(flags & FMINUS))
    {
      if (prefix && (flags & FZERO))
	if (fputs (prefix, stream) == -1)
	  return -1;

      for (i = 0; i < n_pad; i++)
	{
	  if (flags & FZERO)
	    {
	      if (fputc ('0', stream) == EOF)
		return -1;
	    }
	  else if (fputc (' ', stream) == EOF)
	    return -1;
	}

      if (prefix && !(flags & FZERO))
	if (fputs (prefix, stream) != 0)
	  return -1;
    }
  else if (prefix)
    if (fputs (prefix, stream) != 0)
      return -1;

  for (i = 0; i < n_zero; i++)
    if (fputc ('0', stream) == EOF)
      return -1;
  for (i = 0; i < n_chars; i++)
    if (fputc (*s++, stream) == EOF)
      return -1;

  if (n_pad && (flags & FMINUS))
    {
      for (i = 0; i < n_pad; i++)
	if (fputc (' ', stream) == EOF)
	  return -1;
    }

  return (n_chars + n_zero + n_pad + (prefix ? strlen (prefix) : 0));
}

int 
_DEFUN (VFPRINTF, (stream, format, args),
	FILE * stream _AND
	_CONST char *format _AND
	va_list args)
{
  return _VFPRINTF_R (stream->_data, stream, format, args);
}

int 
_DEFUN (_VFPRINTF_R, (data, stream, format, args),
	struct _reent *data _AND
	FILE * stream _AND
	_CONST char *format _AND
	va_list args)
{
  char buffer[CVT_BUF_SIZE];
  int count = 0;
  int flags = 0;
  int field_width = 0;
  int precision = UNSET;
  int *ip;
  long int i;
  char *s;
  char c;
  int sign;
#ifdef __GNUC__
  long long int ll;
#endif

  char f_type, *prefix, *fld;
  int n_chars, n_zero, n_pad;

  CHECK_INIT (stream);
  data = stream->_data;		/* may have changed */

  for (;;)
    {
      if (!flags)
	{
	  while (*format && *format != '%')
	    {
	      if (fputc (*format++, stream) == EOF)
		return -1;
	      count++;
	    }

	  if (*format)
	    {
	      flags |= FPERCENT;
	      format++;
	    }
	  else
	    return count;
	}

      switch (*format++)
	{
	case '-':
	  flags |= FMINUS;
	  break;
	case '+':
	  flags |= FPLUS;
	  break;
	case ' ':
	  flags |= FSPACE;
	  break;
	case '#':
	  flags |= FHASH;
	  break;

	case '.':
	  flags |= FDOT;
	  precision = 0;
	  break;
	case '0':
	  if (field_width == 0 && !(flags & FDOT))
	    {
	      flags |= FZERO;
	      break;
	    }
	  /* fall through */
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
	  if (flags & FDOT)
	    precision = precision * 10 +
	      *(format - 1) - '0';
	  else
	    field_width = field_width * 10 +
	      *(format - 1) - '0';
	  break;
	case '*':
	  if (flags & FDOT)
	    precision = va_arg (args, int);
	  else
	    {
	      if ((field_width = va_arg (args, int)) < 0)
		{
		  flags |= FMINUS;
		  field_width = -field_width;
		}
	    }
	  break;

	case 'h':
	  flags |= FSHORT;
	  break;
	case 'l':
	  flags |= FLONG;
	  break;
	case 'L':
	  flags |= FDLONG;
	  break;

	case 'd':
	case 'i':
	case 'u':
	case 'o':
	case 'x':
	case 'X':
	  /* Newlib must be compilable by compilers other than gcc, and
	     making `i' long long would simplify a lot of the following,
	     but it would unnecessarily slow things down.  So instead we
	     only use long long values when we absolutely have to.  Some
	     targets have options to set the sizes of the various ints, but
	     that won't bother us.
	     `sign' records the signed-ness of the value so we don't have
	     any more conditional compilation than we need.  */
#ifdef __GNUC__
	  if (flags & FDLONG)
	    {
	      ll = va_arg (args, long long);
	      sign = (ll > 0) - (ll < 0);
	    }
	  else
#endif
	    {
	      if (flags & (FLONG + FDLONG))
		i = va_arg (args, long);
	      else
		i = va_arg (args, int);
	      sign = (i > 0) - (i < 0);
	    }

	  /* If precision is specified, 0 flag is ignored */
	  if (precision != UNSET)
	    flags &= ~FZERO;

	  if (precision == UNSET)
	    precision = 1;

	  f_type = *(format - 1);

	  if (precision || sign)
	    {
	      if (flags & FSHORT)
		fld = _sicvt (buffer, (short) i, f_type);
#ifdef __GNUC__
	      else if (flags & FDLONG)
		fld = _llicvt (buffer, (long long) ll, f_type);
#endif
#if LONG_MAX != INT_MAX
	      else if (flags & (FLONG + FDLONG))
		fld = _licvt (buffer, (long) i, f_type);
#endif
	      else
		fld = _icvt (buffer, (int) i, f_type);
	    }
	  else
	    {
	      /* zero precision && zero value */

	      /* reset flags, ... etc */
	      flags = 0;
	      field_width = 0;
	      precision = UNSET;

	      break;
	    }

	  n_chars = (fld ? strlen (fld) : 0);

	  n_zero = 0;
	  if (precision > n_chars)
	    n_zero = precision - n_chars;

	  n_pad = 0;
	  if (field_width > n_chars + n_zero)
	    n_pad = field_width - n_chars - n_zero;

	  if (f_type == 'd' || f_type == 'i')
	    {
	      if (sign < 0)
		prefix = "-";
	      else if (flags & FPLUS)
		prefix = "+";
	      else if (flags & FSPACE)
		prefix = " ";
	      else
		prefix = NULL;
	    }
	  else if (flags & FHASH)
	    {
	      if (f_type == 'o')
		prefix = "0";
	      else if (f_type == 'x')
		prefix = "0x";
	      else if (f_type == 'X')
		prefix = "0X";
	      else
		prefix = NULL;
	    }
	  else
	    prefix = NULL;

	  if (n_pad && prefix)
	    n_pad -= strlen (prefix);

	  if ((n_chars = out_string (fld, n_chars, flags,
				     n_zero, n_pad, prefix, stream)) == -1)
	    return -1;
	  else
	    count += n_chars;

	  /* reset flags, ... etc */
	  flags = 0;
	  field_width = 0;
	  precision = UNSET;
	  break;

#ifndef INTEGER_ONLY
	case 'F':
	case 'f':
	case 'e':
	case 'E':
	case 'g':
	case 'G':
	  {

	    double d;

	    d = va_arg (args, double);

	    if (precision == UNSET || precision < 0)
	      precision = 6;

	    f_type = *(format - 1);

	    fld = _dcvt (data, buffer, d, precision, field_width,
			 f_type, flags & FHASH);

	    n_chars = (fld ? strlen (fld) : 0);

	    n_pad = 0;
	    if (field_width > n_chars)
	      n_pad = field_width - n_chars;

	    if (d < 0)
	      prefix = "-";
	    else if (flags & FPLUS)
	      prefix = "+";
	    else if (flags & FSPACE)
	      prefix = " ";
	    else
	      prefix = NULL;

	    if (n_pad && prefix)
	      n_pad -= strlen (prefix);

	    if ((n_chars = out_string (fld, n_chars, flags, 0,
				       n_pad, prefix, stream)) == -1)
	      return -1;
	    else
	      count += n_chars;

	    /* reset flags, ... etc */
	    flags = 0;
	    field_width = 0;
	    precision = UNSET;
	  }

	  break;
#endif

	case 's':
	  s = va_arg (args, char *);

	  if (s == NULL)
	    s = "{null}";

	  n_chars = strlen (s);
	  if (precision != UNSET && precision < n_chars)
	    n_chars = precision;

	  n_pad = 0;
	  if (field_width > n_chars)
	    n_pad = field_width - n_chars;

	  if ((n_chars = out_string (s, n_chars, flags, 0, n_pad,
				     "", stream)) == -1)
	    return -1;
	  else
	    count += n_chars;

	  /* reset flags, ... etc */
	  flags = 0;
	  field_width = 0;
	  precision = UNSET;
	  break;

	case 'c':
	  c = va_arg (args, int);

	  n_pad = 0;
	  if (field_width > 1)
	    n_pad = field_width - 1;

	  if ((n_chars = out_string (&c, 1, flags, 0, n_pad, "",
				     stream)) == -1)
	    return -1;
	  else
	    count += n_chars;

	  /* reset flags, ... etc */
	  flags = 0;
	  field_width = 0;
	  precision = UNSET;
	  break;

	case 'p':
	  ip = va_arg (args, int *);

#ifdef __GNUC__
	  /* This is the only case where we need long longs.
	     Handle all other cases without them.  */
	  if (sizeof (ip) > sizeof (long))
	    fld = _llicvt (buffer, (long long int) ip, 'x');
	  else
#else
#if LONG_MAX != INT_MAX
	  if (sizeof (ip) > sizeof (int))
	    fld = _licvt (buffer, (long int) ip, 'x');
	  else
#endif
#endif
	  fld = _icvt (buffer, (int) ip, 'x');

	  precision = 2 * sizeof (ip);
	  n_chars = (fld ? strlen (fld) : 0);

	  flags = FZERO;

	  n_zero = 0;
	  if (precision > n_chars)
	    n_zero = precision - n_chars;

	  prefix = "0x";

	  if ((n_chars = out_string (fld, n_chars, flags,
				     n_zero, 0, prefix, stream)) == -1)
	    return -1;
	  else
	    count += n_chars;

	  /* reset flags, ... etc */
	  flags = 0;
	  field_width = 0;
	  precision = UNSET;
	  prefix = NULL;
	  break;

	case 'n':
	  if (flags & FSHORT)
	    *(va_arg (args, short *)) = count;
	  else
	    *(va_arg (args, int *)) = count;

	  /* reset flags, ... etc */
	  flags = 0;
	  field_width = 0;
	  precision = UNSET;
	  break;

	case '%':
	  n_pad = 0;
	  if (field_width > 1)
	    n_pad = field_width - 1;

	  if ((n_chars = out_string ("%", 1, flags, 0, n_pad, "",
				     stream)) == -1)
	    return -1;
	  else
	    count += n_chars;

	  /* reset flags, ... etc */
	  flags = 0;
	  field_width = 0;
	  precision = UNSET;
	  break;

	case '\0':
	  /* reset flags, ... etc */
	  flags = 0;
	  field_width = 0;
	  precision = UNSET;
	  break;
	}
    }
}
