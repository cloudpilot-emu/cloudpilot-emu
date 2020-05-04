/* -*- mode: C++; tab-width: 4 -*- */
/* ===================================================================== *\
	Copyright (c) 1998-2001 Palm, Inc. or its subsidiaries.
	All rights reserved.

	This file is part of the Palm OS Emulator.

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.
\* ===================================================================== */

#ifndef _PALM_H_
#define _PALM_H_

#include "PalmPack.h"

// Remap some types that also appear in the Mac headers.
#define ControlPtr	Palm_ControlPtr
#define ListPtr		Palm_ListPtr
#define AlertType	Palm_AlertType

// Defined in both CharAttr.h and ...\VC\Includes\xlocinfo.h
#undef _XA
#undef _XS
#undef _BB
#undef _CN
#undef _DI
#undef _LO
#undef _PU
#undef _SP
#undef _UP
#undef _XD

// From PalmOS.h

// To override build options in a local component, include <BuildDefines.h>
// first, then define switches as need, and then include <PalmTypes.h>.

	#include "BuildDefines.h"

	// Not really needed (BuildDefaults.h sets this to EMULATION_MAC
	// if undefined, which is fine for us, since all relevent headers
	// only check to see if it's EMULATION_NONE or not), but let's do
	// it right anyway.

	#define EMULATION_LEVEL 	EMULATION_UNIX

	#include "PalmTypes.h"
//	#include <SystemPublic.h>
//	#include <UIPublic.h>

	// From SystemPublic.h

//		#include <AlarmMgr.h>
//		#include <AppLaunchCmd.h>
		#include <Bitmap.h>
		#include <CharAttr.h>
		#include <Chars.h>
//		#include <ConnectionMgr.h>
//		#include <ConsoleMgr.h>
		#include <Incs/Core/System/Crc.h>	// Differentiate this from the one in gdb.
		#include <DataMgr.h>
		#include <DateTime.h>
//		#include <DebugMgr.h>
//		#include <Encrypt.h>
		#include <ErrorBase.h>
//		#include <ErrorMgr.h>
//		#include <ExgLib.h>
//		#include <ExgMgr.h>
		#include <FeatureMgr.h>
//		#include <FileStream.h>
//		#include <FloatMgr.h>
		#include <Font.h>
//		#include <Graffiti.h>
//		#include <HostControl.h>
//		#include <ImcUtils.h>
		#include <IntlMgr.h>
//		#include <IrLib.h>
		#include <KeyMgr.h>
		#include <Localize.h>
		#include <MemoryMgr.h>
//		#include <ModemMgr.h>
		#include <NetBitUtils.h>
		#include <SysEvent.h>			// must come before NetMgr.h due to weird dependency in NetMgr.h
		#include <NetMgr.h>
//		#include <NotifyMgr.h>
		#include <OverlayMgr.h>
		#include <PalmLocale.h>
//		#include <Password.h>
		#include <PenMgr.h>
		#include <Preferences.h>
		#include <Rect.h>
//		#include <ScriptPlugin.h>
//		#include <SerialDrvr.h>
		#include <SerialMgr.h>
		#include <SerialLinkMgr.h>
//		#include <SerialSdrv.h>
//		#include <SerialVdrv.h>
		#include <SoundMgr.h>
//		#include <StringMgr.h>
		#include <SysEvtMgr.h>
		#include <SystemMgr.h>
		#include <SystemResources.h>
		#include <SystemPkt.h>
//		#include <SysUtils.h>
		#include <TextMgr.h>
//		#include <TextServicesMgr.h>
//		#include <TimeMgr.h>
		#include <Window.h>

	// From UIPublic.h

		#include <CoreTraps.h>

//		#include <AboutBox.h>
//		#include <Category.h>
//		#include <Clipboard.h>
		#include <Control.h>
//		#include <Day.h>
		#include <Event.h>
		#include <FatalAlert.h>
		#include <Field.h>
		#include <Find.h>
//		#include <FontSelect.h>
		#include <Form.h>
//		#include <GraffitiReference.h>
//		#include <GraffitiShift.h>
		#include <InsPoint.h>
//		#include <Keyboard.h>
//		#include <Launcher.h>
		#include <List.h>
//		#include <Menu.h>
//		#include <PhoneLookup.h>
//		#include <PrivateRecords.h>
//		#include <Progress.h>
		#include <ScrollBar.h>
//		#include <SelDay.h>
//		#include <SelTime.h>
		#include <Table.h>
//		#include <UIColor.h>
//		#include <UIControls.h>
		#include <UIResources.h>

// Additional bits we need

	// Copied from Clipboard.h
enum clipboardFormats { clipboardText, clipboardInk, clipboardBitmap };
typedef enum clipboardFormats ClipboardFormatType;
#define cbdMaxTextLength		1000

#define NON_PORTABLE
	#include "M68KHwr.h"		// M68KRegsType (used in EmulatorTypes.h)
								// Include before DebugPrv.h, which needs M68KRegsType
	#include "DebugPrv.h"		// BreakpointType (used in EmulatorTypes.h)
	#include "SystemPrv.h"		// SysKernelInfoType (used in MetaMemory::GWH_ExamineChunk)

	#include "DataPrv.h"		// DatabaseDirType, DatabaseHdrType, DmAccessType, DmOpenInfoType
								// RecordListType, RecordEntryType, RsrcEntryType
	#undef small	// Defined in a VC++ header (RPCNDR.H).
	#include "DLCommon.h"		// DlpReqHeaderType, etc.
	#include "DLServer.h"		// DlkServerSessionType, DlkServerSessionPtr

	#include "EZAustin/IncsPrv/SED1375Hwr.h"		// SED1375RegsType

	#include "M68328Hwr.h"		// HwrM68328Type
	#undef HwrDBallType
	#undef HwrDBallPtr

	#include "M68EZ328Hwr.h"	// HwrM68EZ328Type
	#undef HwrDBallType
	#undef HwrDBallPtr

	#include "M68VZ328Hwr.h"	// HwrM68VZ328Type
	#undef HwrDBallType
	#undef HwrDBallPtr

	#include "M68SZ328Hwr.h"	// HwrM68SZ328Type
	#undef HwrDBallType
	#undef HwrDBallPtr
#undef NON_PORTABLE


struct HwrBatCmdReadType	// from HwrBattery.h
{
	UInt16	mVolts;	// level in millivolts (2500 = 2.5 volts)
	UInt16	abs;	// absolute level (0 -> 255)
};


#include "PalmPackPop.h"

#undef ControlPtr
#undef ListPtr
#undef AlertType

// Defined in both CharAttr.h and ...\VC\Includes\xlocinfo.h
#undef _XA
#undef _XS
#undef _BB
#undef _CN
#undef _DI
#undef _LO
#undef _PU
#undef _SP
#undef _UP
#undef _XD

// PalmTypes.h defines these, but they conflict with the STL functions

#undef min
#undef max

// UIResources.h defines this, but it messes up Icon.h on the Mac, which declares
// parameters with this name.

#undef iconType

// DateTime.h defines this, but it messes up Icon.h on the Mac, which declares
// parameters with this name.

#undef maxTime

#endif /* _PALM_H_ */
