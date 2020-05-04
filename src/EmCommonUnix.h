/* -*- mode: C++; tab-width: 4 -*- */
/* ===================================================================== *\
	Copyright (c) 1999-2001 Palm, Inc. or its subsidiaries.
	All rights reserved.

	This file is part of the Palm OS Emulator.

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.
\* ===================================================================== */

#ifndef EmUnixCommon_h
#define EmUnixCommon_h

// Common header file included by all Palm OS Emulator for UNIX files.

// Define this before anything else, especially anything that can pull in
// an STL header.  gcc-2.95.2 has a problem instantiating vector<ptmf>
// classes, which we use in EmRegs.  When it gets around to instantiating
// the allocator class to be used with the vector, the compiler complains
// that alloc::address (an allocator function) is already instantiated.
// By defining this macro, we appear to avoid that problem.

#define __STL_USE_SGI_ALLOCATORS


// Palm headers

#include "Palm.h"


// Std C/C++ Library stuff

// Don't pull these in here.  These would be brought in for *every*
// file, which is too expensive.  We can do this on other platforms,
// as EmCommon.h is the basis for the precompiled header files,
// which load in quickly even with lots of extra stuff in them.

//#include <ctype.h>				// isalpha, tolower
//#include <stdarg.h>
//#include <stddef.h>
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>

//#include <sys/types.h>
//#include <sys/socket.h>
//#include <sys/ioctl.h>
//#include <netinet/in.h>
//#include <netinet/tcp.h>
//#include <netdb.h>

//#include <algorithm>			// find, sort
//#include <deque>				// deque
//#include <list>					// list
//#include <string>				// list
//#include <utility>				// pair
//#include <vector>
//#include <map>


#define down fl_down	// "down" defined both in WindowNew.h and Fl/Enumerations.h


// ============================================
// ========== Windows socket mapping ==========
// ============================================

#include <sys/types.h>
#include <sys/socket.h>			// socklen_t

#if !defined(HAVE_TYPE_SOCKLEN_T)
	typedef int			socklen_t;	// not defined in <sys/socket.h> on solaris or FreeBSD
#endif

typedef int				SOCKET;

#define WSAEINVAL		EINVAL
#define INVALID_SOCKET	((SOCKET) (~0))
#define SOCKET_ERROR	(-1)
#define closesocket		close
#define ioctlsocket		ioctl

#ifndef INADDR_LOOPBACK
#define INADDR_LOOPBACK	0x7f000001
#endif

#endif	/* EmUnixCommon_h */
