/* -*- mode: C++; tab-width: 4 -*- */
/* ===================================================================== *\
        Copyright (c) 1999-2001 Palm, Inc. or its subsidiaries.
        All rights reserved.

        This file is part of the Palm OS Emulator.

        This program is free software; you can redistribute it and/or modify
        it under the terms of the GNU General Public License as published by
        the Free Software Foundation; either version 2 of the License, or
        (at your option) any later version.
\* ===================================================================== */

#ifndef EmDevice_h
#define EmDevice_h

#include <string>   // string
#include <utility>  // pair
#include <vector>   // vector

#include "EmCommon.h"
#include "ScreenDimensions.h"

class EmCPU;
class EmRegs;
class EmSession;
struct DeviceInfo;

class EmROMReader;

class EmDevice;
typedef vector<EmDevice> EmDeviceList;

class EmDevice {
   public:
    EmDevice(void);
    EmDevice(const char*);
    EmDevice(const string&);
    EmDevice(const EmDevice&);
    ~EmDevice(void);

    bool operator==(const EmDevice&) const;
    bool operator!=(const EmDevice&) const;

    Bool Supported(void) const;

    Bool Supports68328(void) const;
    Bool Supports68EZ328(void) const;
    Bool Supports68VZ328(void) const;
    Bool Supports68SZ328(void) const;

    Bool PrismPlatinumEdgeHack(void) const;
    Bool EdgeHack(void) const;

    Bool SupportsROM(const EmROMReader&) const;

    EmCPU* CreateCPU(EmSession*) const;
    void CreateRegs(void) const;

    Bool HasFlash(void) const;

    RAMSizeType MinRAMSize(void) const;

    long HardwareID(void) const;
    long HardwareSubID(void) const;

    string GetMenuString(void) const;
    string GetIDString(void) const;
    vector<string> GetIDStrings(void) const;

    bool IsValid() const;
    bool EmulatesDockStatus() const;
    bool NeedsSDCTLHack() const;

    ScreenDimensions::Kind GetScreenDimensions() const;

   public:
    static EmDeviceList GetDeviceList(void);

   private:
    EmDevice(int);
    const DeviceInfo* GetDeviceInfo(void) const;
    int GetDeviceID(const char*) const;

    int fDeviceID;
};

#endif /* EmDevice_h */
