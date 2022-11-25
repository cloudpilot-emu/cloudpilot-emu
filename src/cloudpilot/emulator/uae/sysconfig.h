#define SIZEOF_CHAR 1
#define SIZEOF_SHORT 2
#define SIZEOF_INT 0	// Goofy, but it gets us what we want: shorts
						// for 2-byte types and longs for 4-byte types
#define SIZEOF_LONG 4

#ifdef _MSC_VER
	#define SIZEOF___INT64 8
#else
	#define SIZEOF_LONG_LONG 8
#endif

#define REGPARAM
#define DONT_HAVE_REAL_POSIX
#define EXEC_TYPES_H

#include <string.h>

#ifdef _MSC_VER
	#pragma warning( disable : 4244 )	// warning C4244: initializing : conversion from unsigned long to unsigned short, possible loss of data
	#pragma warning( disable : 4786 )	// warning C4786: '<foo>' : identifier was truncated to '255' characters in the browser information
#endif


#ifdef _MSC_VER
	#define __inline__	__inline
#else
	#define __inline__	inline
#endif

#ifndef STATIC_INLINE
#define STATIC_INLINE static __inline__
#endif
