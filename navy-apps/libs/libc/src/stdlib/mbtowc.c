/*
FUNCTION
<<mbtowc>>---minimal multibyte to wide char converter

INDEX
	mbtowc

ANSI_SYNOPSIS
	#include <stdlib.h>
	int mbtowc(wchar_t *<[pwc]>, const char *<[s]>, size_t <[n]>);

TRAD_SYNOPSIS
	#include <stdlib.h>
	int mbtowc(<[pwc]>, <[s]>, <[n]>)
	wchar_t *<[pwc]>;
	const char *<[s]>;
	size_t <[n]>;

DESCRIPTION
This is a minimal ANSI-conforming implementation of <<mbtowc>>.  The
only ``multi-byte character sequences'' recognized are single bytes,
and they are ``converted'' to themselves.

Each call to <<mbtowc>> copies one character from <<*<[s]>>> to
<<*<[pwc]>>>, unless <[s]> is a null pointer.

In this implementation, the argument <[n]> is ignored.

RETURNS
This implementation of <<mbtowc>> returns <<0>> if
<[s]> is <<NULL>>; it returns <<1>> otherwise (reporting the length of
the character ``sequence'' used).

PORTABILITY
<<mbtowc>> is required in the ANSI C standard.  However, the precise
effects vary with the locale.

<<mbtowc>> requires no supporting OS subroutines.
*/

#include <stdlib.h>

int
_DEFUN (mbtowc, (pwc, s, n),
        wchar_t *pwc _AND
        const char *s _AND
        size_t n)
{
        if (s == NULL)
                return 0;
        if (n == 0)
                return -1;
        if (pwc)
                *pwc = (wchar_t) *s;
        return (*s != '\0');
}

