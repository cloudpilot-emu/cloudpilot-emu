/* ===================================================================== *\
        Copyright (c) 2000-2001 Palm, Inc. or its subsidiaries.
        All rights reserved.

        This file is part of the Palm OS Emulator.

        This program is free software; you can redistribute it and/or modify
        it under the terms of the GNU General Public License as published by
        the Free Software Foundation; either version 2 of the License, or
        (at your option) any later version.
\* ===================================================================== */

#ifndef EmErrCodes_h
#define EmErrCodes_h

#include "EmCommon.h"
#include "EmTypes.h"  // ErrCode

// Platform-independent errors numbers that can be thrown from
// platform-independent code.  These numbers ideally should not
// collide with platform-specific error codes. Mac error codes
// are 16-bit negative numbers, so take the range 0...-32768.
// Windows error codes are generally in the range 0...<big positive number>.
// Palm OS error codes are 16-bit positive numbers, so are
// in the range 0...32767.  Thus, Mac and Windows errors don't
// collide. Palm OS error codes collide with Windows, so we
// shift them down into the range 0xFFFBxxxx, where xxxx is the
// Palm OS error code. Finally, we have our own Emulator-specific
// error codes. Those we'll keep in 0xFFFDxxxx.

const ErrCode kError_NoError = 0;
const ErrCode kError_EmuErrBase = 0xFFFE0000;   // arbitrary value
const ErrCode kError_EmuErrRange = 0x00010000;  // arbitrary value

const ErrCode kError_OutOfMemory = kError_EmuErrBase + 1;

const ErrCode kError_BadROM = kError_EmuErrBase + 2;
const ErrCode kError_WrongROMForType = kError_EmuErrBase + 3;
const ErrCode kError_UnsupportedROM = kError_EmuErrBase + 4;
const ErrCode kError_InvalidDevice = kError_EmuErrBase + 5;
const ErrCode kError_InvalidSessionFile = kError_EmuErrBase + 6;
const ErrCode kError_InvalidConfiguration = kError_EmuErrBase + 7;

const ErrCode kError_CantDownloadROM_BadBaudRate = kError_EmuErrBase + 10;
const ErrCode kError_CantDownloadROM_SerialPortBusy = kError_EmuErrBase + 11;
const ErrCode kError_CantDownloadROM_Generic = kError_EmuErrBase + 12;

const ErrCode kError_UnimplementedTrap = kError_EmuErrBase + 13;
const ErrCode kError_InvalidLibraryRefNum = kError_EmuErrBase + 14;

const ErrCode kError_CommOpen = kError_EmuErrBase + 15;
const ErrCode kError_CommNotOpen = kError_EmuErrBase + 16;
const ErrCode kError_CantOpenPort = kError_EmuErrBase + 17;
const ErrCode kError_InvalidPort = kError_EmuErrBase + 18;
const ErrCode kError_CantFindSerialPort = kError_EmuErrBase + 19;

const ErrCode kError_OnlySameType = kError_EmuErrBase + 20;
const ErrCode kError_OnlyOnePSF = kError_EmuErrBase + 21;
const ErrCode kError_OnlyOneROM = kError_EmuErrBase + 22;
const ErrCode kError_UnknownType = kError_EmuErrBase + 23;

const ErrCode kError_BadDB_NameNotNULLTerminated = kError_EmuErrBase + 30;
const ErrCode kError_BadDB_NameNotPrintable = kError_EmuErrBase + 31;
const ErrCode kError_BadDB_FileTooSmall = kError_EmuErrBase + 32;
const ErrCode kError_BadDB_nextRecordListIDNonZero = kError_EmuErrBase + 33;
const ErrCode kError_BadDB_ResourceTooSmall = kError_EmuErrBase + 34;
const ErrCode kError_BadDB_RecordTooSmall = kError_EmuErrBase + 35;
const ErrCode kError_BadDB_ResourceOutOfRange = kError_EmuErrBase + 36;
const ErrCode kError_BadDB_RecordOutOfRange = kError_EmuErrBase + 37;
const ErrCode kError_BadDB_OverlappingResource = kError_EmuErrBase + 38;
const ErrCode kError_BadDB_OverlappingRecord = kError_EmuErrBase + 39;
const ErrCode kError_BadDB_ResourceMemError = kError_EmuErrBase + 40;
const ErrCode kError_BadDB_RecordMemError = kError_EmuErrBase + 41;
const ErrCode kError_BadDB_AppInfoMemError = kError_EmuErrBase + 42;
const ErrCode kError_BadDB_DuplicateResource = kError_EmuErrBase + 43;

const ErrCode kError_CorruptedHeap_ChunkNotInHeap = kError_EmuErrBase + 50;
const ErrCode kError_CorruptedHeap_ChunkTooLarge = kError_EmuErrBase + 51;
const ErrCode kError_CorruptedHeap_InvalidFlags = kError_EmuErrBase + 52;
const ErrCode kError_CorruptedHeap_HOffsetNotInMPT = kError_EmuErrBase + 53;
const ErrCode kError_CorruptedHeap_HOffsetNotBackPointing = kError_EmuErrBase + 54;
const ErrCode kError_CorruptedHeap_InvalidLockCount = kError_EmuErrBase + 55;

const ErrCode kError_PowerOff = kError_EmuErrBase + 56;

const ErrCode kError_PalmOSErrBase = 0xFFFC0000;  // arbitrary value
const ErrCode kError_PalmOSErrRange = (1 << (sizeof(Err) * 8));

const ErrCode kError_StdCErrBase = 0xFFFA0000;   // arbitrary value
const ErrCode kError_StdCErrRange = 0x00010000;  // arbitrary range

inline ErrCode ConvertFromStdCError(int err) {
    return (ErrCode)err ? (err + kError_StdCErrBase) : err;
}

inline int ConvertToStdCError(ErrCode err) { return (int)err ? (err - kError_StdCErrBase) : err; }

inline ErrCode ConvertFromPalmError(Err err) {
    return (ErrCode)err ? (err + kError_PalmOSErrBase) : err;
}

inline Err ConvertToPalmError(ErrCode err) { return (Err)err ? (err - kError_PalmOSErrBase) : err; }

inline Bool IsStdCError(ErrCode err) {
    return err >= kError_StdCErrBase && err < kError_StdCErrBase + kError_StdCErrRange;
}

inline Bool IsPalmError(ErrCode err) {
    return err >= kError_PalmOSErrBase && err < kError_PalmOSErrBase + kError_PalmOSErrRange;
}

inline Bool IsEmuError(ErrCode err) {
    return err >= kError_EmuErrBase && err < kError_EmuErrBase + kError_EmuErrRange;
}

inline Bool IsPlatformError(ErrCode err) {
    return !::IsPalmError(err) && !::IsStdCError(err) && !::IsEmuError(err);
}

#endif  // EmErrCodes_h
