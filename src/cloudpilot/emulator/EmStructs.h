/* -*- mode: C++; tab-width: 4 -*- */
/* ===================================================================== *\
        Copyright (c) 2000-2001 Palm, Inc. or its subsidiaries.
        All rights reserved.

        This file is part of the Palm OS Emulator.

        This program is free software; you can redistribute it and/or modify
        it under the terms of the GNU General Public License as published by
        the Free Software Foundation; either version 2 of the License, or
        (at your option) any later version.
\* ===================================================================== */

#ifndef EmStructs_h
#define EmStructs_h

#include <deque>
#include <map>
#include <string>
#include <vector>

#include "EmDevice.h"  // EmDevice
#include "EmTypes.h"   // GremlinEventType, EventIDType

// ---------- Collections ----------

typedef vector<ScaleType> ScaleList;
typedef vector<RAMSizeType> RAMSizeList;

typedef vector<uint8> ByteList;
typedef vector<string> StringList;

// ---------- RGBType ----------
struct RGBType {
    RGBType(void) {}
    RGBType(uint8 red, uint8 green, uint8 blue) : fRed(red), fGreen(green), fBlue(blue) {}

    bool operator==(const RGBType& rhs) const {
        return fRed == rhs.fRed && fGreen == rhs.fGreen && fBlue == rhs.fBlue;
    }

    bool operator!=(const RGBType& rhs) const { return !(*this == rhs); }

    uint8 fRed;
    uint8 fGreen;
    uint8 fBlue;
    uint8 fFiller;  // Pad this out to 4 bytes to make array lookups more efficient.
};
typedef vector<RGBType> RGBList;

// ---------- SystemCallContext ----------

struct SystemCallContext {
    emuptr fPC;         // PC at which the system call was made.
    emuptr fNextPC;     // PC of instruction after system call.
    uint16 fTrapWord;   // Trapword, even for SYSTRAP_FAST calls.
    uint16 fTrapIndex;  // Trapword with the high 4 bits removed.
    emuptr fExtra;      // RefNum for library calls, D2 for "dispatch" calls.
    Bool fViaTrap;      // True if called via TRAP $F.
    Bool fViaJsrA1;     // True if called via SYSTRAP_FASTER.
};
typedef vector<SystemCallContext> SystemCallContextList;

// ---------- DatabaseInfo ----------

struct DatabaseInfo {
    UInt32 creator;
    UInt32 type;
    UInt16 version;
    LocalID dbID;
    UInt16 cardNo;
    UInt32 modDate;
    UInt16 dbAttrs;

    char name[dmDBNameLength];
    char dbName[dmDBNameLength];

    Bool operator<(const DatabaseInfo& other) const {
        if (creator < other.creator) return true;
        if (creator > other.creator) return false;
        if (type < other.type) return true;
        return false;
    }

    Bool operator==(const DatabaseInfo& other) const {
        return (creator == other.creator) && (type == other.type);
    }
};
typedef vector<DatabaseInfo> DatabaseInfoList;

#if 0  // CSTODO
// ---------- SlotInfoType ----------

struct SlotInfoType {
    long fSlotNumber;
    Bool fSlotOccupied;
    EmDirRef fSlotRoot;
};
typedef vector<SlotInfoType> SlotInfoList;

// ---------- Configuration ----------

struct Configuration {
    Configuration() : fDevice(), fRAMSize(1024), fROMFile() {}

    Configuration(const Configuration& cfg)
        : fDevice(cfg.fDevice), fRAMSize(cfg.fRAMSize), fROMFile(cfg.fROMFile) {}

    Configuration(const EmDevice& dt, long size, const EmFileRef& rom)
        : fDevice(dt), fRAMSize(size), fROMFile(rom) {}

    Bool IsValid(void) const {
        // Return whether or not the configuration looks
        // valid.

        if (!fDevice.Supported()) return false;

        if (fRAMSize <= 0) return false;

        if (!fROMFile.IsSpecified()) return false;

        if (!fROMFile.Exists()) return false;

        if (!fDevice.SupportsROM(fROMFile)) return false;

        return true;
    }

    EmDevice fDevice;
    RAMSizeType fRAMSize;
    EmFileRef fROMFile;
};
typedef vector<Configuration> ConfigurationList;
#endif

// ---------- SysLibTblEntryTypeV10 ----------

typedef struct SysLibTblEntryTypeV10 {
    MemPtr* dispatchTblP;  // pointer to library dispatch table
    void* globalsP;        // Library globals
} SysLibTblEntryTypeV10;
typedef SysLibTblEntryTypeV10* SysLibTblEntryV10Ptr;

// ---------- EmStackFrame ----------

struct EmStackFrame {
    emuptr fAddressInFunction;
    emuptr fA6;
};

typedef vector<EmStackFrame> EmStackFrameList;

#endif  // EmStructs_h
