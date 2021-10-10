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
#ifndef EmRegs330CPLD_h
#define EmRegs330CPLD_h

#include "EmHandEra330Defs.h"
#include "EmRegs.h"

#define CpldReg00 0x0000
// CPLD Register 0x0000 Bit Definitions
// -------------------------------------
#define Cpld0Edo 0x0002
#define Cpld0Sdram 0x0000

#define CpldReg02 0x0002
// CPLD Register 0x0002 Bit Definitions
// -------------------------------------
#define Cpld2NoCfDetect 0x8000
#define Cpld2CfDetect 0x0000

#define Cpld2NoSdDetect 0x4000
#define Cpld2SdDetect 0x0000

#define Cpld2NoExPwrDetect 0x2000
#define Cpld2ExPwrDetect 0x0000

#define Cpld2SdUnwriteProt 0x0000
#define Cpld2SdWriteProt 0x1000

#define Cpld2SdPowerOn 0x0000
#define Cpld2SdPowerOff 0x0200

#define Cpld2Kbd3Active 0x0000
#define Cpld2Kbd3Inactive 0x0100

#define Cpld2Kbd2Active 0x0000
#define Cpld2Kbd2Inactive 0x0080

#define Cpld2Kbd1Active 0x0000
#define Cpld2Kbd1Inactive 0x0040

#define Cpld2Kbd0Active 0x0000
#define Cpld2Kbd0Inactive 0x0020

#define Cpld2NoReset 0x0000
#define Cpld2Reset 0x0010

#define Cpld2CfBufsOff 0x0000
#define Cpld2CfBufsOn 0x0008

#define Cpld2SwapOff 0x0000
#define Cpld2SwapOn 0x0004

#define Cpld2CfPowerOn 0x0000
#define Cpld2CfPowerOff 0x0002

#define Cpld2BusWidth16 0x0000
#define Cpld2BusWidth8 0x0001

#define CpldReg04 0x0004
// CPLD Register 0x0004 Bit Definitions
// -------------------------------------
#define Cpld4LcdBiasOff 0x0000
#define Cpld4LcdBiasOn 0x0400

#define Cpld4LcdVccOn 0x0000
#define Cpld4LcdVccOff 0x0200

#define Cpld4LcdOff 0x0000
#define Cpld4LcdON 0x0100

#define Cpld4BlPdOff 0x0000
#define Cpld4BlPdOn 0x0080

#define Cpld4DtrOn 0x0000
#define Cpld4DtrOff 0x0040

#define Cpld4MmcCsOff 0x0000
#define Cpld4MmcCsOn 0x0020

#define Cpld4FiltSdOff 0x0000
#define Cpld4FiltSdOn 0x0010

#define Cpld4Rs232Off 0x0000
#define Cpld4Rs232On 0x0008

#define Cpld4IrdaOn 0x0000
#define Cpld4IrdaOff 0x0004

#define Cpld4MicOn 0x0000
#define Cpld4MicOff 0x0002

#define Cpld4SenseVoltage 0x0000
#define Cpld4SenseCurrent 0x0001

const uint32 kMemoryStartCPLD = 0x10E00000;
const uint32 kMemorySizeCPLD = 0x100;

class EmRegs330CPLD : public EmRegs {
   public:
    EmRegs330CPLD(HandEra330PortManager* fPortManager);
    virtual ~EmRegs330CPLD(void);

    virtual void Save(Savestate&);
    virtual void Save(SavestateProbe&);
    virtual void Load(SavestateLoader&);

    virtual void Reset(Bool hardwareReset);

    uint8* GetRealAddress(emuptr address);
    void SetSubBankHandlers(void);
    emuptr GetAddressStart(void);
    uint32 GetAddressRange(void);

    uint32 GetWord(emuptr iAddress);
    void SetWord(emuptr iAddress, uint32 iWordValue);

   private:
    uint32 Read(emuptr address, int size);
    void Write(emuptr address, int size, uint32 value);

    template <typename T>
    void DoSave(T& savestate);

    template <typename T>
    void DoSaveLoad(T& helper);

    // only word access is allowed to the CPLD
    //    uint32          GetLong                 (emuptr iAddress) { return 0;}
    //    uint32          GetByte                 (emuptr iAddress) { return 0;}
    //    void            SetLong                 (emuptr iAddress, uint32 iLongValue) {}
    //    void            SetByte                 (emuptr iAddress, uint32 iByteValue) {}
   private:
    uint16 Reg0;
    uint16 Reg2;
    uint16 Reg4;
    uint8 Buffer[kMemorySizeCPLD];
    HandEra330PortManager* fPortMgr;
};

#endif  // EmTRGCFDefs_h
