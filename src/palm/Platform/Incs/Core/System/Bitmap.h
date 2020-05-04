/******************************************************************************
 *
 * Copyright (c) 1994-1999 Palm Computing, Inc. or its subsidiaries.
 * All rights reserved.
 *
 * File: Bitmap.h
 *
 * Description:
 *        This file defines bitmap structures and routines.
 *
 * History:
 *		September, 1999	Created by Bertrand Simon
 *			Name	Date		Description
 *			----	----		-----------
 *			BS		9/99		Create
 *			jmp	12/23/99	Fix <> vs. "" problem.
 *
 *****************************************************************************/

#ifndef __BITMAP_H__
#define __BITMAP_H__

#include <PalmTypes.h>
#include <CoreTraps.h>

#include <PalmOptErrorCheckLevel.h>	// #define ERROR_CHECK_LEVEL


//-----------------------------------------------
// The Bitmap Structure.
//-----------------------------------------------

// bitmap version numbers
#define BitmapVersionZero	0
#define BitmapVersionOne	1
#define BitmapVersionTwo	2

// Compression Types for BitMap BitmapVersionTwo.
typedef enum {
	BitmapCompressionTypeScanLine = 0,
	BitmapCompressionTypeRLE,
	
	BitmapCompressionTypeNone = 0xFF
} BitmapCompressionType;

#ifndef PUBLIC_STUFF_STRIPPED
// DOLATER еее╩-- need to add #defines for each bit for endian portableness
#endif // PUBLIC_STUFF_STRIPPED
typedef struct BitmapFlagsType {
	UInt16 compressed:1;  			// Data format:  0=raw; 1=compressed
	UInt16 hasColorTable:1;			// if true, color table stored before bits[]
	UInt16 hasTransparency:1;		// true if transparency is used
	UInt16 indirect:1;				// true if bits are stored indirectly
	UInt16 forScreen:1;				// system use only
	UInt16 reserved:11;
} BitmapFlagsType;

// this definition correspond to the 'Tbmp' and 'tAIB' resource types
typedef struct BitmapType {
	Int16  				width;
	Int16  				height;
	UInt16  				rowBytes;
	BitmapFlagsType	flags;
	UInt8					pixelSize;			// bits/pixel
	UInt8					version;				// version of bitmap. This is vers 2
	UInt16	 			nextDepthOffset;	// # of DWords to next BitmapType
													//  from beginnning of this one
	UInt8					transparentIndex;	// v2 only, if flags.hasTransparency is true,
													// index number of transparent color
	UInt8					compressionType;	// v2 only, if flags.compressed is true, this is
													// the type, see BitmapCompressionType
														
	UInt16	 			reserved;			// for future use, must be zero!
	
	// [colorTableType] pixels | pixels*
	                                    // If hasColorTable != 0, we have:
	                                    //   ColorTableType followed by pixels. 
													// If hasColorTable == 0:
	                                    //   this is the start of the pixels
	                                    // if indirect != 0 bits are stored indirectly.
	                                    //	  the address of bits is stored here
	                                    //   In some cases the ColorTableType will
	                                    //   have 0 entries and be 2 bytes long.
} BitmapType;

typedef BitmapType *BitmapPtr;


// This is the structure of a color table. It maps pixel values into
//  RGB colors. Each element in the table corresponds to the next
//  index, starting at 0.

typedef struct RGBColorType {
	UInt8	index;									// index of color or best match to cur CLUT or unused.
	UInt8	r;											// amount of red, 0->255
	UInt8	g;											// amount of green, 0->255
	UInt8	b;											// amount of blue, 0->255
} RGBColorType;


typedef struct ColorTableType {
														// high bits (numEntries > 256) reserved
	UInt16						numEntries;		// number of entries in table
	// RGBColorType			entry[];			// array 0..numEntries-1 of colors
														// starts immediately after numEntries
} ColorTableType;


// get start of color table entries aray given pointer to ColorTableType
#define ColorTableEntries(ctP)	((RGBColorType *)((ColorTableType *)(ctP)+1))

#ifdef REMOVE_FOR_EMULATOR
#ifdef __cplusplus
extern "C" {
#endif

//-----------------------------------------------
// Routines relating to bitmap management       
//-----------------------------------------------
							
extern BitmapType *BmpCreate (Coord width, Coord height, UInt8 depth, 
	ColorTableType *colortableP, UInt16 *error)
							SYS_TRAP(sysTrapBmpCreate);

extern Err BmpDelete (BitmapType *bitmapP)
							SYS_TRAP(sysTrapBmpDelete);

extern Err BmpCompress(BitmapType *bitmapP, BitmapCompressionType compType )
							SYS_TRAP(sysTrapBmpCompress);

extern void *BmpGetBits(BitmapType *bitmapP)
							SYS_TRAP(sysTrapBmpGetBits);

extern ColorTableType *BmpGetColortable(BitmapType *bitmapP)
							SYS_TRAP(sysTrapBmpGetColortable);

extern UInt16 BmpSize(BitmapType *bitmapP)
							SYS_TRAP(sysTrapBmpSize);

extern UInt16 BmpBitsSize(BitmapType *bitmapP)
							SYS_TRAP(sysTrapBmpBitsSize);

extern UInt16 BmpColortableSize(BitmapType *bitmapP)
							SYS_TRAP(sysTrapBmpColortableSize);

#ifdef __cplusplus 
}
#endif
#endif

#endif //__BITMAP_H__
