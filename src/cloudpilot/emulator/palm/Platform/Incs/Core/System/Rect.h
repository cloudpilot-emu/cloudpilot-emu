/******************************************************************************
 *
 * Copyright (c) 1994-1999 Palm Computing, Inc. or its subsidiaries.
 * All rights reserved.
 *
 * File: Rect.h
 *
 * Description:
 *	  This file defines rectangle structures and routines.
 *
 * History:
 *		November 3, 1994	Created by Roger Flores
 *			Name	Date		Description
 *			----	----		-----------
 *	  		bob	2/9/99	Use Coord abstraction, fix up consts
 *
 *****************************************************************************/

#ifndef __RECT_H__
#define __RECT_H__

// Include elementary types
#include <PalmTypes.h>					// Basic types
#include <CoreTraps.h>					// Trap Numbers.


typedef struct AbsRectType {
  Coord left;
  Coord top;
  Coord right;
  Coord bottom;
} AbsRectType;


typedef struct PointType {
  Coord x;
  Coord y;
} PointType;


typedef struct RectangleType {
  PointType  topLeft;
  PointType  extent;
} RectangleType;

typedef RectangleType *RectanglePtr;



#ifdef REMOVE_FOR_EMULATOR
#ifdef __cplusplus
extern "C" {
#endif

extern void RctSetRectangle (RectangleType *rP,
	Coord left, Coord top, Coord width, Coord height)
							SYS_TRAP(sysTrapRctSetRectangle);

extern void RctCopyRectangle (const RectangleType *srcRectP, 
	RectangleType *dstRectP)
							SYS_TRAP(sysTrapRctCopyRectangle);

extern void RctInsetRectangle (RectangleType *rP, Coord insetAmt)
							SYS_TRAP(sysTrapRctInsetRectangle);

extern void  RctOffsetRectangle (RectangleType *rP, Coord deltaX, Coord deltaY)
							SYS_TRAP(sysTrapRctOffsetRectangle);

extern Boolean RctPtInRectangle (Coord x, Coord y, const RectangleType *rP)
							SYS_TRAP(sysTrapRctPtInRectangle);

extern void RctGetIntersection (const RectangleType *r1P, const RectangleType *r2P, 
	RectangleType *r3P)
							SYS_TRAP(sysTrapRctGetIntersection);


#ifdef __cplusplus
}
#endif
#endif


#endif //__RECT_H__
