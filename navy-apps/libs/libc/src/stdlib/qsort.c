/*
FUNCTION
<<qsort>>---sort an array

INDEX
	qsort

ANSI_SYNOPSIS
	#include <stdlib.h>
	void qsort(void *<[base]>, size_t <[nmemb]>, size_t <[size]>,
		   int (*<[compar]>)(const void *, const void *) );

TRAD_SYNOPSIS
	#include <stdlib.h>
	qsort(<[base]>, <[nmemb]>, <[size]>, <[compar]> )
	char *<[base]>;
	size_t <[nmemb]>;
	size_t <[size]>;
	int (*<[compar]>)();

DESCRIPTION
<<qsort>> sorts an array (beginning at <[base]>) of <[nmemb]> objects.
<[size]> describes the size of each element of the array.

You must supply a pointer to a comparison function, using the argument
shown as <[compar]>.  (This permits sorting objects of unknown
properties.)  Define the comparison function to accept two arguments,
each a pointer to an element of the array starting at <[base]>.  The
result of <<(*<[compar]>)>> must be negative if the first argument is
less than the second, zero if the two arguments match, and positive if
the first argument is greater than the second (where ``less than'' and
``greater than'' refer to whatever arbitrary ordering is appropriate).

The array is sorted in place; that is, when <<qsort>> returns, the
array elements beginning at <[base]> have been reordered.

RETURNS
<<qsort>> does not return a result.

PORTABILITY
<<qsort>> is required by ANSI (without specifying the sorting algorithm).

Supporting OS subroutines required: <<close>>, <<fstat>>, <<isatty>>,
<<lseek>>, <<read>>, <<sbrk>>, <<write>>.
*/

/*
 * qsort.c
 * Original Author:	G. Haley
 *
 * Sorts an array of nmemb objects, the initial member of which is pointed to
 * by base. The size of each object is specified by size. The contents of the
 * array are sorted in ascending order according to a comparison function
 * pointed to by compar, which is called with two arguments pointing to the
 * objects being compared. The function shall return an integer less than,
 * equal to or greater than zero if the first argument is considered to be
 * respectively less than, equal to or greater than the second. If two members
 * compare as equal, their order in the sorted array is unspecified.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

static int
_DEFUN (find_pivot, (base, i, j, size, compar),
	_PTR base _AND
	int i _AND
	int j _AND
	size_t size _AND
	int (*compar) ())
{
  _PTR first_key;
  _PTR next_key;
  int k, res;

  first_key = (_PTR) (((char *) base) + (i * size));
  next_key = first_key;

  for (k = i + 1; k <= j; k++)
    {
      next_key = (_PTR) (((char *) next_key) + size);
      res = (*compar) (next_key, first_key);

      if (res > 0)
	return k;
      else if (res < 0)
	return i;
    }

  return -1;
}

static void
_DEFUN (swap, (base, i, j, size),
	_PTR base _AND
	int i _AND
	int j _AND
	size_t size)
{
#ifdef __GNUC__
  _PTR temp = __builtin_alloca (size);
#else
  static _PTR temp = NULL;
  static size_t max_size = 0;
#endif

  _PTR elem1, *elem2;

#ifndef __GNUC__
  if (size > max_size)
    {
      temp = realloc (temp, size);
      max_size = size;
    }
#endif

  elem1 = (_PTR) (((char *) base) + (i * size));
  elem2 = (_PTR) (((char *) base) + (j * size));

  memcpy (temp, elem1, size);
  memcpy (elem1, elem2, size);
  memcpy (elem2, temp, size);
}

static int
_DEFUN (partition, (base, i, j, pivot_index, size, compar),
	_PTR base _AND
	int i _AND
	int j _AND
	int pivot_index _AND
	size_t size _AND
	int (*compar) ())
{
  int left, right;

  left = i;
  right = j;

  do
    {
      swap (base, left, right, size);

      if (pivot_index == left)
	pivot_index = right;
      else if (pivot_index == right)
	pivot_index = left;

      while (compar ((_PTR) (((char *) base) + (left * size)),
		     (_PTR) (((char *) base) + (pivot_index * size))) < 0)
	left++;

      while (compar ((_PTR) (((char *) base) + (right * size)),
		     (_PTR) (((char *) base) + (pivot_index * size))) >= 0)
	right--;

    }
  while (left <= right);

  return left;
}

static void
_DEFUN (inside_qsort, (base, i, j, size, compar),
	_PTR base _AND
	int i _AND
	int j _AND
	size_t size _AND
	int (*compar) ())
{
  int pivot_index, mid;

  if ((pivot_index = find_pivot (base, i, j, size, compar)) != -1)
    {
      mid = partition (base, i, j, pivot_index, size, compar);

      inside_qsort (base, i, mid - 1, size, compar);
      inside_qsort (base, mid, j, size, compar);
    }
}

_VOID 
_DEFUN (qsort, (base, nmemb, size, compar),
	_PTR base _AND
	size_t nmemb _AND
	size_t size _AND
	int (*compar) ())
{
  inside_qsort (base, 0, nmemb - 1, size, compar);
}
