/*
FUNCTION
        <<fiprintf>>---format output to file (integer only)
INDEX
	fiprintf

ANSI_SYNOPSIS
        #include <stdio.h>

        int fiprintf(FILE *<[fd]>, const char *<[format]>, ...);

TRAD_SYNOPSIS
	#include <stdio.h>

	int fiprintf(<[fd]>, <[format]> [, <[arg]>, ...]);
	FILE *<[fd]>;
	char *<[format]>;

DESCRIPTION
<<fiprintf>> is a restricted version of <<fprintf>>: it has the same
arguments and behavior, save that it cannot perform any floating-point
formatting---the <<f>>, <<g>>, <<G>>, <<e>>, and <<F>> type specifiers
are not recognized.

RETURNS
        <<fiprintf>> returns the number of bytes in the output string,
        save that the concluding <<NULL>> is not counted.
        <<fiprintf>> returns when the end of the format string is
        encountered.  If an error occurs, <<fiprintf>>
        returns <<EOF>>.

PORTABILITY
<<fiprintf>> is not required by ANSI C.

Supporting OS subroutines required: <<close>>, <<fstat>>, <<isatty>>,
<<lseek>>, <<read>>, <<sbrk>>, <<write>>.
*/

