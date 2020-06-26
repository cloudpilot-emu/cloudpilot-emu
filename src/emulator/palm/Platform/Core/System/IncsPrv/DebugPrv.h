/******************************************************************************
 *
 * Copyright (c) 1994-1999 Palm Computing, Inc. or its subsidiaries.
 * All rights reserved.
 *
 * File: DebugPrv.h
 *
 * Description:
 *		Privat«Debugging functions
 *
 * History:
 *   	8/10/95 RM - Created by Ron Marianetti
 *
 *****************************************************************************/

#ifdef	NON_PORTABLE
#ifndef __DEBUGPRV_H__
#define __DEBUGPRV_H__


/***********************************************************************
 * Debugger Constants
 ***********************************************************************/
#define  dbgLoopsPerTick		0x0800
#define	dbgStackSize			0x0380			// DOLATER -- resize???

#define	dbgNormalBreakpoints	5
#define	dbgTempBPIndex			dbgNormalBreakpoints
#define	dbgTotalBreakpoints	(dbgTempBPIndex+1)

#define	dbgFlashCodeSpace		sysInitStack	// where we copy flash code
#define	dbgFlashCodeSize		0x200				// size of flash code

#define	dbgTotalTrapBreaks	5					// up to this many trap breaks at a time




/***********************************************************************
 * Structure of Debugger Breakpoints
 ***********************************************************************/
typedef struct BreakpointType {
	MemPtr		addr;									// address of breakpoint
	Boolean	enabled;								// true if enabled
	Boolean	installed;							// for alignment
	} BreakpointType;





/***********************************************************************
 * Structure of Debugger Specific System Packet Type Bodies.
 ***********************************************************************/
 
#define			sysPktMaxNameLen		32		// Max length of a routine name 
 
//--------------------------------------------------------------
// sysPktStateCmd 
// - get processor state
//------------------------------------------------------------------
// number of remote code words to send in the 'state response' packet
#define sysPktStateRspInstWords	15


typedef struct SysPktStateCmdType {
	_sysPktBodyCommon;								// Common Body header
	} SysPktStateCmdCmdType;
typedef SysPktStateCmdCmdType*	SysPktStateCmdCmdPtr;


// Packet Body structure for the state command response packet
typedef struct SysPktStateRspType {
	_sysPktBodyCommon;								// Common Body header
	Boolean						resetted;			// true if target has just reset
	UInt8 						reserved1;
	UInt16						exceptionId;		// exception which caused the
															// debugger to be entered.
	M68KRegsType				reg;					// current remote registers.
	UInt16						inst[sysPktStateRspInstWords];
															// instruction buffer for
															// code beginning at PC.
	BreakpointType				bp[dbgTotalBreakpoints];	// current breakpoints

	void							*startAddr;			// start address of routine
	void							*endAddr;			// end address of routine
	char							name[sysPktMaxNameLen];
															// routine name (0 or more chars,
															// no null), immediately
															// follows the address range.
	UInt8							trapTableRev;		// rev of trap table. Used to determine
															//  when host's trap table cache is invalid
	UInt8 						reserved2;
	} SysPktStateRspType;
typedef SysPktStateRspType*	SysPktStateRspPtr;

//--------------------------------------------------------------------
// sysPktGetRtnNameCmd
//  - get routine name
//--------------------------------------------------------------------
// The response packet body will contain start address(4 bytes),
// end address(4 bytes), followed by 0 or more bytes of the routine
// name (no null byte).
typedef struct SysPktRtnNameCmdType {
	_sysPktBodyCommon;							// Common Body header
	void							*address;		// -> address to query on
	} SysPktRtnNameCmdType;
typedef SysPktRtnNameCmdType*	SysPktRtnNameCmdPtr;

typedef struct SysPktRtnNameRspType {
	_sysPktBodyCommon;							// Common Body header
	void							*address;		// -> address to query on
	void							*startAddr;		// <- start address of routine
	void							*endAddr;		// <- end address of routine
	char							name[sysPktMaxNameLen];   		// <- routine name, if any immediately
														//    follows the address range.
														//    The balance need not be sent.
	} SysPktRtnNameRspType;
typedef SysPktRtnNameRspType*	SysPktRtnNameRspPtr;


//--------------------------------------------------------------------
// sysPktReadRegsCmd
// - read registers
//--------------------------------------------------------------------
// Packet Body structure for the read regs command response packet
typedef	struct SysPktReadRegsCmdType {
	_sysPktBodyCommon;								// Common Body header
	} SysPktReadRegsCmdType;
typedef SysPktReadRegsCmdType *	SysPktReadRegsCmdPtr;

typedef	struct SysPktReadRegsRspType {
	_sysPktBodyCommon;								// Common Body header
	//M68KRegsType			reg;						// <- return registers
	} SysPktReadRegsRspType;
typedef SysPktReadRegsRspType *	SysPktReadRegsRspPtr;



//--------------------------------------------------------------------
// sysPktWriteRegsCmd
// - write registers
//--------------------------------------------------------------------
// Packet Body structure for the write regs command packet
typedef	struct SysPktWriteRegsCmdType {
	_sysPktBodyCommon;								// Common Body header
	M68KRegsType		reg;							// -> registers to write
	} SysPktWriteRegsCmdType;
typedef SysPktWriteRegsCmdType *	SysPktWriteRegsCmdPtr;


// Packet Body structure for the write regs command packet
typedef	struct SysPktWriteRegsRspType {
	_sysPktBodyCommon;								// Common Body header
	} SysPktWriteRegsRspType;
typedef SysPktWriteRegsRspType *	SysPktWriteRegsRspPtr;


//--------------------------------------------------------------------
// sysPktContinueCmd
// -  continue
//--------------------------------------------------------------------
// Packet Body structure for the continue command packet
typedef struct SysPktContinueCmdType {
	_sysPktBodyCommon;									// Common Body header
	M68KRegsType	 		regs;							// registers
	Boolean					stepSpy;						// set true to do step spy
	UInt8 					reserved;
	UInt32					ssAddr;						// step spy address
	UInt32					ssCount;						// # of bytes
	UInt32					ssCheckSum;					// checksum
	} SysPktContinueCmdType;

typedef SysPktContinueCmdType *SysPktContinueCmdPtr;


//--------------------------------------------------------------------
// sysPktGetBreakPointsCmd
//
// - Get breakpoints
//--------------------------------------------------------------------
typedef struct SysPktGetBreakpointsCmdType {
	_sysPktBodyCommon;								// Common Body header
	} SysPktGetBreakpointsCmdType;
typedef SysPktGetBreakpointsCmdType*	SysPktGetBreakpointsCmdPtr;

typedef struct SysPktGetBreakpointsRspType {
	_sysPktBodyCommon;								// Common Body header
	//BreakpointType	bp[dbgTotalBreakpoints];
	} SysPktGetBreakpointsRspType;
typedef SysPktGetBreakpointsRspType*	SysPktGetBreakpointsRspPtr;


//--------------------------------------------------------------------
// sysPktSetBreakPointsCmd
//
// - Set breakpoints
//--------------------------------------------------------------------
typedef struct SysPktSetBreakpointsCmdType {
	_sysPktBodyCommon;								// Common Body header
	//BreakpointType	bp[dbgTotalBreakpoints];
	} SysPktSetBreakpointsCmdType;
typedef SysPktSetBreakpointsCmdType*	SysPktSetBreakpointsCmdPtr;

typedef struct SysPktSetBreakpointsRspType {
	_sysPktBodyCommon;								// Common Body header
	} SysPktSetBreakpointsRspType;
typedef SysPktSetBreakpointsRspType*	SysPktSetBreakpointsRspPtr;




//--------------------------------------------------------------------
// sysPktDbgBreakToggleCmd
// - Enable/Disable DbgBreak's command
//--------------------------------------------------------------------
typedef struct SysPktDbgBreakToggleCmdType {
	_sysPktBodyCommon;								// Common Body header
	} SysPktDbgBreakToggleCmdType;
typedef SysPktDbgBreakToggleCmdType*	SysPktDbgBreakToggleCmdPtr;

typedef struct SysPktDbgBreakToggleRspType {
	_sysPktBodyCommon;								// Common Body header
	Boolean		newState;
	UInt8 			reserved;
	} SysPktDbgBreakToggleRspType;
typedef SysPktDbgBreakToggleRspType*	SysPktDbgBreakToggleRspPtr;


//--------------------------------------------------------------------
// sysPktFlashCmd - OBSOLETE AS OF 3.0! SEE sysPktExecFlashCmd BELOW!
// Program Flash command - programs one sector of the FLASH ram
// If numBytes is 0, this routine returns info on the flash in:
//   manuf - manufacturer code
//   device - device code
//   width - width of FLASH
//   ramSpaceP - buffer in RAM for flash sector
//--------------------------------------------------------------------
typedef struct SysPktFlashWriteType {
	_sysPktBodyCommon;								// Common Body header
	UInt16		numBytes;							// -> # of bytes in sector
	UInt32		flashBase;							// -> base address of FLASH, or 0 for default
	UInt32		srcAddr;								// -> source address of new data
	UInt32		dstAddr;								// -> dest address in flash memory
	UInt8			manuf;								// <- manufacturer
	UInt8			device;								// <- device
	UInt8			width;								// <- Flash memory width 
	UInt8			filler;
	UInt32		ramSpaceP;							// <-  available space in RAM for sector
	} SysPktFlashWriteType;
typedef SysPktFlashWriteType *SysPktFlashWritePtr;


//--------------------------------------------------------------------
// sysPktExecFlashCmd
// NEW Program Flash command - executes flash ident/write code
// If codeAddr is 0, this routine returns address to write flash code.
// Replaces 1.0 and 2.0 sysPktFlashCmd call above in the 3.0 ROM
//--------------------------------------------------------------------
typedef struct sysPktExecFlashType {
	_sysPktBodyCommon;								// Common Body header
	UInt16		numBytes;							//  -> # of bytes in sector
	UInt32		flashBase;							//  -> base address of FLASH
	UInt32		srcAddr;								//  -> source address of new data
	UInt32		dstAddr;								//  -> dest address in flash memory
	UInt32		codeAddr;							// <-> address of flash code
	UInt16		flashResID;							// <-  resource ID of flash write code
	UInt8			manuf;								// <-  manufacturer
	UInt8			device;								// <-  device
	UInt8			devInfo;								// <-  device info
	UInt8			remoteCmd;							//  -> remote command (e.g.: flash/erase/both)
	UInt8			response;							// <-  response from flash/ident code
	UInt8			spareByte;							// <-> filler (byte for word alignment)
	UInt32		spareDWord;							// <-> spare
	} sysPktExecFlashType;
typedef sysPktExecFlashType *sysPktExecFlashPtr;

// remoteCmd values sent to flash code:
#define remoteCmdErase		0x01		// tells flash code to erase block(s)
#define remoteCmdFlash		0x02		// tells flash code to flash block(s)
#define remoteCmdROMSig		0x04		// tells flash code to write ROM signature
#define remoteCmdUnlocked	0x08		// tells flash code to check block lock status

// response values returned by flash write/identify code:
#define respNoErr				0		// no error
#define respNoFlashBase		1		// flashBase not specified
#define respIdentFailed		2		// can't identify flash part
#define respNoBytes			3		// numBytes not specified
#define respPartialBlock	4		// numBytes not multiple of flash sub-block size
#define respTooManyBytes	5		// numBytes too high
#define respEraseFailed		6		// erase command failed
#define respNotErased		7		// erase verification failed
#define respFlashFailed		8		// flash command failed
#define respInvalidFlashDeviceID 9		// unrecognized device code
#define respBlockLocked 	10		// block is locked
#define respBadCommand	 	11		// remoteCmd not supported

//--------------------------------------------------------------------
// sysPktGetTrapBreaksCmd
//
// - Get Trap Breaks
//--------------------------------------------------------------------
typedef struct SysPktGetTrapBreaksCmdType {
	_sysPktBodyCommon;								// Common Body header
	} SysPktGetTrapBreaksCmdType;
typedef SysPktGetTrapBreaksCmdType*	SysPktGetTrapBreaksCmdPtr;

typedef struct SysPktGetTrapBreaksRspType {
	_sysPktBodyCommon;								// Common Body header
	//UInt16		trapBP[dbgTotalTrapBreaks];
	} SysPktGetTrapBreaksRspType;
typedef SysPktGetTrapBreaksRspType*	SysPktGetTrapBreaksRspPtr;


//--------------------------------------------------------------------
// sysPktSetTrapBreaksCmd
//
// - Set Trap Breaks
//--------------------------------------------------------------------
typedef struct SysPktSetTrapBreaksCmdType {
	_sysPktBodyCommon;								// Common Body header
	//UInt16		trapBP[dbgTotalTrapBreaks];
	} SysPktSetTrapBreaksCmdType;
typedef SysPktSetTrapBreaksCmdType*	SysPktSetTrapBreaksCmdPtr;

typedef struct SysPktSetTrapBreaksRspType {
	_sysPktBodyCommon;								// Common Body header
	} SysPktSetTrapBreaksRspType;
typedef SysPktSetTrapBreaksRspType*	SysPktSetTrapBreaksRspPtr;



//--------------------------------------------------------------------
// sysPktGetTrapConditionsCmd
//
// Get Trap Conditionals. These are used to tell the debugger
//  to conditionally break on a trap depending on the value of the
//  first word on the stack. They are used when setting a-traps on
//  library calls. This is a 3.0 feature. 
//--------------------------------------------------------------------
typedef struct SysPktGetTrapConditionsCmdType {
	_sysPktBodyCommon;								// Common Body header
	} SysPktGetTrapConditionsCmdType;
typedef SysPktGetTrapConditionsCmdType*	SysPktGetTrapConditionsCmdPtr;

typedef struct SysPktGetTrapConditionsRspType {
	_sysPktBodyCommon;								// Common Body header
	//UInt16		trapParam[dbgTotalTrapBreaks];
	} SysPktGetTrapConditionsRspType;
typedef SysPktGetTrapConditionsRspType*	SysPktGetTrapConditionsRspPtr;


//--------------------------------------------------------------------
// sysPktSetTrapConditionsCmd
//
// Set Trap Conditionals. These are used to tell the debugger
//  to conditionally break on a trap depending on the value of the
//  first word on the stack. They are used when setting a-traps on
//  library calls. This is a 3.0 feature. 
//--------------------------------------------------------------------
typedef struct SysPktSetTrapConditionsCmdType {
	_sysPktBodyCommon;								// Common Body header
	//UInt16		trapParam[dbgTotalTrapBreaks];
	} SysPktSetTrapConditionsCmdType;
typedef SysPktSetTrapConditionsCmdType*	SysPktSetTrapConditionsCmdPtr;

typedef struct SysPktSetTrapConditionsRspType {
	_sysPktBodyCommon;								// Common Body header
	} SysPktSetTrapConditionsRspType;
typedef SysPktSetTrapConditionsRspType*	SysPktSetTrapConditionsRspPtr;





// The following indicates how much space in debugger globals we've reserved for
// the SerialLibrary globals for the Debugger.  Reserve 8 extra bytes for
// patching.
//
// ADH 12/21/99	Increased to 64 to allow for buffering space to be added
// to the serial debug globals.
#define	dbgSerRsvGlobalsSize			64



/***********************************************************************
 * Stucture of Debugger Globals
 ***********************************************************************/
struct DbgGlobalsType {
	// Saved registers
	M68KRegsType	reg;
	
	// Breakpoints and saved opcodes behind each one
	Boolean			romBP;
	Boolean			stepOverBP;							// true if single-stepping over a BP
	BreakpointType	bp[dbgTotalBreakpoints];
	UInt16			bpOpcode[dbgTotalBreakpoints];
	
	// Current trap breaks
	UInt16			trapBreak[dbgTotalTrapBreaks];
	UInt16			trapParam[dbgTotalTrapBreaks];
	UInt32			oldDispatcher;						// saved trap dispatcher
	Boolean			breakingOnATrap;					// set by debugger trap dispatcher
	
	// Mode settings
	Boolean			ignoreDbgBreaks;					// if true, ignore DbgBreak's
	Boolean			commOpened;							// if true, comm library and socket have been
																//  opened.
	
	// Step spy support
	Boolean			stepSpy;								// true if step spying.
	UInt32			ssAddr;								// address to step spy on 
	UInt32			ssValue;								// saved value
	
	// Exception type
	UInt16			excType;								// why we entered debugger
	UInt8				reEntered;							// true if we re-entered
	UInt8				firstEntrance;						// true first time we enter debugger

	// Debugger Serial Library globals
	UInt8				serGlobals[dbgSerRsvGlobalsSize];	// Debugger Serial globals
	
	// Saved bus error exception info
	UInt32			busErrStack[2];					// first 2 long words of bus error stack frame
	
	// Debugger Stack
	UInt8				stack[dbgStackSize];				// Debugger stack
	UInt16			stackEnd;							// End of stack
	
	};
	
typedef struct DbgGlobalsType DbgGlobalsType;
	
typedef DbgGlobalsType*		DbgGlobalsPtr;



#endif // __DEBUGMGR_H__
#endif // NON_PORTABLE
