/*
FUNCTION
<<wctomb>>---minimal wide char to multibyte converter

INDEX
	wctomb

ANSI_SYNOPSIS
	#include <stdlib.h>
	int wctomb(char *<[s]>, wchar_t <[wchar]>);

TRAD_SYNOPSIS
	#include <stdlib.h>
	int wctomb(<[s]>, <[wchar]>)
	char *<[s]>;
	wchar_t <[wchar]>;

DESCRIPTION
This is a minimal ANSI-conforming implementation of <<wctomb>>.  The
only ``wide characters'' recognized are single bytes,
and they are ``converted'' to themselves.

Each call to <<wctomb>> copies the character <[wchar]> to <<*<[s]>>>,
unless <[s]> is a null pointer.

RETURNS
This implementation of <<wctomb>> returns <<0>> if
<[s]> is <<NULL>>; it returns <<1>> otherwise (reporting the length of
the character ``sequence'' generated).

PORTABILITY
<<wctomb>> is required in the ANSI C standard.  However, the precise
effects vary with the locale.

<<wctomb>> requires no supporting OS subroutines.
*/

#include <stdlib.h>
int
_DEFUN (wctomb, (s, wchar),
        char *s _AND
        wchar_t wchar)
{
        if (s == NULL)
                return 0;

        *s = (char) wchar;
        return 1;
}
