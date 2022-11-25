/******************************************************************************
 *
 * Copyright (c) 1995-1999 Palm Computing, Inc. or its subsidiaries.
 * All rights reserved.
 *
 * File: SerialMgr.h
 *
 * Description:
 *		Include file for Serial manager
 *
 * History:
 *		1/14/98		SerialMgr.h created by Ben Manuto
 *
 *****************************************************************************/

#ifndef __SERIALMGR_H
#define __SERIALMGR_H


// Pilot common definitions
#include <PalmTypes.h>
#include <ErrorBase.h>

// New Serial manager feature number
#define sysFtrNewSerialPresent     1

/********************************************************************
 * Serial Manager Errors
 * the constant serErrorClass is defined in SystemMgr.h
 ********************************************************************/

#define	serErrBadParam				(serErrorClass | 1)
#define	serErrBadPort				(serErrorClass | 2)
#define	serErrNoMem					(serErrorClass | 3)
#define	serErrBadConnID			(serErrorClass | 4)
#define	serErrTimeOut				(serErrorClass | 5)
#define	serErrLineErr				(serErrorClass | 6)
#define	serErrAlreadyOpen			(serErrorClass | 7)
#define  serErrStillOpen			(serErrorClass | 8)
#define	serErrNotOpen				(serErrorClass | 9)
#define	serErrNotSupported		(serErrorClass | 10)		// functionality not supported
#define	serErrNoDevicesAvail		(serErrorClass	| 11)		// No serial devices were loaded or are available.


//
// mask values for the lineErrors  from SerGetStatus
//

#define	serLineErrorParity		0x0001			// parity error
#define	serLineErrorHWOverrun	0x0002			// HW overrun
#define	serLineErrorFraming		0x0004			// framing error
#define 	serLineErrorBreak			0x0008			// break signal asserted
#define 	serLineErrorHShake		0x0010			// line hand-shake error
#define	serLineErrorSWOverrun	0x0020			// HW overrun
#define	serLineErrorCarrierLost	0x0040			// CD dropped


/********************************************************************
 * Serial Port Definitions
 ********************************************************************/

#define serPortLocalHotSync	0x8000		// Use physical HotSync port

#define serPortCradlePort		0x8000		// Use the RS-232 cradle port.
#define serPortIrPort			0x8001		// Use available IR port.
		

// This constant is used by the Serial Link Mgr only
#define serPortIDMask			0xC000


/********************************************************************
 * Serial Settings Descriptor
 ********************************************************************/
	
#define		srmSettingsFlagStopBitsM			0x00000001		// mask for stop bits field
#define		srmSettingsFlagStopBits1			0x00000000		//  1 stop bits	
#define		srmSettingsFlagStopBits2			0x00000001		//  2 stop bits	
#define		srmSettingsFlagParityOnM			0x00000002		// mask for parity on
#define		srmSettingsFlagParityEvenM			0x00000004		// mask for parity even
#define		srmSettingsFlagXonXoffM				0x00000008		// (NOT IMPLEMENTED) mask for Xon/Xoff flow control
#define		srmSettingsFlagRTSAutoM				0x00000010		// mask for RTS rcv flow control
#define		srmSettingsFlagCTSAutoM				0x00000020		// mask for CTS xmit flow control
#define		srmSettingsFlagBitsPerCharM		0x000000C0		// mask for bits/char
#define		srmSettingsFlagBitsPerChar5		0x00000000		//  5 bits/char	
#define		srmSettingsFlagBitsPerChar6		0x00000040		//  6 bits/char	
#define		srmSettingsFlagBitsPerChar7		0x00000080		//  7 bits/char	
#define		srmSettingsFlagBitsPerChar8		0x000000C0		//  8 bits/char
#define		srmSettingsFlagFlowControl			0x00000100		// mask for enabling/disabling special flow control feature
																				//  for the software receive buffer.	


// Default settings
#define		srmDefaultSettings		(srmSettingsFlagBitsPerChar8	|		\
												 srmSettingsFlagStopBits1		|		\
												 srmSettingsFlagRTSAutoM)
												
#define		srmDefaultCTSTimeout		(5*sysTicksPerSecond)


// Status bitfield constants

#define srmStatusCtsOn				0x00000001
#define srmStatusRtsOn				0x00000002
#define srmStatusDsrOn				0x00000004
#define srmStatusBreakSigOn		0x00000008


//
// Info fields describing serial HW capabilities.
//

#define serDevCradlePort			0x00000001		// Serial HW controls RS-232 serial from cradle connector of Pilot.
#define serDevRS232Serial			0x00000002		// Serial HW has RS-232 line drivers
#define serDevIRDACapable			0x00000004		// Serial Device has IR line drivers and generates IRDA mode serial.	
#define serDevModemPort				0x00000008		// Serial deivce drives modem connection.
#define serDevCncMgrVisible		0x00000010		// Serial device port name string to be displayed in Connection Mgr panel.


typedef struct DeviceInfoType {
	UInt32 serDevCreator;								// Four Character creator type for serial driver ('sdrv')
	UInt32 serDevFtrInfo;								// Flags defining features of this serial hardware.
	UInt32 serDevMaxBaudRate;							// Maximum baud rate for this device.
	UInt32 serDevHandshakeBaud;						// HW Handshaking is reccomended for baud rates over this
	Char *serDevPortInfoStr;							// Description of serial HW device or virtual device.			
	UInt8 reserved[8];									// Reserved.
} DeviceInfoType;

typedef DeviceInfoType *DeviceInfoPtr;


/********************************************************************
 * Type of a wakeup handler procedure which can be installed through the
 *   SerSetWakeupHandler() call.
 ********************************************************************/
typedef void (*WakeupHandlerProcPtr)(UInt32 refCon);

/********************************************************************
 * Type of an emulator-mode only blocking hook routine installed via
 * SerControl function serCtlEmuSetBlockingHook.  This is supported only
 * under emulation mode.  The argument to the function is the value
 * specified in the SerCallbackEntryType structure.  The intention of the
 * return value is to return false if serial manager should abort the
 * current blocking action, such as when an app quit event has been received;
 * otherwise, it should return true.  However, in the current implementation,
 * this return value is ignored.  The callback can additionally process
 * events to enable user interaction with the UI, such as interacting with the
 * debugger. 
 ********************************************************************/
typedef Boolean (*BlockingHookProcPtr)  (UInt32 userRef);


/********************************************************************
 * Serial Library Control Enumerations (Pilot 2.0)
 ********************************************************************/

/********************************************************************
 * Structure for specifying callback routines.
 ********************************************************************/
typedef struct SrmCallbackEntryType {
	BlockingHookProcPtr	funcP;					// function pointer
	UInt32					userRef;					// ref value to pass to callback
} SrmCallbackEntryType;
typedef SrmCallbackEntryType*	SrmCallbackEntryPtr;


typedef enum SrmCtlEnum {
	srmCtlFirstReserved = 0,		// RESERVE 0
	
	srmCtlSetBaudRate,				// Sets the current baud rate for the HW.
											// valueP = MemPtr to Int32, valueLenP = MemPtr to sizeof(Int32)
											
	srmCtlGetBaudRate,				// Gets the current baud rate for the HW.
											
	srmCtlSetFlags,					// Sets the current flag settings for the serial HW.
	
	srmCtlGetFlags,					// Gets the current flag settings the serial HW.
	
	srmCtlSetCtsTimeout,				// Sets the current Cts timeout value.
	
	srmCtlGetCtsTimeout,				// Gets the current Cts timeout value.
	
	srmCtlStartBreak,					// turn RS232 break signal on:
											// users are responsible for ensuring that the break is set
											// long enough to genearate a valid BREAK!
											// valueP = 0, valueLenP = 0
											
	srmCtlStopBreak,					// turn RS232 break signal off:
											// valueP = 0, valueLenP = 0

	srmCtlStartLocalLoopback,		// Start local loopback test
											// valueP = 0, valueLenP = 0
											
	srmCtlStopLocalLoopback,		// Stop local loopback test
											// valueP = 0, valueLenP = 0


	srmCtlIrDAEnable,					// Enable  IrDA connection on this serial port
											// valueP = 0, valueLenP = 0

	srmCtlIrDADisable,				// Disable  IrDA connection on this serial port
											// valueP = 0, valueLenP = 0

	srmCtlRxEnable,					// enable receiver  ( for IrDA )
	
	srmCtlRxDisable,					// disable receiver ( for IrDA )

	srmCtlEmuSetBlockingHook,		// Set a blocking hook routine FOR EMULATION
											// MODE ONLY - NOT SUPPORTED ON THE PILOT
											//PASS:
											// valueP = MemPtr to SerCallbackEntryType
											// *valueLenP = sizeof(SerCallbackEntryType)
											//RETURNS:
											// the old settings in the first argument										

	srmCtlUserDef,						// Specifying this opCode passes through a user-defined
											//  function to the DrvControl function. This is for use
											//  specifically by serial driver developers who need info
											//  from the serial driver that may not be available through the
											//  standard SrmMgr interface.
											
	srmCtlGetOptimalTransmitSize,	// This function will ask the port for the most efficient buffer size
											// for transmitting data packets.  This opCode returns serErrNotSupported
											// if the physical or virtual device does not support this feature.
											// The device can return a transmit size of 0, if send buffering is
											// requested, but the actual size is up to the caller to choose.
											// valueP = MemPtr to UInt32 --> return optimal buf size
											// ValueLenP = sizeof(UInt32)
	
	srmCtlSetDTRAsserted,			// Enable or disable DTR.
	
	srmCtlGetDTRAsserted,			// Determine if DTR is enabled or disabled.
	
	srmCtlLAST							// ***** ADD NEW ENTRIES BEFORE THIS ONE
	
} SrmCtlEnum;



/********************************************************************
 * Serial Hardware Library Routines
 ********************************************************************/

#ifdef BUILDING_NEW_SERIAL_MGR
	#define SERIAL_TRAP(serialSelectorNum)
#else
	#define SERIAL_TRAP(serialSelectorNum) \
		_SYSTEM_API(_CALL_WITH_SELECTOR)(_SYSTEM_TABLE, sysTrapSerialDispatch, \
													serialSelectorNum)
#endif


// *****************************************************************
// * New Serial Manager trap selectors
// *****************************************************************

typedef enum {					// The order of this enum *MUST* match the sysSerialSelector in SerialMgr.c
	sysSerialInstall = 0,
	sysSerialOpen,
	sysSerialOpenBkgnd,	
	sysSerialClose,
	sysSerialSleep,
	sysSerialWake,
	sysSerialGetDeviceCount,
	sysSerialGetDeviceInfo,
	sysSerialGetStatus,
	sysSerialClearErr,
	sysSerialControl,
	sysSerialSend,
	sysSerialSendWait,
	sysSerialSendCheck,
	sysSerialSendFlush,
	sysSerialReceive,
	sysSerialReceiveWait,
	sysSerialReceiveCheck,
	sysSerialReceiveFlush,
	sysSerialSetRcvBuffer,
	sysSerialRcvWindowOpen,
	sysSerialRcvWindowClose,
	sysSerialSetWakeupHandler,
	sysSerialPrimeWakeupHandler,
	
	maxSerialSelector = sysSerialPrimeWakeupHandler		// Used by SerialMgrDispatch.c
} sysSerialSelector;
	

#ifdef REMOVE_FOR_EMULATOR
#ifdef __cplusplus
extern "C" {
#endif

Err SerialMgrInstall(void)
	SERIAL_TRAP(sysSerialInstall);

Err SrmOpen(UInt32 port, UInt32 baud, UInt16 *newPortIdP)
	SERIAL_TRAP(sysSerialOpen);

Err SrmOpenBackground(UInt32 port, UInt32 baud, UInt16 *newPortIdP)
	SERIAL_TRAP(sysSerialOpenBkgnd);

Err SrmClose(UInt16 portId)
	SERIAL_TRAP(sysSerialClose);
	
Err SrmSleep()
	SERIAL_TRAP(sysSerialSleep);
	
Err SrmWake()
	SERIAL_TRAP(sysSerialWake);

Err SrmGetDeviceCount(UInt16 *numOfDevicesP)
	SERIAL_TRAP(sysSerialGetDeviceCount);

Err SrmGetDeviceInfo(UInt32 deviceID, DeviceInfoType *deviceInfoP)
	SERIAL_TRAP(sysSerialGetDeviceInfo);

Err SrmGetStatus(UInt16 portId, UInt32 *statusFieldP, UInt16 *lineErrsP)
	SERIAL_TRAP(sysSerialGetStatus);

Err SrmClearErr (UInt16 portId)
	SERIAL_TRAP(sysSerialClearErr);

Err SrmControl(UInt16 portId, UInt16 op, void *valueP, UInt16 *valueLenP)
	SERIAL_TRAP(sysSerialControl);

UInt32 SrmSend (UInt16 portId, void *bufP, UInt32 count, Err *errP)
	SERIAL_TRAP(sysSerialSend);

Err SrmSendWait(UInt16 portId)
	SERIAL_TRAP(sysSerialSendWait);

Err SrmSendCheck(UInt16 portId, UInt32 *numBytesP)
	SERIAL_TRAP(sysSerialSendCheck);

Err SrmSendFlush(UInt16 portId)
	SERIAL_TRAP(sysSerialSendFlush);

UInt32 SrmReceive(UInt16 portId, void *rcvBufP, UInt32 count, Int32 timeout, Err *errP)
	SERIAL_TRAP(sysSerialReceive);

Err SrmReceiveWait(UInt16 portId, UInt32 bytes, Int32 timeout)
	SERIAL_TRAP(sysSerialReceiveWait);

Err SrmReceiveCheck(UInt16 portId,  UInt32 *numBytesP)
	SERIAL_TRAP(sysSerialReceiveCheck);

Err SrmReceiveFlush(UInt16 portId, Int32 timeout)
	SERIAL_TRAP(sysSerialReceiveFlush);

Err SrmSetReceiveBuffer(UInt16 portId, void *bufP, UInt16 bufSize)
	SERIAL_TRAP(sysSerialSetRcvBuffer);

Err SrmReceiveWindowOpen(UInt16 portId, UInt8 **bufPP, UInt32 *sizeP)
	SERIAL_TRAP(sysSerialRcvWindowOpen);

Err SrmReceiveWindowClose(UInt16 portId, UInt32 bytesPulled)
	SERIAL_TRAP(sysSerialRcvWindowClose);

Err SrmSetWakeupHandler(UInt16 portId, WakeupHandlerProcPtr procP, UInt32 refCon)
	SERIAL_TRAP(sysSerialSetWakeupHandler);

Err SrmPrimeWakeupHandler(UInt16 portId, UInt16 minBytes)
	SERIAL_TRAP(sysSerialPrimeWakeupHandler);

void SrmSelectorErrPrv (UInt16 serialSelector);		// used only by SerialMgrDispatch.c

#ifdef __cplusplus
}
#endif
#endif

#endif		// __SERIALMGR_H
