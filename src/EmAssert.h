/* -*- mode: C++; tab-width: 4 -*- */
/* ===================================================================== *\
	Copyright (c) 2001 Palm, Inc. or its subsidiaries.
	All rights reserved.

	This file is part of the Palm OS Emulator.

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.
\* ===================================================================== */

#ifndef EmAssert_h
#define EmAssert_h

#ifdef __cplusplus
extern "C"
#endif
void MyAssertFailed (
	const char* assertion,
	const char* file,
	unsigned int line);

// Use EmAssert to assert only in debug mode.
// Use EmASSERT to assert even in release mode.
// The latter should be used only temporarily to
// find bugs that occur only in release mode.

#define EmASSERT(expr)					\
  ((void) ((expr) ? 0 :					\
	   (MyAssertFailed (#expr,			\
			   __FILE__, __LINE__), 0)))

#ifdef NDEBUG

	#define EmAssert(expr) ((void) 0)

#else

	#define EmAssert(expr)	EmASSERT (expr)

#endif

#endif	// EmAssert.h
