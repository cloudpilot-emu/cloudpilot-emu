/******************************************************************************
 *
 * Copyright (c) 1994-1999 Palm Computing, Inc. or its subsidiaries.
 * All rights reserved.
 *
 * File: SysEvtPrv.h
 *
 * Description:
 *		Private Header for the System Event Manager
 *
 * History:
 *   	08/10/95	RM		Created by Ron Marianetti
 *		07/14/99	kwk	Added silkscreen area info structures.
 *		09/13/99	kwk	Moved evtKeyStringEscape to SysEvtMgr.c, since
 *							that's the only file using it.
 *		11/12/99	kwk	Moved EvtGetPenBtnList and EvtGetSilkscreenAreaList
 *							to SysEvtMgr.h, since they're now public, as well
 *							as associated data structures.
 *
 *****************************************************************************/

#ifdef	NON_PORTABLE
#ifndef __SYSEVTPRV_H__
#define __SYSEVTPRV_H__

#include <SysEvtMgr.h>	// SilkscreenAreaType & PenBtnListType


/************************************************************
 * Structure of a Pen Capture Queue
 *
 * This structure is used to hold one or more pen strokes
 *   and is filled by the digitizer interrupt routine and emptied
 *   by application level code.
 *
 * A default size queue is allocated by the system and used to capture
 *   strokes for subsequent processing by the Graffiti recognizer
 *   or the hard icon recognizer. This queue pointer is stored in
 *   the System Event Manager Globals.
 *
 * Optionally, applications can replace the system pen queue with
 *   one of their own if it they need to hold larger strokes
 *   or more of them.
 *   
 *
 * Stroke data is stored in compressed form in this queue - refer
 *  to the comments on the routine EvtEnqueuPenPoint  for the format
 *  of the queue data.
 *************************************************************/
typedef struct PenQueueType {
	Int16			start;							// start byte offset
	Int16			end;								// end byte offset
	Int16			size;								// size of queue in bytes
	
	UInt16		strokeCount;					// # of complete strokes currently in pen queue
	Boolean		returnedPenDown;				// set after we return a pen-down event for 
														// the next stroke in the queue. Cleared after
														// we return the pen-up for the stroke.

	UInt8			reserved;
	
	PointType	addLast;							// last point captured
	Int16			addStrokeStart;				// offset in queue to start of stroke
														// currently being added to queue
														// -1  : no stroke being captured
	
	
	UInt16		strokesRemoved;				// running count: # of strokes removed from queue.
	UInt16		strokesAdded;					// running count: # of strokes added to queue.

	Int16			rmvStrokeStage;				// -1  : not started
														//  0  : returned stroke info
														//  1  : returned start point
														
	PointType	rmvLast;							// last point dequeued
	PointType 	rmvStartPt;						// copy of start point of stroke currently
														//  being removed.
	
	UInt8			data[2];							// 'size' bytes of data
	} PenQueueType;
typedef PenQueueType*	PenQueuePtr;



/************************************************************
 * Structure of the Key Queue
 *
 * This structure is used to hold one or more keystrokes 
 *   and is filled by numerous places including the low level
 *	  hardware key interrupt routine, the Graffiti recognizer, and
 *	  the digitizer buttons recognizer.
 *
 * A default size queue is allocated by the system. 
 *		This queue pointer is stored in the System Event Manager globals.
 *
 * Optionally, applications can replace the system key queue with
 *   one of their own if it they need to hold more keys in the queue
 *	  at one time. This may be necessary when processing large macro
 *	  strings from Graffiti.
 *
 * Key data is stored in compressed form in this queue. Refer to
 *  the comments for the function EvtEnqueueKeyString for the format
 *  of the compressed data.
 *   
 *************************************************************/
typedef struct KeyQueueType {
	UInt16	start;							// start byte offset
	UInt16	end;								// end byte offset
	UInt16	size;								// size of queue in bytes
	
	UInt8		data[2];							// 'size' bytes of data
	} KeyQueueType;
typedef KeyQueueType*	KeyQueuePtr;


/************************************************************
 * Structure of the silk resource in system overlay DBs. Note,
 * this structure must mirror the structure of the sysResTSilkscreen
 * resource as defined in PalmTypes.r.
 *************************************************************/

// Version of the SilkscreenInfoType structure.
#define	sysSilkscreenVers				1

// Structure of the silkscreen resource (sysResTSilkscreen)
typedef struct SilkscreenInfoType {
	UInt16	version;						// version of structure.
	
	// The next three fields aren't used to locate the silkscreen,
	// but help identify different silkscreen layouts.
	UInt32	vendorCreator;				// creator assigned to vendor
												// (psys for 3Com)
	UInt16	localeLanguage;			// language for locale
	UInt16	localeCountry;				// country for locale.
	
	UInt16	numAreas;					// Count of number of silkscreen areas.
	SilkscreenAreaType areas[1];		// Placeholder for one or more areas.
	
	PenBtnListType buttonList;			// Count + list of buttons.
	} SilkscreenInfoType;


/************************************************************
 * Key constants
 *************************************************************/

#define	evtDefaultPenQSize	0x100
#define	evtDefaultKeyQSize	0x40

#define	evtEffectiveKeyQStorage	(evtDefaultKeyQSize - sizeof(KeyQueueType))


// gremlinsFlags
#define	grmGremlinsOn			0x01
#define  grmEventTraceOn		0x02
#define	grmGremlinsIdle		0x04

/************************************************************
 * Structure of System Event Manager Globals
 *************************************************************/
typedef struct SysEvtMgrGlobalsType {
	Boolean			sendNullEvent;				// set by EvtWakeup 		
	Boolean			enableGraffiti;			// If true, enable Graffiti translation
	Boolean			enableSoftKeys;			// If true, enable digitizer button translation 
												
												
	Boolean			removeTopStroke;			// if true, remove top stroke from queue
														//  next time EvtGetSysEvent is called;
	PenQueuePtr		penQP;						// pointer to current pen queue
	UInt16			penQStrokesRemoved;		// saved value of penQP->strokesRemoved. Used to
														// determine if app removed stroke from penUp
														// event or not.
														
	KeyQueuePtr		keyQP;						// pointer to current key queue
	
	RectangleType	writingR;					// enclosing rect for writing area
	UInt16			appAreaBottom;				// bottom coordinate of application area.
	
	Int16				penX;							// current pen location and state
	Int16				penY;				
	Boolean			penDown;
	Boolean			displayingBatteryAlert; // to avoid re-entering battery dialog
	
	Int16				lastPenX;					// last pen location and state
	Int16				lastPenY;
	Boolean			lastPenDown;
	
	// This boolean is set by the Remote Screen Driver timer procedure periodically
	//  to force the event manager to call ScrSendUpdateArea which will send the update
	//  area of the screen to the host.
	Boolean			needRemoteScrUpdate;		// If true, call ScrSendUpdateArea 
	
	
	UInt8				gremlinsFlags;				// flag to indicate if gremlins is running and
														// if we'er doing an event trace.

	Boolean			idle;							// DOLATER ron - Who put this here?? What is it for???
	
	// The following flag is used to better support background wakeup jobs. 
	// See the comments on the new EvtGotUserEvent() for details
	Boolean			gotUserEvent;				// Cleared when going to sleep, set as soon as
														//  we get a user event (key press, app alarm, etc).
															
	UInt8				reserved;
	} SysEvtMgrGlobalsType;
	
typedef SysEvtMgrGlobalsType* SysEvtMgrGlobalsPtr;



/************************************************************
 * System Event Manager procedures
 *************************************************************/
#ifdef __cplusplus
extern "C" {  
#endif



#ifdef __cplusplus
}
#endif


#endif //__SYSEVTPRV_H__

#endif // NON_PORTABLE
