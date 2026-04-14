/* no user doc; fns defined here appear to be internal to library.
   R. Pesch 15apr92
*/

#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <float.h>
#include <ieeefp.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "local.h"
#if defined(__v800) && defined(CA732_BUG)
static _CONST char * lcset = "0123456789abcdef";
static _CONST char * ucset = "0123456789ABCDEF";
#else
static _CONST char *_CONST lcset = "0123456789abcdef";
static _CONST char *_CONST ucset = "0123456789ABCDEF";
#endif

#ifdef __GNUC__

char *
_DEFUN (_llicvt, (buffer, value, type),
	char *buffer _AND
	long long value _AND
	char type)
{
  assert(0); // @jyy: well, many platforms doesn't suppor this.
  return buffer;
}

#endif

#if LONG_MAX != INT_MAX

char *
_DEFUN (_licvt, (buffer, value, type),
	char *buffer _AND
	long value _AND
	char type)
{
  int base, i;

  i = CVT_BUF_SIZE - 1;
  buffer[i--] = '\0';

  switch (type)
    {
    case 'd':
    case 'i':
      base = 10;

      if (value < 0)
	{
	  if (value == LONG_MIN)
	    {			/* Special case */
	      long temp;
	      temp = (LONG_MAX % base) + 1;
	      value = LONG_MAX / base;
	      if (temp == base)
		{
		  value += 1;
		  temp = 0;
		}
	      buffer[i--] = lcset[temp];
	    }
	  else
	    value = -value;
	}

      while (i >= 0)
	{
	  buffer[i--] = lcset[value % base];

	  if ((value /= base) == 0)
	    break;
	}
      break;

    case 'u':
    case 'o':
    case 'x':
    case 'X':
      if (type == 'u')
	base = 10;
      else if (type == 'o')
	base = 8;
      else
	base = 16;

      while (i >= 0)
	{
	  if (type == 'X')
	    buffer[i--] = ucset[((unsigned long) value) % base];
	  else
	    buffer[i--] = lcset[((unsigned long) value) % base];

	  if ((value = ((unsigned long) value) / base) == 0)
	    break;
	}
      break;

    default:
      break;
    }

  return (&buffer[++i]);
}

#endif

char *
_DEFUN (_icvt, (buffer, value, type),
	char *buffer _AND
	int value _AND
	char type)
{
  int base, i;

  i = CVT_BUF_SIZE - 1;
  buffer[i--] = '\0';

  switch (type)
    {
    case 'd':
    case 'i':
      base = 10;

      if (value < 0)
	{
	  if (value == INT_MIN)
	    {			/* Special case */
	      int temp;
	      temp = (INT_MAX % base) + 1;
	      value = INT_MAX / base;
	      if (temp == base)
		{
		  value += 1;
		  temp = 0;
		}
	      buffer[i--] = lcset[temp];
	    }
	  else
	    value = -value;
	}

      while (i >= 0)
	{
	  buffer[i--] = lcset[value % base];

	  if ((value /= base) == 0)
	    break;
	}
      break;

    case 'u':
    case 'o':
    case 'x':
    case 'X':
      if (type == 'u')
	base = 10;
      else if (type == 'o')
	base = 8;
      else
	base = 16;

      while (i >= 0)
	{
	  if (type == 'X')
	    buffer[i--] = ucset[((unsigned) value) % base];
	  else
	    buffer[i--] = lcset[((unsigned) value) % base];

	  if ((value = ((unsigned) value) / base) == 0)
	    break;
	}
      break;

    default:
      break;
    }

  return (&buffer[++i]);
}

char *
_DEFUN (_sicvt, (buffer, value, type),
	char *buffer _AND
	short value _AND
	char type)
{
  if (sizeof(short) == sizeof(int))
    {
      return (_icvt(buffer, value, type));
    }
  else 
    {
      int base, i;
      i = CVT_BUF_SIZE - 1;
      buffer[i--] = '\0';

      switch (type)
	{
	case 'd':
	case 'i':
	  base = 10;

	  if (value < 0)
	    value = -value;

	  while (i >= 0)
	    {
	      buffer[i--] = lcset[value % base];

	      if ((value /= base) == 0)
		break;
	    }
	  break;

	case 'u':
	case 'o':
	case 'x':
	case 'X':
	  value &= 0xffff;

	  if (type == 'u')
	    base = 10;
	  else if (type == 'o')
	    base = 8;
	  else
	    base = 16;

	  while (i >= 0)
	    {
	      if (type == 'X')
		buffer[i--] = ucset[value % base];
	      else
		buffer[i--] = lcset[value % base];

	      if ((value /= base) == 0)
		break;
	    }
	  break;

	default:
	  break;
	}

      return (&buffer[++i]);
    }
}
