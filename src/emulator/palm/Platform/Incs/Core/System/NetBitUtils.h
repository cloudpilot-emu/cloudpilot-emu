/******************************************************************************
 *
 * Copyright (c) 1997-1999 Palm Computing, Inc. or its subsidiaries.
 * All rights reserved.
 *
 * File: NetBitUtils.h
 *
 * Description:
 *	  Header file for bit manipulation routines used primarily
 *	by wireless network protocols.
 *	
 *	  These routines live in the NetLib but are broken out here into
 *	a separate header so that they can be more easily used by source
 *	files that don't need access to the other NetLib functions. 
 *
 * History:
 *		6/11/97	Created by Ron Marianetti
 *	Name	Date		Description
 *	----	----		-----------
 *	vmk		8/25/98		Defined bitVarIntMaxBits and bitVarIntMaxBytes
 *
 *****************************************************************************/

#ifndef	__NETBITUTILS_H__
#define	__NETBITUTILS_H__

#include <PalmTypes.h>
#include <CoreTraps.h>

#include <NetMgr.h>

// # of bits in a byte
#define	bitsInByte			8


// Maximum size of variable-size ints in # of bits and bytes.  This is based on
// the largest variable size int, which is encoded as follows: 1111  Bit[32]
#define bitVarIntMaxBits	36
#define bitVarIntMaxBytes	((bitVarIntMaxBits + bitsInByte - 1) / bitsInByte)


//=====================================================
// BitXXX Macros
//
// The following macros are handy because they don't require that
//  the source code pass in the NetLib library refnum to every
//  call. 
//
// When building server code or other emulation code where th
//   library trap dispatcher is not used, the libRefNUm is unused 
//	 and can be 0.
// 
// When building for the viewer, the libRefNum must be the refNum
//  of the NetLib. For applications, this libRefNum must be put
//  into an application global named 'AppNetRefnum'.
//
//====================================================
#if USE_TRAPS == 0
	#define	netPrvRefnum 0
#else
	#define	netPrvRefnum AppNetRefnum
#endif


#define	BitMove( dstP,  dstBitOffsetP, srcP,  srcBitOffsetP, numBits)	\
	NetLibBitMove(netPrvRefnum, dstP, dstBitOffsetP, srcP, srcBitOffsetP, numBits)

#define	BitPutFixed( dstP,  dstBitOffsetP, value,  numBits)	\
	NetLibBitPutFixed(netPrvRefnum, dstP,  dstBitOffsetP, value,  numBits) 

#define	BitGetFixed(srcP, srcBitOffsetP, numBits)	\
	NetLibBitGetFixed(netPrvRefnum, srcP, srcBitOffsetP, numBits)	

#define	BitPutUIntV(dstP, dstBitOffsetP, value)	\
	NetLibBitPutUIntV(netPrvRefnum, dstP, dstBitOffsetP, value)

#define	BitGetUIntV(srcP, srcBitOffsetP)	\
	NetLibBitGetUIntV(netPrvRefnum, srcP, srcBitOffsetP)

#define	BitPutIntV(dstP, dstBitOffsetP,	value) \
	NetLibBitPutIntV(netPrvRefnum, dstP, dstBitOffsetP, value)

#define	BitGetIntV(srcP, srcBitOffsetP)	\
	NetLibBitGetIntV(netPrvRefnum, srcP, srcBitOffsetP)




//=====================================================
// Functions
//====================================================
#ifdef REMOVE_FOR_EMULATOR
#ifdef __cplusplus
extern "C" {
#endif

//--------------------------------------------------
// Bit Moving functions. For "slim" bit packing protocols
// used over wireless. 
//--------------------------------------------------
void		NetLibBitMove(UInt16 libRefNum, UInt8 *dstP, UInt32 *dstBitOffsetP,
					UInt8 *srcP, UInt32 *srcBitOffsetP,
					UInt32		numBits)
				SYS_TRAP(netLibTrapBitMove);

						
void		NetLibBitPutFixed(UInt16 libRefNum, UInt8 *dstP, UInt32 *dstBitOffsetP,
					UInt32 value, UInt16 numBits)
				SYS_TRAP(netLibTrapBitPutFixed);
						
UInt32 	NetLibBitGetFixed(UInt16 libRefNum, UInt8 *dstP, UInt32 *dstBitOffsetP, 
					UInt16 numBits)
				SYS_TRAP(netLibTrapBitGetFixed);
						
void		NetLibBitPutUIntV(UInt16 libRefNum, UInt8 *dstP, UInt32 *dstBitOffsetP, 
					UInt32 value)
				SYS_TRAP(netLibTrapBitPutUIntV);

UInt32 	NetLibBitGetUIntV(UInt16 libRefNum, UInt8 *dstP, UInt32 *dstBitOffsetP)
				SYS_TRAP(netLibTrapBitGetUIntV);

void		NetLibBitPutIntV(UInt16 libRefNum, UInt8 *dstP, UInt32 *dstBitOffsetP, 
					Int32 value)
				SYS_TRAP(netLibTrapBitPutUIntV);

Int32 	NetLibBitGetIntV(UInt16 libRefNum, UInt8 *dstP, UInt32 *dstBitOffsetP)
				SYS_TRAP(netLibTrapBitGetUIntV);

#ifdef __cplusplus
}
#endif
#endif


#endif	// __NETBITUTILS_H__
