#define REGPARAM

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
