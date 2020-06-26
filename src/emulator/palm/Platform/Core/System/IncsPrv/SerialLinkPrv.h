/******************************************************************************
 *
 * Copyright (c) 1994-1999 Palm Computing, Inc. or its subsidiaries.
 * All rights reserved.
 *
 * File: SerialLinkPrv.h
 *
 * Description:
 *		Private Serial Link includes
 *
 * History:
 *   	2/6/95 replaces DSerial.h from Debugger 
 *
 *****************************************************************************/

#ifdef	NON_PORTABLE

#ifndef __SERIALLINKPRV_H
#define __SERIALLINKPRV_H

#include <SerialLinkMgr.h>


//*************************************************************************
//  Misc Constants
//*************************************************************************
// Default timeout  value once into receiving a packet in ticks
#define		slkDefaultInPktTimeout		(sysTicksPerSecond*2)




/*******************************************************************
 * Serial Link Globals
 *
 *******************************************************************/
#define	slkMaxSockets		6						// max # of sockets
#define	slkMaxRefNums		slkMaxSockets		// max # of refNums in use at any one time
#define	slkAnySocket		0xFF		

#define	slkEmptyRefNum		0xFF					// used to track empty refInfos
#define	slkSendBufferSize	252

// We keep an array of these (slkMaxSockets in size) to track which
//  librarie's we've received packet headers for. Since we can have up
//  to slkMaxSockets unique refNums in use at any one time, we must make
//  the array at least that size.
typedef struct SlkRefInfoType {
	UInt16	refNum;									// refNum of library, or 0xFF if empty
	Int8		destID;									// dest socketID, or -1 if none
	UInt8 		padding;
	} SlkRefInfoType;
typedef SlkRefInfoType *SlkRefInfoPtr;	

typedef struct SlkGlobalsType {
	UInt16					openCount;
	
	UInt16					socketRefNum[slkMaxSockets];
	Boolean					socketOpen[slkMaxSockets];
	SlkSocketListenPtr	socketListenP[slkMaxSockets];
	UInt16					socketTimeout[slkMaxSockets];		// inPacket timeout
	
	
	// We can keep track of packets coming in for up to slkMaxRefNums libraries.
	//  This array holds the destination socket ID for packets received from
	//  each library.
	SlkRefInfoType			refInfo[slkMaxRefNums];
	
	UInt8						transId;					// next transaction ID
	UInt8 					padding;
	UInt32					xmitSemaphoreID;		// Semaphore ID for transmitting
	UInt16					numReceived;			// # of packets received since serial link
															// was opened.
	// New fields to buffer sends to prevent
	// the header, body and checksum from being
	// sent out as three distinct send requests.
	Boolean					socketWantsBuffering[slkMaxSockets];

	UInt8						*sendBufferP;				// Send buffer
	UInt16					sendBufferSize;			// Size of the allocated send buffer
	
	} SlkGlobalsType;

typedef SlkGlobalsType*	SlkGlobalsPtr;


	
#endif	//__SERIALLINKPRV_H

#endif 	//NON_PORTABLE
