/*
FUNCTION
	<<strtok>>---get next token from a string

INDEX
	strtok

INDEX
	_strtok_r

ANSI_SYNOPSIS
	#include <string.h>
      	char *strtok(char *<[source]>, const char *<[delimiters]>)

      	char *_strtok_r(void *<[reent]>, 
                        const char *<[source]>, const char *<[delimiters]>)

TRAD_SYNOPSIS
	#include <string.h>
	char *strtok(<[source]>, <[delimiters]>)
	char *<[source]>;
	char *<[delimiters]>;

	char *_strtok_r(<[reent]>, <[source]>, <[delimiters]>)
	char *<[reent]>;
	char *<[source]>;
	char *<[delimiters]>;

DESCRIPTION
	A series of calls to <<strtok>> break the string starting at
	<<*<[source]>>> into a sequence of tokens.  The tokens are
	delimited from one another by characters from the string at
	<<*<[delimiters]>>>, at the outset.
	The first call to <<strtok>> normally has a string address as
	the first argument; subsequent calls can use <<NULL>> as the
	first argument, to continue searching the same string.  You
	can continue searching a single string with different
	delimiters by using a different delimiter string on each call.

	<<strtok>> begins by searching for any character not in the
	<[delimiters]> string: the first such character is the
	beginning of a token (and its address will be the result of
	the <<strtok>> call). <<strtok>> then continues searching
	until it finds another delimiter character; it replaces that
	character by <<NULL>> and returns.  (If <<strtok>> comes to
	the end of the <<*<[source]>>> string without finding any more
	delimiters, the entire remainder of the string is treated as
	the next token).
	<<strtok>> starts its search at <<*<[source]>>>, unless you
	pass <<NULL>> as the first argument;  if <[source]> is
	<<NULL>>, <<strtok>> continues searching from the end of the
	last search. Exploiting the <<NULL>> first argument leads to
	non-reentrant code. You can easily circumvent this problem by
	saving the last delimiter address in your application, and
	always using it to pass a non-null <[source]> argument.

	<<_strtok_r>> performs the same function as <<strtok>>, but
	is reentrant.  The extra argument <[reent]> is a pointer to
	a reentrancy structure.

RETURNS
	<<strtok>> returns a pointer to the next token, or <<NULL>> if
	no more tokens can be found.

PORTABILITY
<<strtok>> is ANSI C.

<<strtok>> requires no supporting OS subroutines.

QUICKREF
	strtok ansi impure
*/

#include <string.h>
#include <reent.h>

char *				/* NULL if no token left */
_DEFUN (_strtok_r, (r, s, delim),
	struct _reent *r _AND
	char *s _AND
	_CONST char *delim)
{
  char *scan;
  char *tok;
  _CONST char *dscan;

  if (s == NULL && r->_scanpoint == NULL)
    {
      return NULL;
    }
  if (s != NULL)
    scan = s;
  else
    scan = r->_scanpoint;

  /*
   * Scan leading delimiters.
   */

  for (; *scan != '\0'; scan++)
    {
      for (dscan = delim; *dscan != '\0'; dscan++)
	{
	  if (*scan == *dscan)
	    break;
	}

      if (*dscan == '\0')
	break;
    }
  if (*scan == '\0')
    {
      r->_scanpoint = NULL;
      return NULL;
    }

  tok = scan;

  /*
   * Scan token.
   */

  for (; *scan != '\0'; scan++)
    {
      for (dscan = delim; *dscan != '\0';)	/* ++ moved down. */
	if (*scan == *dscan++)
	  {
	    r->_scanpoint = scan + 1;
	    *scan = '\0';
	    return tok;
	  }
    }

  /* Reached end of string. */
  r->_scanpoint = NULL;
  return tok;
}

#ifndef _REENT_ONLY

char *
_DEFUN (strtok, (s, delim),
	char *s _AND
	_CONST char *delim)
{
  return _strtok_r (_REENT, s, delim);
}

#endif
