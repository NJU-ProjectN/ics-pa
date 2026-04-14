/*
FUNCTION
	<<memcmp>>---compare two memory areas

INDEX
	memcmp

ANSI_SYNOPSIS
	#include <string.h>
	int memcmp(const void *<[s1]>, const void *<[s2]>, size_t <[n]>);

TRAD_SYNOPSIS
	#include <string.h>
	int memcmp(<[s1]>, <[s2]>, <[n]>)
	void *<[s1]>;
	void *<[s2]>;
	size_t <[n]>;

DESCRIPTION
	This function compares not more than <[n]> characters of the
	object pointed to by <[s1]> with the object pointed to by <[s2]>.


RETURNS
	The function returns an integer greater than, equal to or
	less than zero 	according to whether the object pointed to by
	<[s1]> is greater than, equal to or less than the object
	pointed to by <[s2]>.

PORTABILITY
<<memcmp>> is ANSI C.

<<memcmp>> requires no supporting OS subroutines.

QUICKREF
	memcmp ansi pure
*/

#include <string.h>

int
_DEFUN (memcmp, (m1, m2, n),
	_CONST _PTR m1 _AND
	_CONST _PTR m2 _AND
	size_t n)
{
  unsigned char *s1 = (unsigned char *) m1;
  unsigned char *s2 = (unsigned char *) m2;

  while (n--)
    {
      if (*s1 != *s2)
	{
	  return *s1 - *s2;
	}
      s1++;
      s2++;
    }
  return 0;
}
