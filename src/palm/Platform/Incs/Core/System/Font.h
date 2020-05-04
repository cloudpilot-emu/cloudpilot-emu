/******************************************************************************
 *
 * Copyright (c) 1994-1999 Palm Computing, Inc. or its subsidiaries.
 * All rights reserved.
 *
 * File: Font.h
 *
 * Description:
 *	  This file defines font structures and routines.
 *
 * History:
 *		September 13, 1994	Created by Art Lamb
 *		05/05/98	art	Add structures for font mapping table.
 *		07/03/98	kwk	Added FntWidthToOffset.
 *		10/23/98	kwk	Changed fontMapTable to 0xC000 (was 0xFFFF).
 *		10/20/99	kwk	Moved private values to FontPrv.h
 *
 *****************************************************************************/

#ifndef __FONT_H__
#define __FONT_H__

#include <PalmTypes.h>
#include <CoreTraps.h>

// Pixel width of tab stops in fields
#define  fntTabChrWidth      20

// Width of character missing from font.
#define	fntMissingChar		  -1

typedef struct {
	Int8		offset;
	Int8		width;
} FontCharInfoType, * FontCharInfoPtr;

typedef struct {
	Int16 fontType; 			// font type
	Int16 firstChar; 			// ASCII code of first character
	Int16 lastChar; 			// ASCII code of last character
	Int16 maxWidth; 			// maximum character width
	Int16 kernMax; 			// negative of maximum character kern
	Int16 nDescent; 			// negative of descent
	Int16 fRectWidth; 		// width of font rectangle
	Int16 fRectHeight; 		// height of font rectangle
	Int16 owTLoc; 				// offset to offset/width table
	Int16 ascent; 				// ascent
	Int16 descent; 			// descent
	Int16 leading; 			// leading
	Int16 rowWords; 			// row width of bit image / 2
} FontType;

typedef FontType *FontPtr;

typedef FontPtr *FontTablePtr;

enum fontID { 
	stdFont = 0x00,					// Small font used for the user's writing.  Shows a good amount
	boldFont, 							// Small font.  Bold for easier reading.  Used often for ui.
	largeFont,							// Larger font for easier reading.  Shows a lot less.
	symbolFont,							// Various ui images like check boxes and arrows
	symbol11Font, 						// Larger various ui images
	symbol7Font,						// Smaller various ui images
	ledFont,								// Calculator specific font
	largeBoldFont,						// A thicker version of the large font.  More readable.
	fntAppFontCustomBase = 0x80	// First available application-defined font ID
};

typedef enum fontID FontID;

#define checkboxFont symbol11Font

#define FntIsAppDefined(fnt) (fnt >= fntAppFontCustomBase)



//--------------------------------------------------------------------
//
// Font Function
//
//--------------------------------------------------------------------

#ifdef REMOVE_FOR_EMULATOR
#ifdef __cplusplus
extern "C" {
#endif

FontID FntGetFont (void)
							SYS_TRAP(sysTrapFntGetFont);

FontID FntSetFont (FontID font)
							SYS_TRAP(sysTrapFntSetFont);

FontPtr FntGetFontPtr (void)
							SYS_TRAP(sysTrapFntGetFontPtr);

Int16 FntBaseLine (void)
							SYS_TRAP(sysTrapFntBaseLine);

Int16 FntCharHeight (void)
							SYS_TRAP(sysTrapFntCharHeight);

Int16 FntLineHeight (void)
							SYS_TRAP(sysTrapFntLineHeight);

Int16 FntAverageCharWidth (void)
							SYS_TRAP(sysTrapFntAverageCharWidth);

Int16 FntCharWidth (Char ch)
							SYS_TRAP(sysTrapFntCharWidth);

Int16 FntCharsWidth (Char const *chars, Int16 len)
							SYS_TRAP(sysTrapFntCharsWidth);

Int16 FntWidthToOffset (Char const *pChars, UInt16 length,
	Int16 pixelWidth, Boolean *leadingEdge, Int16 *truncWidth)
							SYS_TRAP(sysTrapFntWidthToOffset);

void FntCharsInWidth (Char const *string, 
	Int16 *stringWidthP, Int16 *stringLengthP, 
	Boolean *fitWithinWidth)
							SYS_TRAP(sysTrapFntCharsInWidth);

Int16 FntDescenderHeight (void)
							SYS_TRAP(sysTrapFntDescenderHeight);

Int16 FntLineWidth (Char const *pChars, UInt16 length)
							SYS_TRAP(sysTrapFntLineWidth);

UInt16 FntWordWrap (Char const *chars, UInt16 maxWidth)
							SYS_TRAP(sysTrapFntWordWrap);

void FntWordWrapReverseNLines (Char const *const chars, 
	UInt16 maxWidth, UInt16 *linesToScrollP, UInt16 *scrollPosP)
							SYS_TRAP(sysTrapFntWordWrapReverseNLines);

void FntGetScrollValues (Char const *chars, UInt16 width, 
	UInt16 scrollPos, UInt16 *linesP, UInt16 *topLine)
							SYS_TRAP(sysTrapFntGetScrollValues);

Err FntDefineFont (FontID font, FontPtr fontP)
							SYS_TRAP(sysTrapFntDefineFont);

#ifdef __cplusplus
}
#endif
#endif


#endif // __FONT_H__
