/******************************************************************************
 *
 * Copyright (c) 1998-1999 Palm Computing, Inc. or its subsidiaries.
 * All rights reserved.
 *
 * File: IntlMgr.h
 *
 * Description:
 *	  This file defines public Int'l Mgr structures and routines.
 *
 * History:
 *		03/21/98	kwk	Created by Ken Krugler.
 *		10/14/98	kwk	Added intlIntlGetRoutineAddress selector and
 *							IntlGetRoutineAddress routine declaration.
 *		08/05/99	kwk	Added intlIntlHandleEvent selector and the
 *							IntlHandleEvent routine declaration.
 *		09/22/99	kwk	Added intlTxtParamString selector.
 *		10/20/99	kwk	Moved private stuff to IntlPrv.h
 *
 *****************************************************************************/

#ifndef __INTLMGR_H__
#define __INTLMGR_H__

#include <PalmTypes.h>
#include <CoreTraps.h>

// If nobody has explicitly specified that we should or should not
// use our international trap dispatcher, set it based on the emulation
// level.

#ifndef USE_INTL_TRAPS
	#if EMULATION_LEVEL == EMULATION_NONE
		#define	USE_INTL_TRAPS	1
	#else
		#define	USE_INTL_TRAPS	0
	#endif
#endif

/***********************************************************************
 * Public constants
 ***********************************************************************/

// Bits set for the Intl Mgr feature.

#define	intlMgrExists	0x00000001

// International Manager trap macros.

#if USE_INTL_TRAPS
	#define INTL_TRAP(sel) \
		_SYSTEM_API(_CALL_WITH_SELECTOR)(_SYSTEM_TABLE, sysTrapIntlDispatch, sel)
#else
	#define INTL_TRAP(intlSelectorNum)
#endif

// Selectors for routines found in the international manager. The order
// of these selectors MUST match the jump table in IntlDispatch.c.

#define intlIntlInit					0
#define intlTxtByteAttr					1
#define intlTxtCharAttr					2
#define intlTxtCharXAttr				3
#define intlTxtCharSize					4
#define intlTxtGetPreviousChar			5
#define intlTxtGetNextChar				6
#define intlTxtGetChar					7
#define intlTxtSetNextChar				8
#define intlTxtCharBounds				9
#define intlTxtPrepFindString			10
#define intlTxtFindString				11
#define intlTxtReplaceStr				12
#define intlTxtWordBounds				13
#define intlTxtCharEncoding				14
#define intlTxtStrEncoding				15
#define intlTxtEncodingName				16
#define intlTxtMaxEncoding				17
#define intlTxtTransliterate			18
#define intlTxtCharIsValid				19
#define intlTxtCompare					20
#define intlTxtCaselessCompare			21
#define intlTxtCharWidth				22
#define intlTxtGetTruncationOffset		23
#define intlIntlGetRoutineAddress		24
#define intlIntlHandleEvent				25	// New for Palm OS 3.5
#define intlTxtParamString				26
#define intlTxtConvertEncodingV35		27	// Patched for Palm OS 3.5.2
#define intlTxtConvertEncoding			28	// New for Palm OS 4.0
#define intlIntlSetRoutineAddress		29
#define intlTxtGetWordWrapOffset		30
#define intlTxtNameToEncoding			31	
#define	intlIntlStrictChecks			32

/***********************************************************************
 * Public routines
 ***********************************************************************/

#ifdef REMOVE_FOR_EMULATOR
#ifdef __cplusplus
	extern "C" {
#endif

// Return back the address of the routine indicated by <inSelector>. If
// <inSelector> isn't a valid routine selector, return back NULL.
void *IntlGetRoutineAddress(IntlSelector inSelector)
		INTL_TRAP(intlIntlGetRoutineAddress);

#ifdef __cplusplus
	}
#endif
#endif

#endif // __INTLMGR_H__
