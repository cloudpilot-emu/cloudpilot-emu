/******************************************************************************
 *
 * Copyright (c) 1994-1999 Palm Computing, Inc. or its subsidiaries.
 * All rights reserved.
 *
 * File: Globals.h
 *
 * Description:
 *		Structure definition of Pilot globals
 *
 * History:
 *   	10/26/94 RM		Created by Ron Marianetti
 *		 4/13/95	VMK	Added Sound Manager globals
 *		 8/15/97	vmk	Reused padGlobalsP for sndOffTicks
 *		 1/18/98	scl	Removed lastActionCodeAppInfoP, added irq globals.
 *  	 3/13/98	srj	Added new battery subsystem
 *   	 6/22/98	srj	Added new global for software shutdown
 *     8/17/98	SCL	Synchronized globals between Sumo/Eleven
 *		 8/26/98	kwk	Deleted unused FEP globals pointer.
 *     8/28/98	SCL	Synchronized globals between Sumo/Eleven
 *		 9/15/98	jfs	Added two new UInt32 globals exclusively for OEM use
 *    10/28/98	SCL	Synchronized globals between Sumo/Eleven
 *		04/05/99	jrb	Added UInt16 for Jerry PLD shadow register.
 *		04/13/99	jhl	Added RadioMgr globals pointer
 *		07/12/99	kwk	Added fatal alert template ptr/btn width.
 *		07/14/99	kwk	Added silkscreen info pointer.
 *		10/29/99 SCL	REMOVED dbgHalDispatchP since it was never released.
 *
 *****************************************************************************/

#ifdef	NON_PORTABLE									// So app's don't mistakenly include this

#ifndef 	__GLOBALS_H__
#define	__GLOBALS_H__

#include <M68KHwr.h>

// The following indicates how much space in low memory we've reserved for
//  the UI globals. The UI initialization code makes sure that we don't
//  overrun this space by comparing the size of the structure UIGlobalsType
//  with this constant.

// Changing this will cause globals after uiGlobals to move including flpSoftFPSCR.
// See note below regarding flpSoftFPSCR and __SoftFPSCR__ in mc68ksfpe.s.
#define	sysUIRsvGlobalsSize			0xB0



/************************************************************
 * Structure of Fixed System Globals
 *************************************************************/
typedef struct FixedGlobalsType {

	// Memory Manager Globals
	UInt8						memCardSlots;							// number of memory card slots
	UInt8						dbgWasEntered;							// set true the first time debugger is entered
	void *					memCardInfoP;							// pointer to array of CardInfoTypes stored
	UInt32					memSemaphoreID;						// Memory Manager semaphore
	UInt16					memDebugMode;							// Current debug mode of Memory Manager

	// Data Manager Globals
	void *					dmOpenList;								// Linked list of open databases (DmOpenInfoPtr)
	
	// Debugger/System Shared globals
	UInt8						dbgInDebugger;							// true if in debugger
	UInt8						dbgTracing;								// tracing in debugger
	void *					dbgGlobalsP;							// pointer to dbgGlobals
	void *					dbgSerGlobalsP;						// pointer to Debugger Serial globals
	
	
	// System Manager Globals
	void *					sysAppInfoP;							// pointer to SysAppInfoType for system
	void *					sysKernelDataP;						// pointer to Kernel data
	void **					sysDispatchTableP;					// pointer to dispatch table
	UInt32					sysOwnerIDsInUse;						// 1 bit for each owner ID in use
	void *					sysAMXAppInfoP;						// SysAppInfoType for AMX app.
	UInt32					sysClockFreq;							// processor clock frequency
	UInt32 *					sysHardKeyCreators;					// ptr to array of App Creator types

	Int16						sysBatteryCheckTimer;				// # of ticks left before we check battery again
	UInt8						sysBatteryMinThreshold;				// System will shut down if battery falls below this
	UInt8						hwrBatteryLevel;						// current battery level
	Int16						sysNextBatteryAlertTimer;			// # of ticks till next battery warning (OBSOLETE, See Below)
	UInt8						sysBatteryWarnThreshold;			// If battery below this level, post low battery keyboard event

	UInt8						sysDispatchTableRev;					// incremented every time a trap is patched. Used
																			//  by host debugger to invalidate it's cache.
	UInt16					sysDispatchTableSize;				// # of entries in dispatch table
	void *					sysLibTableP;							// (SysLibTblEntryPtr) pointer to Library entries
	UInt16					sysLibTableEntries;					// size of library table
	void *					sysConsoleStackChunkP;				// pointer to console task stack chunk
	void *					sysUIShellAppInfoP;					// AppInfoPtr for UIAppShell
	
	UInt32					sysTimerID;								// TimerID of timer used for periodic system stuff
	UInt32					sysAutoOffEvtTicks;					// Ticks of last event - used to support auto-off
	UInt16					sysAutoOffSeconds;					// Auto-sleep timeout in seconds.
	
	Int32						sysRandomSeed;							// Random # seed 
	
	
	
	// Other Manager's globals
	void *					slkGlobalsP;							// pointer to serial link globals
	void *					serGlobalsP;							// pointer to serial manager globals
	void *					scrGlobalsP;							// pointer to screen driver globals
	void *					fplGlobalsP;							// pointer to Floating Point Library globals
	void *					penGlobalsP;							// Pen manager globals ptr
	void *					sysEvtMgrGlobalsP;					// System Event Manager Globals
	void *					sndGlobalsP;							// Sound Manager globals
	void *					timGlobalsP;							// Time Manager globals
	void *					almGlobalsP;							// Alarm Manager globals
	void *					ftrGlobalsP;							// pointer to feature manager globals																		
	void *					grfGlobalsP;							// pointer to Graffiti globals
	void *					keyGlobalsP;							// Key manager globals
	
	// UI Globals
	UInt8						uiGlobals[sysUIRsvGlobalsSize];	// Basic UI Globals	
	void *					uiExtensionsP;							// for future use...																	 


	// Globals used by UIAppShell in order to manage the launching of UI Apps. 
#if EMULATION_LEVEL != EMULATION_NONE
	void *					curUIAppInfoP;							// SysAppInfoPtr of current UI App or
																			// last action code
#else
	void *					unusedPtr;
#endif

	UInt32					nextUIAppDBID;							// Database LocalID of next UI App to launch 
	UInt16					nextUIAppCmd;							// Cmd for next UI App
	void *					nextUIAppCmdPBP;						// Cmd ParamBlock Pointer for next UI App
	UInt8						nextUIAppCardNo;						// Card number of next UI App to launch
	
	
	// Hardware Manager Globals
	Int8						memDataWELevel;						// if > 0, Data area is write enabled
	UInt16					hwrWakeUp;								// Bit mask of which peripherals need to be 
																			//  enabled when system wakes
	UInt8						hwrCPUDutyCycle;						// Desired Duty cycle of CPU in 31ths.
	UInt8						hwrPenDown;								// last state of pen-down (zero = up
																			//   non-zero = down, number of ticks 'till next sample)
	UInt32					hwrCurTicks;							// Current hardware tickCount - 
																			//  maintained separately from AMX ticks for performance reasons.
	UInt32					hwrTotalRAMSize;						// total RAM Size, used by HwrMemWritable
	UInt32					hwrDozeSubTicks;						// # of subTicks we've dozed for since reset
																			// a sub-tick is 1/4096 seconds.
	
	// Sound Manager "sound off" ticks
	// WAS: Pad server globals pointer
	// WAS: void *			padGlobalsP;							// PAD Server globals
	UInt32					sndOffTicks;							// # of ticks til disabling the PWM (0 = do nothing)

	// New System globals
	UInt8						sysResetFlags;							// Reset flags sysResetFlagXXXX
	UInt8						sysBatteryKind;						// SysBatteryKind enumerated type

	// New Memory Manager global for recording minimum heap free - used for debugging only
	UInt32					memMinDynHeapFree;					// Minimum free space in dynamic heap
	
	
	// System Preferences flags
	UInt16					sysPrefFlags;							// System Pref flags: sysPrefFlagXXXX
	
	
	// This is a pointer to the System Manager variable size globals
	void *					sysGlobalsP;							// pointer to System Globals 
	
	// Miscellaneous System flags
	UInt16					sysMiscFlags;							// System Misc flags: sysMiscFlagXXX
	
	
	//-----------------------------------------------------------------------
	// New Globals added for the 2.0 release...
	//-----------------------------------------------------------------------
	UInt16					sysLibNet;								// refnum of TCP/IP library
	void *					netPktFreeQ;							// global needed by Net Library
	

	// EventGroupID used by the System Event Manager for blocking on user input
	// and by the Net Library for implementing NetLibSelect()
	UInt32					sysEvGroupID;							// AMX EventGroup ID.
	
	
	void *					irq3GlobalsP;							// IRQ3 handler globals
	
	// Tick count last time we displayed a low battery warning. This is used by
	// the system timer procedure in AMXHardwareTD1.c to insure that it doesn't
	// display low battery warnings more than once/minute. 
	UInt32					sysLastBatteryWarning;				// tickcount of last warning. 
	
	// Checksum of exception vector area of low memory. Used when ROM built for full
	//  error checking to make sure no apps overwrite low memory by using null pointers.
	UInt32					sysLowMemChecksum;					// checksum of low memory.

	// Hardware rev. This is determined during bootup time. The first shipping rev
	//  is 1.
	UInt16					hwrHardwareRev;						// hardware rev.
	
	
	// MemHandle to list of protected databases 
	MemHandle				dmProtectListH;						// handle to array of protected 
																			// databases (DmProtectEntryType)
	// Miscellaneous hardware flags
	UInt16					hwrMiscFlags;							// Hardware Misc Flags: hwrMiscFlagXXX
	
	// Support for profiling
	UInt32					sysProfileRefcon;						// Refcon value for profiler function
	void *					sysProfileProcP;						// Profiler procedure, called if non-zero
																			//  during tick interrupt (100/sec).

	// NewFloatMgr's MWSFPE (MetroWerks Software Floating Point Emulator)
	// needs a global to keep track of its status and control.
	// This global (__SoftFPSCR__) is referenced by the 68K code located at
	// :Libraries:NewFloatMgr:mc68ksfpe.s
	UInt32						flpSoftFPSCR;


	//-----------------------------------------------------------------------
	// New Globals added for the 3.0 release...
	//-----------------------------------------------------------------------
	void *					irLibGlobalsP;							// Ir Library globals pointer
	UInt16					exgActiveLib;							// active Exchange library

	UInt16					uiBusyCount;							// UI busy counter
	
	void *					irq1GlobalsP;							// IRQ1 handler globals
	void *					irq2GlobalsP;							// IRQ2 handler globals
	void *					irq6GlobalsP;							// IRQ6 handler globals


	//-----------------------------------------------------------------------
	// New Globals added for the 3.1 release...
	//-----------------------------------------------------------------------

	// Globals for Language support.
	void *					intlMgrGlobalsP;						// Int16'l Manager globals

	// Globals for Field inline Text Services
	UInt16					inlineStart;							// Starting position of the inline text
	UInt16					inlineEnd;								// Ending position of the inline text
	Boolean					inlineActive;							// True if inline text support is active.
	UInt8						inlineDefTerm;							// True if deferred termination
																			// (inlineDefChecksum/inlineDefField are valid)
	void *					tsmFepLibStatusP;						// Pointer to FEP status record.
	UInt16					tsmFepLibRefNum;						// Text Services Library Reference number	
	UInt16					inlineHighlightLen;					// Length of highlighted inline text

	// <chg 3/13/98 srj> Added for the battery subsystem
	void *					sysBatteryDataP;						// pointer to current battery data
	void *					sysBatteryValidKindsP;				// pointer to list of valid batteries

	// <chg 6/22/98 srj> Added for the battery subsystem
	Boolean					sysInShutdown;							// True: We are in lockout (Sumo)
	UInt8						sysMinWakeupThreshold;				// System can wake up once we're over this voltage

	// <chg 7/8/98 srj> More miscellaneous hardware flags
	UInt32					hwrMiscFlagsExt;						// Hardware Misc Flags: hwrMiscFlagExtXXX
	
	// <chg 8/15/98 srj> Added to provide critical warning timer
	Boolean					sysUseCritWarnTimer;
	Boolean					sysCritWarnTimerRunning;
	Int16						sysCritWarnRTCDayStarted;
	Int32						sysCritWarnTickTimer;
	UInt32					sysCritWarnLastTick;
	Boolean					sysCharging;
	Boolean					sysUpdateVoltageLevel;

	// Notification Manager globals:
	MemHandle				notifyGlobalsH;			// handle to NotifyMgr globals
	
	// Some globals reserved for future use (only available in 3.1 or later):
	UInt32					sysReserved31DWord1;
	UInt32					sysReserved31DWord2;
	UInt32					sysReserved31DWord3;
	UInt32					sysReserved31DWord4;


	//-----------------------------------------------------------------------
	// New Globals added for the 3.2 release...
	//-----------------------------------------------------------------------

	// <chg 1-28-98 RM> Added for the RAM Mobitex Network Interface
	void *					ramGlobalsP;							// For Jerry radio
	
	// <chg 4-1-98 RM> New global for the HwrBattery() call
	void *					hwrBatteryGlobalsP;					// used to hold charging state, etc.

	// <chg 7-10-98 JB> New global for Battery Percent 
	UInt8						hwrBatteryPercent;

	// <chg 10/9/98 SOE> New global with flags indicating various types of background communications activity
	// Used to hold off form alerts and other activities (beaming) that might screw up comm transfers
	UInt8						commActivityFlags;
	
	// For the new serial mgr, the 328 serial plugin needs its own global space
	// since there is no global for the irq4 line.
	void *					uart328GlobalsP;
	
	
	// 9/15/98 jfs		New Globals added for Licensees, Partners, OEMs, etc.
	UInt32					sysOEMGlobal1;				// 4 bytes for OEM use only!
	UInt32					sysOEMGlobal2;				// 4 bytes for OEM use only!
	
	// soe added 10/11/98 - many routines think this is constant across platforms
	// but it was 78 on old pilots (3.3v reference) and is now 85 (3.0v reference on new pilots)
	// The #defines in hardware.h are now specific by reference
	UInt16					hwrBatteryStepsPerVolt;
	
	// <chg 11/19/98 TLW> New global to maintain a list of appInfo blocks
	// that reference the stack used by a task so both the appInfo block
	// and the stack can be deleted after task termination is complete.
	// The stack can't be deleted before terminating the task because the
	// task delete call is using the stack.  The appInfo block is deleted
	// later because already has a taskId reference and a pointer to the stack.
	void *					sysPendingStackFrees;


	//-----------------------------------------------------------------------
	// New Globals added for the 3.3 release...
	//-----------------------------------------------------------------------

	// jed 1/18-99		Software day counter for the time-based critical warnings
	UInt16					sysDayCounter;
	
	// jed 1/15/99		New Global added during the Sumo merge
	// jfs 4/26/99		Changed from unsigned to signed Int16
	Int16						sysBatteryCheckPeriod;
	
	// jrb	4/5/99	Shadow register for  Jerry EZ PLD
	UInt16					hwrJerryPldShadow;

	// jhl 4/13/99	RadioMgr globals pointer
	void *					radioGlobalsP;


	//-----------------------------------------------------------------------
	// New Globals added for the 3.5 release...
	//-----------------------------------------------------------------------

	// kwk 07/12/99 Info used by SysFatalAlert to display localizable alert.
	void *					fatalAlertTemplateP;
	UInt16					fatalAlertBtnWidth;
	
	// kwk 07/14/99 Ptr to silkscreen/button list information.
	void *					silkscreenInfoP;

	// kwk 08/04/99 Deferred inline termination globals.
	UInt16					inlineDefChecksum;
	void *					inlineDefField;
	
	// Are there any pending deferred broadcasts for the NotifyMgr to handle?
	Boolean					notifyBroadcastPending;
	
	UInt8						sysReserved35Byte1;

	// SCL 10/29/99 Globals in which to store OEM/Device/HAL identification info.
	UInt32					hwrOEMCompanyID;
	UInt32					hwrOEMDeviceID;
	UInt32					hwrOEMHALID;

	// mchen 11/11/99 Global used by system update patches.
	void *					sysUpdateGlobalsP;
	
	// SCL 11/15/99 Global used to identify which SmallROM booted the OS.
	UInt16					smallROMChecksum;
	
	//-----------------------------------------------------------------------
	// Put New globals here...
	//-----------------------------------------------------------------------
	// type 					newGlobal;

	} FixedGlobalsType;
	
		
/************************************************************ 
 * This structure defines the start of the Low Memory Area.
 * The complete structure of LowMemory is defined in SystemPrv.h
 *  and includes this low memory header followed by some variable
 *  size globals that may change from rev to rev of the System Software.
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
typedef struct LowMemHdrType {
	//---------------------------------------------------------------------------
	// Processor exception table
	//---------------------------------------------------------------------------
	M68KExcTableType		vectors;						// Exception vectors, 
																// or Card header for RAM only cards
	
	//---------------------------------------------------------------------------
	// System Globals
	//---------------------------------------------------------------------------
	FixedGlobalsType		globals;						// System Globals
	
	} LowMemHdrType;



#if EMULATION_LEVEL == EMULATION_NONE
	#define	PilotGlobalsP	0
#else
	#ifdef __cplusplus
		extern "C"	void *PilotGlobalsP;						// pointer to start of Memory card 0
	#else
		extern		void *PilotGlobalsP;						// pointer to start of Memory card 0
	#endif
#endif    





/************************************************************
 * Macros for accessing globals. 
 * When emulating, the globals are allocated in a memory chunk and our
 *  global variable Card0P points to this chunk.
 * In native mode, Card0P is defined to be 0 at all times.
 *************************************************************/
#define	GMemCardSlots				(((LowMemHdrType*)PilotGlobalsP)->globals.memCardSlots)
#define	GMemCardInfoP				(((LowMemHdrType*)PilotGlobalsP)->globals.memCardInfoP)
#define	GMemSemaphoreID			(((LowMemHdrType*)PilotGlobalsP)->globals.memSemaphoreID)
#define  GMemDebugMode				(((LowMemHdrType*)PilotGlobalsP)->globals.memDebugMode)
#define  GMemMinDynHeapFree		(((LowMemHdrType*)PilotGlobalsP)->globals.memMinDynHeapFree)

#define	GDmOpenList		  			(((LowMemHdrType*)PilotGlobalsP)->globals.dmOpenList)
#define	GDmProtectListH 			(((LowMemHdrType*)PilotGlobalsP)->globals.dmProtectListH)

#define	GDbgWasEntered	  			(((LowMemHdrType*)PilotGlobalsP)->globals.dbgWasEntered)
#define	GDbgInDebugger	  			(((LowMemHdrType*)PilotGlobalsP)->globals.dbgInDebugger)
#define	GDbgTracing		  			(((LowMemHdrType*)PilotGlobalsP)->globals.dbgTracing)
#define	GDbgGlobalsP	  			(((LowMemHdrType*)PilotGlobalsP)->globals.dbgGlobalsP)

#define	GSysAppInfoP	  			(((LowMemHdrType*)PilotGlobalsP)->globals.sysAppInfoP)
#define	GSysKernelDataP	  		(((LowMemHdrType*)PilotGlobalsP)->globals.sysKernelDataP)
#define	GSysDispatchTableP	  	(((LowMemHdrType*)PilotGlobalsP)->globals.sysDispatchTableP)
#define	GSysDispatchTableSize	(((LowMemHdrType*)PilotGlobalsP)->globals.sysDispatchTableSize)
#define	GSysDispatchTableRev		(((LowMemHdrType*)PilotGlobalsP)->globals.sysDispatchTableRev)
#define	GSysOwnerIDsInUse		  	(((LowMemHdrType*)PilotGlobalsP)->globals.sysOwnerIDsInUse)
#define	GSysAMXAppInfoP			(((LowMemHdrType*)PilotGlobalsP)->globals.sysAMXAppInfoP)
#define	GSysClockFreq				(((LowMemHdrType*)PilotGlobalsP)->globals.sysClockFreq)
#define  GSysHardKeyCreators		(((LowMemHdrType*)PilotGlobalsP)->globals.sysHardKeyCreators)
#define  GSysNextBatteryAlertTimer		(((LowMemHdrType*)PilotGlobalsP)->globals.sysNextBatteryAlertTimer)
#define  GSysBatteryCheckTimer	(((LowMemHdrType*)PilotGlobalsP)->globals.sysBatteryCheckTimer)
#define  GSysBatteryWarnThreshold		(((LowMemHdrType*)PilotGlobalsP)->globals.sysBatteryWarnThreshold)
#define  GHwrBatteryLevel			(((LowMemHdrType*)PilotGlobalsP)->globals.hwrBatteryLevel)
#define  GSysBatteryMinThreshold		(((LowMemHdrType*)PilotGlobalsP)->globals.sysBatteryMinThreshold)
#define  GSysBatteryKind			(((LowMemHdrType*)PilotGlobalsP)->globals.sysBatteryKind)
#define  GSysConsoleStackChunkP	(((LowMemHdrType*)PilotGlobalsP)->globals.sysConsoleStackChunkP)
#define  GSysUIShellAppInfoP		(((LowMemHdrType*)PilotGlobalsP)->globals.sysUIShellAppInfoP)
#define  GSysTimerID					(((LowMemHdrType*)PilotGlobalsP)->globals.sysTimerID)
#define  GSysAutoOffEvtTicks		(((LowMemHdrType*)PilotGlobalsP)->globals.sysAutoOffEvtTicks)
#define  GSysAutoOffSeconds		(((LowMemHdrType*)PilotGlobalsP)->globals.sysAutoOffSeconds)
#define  GSysRandomSeed				(((LowMemHdrType*)PilotGlobalsP)->globals.sysRandomSeed)
#define  GSysResetFlags				(((LowMemHdrType*)PilotGlobalsP)->globals.sysResetFlags)
#define  GSysPrefFlags				(((LowMemHdrType*)PilotGlobalsP)->globals.sysPrefFlags)
#define  GSysMiscFlags				(((LowMemHdrType*)PilotGlobalsP)->globals.sysMiscFlags)
#define  GSysGlobalsP				(((LowMemHdrType*)PilotGlobalsP)->globals.sysGlobalsP)
#define  GSysLibNet					(((LowMemHdrType*)PilotGlobalsP)->globals.sysLibNet)
#define  GSysEvGroupID				(((LowMemHdrType*)PilotGlobalsP)->globals.sysEvGroupID)
#define  GSysLastBatteryWarning	(((LowMemHdrType*)PilotGlobalsP)->globals.sysLastBatteryWarning)
#define  GSysLowMemChecksum		(((LowMemHdrType*)PilotGlobalsP)->globals.sysLowMemChecksum)
#define  GSysProfileRefcon			(((LowMemHdrType*)PilotGlobalsP)->globals.sysProfileRefcon)
#define  GSysProfileProcP			(((LowMemHdrType*)PilotGlobalsP)->globals.sysProfileProcP)

#define  GNetPktFreeQ				(((LowMemHdrType*)PilotGlobalsP)->globals.netPktFreeQ) 

#define	GMemDataWELevel			(((LowMemHdrType*)PilotGlobalsP)->globals.memDataWELevel)

#define	GSlkGlobalsP	  			(((LowMemHdrType*)PilotGlobalsP)->globals.slkGlobalsP)
#define	GSerGlobalsP	  			(((LowMemHdrType*)PilotGlobalsP)->globals.serGlobalsP)
#define	GSer681GlobalsP	  		(((LowMemHdrType*)PilotGlobalsP)->globals.irq3GlobalsP)
#define	GScrGlobalsP				(((LowMemHdrType*)PilotGlobalsP)->globals.scrGlobalsP)
#define	GFplGlobalsP				(((LowMemHdrType*)PilotGlobalsP)->globals.fplGlobalsP)
#define	GPenGlobalsP				(((LowMemHdrType*)PilotGlobalsP)->globals.penGlobalsP)
#define	GSysEvtMgrGlobalsP		(((LowMemHdrType*)PilotGlobalsP)->globals.sysEvtMgrGlobalsP)
#define	GSndGlobalsP				(((LowMemHdrType*)PilotGlobalsP)->globals.sndGlobalsP)
#define	GTimGlobalsP				(((LowMemHdrType*)PilotGlobalsP)->globals.timGlobalsP)
#define	GAlmGlobalsP				(((LowMemHdrType*)PilotGlobalsP)->globals.almGlobalsP)
#define	GFtrGlobalsP				(((LowMemHdrType*)PilotGlobalsP)->globals.ftrGlobalsP)
#define	GGrfGlobalsP				(((LowMemHdrType*)PilotGlobalsP)->globals.grfGlobalsP)
#define	GKeyGlobalsP				(((LowMemHdrType*)PilotGlobalsP)->globals.keyGlobalsP)
//#define	GPADGlobalsP				(((LowMemHdrType*)PilotGlobalsP)->globals.padGlobalsP) // not used vmk 8/15/97

#define	GSndOffTicks				(((LowMemHdrType*)PilotGlobalsP)->globals.sndOffTicks)

#define	GUIGlobalsP					(((LowMemHdrType*)PilotGlobalsP)->globals.uiGlobals)

#define	GNextUIAppDBID				(((LowMemHdrType*)PilotGlobalsP)->globals.nextUIAppDBID)
#define	GNextUIAppCardNo			(((LowMemHdrType*)PilotGlobalsP)->globals.nextUIAppCardNo)
#define	GNextUIAppCmd				(((LowMemHdrType*)PilotGlobalsP)->globals.nextUIAppCmd)
#define	GNextUIAppCmdPBP			(((LowMemHdrType*)PilotGlobalsP)->globals.nextUIAppCmdPBP)

#if EMULATION_LEVEL != EMULATION_NONE
#define	GCurUIAppInfoP				(((LowMemHdrType*)PilotGlobalsP)->globals.curUIAppInfoP)
#endif

#define  GSysLibTableP				(((LowMemHdrType*)PilotGlobalsP)->globals.sysLibTableP)
#define  GSysLibTableEntries		(((LowMemHdrType*)PilotGlobalsP)->globals.sysLibTableEntries)
#define	GDbgSerGlobalsP			(((LowMemHdrType*)PilotGlobalsP)->globals.dbgSerGlobalsP)

#define  GIrLibGlobalsP				(((LowMemHdrType*)PilotGlobalsP)->globals.irLibGlobalsP)
#define	GExgActiveLib				(((LowMemHdrType*)PilotGlobalsP)->globals.exgActiveLib)
#define	GSysUIBusyCount			(((LowMemHdrType*)PilotGlobalsP)->globals.uiBusyCount)

#define	GIrq1GlobalsP				(((LowMemHdrType*)PilotGlobalsP)->globals.irq1GlobalsP)
#define	GIrq2GlobalsP				(((LowMemHdrType*)PilotGlobalsP)->globals.irq2GlobalsP)
#define	GIrq3GlobalsP		  		(((LowMemHdrType*)PilotGlobalsP)->globals.irq3GlobalsP)
#define	GIrq6GlobalsP				(((LowMemHdrType*)PilotGlobalsP)->globals.irq6GlobalsP)
#define	GUart328GlobalsP			(((LowMemHdrType*)PilotGlobalsP)->globals.uart328GlobalsP)
#define	GSysOEMGlobal1				(((LowMemHdrType*)PilotGlobalsP)->globals.sysOEMGlobal1)
#define	GSysOEMGlobal2				(((LowMemHdrType*)PilotGlobalsP)->globals.sysOEMGlobal2)

#define	GIntlMgrGlobalsP			(((LowMemHdrType*)PilotGlobalsP)->globals.intlMgrGlobalsP)

#define	GInlineStart				(((LowMemHdrType*)PilotGlobalsP)->globals.inlineStart)
#define	GInlineEnd					(((LowMemHdrType*)PilotGlobalsP)->globals.inlineEnd)
#define	GInlineActive				(((LowMemHdrType*)PilotGlobalsP)->globals.inlineActive)
#define	GInlineDefTerm				(((LowMemHdrType*)PilotGlobalsP)->globals.inlineDefTerm)
#define	GInlineHighlightLen		(((LowMemHdrType*)PilotGlobalsP)->globals.inlineHighlightLen)
#define	GInlineDefChecksum		(((LowMemHdrType*)PilotGlobalsP)->globals.inlineDefChecksum)
#define	GInlineDefField			(((LowMemHdrType*)PilotGlobalsP)->globals.inlineDefField)
#define	GTsmFepLibStatusP			(((LowMemHdrType*)PilotGlobalsP)->globals.tsmFepLibStatusP)
#define	GTsmFepLibRefNum			(((LowMemHdrType*)PilotGlobalsP)->globals.tsmFepLibRefNum)

#define	GSysBatteryDataP			(((LowMemHdrType*)PilotGlobalsP)->globals.sysBatteryDataP)
#define	GSysBatteryValidKindsP	(((LowMemHdrType*)PilotGlobalsP)->globals.sysBatteryValidKindsP)
#define	GSysInShutdown				(((LowMemHdrType*)PilotGlobalsP)->globals.sysInShutdown)
#define	GSysMinWakeupThreshold	(((LowMemHdrType*)PilotGlobalsP)->globals.sysMinWakeupThreshold)

#define	GSysUseCritWarnTimer			(((LowMemHdrType*)PilotGlobalsP)->globals.sysUseCritWarnTimer)
#define	GSysCritWarnTimerRunning	(((LowMemHdrType*)PilotGlobalsP)->globals.sysCritWarnTimerRunning)
#define	GSysCritWarnRTCDayStarted	(((LowMemHdrType*)PilotGlobalsP)->globals.sysCritWarnRTCDayStarted)
#define	GSysCritWarnTickTimer		(((LowMemHdrType*)PilotGlobalsP)->globals.sysCritWarnTickTimer)
#define	GSysCritWarnLastTick    	(((LowMemHdrType*)PilotGlobalsP)->globals.sysCritWarnLastTick)
#define	GSysCharging				(((LowMemHdrType*)PilotGlobalsP)->globals.sysCharging)
#define	GSysUpdateVoltageLevel	(((LowMemHdrType*)PilotGlobalsP)->globals.sysUpdateVoltageLevel)

#define	GSysNotifyGlobalsH		(((LowMemHdrType*)PilotGlobalsP)->globals.notifyGlobalsH)
#define	GSysNotifyBroadcastPending		(((LowMemHdrType*)PilotGlobalsP)->globals.notifyBroadcastPending)

#define	GRAMGlobalsP				(((LowMemHdrType*)PilotGlobalsP)->globals.ramGlobalsP)
#define	GCommActivityFlags		(((LowMemHdrType*)PilotGlobalsP)->globals.commActivityFlags)
#define  GSysPendingStackFrees	(((LowMemHdrType*)PilotGlobalsP)->globals.sysPendingStackFrees)

#define  GSysBatteryCheckPeriod	(((LowMemHdrType*)PilotGlobalsP)->globals.sysBatteryCheckPeriod)
#define  GSysDayCounter				(((LowMemHdrType*)PilotGlobalsP)->globals.sysDayCounter)

#define	GSysJerryPldShadow		(((LowMemHdrType*)PilotGlobalsP)->globals.hwrJerryPldShadow)

#define	GRadioGlobalsP				(((LowMemHdrType*)PilotGlobalsP)->globals.radioGlobalsP)

#define	GFatalAlertTemplateP		(((LowMemHdrType*)PilotGlobalsP)->globals.fatalAlertTemplateP)
#define	GFatalAlertBtnWidth		(((LowMemHdrType*)PilotGlobalsP)->globals.fatalAlertBtnWidth)

#define	GSilkscreenInfoP		(((LowMemHdrType*)PilotGlobalsP)->globals.silkscreenInfoP)

#define	GHwrOEMCompanyID		(((LowMemHdrType*)PilotGlobalsP)->globals.hwrOEMCompanyID)
#define	GHwrOEMDeviceID		(((LowMemHdrType*)PilotGlobalsP)->globals.hwrOEMDeviceID)
#define	GHwrOEMHALID			(((LowMemHdrType*)PilotGlobalsP)->globals.hwrOEMHALID)

#define  GSysUpdateGlobalsP	(((LowMemHdrType*)PilotGlobalsP)->globals.sysUpdateGlobalsP)
#define	GSmallROMChecksum		(((LowMemHdrType*)PilotGlobalsP)->globals.smallROMChecksum)

#endif 	//__GLOBALS_H__

#endif 	// NON_PORTABLE
