/******************************************************************************
 *
 * Copyright (c) 1994-1999 Palm Computing, Inc. or its subsidiaries.
 * All rights reserved.
 *
 * File: SystemPrv.h
 *
 * Description:
 *		Private Pilot system equates
 *
 * History:
 *		08/09/95	RM		Created by Ron Marianetti   
 *		01/28/98	SCL	Added HotSync port "hint" for SerialHWMgr
 *		02/04/98 srj	Added HW_TARGET_EZ in appropriate places.
 *							HW_TARGET_TD1 in this file means "any
 *							handheld", not non-EZ platforms.
 *		04/05/99	jrb	bumped sysLowMemSize
 *		04/13/99	kwk	Bumped sysLowMemSize to 15B0.
 *		05/21/99	kwk	Bumped sysLowMemSize to 1700.
 *		06/24/99	kwk	Added four locale-related fields to SysNVParamsType.
 *		07/01/99	kwk	Added SysExtPrefsType structure & version/flag defs.
 *		07/13/99	kwk	Bumped sysLowMemSize to 1800.
 *		11/01/99	kwk	Moved SysWantEvent here from SystemMgr.h
 *		12/03/99	SCL	Moved SysAppInfoType, SysAppStartup, and SysAppExit
 *							to SystemMgr.h (for StartupCode/Runtime)
 *		01/21/00 ADH	Bumped sysLowMemSize to 1880
 *		03/30/00 jmp	Bumped sysLowMemSize to 1900 for the device and to 2000
 *							for the Simulator.
 *
 *****************************************************************************/

#ifdef	NON_PORTABLE

#ifndef __SYSTEMPRV_H__
#define __SYSTEMPRV_H__

#include <DataMgr.h>
#include <Event.h>
#include <SystemMgr.h>		// for SysAppInfoType

// Include fixed global type
#include "Globals.h"
#include "SerialMgr.h"		// for serPortLocalHotSync define


/************************************************************
 * Common Equates between Native and Emulation mode
 *************************************************************/
#define	sysCardSignature				0xFEEDBEEFL			// card signature long word
#define  sysStoreSignature				0xFEEDFACEL			// store signature long word


// This should be set to sizeof(LowMemType) for the final version, but
//  until then we'll make it bigger than LowMemType so that we can
//  add more globals without having to re-format memory cards.
//
// Note:  sysLowMemSize needs to be an even multiple of 0x1000 in the Simulator
//			 so that MemStoreSearch() will be able to find the sysStoreSignature.
//
#if EMULATION_LEVEL == EMULATION_NONE
#define	sysLowMemSize					0x1900						// size of Low Memory Area
#else
#define	sysLowMemSize					0x2000						// size of Low Memory Area
#endif
#define	sysInitStack					(sysLowMemSize+0x1000)	// stack ptr during boot



// The location of the Card Header (start of ROM) is plugged in at run
//  time to the global variable GSysCardHeaderOffset. This MUST be done
//  by the startup code. We make this a run-time variable so that the same
//  executable ROM image can be run from ROM or RAM.
// #define sysCardHeaderOffset		someConstant
#define	sysRAMOnlyCardHeaderOffset 0x0000000L			// RAM only card header created here 
#define	sysCardHeaderSize				0x0100				// size of card header


// Offset to ROM storage header from card header
#define	sysROMStoreRelOffset			sysCardHeaderSize
#define	sysStoreHeaderSize			0x0100				// size of storage header


// This equate tells us how large of a heap info table to reserve in the
//  globals for each card
#define	sysMaxHeapsPerCard			128					// max # of heaps per card


// This equtes tells us the initial size of the Library table as held in
//  the Low Memory area.  The table is allocated in the dynamic heap by
//  the first call to SysLibInstall(), in SystemMgr.c.
#define	sysDefaultLibraries			DEFAULT_LIB_ENTRIES	// default size of library table


// The default port number and baud rate for the serial port
//  (send "HotSync" port hint for SerialHWMgr)
#define	sysDefaultSerPort				serPortLocalHotSync	// Use port #0
#define	sysDefaultSerBaud				57600						// baud rate 

/************************************************************
 * Structure of the pref=0 resource in applications. Note, this
 *  structure must mirror the structure of the sysResTAppPrefs
 *	 resource as defined in SystemResourcesPrv.h.
 *************************************************************/
typedef struct SysAppPrefs {
	UInt16	priority;					// task priority
	UInt32	stackSize;					// required stack space
	UInt32	minHeapSpace;				// minimum heap space required
	} SysAppPrefsType;
typedef SysAppPrefsType *SysAppPrefsPtr;


// Extra stack space allocated for each app to 
// account for increased stack usage by the system.
#define sysAppStackSpaceFudgeFactor	0xD0

/************************************************************
 * Structure of the xprf=0 resource in resource DBs. Note, this
 * structure must mirror the structure of the sysResTExtPrefs
 * resource as defined in SystemResourcesPrv.h. Also, fields can only
 *	be added (at the end), never removed or changed.
 *************************************************************/

#define	sysExtPrefsVers				1

// Flags defined for SysExtPrefsType.flags
#define	sysExtPrefsNoOverlayFlag	0x00000001

typedef struct SysExtPrefsType {
	UInt16	version;						// version of structure.
	UInt32	flags;						// 32 boolean flags.
	} SysExtPrefsType;

/************************************************************
 * Structure of the user info in the TCB of a task. This structure
 *	is stored in the xtcbuser fields of the AMX TCB for a task and 
 * must not exceed 4 DWords in length (the size reserved by AMX for
 * user info).
 *************************************************************/
typedef struct SysTCBUserInfoType {
	SysAppInfoPtr	tmpAppInfoP;				// used to pass appInfo to nested launches
	SysAppInfoPtr	rootAppInfoP;				// App info for the root application on this task
	UInt32			initialA5;					// Inital value for A5 globals MemPtr
	} SysTCBUserInfoType;
typedef SysTCBUserInfoType *SysTCBUserInfoPtr;


/************************************************************
 * An alternate use of the user info area in the TCB of a task.
 * This is used to pass info back to the kernel when a task is
 * being deleted.  When the termination proc is called it can
 * fill in the field stackInUseByP field to point to a taskID
 * field.  This field will be cleared by the TaskDelete routine
 * when it is safe to delete the tasks stack.
 *************************************************************/
typedef struct SysExitingTaskInfoType {
	UInt32 *			stackInUseByP;				// points to taskid of task using this stack
	} SysExitingTaskInfoType;
typedef SysExitingTaskInfoType *SysExitingTaskInfoPtr;


//*****************************************************************
// Structure of ParamBlock for the SysKernelInfo call
//*****************************************************************

// Selector codes
typedef enum	 {
	sysKernelInfoSelCurTaskInfo,
	sysKernelInfoSelTaskInfo,
	sysKernelInfoSelSemaphoreInfo,
	sysKernelInfoSelTimerInfo
	} SysKernelInfoSelector;

typedef struct SysKernelInfoType {
//	SysKernelInfoSelector	selector;		// Which info to get
	UInt8	selector;		// Which info to get
	UInt8 						reserved;
	UInt32						id;				// which object to get info on
	
	union {
		struct {
			UInt32	id;							// ID of task
			UInt32	nextID;						// ID of next task
			
			UInt32	tag;							// task tag
			UInt32	status;						// task status
			UInt32	timer;						// # ticks left if task's timeout
			UInt32	timeSlice;					// task's time slice
			Int16		priority;					// task priority
			UInt16	attributes;					// task attributes
			Int16		pendingCalls;				// # of pending calls
			UInt32	senderTaskID;				// task ID of message sender
			UInt32	msgExchangeID;				// task's message exchange ID, if any
			
			UInt32	tcbP;							// pointer to TCB
			UInt32	stackP;						// stack pointer of task
			UInt32	stackStart;					// top of task stack (where it started).
			UInt32	stackSize;					// size of stack
			} task;

		struct {
			UInt32	id;
			UInt32	nextID;						// ID of next semaphore

			UInt32	tag;							// semaphore tag
			Int16		initValue;					// semaphore initial value 
														//  >= 0 for counting, -1 for resource
			Int16		curValue;					// semaphore current value
														//  >0 available
														//   0 not available
														//  -n not available, n tasks waiting
			Int16		nestLevel;					// 0 for counting, >= 0 for resource
			UInt32	ownerID;						// owner ID
														//   0 for counting or resource free
			} semaphore;
			
		struct {
			UInt32	id;
			UInt32	nextID;						// ID of next timer
			
			UInt32	tag;							// timer tag
			UInt32	ticksLeft;					// ticks left till timer runs
			UInt32	period;						// timer period, if periodic
			UInt32	proc;							// timer procedure
			} timer;
		} param;
		
	} SysKernelInfoType;
	
	
typedef SysKernelInfoType*	SysKernelInfoPtr;




//-----------------------------------------------------------------
// Bits for status field in task info parameter block
//------------------------------------------------------------------
#define	sysTaskStatusTriggerWait		0x01		// (CJ_MATSWTR) Task idle (waiting for trigger)
#define	sysTaskStatusSuspended			0x02		// (CJ_MATSWSUS) Task suspended (waiting for resume)
#define	sysTaskStatusWaiting				0x04		// (CJ_MATSWAIT) Task waiting (see other bits)
#define	sysTaskStatusHalted				0x80		// (CJ_MATSWHLT) Task halted

// If sysTaskStatusWaiting set, these bits tell why....
#define	sysTaskStatusWaitSemaphore		0x00100	// (CJ_MATSWSM) waiting on semaphore
#define	sysTaskStatusWaitEvent			0x00200	// (CJ_MATSWEV) waiting on event group
#define	sysTaskStatusWaitMailbox		0x00400	// (CJ_MATSWMB) waiting on mailbox
#define	sysTaskStatusWaitMsgExchange	0x00800	// (CJ_MATSWMX) waiting on message exchange
#define	sysTaskStatusWaitBuffer			0x01000	// (CJ_MATSWBUF) waiting on buffer
#define	sysTaskStatusWaitTimer			0x08000	// (CJ_MATSWTM) waiting on timer

#define	sysTaskStatusWaitMailboxAck	0x00010000L	// (CJ_MATSWMBAK) mailbox ack
#define	sysTaskStatusWaitMsgXAck		0x00020000L	// (CJ_MATSWMXAK) msg exchange ack
#define	sysTaskStatusWaitTaskSig		0x00040000L	// (CJ_MATSWSIG) task signal wait

//-----------------------------------------------------------------
// Bits for attributes field in task info parameter block
//------------------------------------------------------------------
#define	sysTaskAttrNoHalt					0x02		// (CJ_MATAHLT) Task cannot be halted
#define	sysTaskAttrMsgStack				0x08		// (CJ_MATAMSG) receives msgs on stack
#define	sysTaskAttrMsgByRef				0x10		// (CJ_MATAPBR) messages passed by reference
#define	sysTaskAttrMsgOnStack			0x4000	// (CJ_MATAMRDY) message ready on stack
#define	sysTaskAttrMsgSenderWaiting	0x8000	// (CJ_MATAMSW) msg sender waiting for ack



/************************************************************
 * This structure defines a section within the RAM storage header
 *  on Card#0 used to hold non-volatile System information. We store
 *  System information that can not be conveniently stored or accessed
 *  from a Database in this area because:
 *	1.) it can be accessed earlier during the boot-up process and 
 * 2.) It can be accessed from an interrupt routine.
 *************************************************************/ 
typedef struct SysNVParamsType {
	UInt32	rtcHours;									// Real-Time clock hours - add to value
																// in DragonBall RTC register to get
																// actual date & time.
	UInt32	rtcHourMinSecCopy;						// Copy of latest value in rtcHourMinSec reg of 
																// DBall. Used as default RTC value on Reset.
	UInt8		swrLCDContrastValue;						// Contrast Value for LCD on EZ-based products
																// that use the software contrast PWM (such as Sumo)
	UInt8 	swrLCDBrightnessValue;					// Brightness value for screens with adjustable brightness.
	
	// Note that in the ROM store, these next four fields contain the default
	// settings for card 0's RAM store, when it has to be initialized.
	void*		splashScreenPtr;							// MemPtr to splash screen bitmap
	void*		hardResetScreenPtr;						// MemPtr to hard reset screen bitmap.
	UInt16	localeLanguage;							// Language for locale.
	UInt16	localeCountry;								// Country for locale.
	
	// 11/15/99 SCL: New Globals added for Licensees, Partners, OEMs, etc.
	// These storage locations are reserved for the HAL running on a given device.
	UInt32	sysNVOEMStorage1;							// 4 bytes for Device OEM use only!
	UInt32	sysNVOEMStorage2;							// 4 bytes for Device OEM use only!

	} SysNVParamsType;
typedef SysNVParamsType *SysNVParamsPtr;



/************************************************************
 * Structure of the System Globals. This is where we place
 *  variable size globals that the System Manager uses. This
 *  is currently not used but a pointer for it is reserved
 *  in the low memory globals
 *************************************************************/
/*
typedef struct SysGlobalsType {
	} SysGlobalsType;
typedef SysGlobalsType *SysGlobalsPtr;
*/



/************************************************************
 * Assignment of bits to the event group used for implementing the
 *   NetLibSelect() call. This event group is also used by the System
 *   Event Manager (SysEvtMgr.c) and includes a bit for "stdin" 
 *   (pen & keyboard) besides the other bits used for Net Library socket refNums. 
 *************************************************************/
#define	sysFileDescNet	(sysFileDescStdIn+1)		// first available refNum for use by
																//  Net library.


// Macro to generate a bitmask for the event group corresponding to a particular
//  refNum id and boolean indicating whether it's for input or output. The outputs
// are in the upper 16 bits of the event group and the inputs are in the
// lower 16 bits.
#define	sysEvtGroupMask(fd,output)	\
		(output ?  (0x00010000 << (fd)) : (0x00000001 << (fd)) )


/************************************************************
 * This structure defines the entire the Low Memory Area
 *  and includes the low memory header defined in Globals.h,
 *  followed by some variable size globals that may change 
 *  from rev to rev of the System Software.
 *
 * When emulating under Windows, this structure ends up at some random
 * place in memory and the global variable GlobalsP points to it.
 *
 * When running in the native environment, this structure starts at
 * address sysGlobalsOffset. 
 *
 * In order to simplify the source code, macros are defined for
 *  accessing commonly used globals in the optimim manner for
 *  each environment.
 *************************************************************/ 
#include "SerialLinkPrv.h"
#include "MemoryPrv.h"
#include "SystemPkt.h"
#include "DebugPrv.h"
#include "Hardware.h"
typedef struct LowMemType {


	// Fixed size globals, defined in Globals.h
	LowMemHdrType			fixed; 
	

	//---------------------------------------------------------------------------
	// These structures are always placed at the end of the LowMem area so
	// that other globals don't generally have to move when their sizes change
	//---------------------------------------------------------------------------
	CardInfoType			memCardInfo[hwrNumCardSlots];		// Memory Card Info Array
	
	SlkGlobalsType			slkGlobals;								// Serial Link Mgr globals
	
	DbgGlobalsType			dbgGlobals;								// Debugger globals
	
	SysAppInfoType			sysAppInfo;								// SysAppInfoType for system
	
	MemPtr					sysDispatchTable[sysNumTraps];	// trap table
	
	} LowMemType;
	

// Access to variable size globals
#define	GMemCardInfo				(((LowMemType*)PilotGlobalsP)->memCardInfo)
#define	GSlkGlobals					(((LowMemType*)PilotGlobalsP)->slkGlobals)
#define	GDbgGlobals					(((LowMemType*)PilotGlobalsP)->dbgGlobals)
#define	GSysAppInfo					(((LowMemType*)PilotGlobalsP)->sysAppInfo)
#define	GSysLibTable			  	(((LowMemType*)PilotGlobalsP)->sysLibTable)
#define	GSysDispatchTable		  	(((LowMemType*)PilotGlobalsP)->sysDispatchTable)


// Flags for the GSysResetFlags global
#define	sysResetFlagHardReset			0x01					// HardReset
#define	sysResetFlagCreateDefaultDBs	0x02					// create default databases
#define	sysResetFlagNoExtensions		0x04					// don't execute extensions
#define	sysResetFlagGoingToDebugger	0x08					// going into boot debugger
// Flag to mark the boot start so that if we get into the loop:
// busErr, sysReset, busErr, ... we can get out.
#define	sysResetFlagBootStarted			0x10					



// Flags for the GSysPrefFlags global
#define	sysPrefFlagTaxiDisable			0x0001				// Not used in >1.0 ROM.
#define	sysPrefFlagEnableEasterEggs	0x0002				// Enable easter eggs
#define	sysPrefFlagTaxiDisIdle			0x8000				// Disable Idle Taxi temporarily, 
																			//  (not saved in preferences)
#define	sysPrefFlagTaxiDisIdleTime		0x4000				// Copy of the 16th bit of the system
																			//  time when sysPrefFlagTaxiDisIdle bit
																			//	 was set (not saved in preferences).


// Flags for the GSysMiscFlags global
#define	sysMiscFlagInFatalAlert			0x0001				// Currently displaying fatal alert
#define	sysMiscFlagAlwaysSwitchApp		0x0002				// Inform UI App Shell to switch
																			//  apps even if already running
#define	sysMiscFlagProfile				0x0004				// true when profiling code. 
#define	sysMiscFlagGrfDisable			0x0008				// true when Graffiti is disabled
#define	sysMiscFlagInDemoAlert			0x0010				// true when Demo unit alert is displayed
#define 	sysMiscFlagBacklightDisable	0x0020				// true if backlight is disabled
#define 	sysMiscFlagUIInitialized		0x0040				// true after UI has been inited
#define 	sysMiscFlagExgEvent				0x0080				// true when a library (Ir) has data


// Flags for the GDbgWasEntered global

// dbgEnteredFlagPseudoNMI: manually setting this flag in the GDbgWasEntered low mem global
// causes the HotSync cradle button IRQ handler to enter the debugger.  Have to
// set each time because entering the debugger resets the flag. (this is a hack)
#define dbgEnteredFlagPseudoNMI		0x80
#define dbgEnteredFlagTrue				0x01

// Flags for the GCommActivityFlags global
#define sysCommActivityWlsSockOpen		0x01						// true when a wireless socket is open (Jerry Only)
//#define sysCommActivityAntennaAlert		0x02						// true when an antenna down alert is on the screen
#define sysCommActivityHotSync			0x04						// true when a hot sync is in progress
//#define sysCommActivityDiscardAntKey	0x08						// true when a wireless app want antenna hard keys discarded


/************************************************************
 * Function Prototypes
 *************************************************************/
#ifdef REMOVE_FOR_EMULATOR
#ifdef __cplusplus
extern "C" {
#endif

							
SysTCBUserInfoPtr	SysTaskUserInfoPtr(UInt32 taskID)
							SYS_TRAP(sysTrapSysTaskUserInfoPtr);
							
SysAppInfoPtr 	SysCurAppInfoPV20(void)
							SYS_TRAP(sysTrapSysCurAppInfoPV20);
							
SysAppInfoPtr 	SysGetAppInfo(SysAppInfoPtr *uiAppPP, SysAppInfoPtr *actionCodeAppPP)
							SYS_TRAP(sysTrapSysGetAppInfo);
							
Boolean	SysWantEvent(EventType* eventP, Boolean *needsUI)
							SYS_TRAP(sysTrapSysWantEvent);
	
#ifdef __cplusplus 
}
#endif
#endif

/************************************************************
 * Assembly Function Prototypes
 *************************************************************/
#define	_SysFatalAlert		\
				ASM_SYS_TRAP(sysTrapSysFatalAlert)

#define	_SysReset		\
				ASM_SYS_TRAP(sysTrapSysReset)



#endif  //__SYSTEMPRV_H__

#endif // NON_PORTABLE
