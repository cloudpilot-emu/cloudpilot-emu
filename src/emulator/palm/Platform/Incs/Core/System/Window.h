/******************************************************************************
 *
 * Copyright (c) 1994-1999 Palm Computing, Inc. or its subsidiaries.
 * All rights reserved.
 *
 * File: Window.h
 *
 * Description:
 *        This file defines window structures and routines that support color.
 *
 * History:
 *		January 20, 1999	Created by Bob Ebert
 *			Name	Date		Description
 *			----	----		-----------
 *			bob	1/20/99	Branch off WindowNew.h
 *			BS		4/20/99	Re-design of the screen driver
 *			bob	5/26/99	Cleanup/reorg
 *			jmp	12/23/99	Fix <> vs. "" problem.
 *
 *****************************************************************************/

#ifndef __WINDOW_H__
#define __WINDOW_H__

#include <PalmTypes.h>
#include <CoreTraps.h>
#include <Font.h>
#include <Rect.h>
#include <Bitmap.h>

#include <PalmOptErrorCheckLevel.h>	// #define ERROR_CHECK_LEVEL

#ifdef _WIN32
  #pragma warning(disable: 4214)  // nonstandard extension used : bit field types other than int
#endif

// enum for WinScrollRectangle
typedef enum { winUp = 0, winDown, winLeft, winRight } WinDirectionType;


// enum for WinCreateOffscreenWindow
typedef enum { screenFormat = 0, genericFormat } WindowFormatType;


// enum for WinLockScreen
typedef enum {winLockCopy, winLockErase, winLockDontCare} WinLockInitType;


// operations for the WinScreenMode function
typedef enum {
	winScreenModeGetDefaults,
	winScreenModeGet,
	winScreenModeSetToDefaults,
	winScreenModeSet,
	winScreenModeGetSupportedDepths,
	winScreenModeGetSupportsColor
} WinScreenModeOperation;

// Operations for the WinPalette function
#define winPaletteGet 				0
#define winPaletteSet 				1
#define winPaletteSetToDefault	2

// transfer modes for color drawing
typedef enum {winPaint, winErase, winMask, winInvert, winOverlay, winPaintInverse, winSwap} WinDrawOperation;


typedef enum { blackPattern, whitePattern, grayPattern, customPattern } PatternType;
#define noPattern						blackPattern
#define grayHLinePattern			0xAA
#define grayHLinePatternOdd		0x55


// grayUnderline means dotted current foreground color
// solidUnderline means solid current foreground color
// colorUnderline redundant, use solidUnderline instead
typedef enum { noUnderline, grayUnderline, solidUnderline, colorUnderline } UnderlineModeType;

#define WinMaxSupportedDepth	8
#define WinNumSupportedColors 4

typedef UInt8 IndexedColorType;				// 1-, 2-, 4-, or 8-bit index

typedef UInt8 CustomPatternType [8];		// 8x8 1-bit deep pattern

// for WinPalette startIndex value, respect indexes in passed table
#define WinUseTableIndexes -1

//-----------------------------------------------
// Draw state structures.
//-----------------------------------------------


typedef struct DrawStateType {
	WinDrawOperation	transferMode;
	PatternType			pattern;
	UnderlineModeType	underlineMode;
	FontID				fontId;
	FontPtr				font;
	CustomPatternType	patternData;
	IndexedColorType	foreColor;
	IndexedColorType	backColor;
	IndexedColorType	textColor;
	UInt8					reserved;	
} DrawStateType;


#define DrawStateStackSize 5		// enough for a control in a field in a window




//-----------------------------------------------
// The Window Structures.
//-----------------------------------------------

#ifndef PUBLIC_STUFF_STRIPPED
// DOLATER еее╩-- need to add #defines for each bit for endian portableness
#endif // PUBLIC_STUFF_STRIPPED
typedef union FrameBitsType {
	struct {
		UInt16 cornerDiam		: 8;				// corner diameter, max 38
		UInt16 reserved_3		: 3; 
		UInt16 threeD			: 1;				// Draw 3D button    
		UInt16 shadowWidth	: 2;				// Width of shadow
		UInt16 width			: 2;				// Width frame
	} bits;
	UInt16 word;									// IMPORTANT: INITIALIZE word to zero before setting bits!
} FrameBitsType;

typedef UInt16 FrameType;

//  Standard Frame Types
#define noFrame         0
#define simpleFrame     1
#define rectangleFrame  1
#define simple3DFrame   0x0012			 // 3d, frame = 2
#define roundFrame      0x0401          // corner = 7, frame = 1
#define boldRoundFrame  0x0702          // corner = 7, frame = 2
#define popupFrame      0x0205          // corner = 2,  frame = 1, shadow = 1
#define dialogFrame     0x0302          // corner = 3,  frame = 2
#define menuFrame       popupFrame


#define winDefaultDepthFlag		0xFF

#ifndef PUBLIC_STUFF_STRIPPED
// DOLATER еее╩-- need to add #defines for each bit for endian portableness
#endif // PUBLIC_STUFF_STRIPPED
typedef struct WindowFlagsType {
	UInt16 format:1;      // window format:  0=screen mode; 1=generic mode
	UInt16 offscreen:1;   // offscreen flag: 0=onscreen ; 1=offscreen
	UInt16 modal:1;       // modal flag:     0=modeless window; 1=modal window
	UInt16 focusable:1;   // focusable flag: 0=non-focusable; 1=focusable
	UInt16 enabled:1;     // enabled flag:   0=disabled; 1=enabled
	UInt16 visible:1;     // visible flag:   0-invisible; 1=visible
	UInt16 dialog:1;      // dialog flag:    0=non-dialog; 1=dialog
	UInt16 freeBitmap:1;	 // free bitmap w/window: 0=don't free, 1=free
	UInt16 reserved :8;
} WindowFlagsType;

typedef struct WindowType {
  Coord							displayWidthV20;		// use WinGetDisplayExtent instead
  Coord							displayHeightV20;		// use WinGetDisplayExtent instead
  void *							displayAddrV20;		// use the drawing functions instead
  WindowFlagsType				windowFlags;
  RectangleType				windowBounds;
  AbsRectType					clippingBounds;
  BitmapPtr						bitmapP;
  FrameBitsType   			frameType;
  DrawStateType *				drawStateP;				// was GraphicStatePtr
  struct WindowType *		nextWindow;
} WindowType;

typedef WindowType *WinPtr;
typedef WindowType *WinHandle;


//-----------------------------------------------
//  More graphics shapes
//-----------------------------------------------
typedef struct WinLineType {
	Coord x1;
	Coord y1;
	Coord x2;
	Coord y2;
} WinLineType;

// Rectangles, Points defined in Rect.h



//-----------------------------------------------
//  Low Memory Globals
//-----------------------------------------------

// This is the structure of a low memory global reserved for the Window Manager
// In GRAPHIC_VERSION_2, it held a single drawing state.  In this version, it
// holds stack information for structures that are allocated from the dynamic heap
typedef struct GraphicStateType {
	DrawStateType *	drawStateP;
	DrawStateType *	drawStateStackP;
	Int16					drawStateIndex;
	UInt16				screenLockCount;
} GraphicStateType;

// ----------------------
// Window manager errors
// ----------------------
#define	winErrPalette							(winErrorClass | 1)



//-----------------------------------------------
//  Macros
//-----------------------------------------------

// For now, the window handle is a pointer to a window structure,
// this however may change, so use the following macros. 

#define WinGetWindowPointer(winHandle) ((WindowType *) winHandle)

#define WinGetWindowHandle(winPtr) ((WinHandle) winPtr)

#if ERROR_CHECK_LEVEL == ERROR_CHECK_FULL
#define ECWinValidateHandle(winHandle) WinValidateHandle(winHandle)
#else
#define ECWinValidateHandle(winHandle) 
#endif

	
	
	
#ifdef REMOVE_FOR_EMULATOR
#ifdef __cplusplus
extern "C" {
#endif

//-----------------------------------------------
// Routines relating to windows management       
//-----------------------------------------------

extern Boolean WinValidateHandle (WinHandle winHandle)
							SYS_TRAP(sysTrapWinValidateHandle);
							
extern WinHandle WinCreateWindow (const RectangleType *bounds, FrameType frame, 
	Boolean modal, Boolean focusable, UInt16 *error)
							SYS_TRAP(sysTrapWinCreateWindow);

extern WinHandle WinCreateOffscreenWindow (Coord width, Coord height, 
	WindowFormatType format, UInt16 *error)
							SYS_TRAP(sysTrapWinCreateOffscreenWindow);

extern WinHandle WinCreateBitmapWindow (BitmapType *bitmapP, UInt16 *error)
							SYS_TRAP(sysTrapWinCreateBitmapWindow);

extern void WinDeleteWindow (WinHandle winHandle, Boolean eraseIt)
							SYS_TRAP(sysTrapWinDeleteWindow);

extern void WinInitializeWindow (WinHandle winHandle)
							SYS_TRAP(sysTrapWinInitializeWindow);

extern void WinAddWindow (WinHandle winHandle)
							SYS_TRAP(sysTrapWinAddWindow);

extern void WinRemoveWindow (WinHandle winHandle)
							SYS_TRAP(sysTrapWinRemoveWindow);

extern void WinMoveWindowAddr (WindowType *oldLocationP, WindowType *newLocationP)
							SYS_TRAP(sysTrapWinMoveWindowAddr);

extern void WinSetActiveWindow (WinHandle winHandle)
							SYS_TRAP(sysTrapWinSetActiveWindow);

extern WinHandle WinSetDrawWindow (WinHandle winHandle)
							SYS_TRAP(sysTrapWinSetDrawWindow);

extern WinHandle WinGetDrawWindow (void)
							SYS_TRAP(sysTrapWinGetDrawWindow);

extern WinHandle WinGetActiveWindow (void)
							SYS_TRAP(sysTrapWinGetActiveWindow);

extern WinHandle WinGetDisplayWindow (void)
							SYS_TRAP(sysTrapWinGetDisplayWindow);

extern WinHandle WinGetFirstWindow (void)
							SYS_TRAP(sysTrapWinGetFirstWindow);

extern void WinEnableWindow (WinHandle winHandle)
							SYS_TRAP(sysTrapWinEnableWindow);

extern void WinDisableWindow (WinHandle winHandle)
							SYS_TRAP(sysTrapWinDisableWindow);

extern void WinGetWindowFrameRect (WinHandle winHandle, RectangleType *r)
							SYS_TRAP(sysTrapWinGetWindowFrameRect);

extern void WinDrawWindowFrame (void)
							SYS_TRAP(sysTrapWinDrawWindowFrame);

extern void WinEraseWindow (void)
							SYS_TRAP(sysTrapWinEraseWindow);

extern WinHandle WinSaveBits (const RectangleType *source, UInt16 *error)
							SYS_TRAP(sysTrapWinSaveBits);

extern void WinRestoreBits (WinHandle winHandle, Coord destX, Coord destY)
							SYS_TRAP(sysTrapWinRestoreBits);

extern void WinCopyRectangle (WinHandle srcWin, WinHandle dstWin, 
	const RectangleType *srcRect, Coord destX, Coord destY, WinDrawOperation mode)
							SYS_TRAP(sysTrapWinCopyRectangle);

extern void WinScrollRectangle (const RectangleType *rP, WinDirectionType direction,
	Coord distance, RectangleType *vacatedP)
							SYS_TRAP(sysTrapWinScrollRectangle);

extern void WinGetDisplayExtent (Coord *extentX, Coord *extentY)
							SYS_TRAP(sysTrapWinGetDisplayExtent);

extern void WinGetWindowBounds (RectangleType *rP)
							SYS_TRAP(sysTrapWinGetWindowBounds);

extern void WinSetWindowBounds (WinHandle winHandle, const RectangleType *rP)
							SYS_TRAP(sysTrapWinSetWindowBounds);

extern void WinGetWindowExtent (Coord *extentX, Coord *extentY)
							SYS_TRAP(sysTrapWinGetWindowExtent);

extern void WinDisplayToWindowPt (Coord *extentX, Coord *extentY)
							SYS_TRAP(sysTrapWinDisplayToWindowPt);

extern void WinWindowToDisplayPt (Coord *extentX, Coord *extentY)
							SYS_TRAP(sysTrapWinWindowToDisplayPt);

extern BitmapType *WinGetBitmap (WinHandle winHandle)
							SYS_TRAP(sysTrapWinGetBitmap);

extern void WinGetClip (RectangleType *rP)
							SYS_TRAP(sysTrapWinGetClip);

extern void WinSetClip (const RectangleType *rP)
							SYS_TRAP(sysTrapWinSetClip);

extern void WinResetClip (void)
							SYS_TRAP(sysTrapWinResetClip);

extern void WinClipRectangle (RectangleType *rP)
							SYS_TRAP(sysTrapWinClipRectangle);

extern Boolean WinModal (WinHandle winHandle)
							SYS_TRAP(sysTrapWinModal);

//-----------------------------------------------
// Routines to draw shapes or frames shapes      
//-----------------------------------------------

// Pixel(s)
extern IndexedColorType WinGetPixel (Coord x, Coord y)
							SYS_TRAP(sysTrapWinGetPixel);

extern void WinPaintPixel (Coord x, Coord y)				// uses drawing mode
							SYS_TRAP(sysTrapWinPaintPixel);

extern void WinDrawPixel (Coord x, Coord y)
							SYS_TRAP(sysTrapWinDrawPixel);

extern void WinErasePixel (Coord x, Coord y)
							SYS_TRAP(sysTrapWinErasePixel);

extern void WinInvertPixel (Coord x, Coord y)
							SYS_TRAP(sysTrapWinInvertPixel);

extern void WinPaintPixels (UInt16 numPoints, PointType pts[])
							SYS_TRAP(sysTrapWinPaintPixels);

// Line(s)
extern void WinPaintLines (UInt16 numLines, WinLineType lines[])
							SYS_TRAP(sysTrapWinPaintLines);

extern void WinPaintLine (Coord x1, Coord y1, Coord x2, Coord y2)
							SYS_TRAP(sysTrapWinPaintLine);

extern void WinDrawLine (Coord x1, Coord y1, Coord x2, Coord y2)
							SYS_TRAP(sysTrapWinDrawLine);

extern void WinDrawGrayLine (Coord x1, Coord y1, Coord x2, Coord y2)
							SYS_TRAP(sysTrapWinDrawGrayLine);

extern void WinEraseLine (Coord x1, Coord y1, Coord x2, Coord y2)
							SYS_TRAP(sysTrapWinEraseLine);

extern void WinInvertLine (Coord x1, Coord y1, Coord x2, Coord y2)
							SYS_TRAP(sysTrapWinInvertLine);

extern void WinFillLine (Coord x1, Coord y1, Coord x2, Coord y2)
							SYS_TRAP(sysTrapWinFillLine);


// Rectangle
extern void WinPaintRectangle (const RectangleType *rP, UInt16 cornerDiam)
							SYS_TRAP(sysTrapWinPaintRectangle);

extern void WinDrawRectangle (const RectangleType *rP, UInt16 cornerDiam)
							SYS_TRAP(sysTrapWinDrawRectangle);

extern void WinEraseRectangle (const RectangleType *rP, UInt16 cornerDiam)
							SYS_TRAP(sysTrapWinEraseRectangle);

extern void WinInvertRectangle (const RectangleType *rP, UInt16 cornerDiam)
							SYS_TRAP(sysTrapWinInvertRectangle);

extern void WinFillRectangle (const RectangleType *rP, UInt16 cornerDiam)
							SYS_TRAP(sysTrapWinFillRectangle);

// Rectangle frames
extern void WinPaintRectangleFrame (FrameType frame, const RectangleType *rP)
							SYS_TRAP(sysTrapWinPaintRectangleFrame);

extern void WinDrawRectangleFrame (FrameType frame, const RectangleType *rP)
							SYS_TRAP(sysTrapWinDrawRectangleFrame);

extern void WinDrawGrayRectangleFrame (FrameType frame, const RectangleType *rP)
							SYS_TRAP(sysTrapWinDrawGrayRectangleFrame);

extern void WinEraseRectangleFrame (FrameType frame, const RectangleType *rP)
							SYS_TRAP(sysTrapWinEraseRectangleFrame);

extern void WinInvertRectangleFrame (FrameType frame, const RectangleType *rP)
							SYS_TRAP(sysTrapWinInvertRectangleFrame);

extern void WinGetFramesRectangle (FrameType  frame, const RectangleType *rP, 
	RectangleType *obscuredRect)
							SYS_TRAP(sysTrapWinGetFramesRectangle);


// Bitmap            
extern void WinDrawBitmap (BitmapPtr bitmapP, Coord x, Coord y)
							SYS_TRAP(sysTrapWinDrawBitmap);

extern void WinPaintBitmap (BitmapType *bitmapP, Coord x, Coord y)
							SYS_TRAP(sysTrapWinPaintBitmap);


// Characters 
extern void WinDrawChar (WChar theChar, Coord x, Coord y)
							SYS_TRAP(sysTrapWinDrawChar);

extern void WinDrawChars (const Char *chars, Int16 len, Coord x, Coord y)
							SYS_TRAP(sysTrapWinDrawChars);

extern void WinPaintChar (WChar theChar, Coord x, Coord y)
							SYS_TRAP(sysTrapWinPaintChar);

extern void WinPaintChars (const Char *chars, Int16 len, Coord x, Coord y)
							SYS_TRAP(sysTrapWinPaintChars);

extern void WinDrawInvertedChars (const Char *chars, Int16 len, Coord x, Coord y)
							SYS_TRAP(sysTrapWinDrawInvertedChars);

extern void WinDrawTruncChars(const Char *chars, Int16 len, Coord x, Coord y, Coord maxWidth)
							SYS_TRAP(sysTrapWinDrawTruncChars);

extern void WinEraseChars (const Char *chars, Int16 len, Coord x, Coord y)
							SYS_TRAP(sysTrapWinEraseChars);

extern void WinInvertChars (const Char *chars, Int16 len, Coord x, Coord y)
							SYS_TRAP(sysTrapWinInvertChars);

extern UnderlineModeType WinSetUnderlineMode (UnderlineModeType mode)
							SYS_TRAP(sysTrapWinSetUnderlineMode);



//-----------------------------------------------
// Routines for patterns and colors                 
//-----------------------------------------------

extern void WinPushDrawState (void)	// "save" fore, back, text color, pattern, underline mode, font
							SYS_TRAP(sysTrapWinPushDrawState);

extern void WinPopDrawState (void)		// "restore" saved drawing variables
							SYS_TRAP(sysTrapWinPopDrawState);


extern WinDrawOperation WinSetDrawMode (WinDrawOperation newMode)
							SYS_TRAP(sysTrapWinSetDrawMode);


extern IndexedColorType WinSetForeColor (IndexedColorType foreColor)
							SYS_TRAP(sysTrapWinSetForeColor);

extern IndexedColorType WinSetBackColor (IndexedColorType backColor)
							SYS_TRAP(sysTrapWinSetBackColor);

extern IndexedColorType WinSetTextColor (IndexedColorType textColor)
							SYS_TRAP(sysTrapWinSetTextColor);

// "obsolete" color call, supported for backwards compatibility
extern void WinSetColors(const RGBColorType *newForeColorP, RGBColorType *oldForeColorP,
	const RGBColorType *newBackColorP, RGBColorType *oldBackColorP)
							SYS_TRAP(sysTrapWinSetColors);

extern void WinGetPattern (CustomPatternType *patternP)
							SYS_TRAP(sysTrapWinGetPattern);

extern PatternType WinGetPatternType (void)
							SYS_TRAP(sysTrapWinGetPatternType);

extern void WinSetPattern (const CustomPatternType *patternP)
							SYS_TRAP(sysTrapWinSetPattern);

extern void WinSetPatternType (PatternType newPattern)
							SYS_TRAP(sysTrapWinSetPatternType);

extern Err WinPalette(UInt8 operation, Int16 startIndex, 
			 	  			 UInt16 paletteEntries, RGBColorType *tableP)
							SYS_TRAP(sysTrapWinPalette);

extern IndexedColorType WinRGBToIndex(const RGBColorType *rgbP)
							SYS_TRAP(sysTrapWinRGBToIndex);

extern void WinIndexToRGB(IndexedColorType i, RGBColorType *rgbP)
							SYS_TRAP(sysTrapWinIndexToRGB);



//-----------------------------------------------
// New WinScreen functions            
//-----------------------------------------------

extern void WinScreenInit(void) 
							SYS_TRAP(sysTrapWinScreenInit);

extern Err WinScreenMode(WinScreenModeOperation operation, 
						UInt32 *widthP,
						UInt32 *heightP, 
						UInt32 *depthP, 
						Boolean *enableColorP)
							SYS_TRAP(sysTrapWinScreenMode);


//-----------------------------------------------
// Screen tracking (double buffering) support            
//-----------------------------------------------
extern UInt8 *WinScreenLock(WinLockInitType initMode)
							SYS_TRAP(sysTrapWinScreenLock);
							
extern void WinScreenUnlock(void)
							SYS_TRAP(sysTrapWinScreenUnlock);

#ifdef __cplusplus 
}
#endif
#endif


#endif //__WINDOW_H__
