/******************************************************************************
 *
 * Copyright (c) 1996-1999 Palm Computing, Inc. or its subsidiaries.
 * All rights reserved.
 *
 * File: ScrollBar.h
 *
 * Description:
 *	  This file defines scroll bar structures and routines.
 *
 * History:
 *		Feb 6, 1996	Created by Art Lamb
 *
 *****************************************************************************/

#ifndef __SCROLLBAR_H__
#define __SCROLLBAR_H__

#include <PalmTypes.h>
#include <CoreTraps.h>

#include <Rect.h>
#include <Event.h>

typedef enum { sclUpArrow, sclDownArrow, sclUpPage, sclDownPage, sclCar } 
	ScrollBarRegionType;


typedef struct {
	UInt16 usable			:1;		// Set if part of ui 
	UInt16 visible			:1;		// Set if drawn, used internally
	UInt16 hilighted		:1;		// Set if region is hilighted
	UInt16 shown			:1;		// Set if drawn and maxValue > minValue
	UInt16 activeRegion	:4;		// ScrollBarRegionType
} ScrollBarAttrType;


typedef struct ScrollBarType {
	RectangleType		bounds;
	UInt16				id;
	ScrollBarAttrType	attr;
	Int16					value;
	Int16					minValue;
	Int16					maxValue;
	Int16					pageSize;
	Int16					penPosInCar;
	Int16					savePos;
} ScrollBarType;

typedef ScrollBarType *ScrollBarPtr;

#ifdef REMOVE_FOR_EMULATOR
#ifdef __cplusplus
extern "C" {
#endif

extern void 	SclGetScrollBar (const ScrollBarPtr bar, Int16 *valueP, 
	Int16 *minP, Int16 *maxP, Int16 *pageSizeP)
							SYS_TRAP(sysTrapSclGetScrollBar);

extern void		SclSetScrollBar (const ScrollBarPtr bar, Int16 value, 
						const Int16 min, const Int16 max, const Int16 pageSize)
							SYS_TRAP(sysTrapSclSetScrollBar);

extern void		SclDrawScrollBar (const ScrollBarPtr bar)
							SYS_TRAP(sysTrapSclDrawScrollBar);

extern Boolean	SclHandleEvent (const ScrollBarPtr bar, const EventType *event)
							SYS_TRAP(sysTrapSclHandleEvent);

#ifdef __cplusplus 
}
#endif
#endif


#endif //__SCROLLBAR_H__
