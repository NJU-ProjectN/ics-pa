/*
FUNCTION
<<rand>>, <<srand>>---pseudo-random numbers

INDEX
	rand
INDEX
	srand
INDEX
	_rand_r
INDEX
	_srand_r

ANSI_SYNOPSIS
	#include <stdlib.h>
	int rand(void);
	void srand(unsigned int <[seed]>);

	int _rand_r(void *<[reent]>);
	void _srand_r(void *<[reent]>, unsigned int <[seed]>);

TRAD_SYNOPSIS
	#include <stdlib.h>
	int rand();

	void srand(<[seed]>)
	unsigned int <[seed]>;

	int _rand_r(<[reent]>);
	char *<[reent]>

	void _srand_r(<[data]>,<[seed]>)
	char *<[reent]>;
	unsigned int <[seed]>;


DESCRIPTION
<<rand>> returns a different integer each time it is called; each
integer is chosen by an algorithm designed to be unpredictable, so
that you can use <<rand>> when you require a random number.
The algorithm depends on a static variable called the ``random seed'';
starting with a given value of the random seed always produces the
same sequence of numbers in successive calls to <<rand>>.

You can set the random seed using <<srand>>; it does nothing beyond
storing its argument in the static variable used by <<rand>>.  You can
exploit this to make the pseudo-random sequence less predictable, if
you wish, by using some other unpredictable value (often the least
significant parts of a time-varying value) as the random seed before
beginning a sequence of calls to <<rand>>; or, if you wish to ensure
(for example, while debugging) that successive runs of your program
use the same ``random'' numbers, you can use <<srand>> to set the same
random seed at the outset.

<<_rand_r>> and <<_srand_r>> are reentrant versions of <<rand>> and
<<srand>>.  The extra argument <[reent]> is a pointer to a reentrancy
structure.

RETURNS
<<rand>> returns the next pseudo-random integer in sequence; it is a
number between <<0>> and <<RAND_MAX>> (inclusive).

<<srand>> does not return a result.

PORTABILITY
<<rand>> is required by ANSI, but the algorithm for pseudo-random
number generation is not specified; therefore, even if you use
the same random seed, you cannot expect the same sequence of results
on two different systems.

<<rand>> requires no supporting OS subroutines.
*/

#include <stdlib.h>
#include <reent.h>

int
_DEFUN (_rand_r, (ptr),
	struct _reent *ptr)
{
  ptr->_next = (ptr->_next * 1103515245) + 12345;
  return ((ptr->_next >> 16) & 0x7fff);
}

void
_DEFUN (_srand_r, (ptr, seed),
	struct _reent *ptr _AND
	unsigned int seed)
{
  ptr->_next = seed;
}

#ifndef _REENT_ONLY

int
_DEFUN_VOID (rand)
{
  return _rand_r (_REENT);
}

void
_DEFUN (srand, (seed),
	unsigned int seed)
{
  _srand_r (_REENT, seed);
}

#endif
