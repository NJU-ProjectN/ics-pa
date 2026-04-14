#ifndef __IEEE_BIG_ENDIAN
#ifndef __IEEE_LITTLE_ENDIAN

#ifdef __arm__
/* ARM always has big-endian words.  Within those words the byte ordering
   appears to be big or little endian.  Newlib doesn't seem to care about
   the byte ordering within words.  */
#define __IEEE_BIG_ENDIAN
#endif
#ifdef __hppa__
#define __IEEE_BIG_ENDIAN
#endif
#ifdef __sparc__
#define __IEEE_BIG_ENDIAN
#endif
#ifdef __m68k__
#define __IEEE_BIG_ENDIAN
#endif
#ifdef __mc68000__
#define __IEEE_BIG_ENDIAN
#endif
#if defined (__H8300__) || defined (__H8300H__)
#define __IEEE_BIG_ENDIAN
#define __SMALL_BITFIELDS
#endif

#ifdef __H8500__
#define __IEEE_BIG_ENDIAN
#define __SMALL_BITFIELDS
#endif
#ifdef __sh__
#define __IEEE_BIG_ENDIAN
#endif
#ifdef _AM29K
#define __IEEE_BIG_ENDIAN
#endif
#ifdef __i386__
#define __IEEE_LITTLE_ENDIAN
#endif
#ifdef __MIPSEL__
#define __IEEE_LITTLE_ENDIAN
#endif
#ifdef __MIPSEB__
#define __IEEE_BIG_ENDIAN
#endif
/* necv70 was __IEEE_LITTLE_ENDIAN. */
#ifdef __i960__
#define __IEEE_LITTLE_ENDIAN
#endif

#ifdef __W65__
#define __IEEE_LITTLE_ENDIAN
#define __SMALL_BITFIELDS
#define _DOUBLE_IS_32BITS
#endif

#if defined(__Z8001__) || defined(__Z8002__)
#define __IEEE_BIG_ENDIAN
#endif

#ifdef __m88k__
#define __IEEE_BIG_ENDIAN
#endif

#ifdef __v800
#define __IEEE_LITTLE_ENDIAN
#endif

#ifdef __PPC__
#ifdef _BIG_ENDIAN
#define __IEEE_BIG_ENDIAN
#else
#define __IEEE_LITTLE_ENDIAN
#endif
#endif


#ifndef __IEEE_BIG_ENDIAN
#ifndef __IEEE_LITTLE_ENDIAN
#error Endianess not declared!!
#endif /* not __IEEE_LITTLE_ENDIAN */
#endif /* not __IEEE_BIG_ENDIAN */

#endif /* not __IEEE_LITTLE_ENDIAN */
#endif /* not __IEEE_BIG_ENDIAN */

