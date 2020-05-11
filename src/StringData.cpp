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

#include "StringData.h"

#include "EmCommon.h"

// ---------------------------------------------------------------------------
//		� Names of exception types
// ---------------------------------------------------------------------------

const char* kExceptionNames[] = {"initStack",  // initial stack pointer
                                 "initPC",     // initial PC

                                 "busErr",          // 08 / 2
                                 "addressErr",      // 0C / 3
                                 "illegalInstr",    // 10 / 4
                                 "divideByZero",    // 14 / 5
                                 "chk",             // 18 / 6
                                 "trap",            // 1C / 7
                                 "privilege",       // 20 / 8
                                 "trace",           // 24 / 9
                                 "aTrap",           // 28 / A
                                 "fTrap",           // 2C / B
                                 "reserved12",      // 30 / C
                                 "coproc",          // 34 / D
                                 "formatErr",       // 38 / E
                                 "unitializedInt",  // 3C / F

                                 "reserved0",  // 40-5C / 10-17
                                 "reserved1",
                                 "reserved2",
                                 "reserved3",
                                 "reserved4",
                                 "reserved5",
                                 "reserved6",
                                 "reserved7",

                                 "spuriousInt",  // 60 / 18
                                 "autoVec1",     // 64 / 19
                                 "autoVec2",     // 68 / 1A
                                 "autoVec3",     // 6C / 1B
                                 "autoVec4",     // 70 / 1C
                                 "autoVec5",     // 74 / 1D
                                 "autoVec6",     // 78 / 1E
                                 "autoVec7",     // 7C / 1F

                                 "trap0",  // 80 - BC / 20 - 2F
                                 "trap1",
                                 "trap2",
                                 "trap3",
                                 "trap4",
                                 "trap5",
                                 "trap6",
                                 "trap7",
                                 "trap8",
                                 "trap9",
                                 "trap10",
                                 "trap11",
                                 "trap12",
                                 "trap13",
                                 "trap14",
                                 "trap15",

                                 "unassigned0",  // C0 - FC / 30 - 3F
                                 "unassigned1",
                                 "unassigned2",
                                 "unassigned3",
                                 "unassigned4",
                                 "unassigned5",
                                 "unassigned6",
                                 "unassigned7",
                                 "unassigned8",
                                 "unassigned9",
                                 "unassigned10",
                                 "unassigned11",
                                 "unassigned12",
                                 "unassigned13",
                                 "unassigned14",
                                 "unassigned15"};

// ---------------------------------------------------------------------------
//		� Names of system packets
// ---------------------------------------------------------------------------

const char* kPacketNames[256] = {
    "sysPktStateCmd",                                // 0x00
    "sysPktReadMemCmd",                              // 0x01
    "sysPktWriteMemCmd",                             // 0x02
    "sysPktSingleStepCmd",                           // 0x03
    "sysPktGetRtnNameCmd",                           // 0x04
    "sysPktReadRegsCmd",                             // 0x05
    "sysPktWriteRegsCmd",                            // 0x06
    "sysPktContinueCmd",                             // 0x07
    NULL,                                            // 0x08
    NULL,                                            // 0x09
    "sysPktRPCCmd",                                  // 0x0A
    "sysPktGetBreakpointsCmd",                       // 0x0B
    "sysPktSetBreakpointsCmd",                       // 0x0C
                                                     //	"sysPktRemoteUIUpdCmd", 		// 0x0B
                                                     //	"sysPktRemoteEvtCmd",			// 0x0C
    "sysPktDbgBreakToggleCmd",                       // 0x0D
    "sysPktFlashCmd",                                // 0x0E
    "sysPktCommCmd",                                 // 0x0F
    "sysPktGetTrapBreaksCmd",                        // 0x10
    "sysPktSetTrapBreaksCmd",                        // 0x11
    "sysPktGremlinsCmd",                             // 0x12
    "sysPktFindCmd",                                 // 0x13
    "sysPktGetTrapConditionsCmd",                    // 0x14
    "sysPktSetTrapConditionsCmd",                    // 0x15
    "sysPktChecksumCmd",                             // 0x16
    "sysPktExecFlashCmd",                            // 0x17
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,  // 0x18 - 0x1F
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,  // 0x20 - 0x27
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,  // 0x28 - 0x2F
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,  // 0x30 - 0x37
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,  // 0x38 - 0x3F
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,  // 0x40 - 0x47
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,  // 0x48 - 0x4F
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,  // 0x50 - 0x57
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,  // 0x58 - 0x5F
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,  // 0x60 - 0x67
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,  // 0x68 - 0x6F
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,  // 0x70 - 0x77
    NULL, NULL, NULL, NULL, NULL, NULL, NULL,        // 0x78 - 0x7E
    "sysPktRemoteMsg",                               // 0x7F

    "sysPktStateRsp",                                // 0x80
    "sysPktReadMemRsp",                              // 0x81
    "sysPktWriteMemRsp",                             // 0x82
    "sysPktSingleStepRsp",                           // 0x83
    "sysPktGetRtnNameRsp",                           // 0x84
    "sysPktReadRegsRsp",                             // 0x85
    "sysPktWriteRegsRsp",                            // 0x86
    "sysPktContinueRsp",                             // 0x87
    NULL,                                            // 0x88
    NULL,                                            // 0x89
    "sysPktRPCRsp",                                  // 0x8A
    "sysPktGetBreakpointsRsp",                       // 0x8B
    "sysPktSetBreakpointsRsp",                       // 0x8C
                                                     //	"sysPktRemoteUIUpdRsp", 		// 0x8B
                                                     //	"sysPktRemoteEvtRsp",			// 0x8C
    "sysPktDbgBreakToggleRsp",                       // 0x8D
    "sysPktFlashRsp",                                // 0x8E
    "sysPktCommRsp",                                 // 0x8F
    "sysPktGetTrapBreaksRsp",                        // 0x90
    "sysPktSetTrapBreaksRsp",                        // 0x91
    "sysPktGremlinsRsp",                             // 0x92
    "sysPktFindRsp",                                 // 0x93
    "sysPktGetTrapConditionsRsp",                    // 0x94
    "sysPktSetTrapConditionsRsp",                    // 0x95
    "sysPktChecksumRsp",                             // 0x96
    "sysPktExecFlashRsp",                            // 0x97
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,  // 0x98 - 0x9F
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,  // 0xA0 - 0xA7
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,  // 0xA8 - 0xAF
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,  // 0xB0 - 0xB7
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,  // 0xB8 - 0xBF
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,  // 0xC0 - 0xC7
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,  // 0xC8 - 0xCF
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,  // 0xD0 - 0xD7
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,  // 0xD8 - 0xDF
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,  // 0xE0 - 0xE7
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,  // 0xE8 - 0x6F
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,  // 0xF0 - 0xF7
    NULL, NULL, NULL, NULL, NULL, NULL, NULL,        // 0xF8 - 0xFE
    "sysPktRemoteMsg"                                // 0xFF
};

// ---------------------------------------------------------------------------
//		� VirtualKeyDescriptions
// ---------------------------------------------------------------------------
// Return a key description

const char* kVirtualKeyDescriptions[] = {
    " (vchrLowBattery)",           // 0x0101
    " (vchrEnterDebugger)",        // 0x0102
    " (vchrNextField)",            // 0x0103
    " (vchrStartConsole)",         // 0x0104
    " (vchrMenu)",                 // 0x0105
    " (vchrCommand)",              // 0x0106
    " (vchrConfirm)",              // 0x0107
    " (vchrLaunch)",               // 0x0108
    " (vchrKeyboard)",             // 0x0109
    " (vchrFind)",                 // 0x010A
    " (vchrCalc)",                 // 0x010B
    " (vchrPrevField)",            // 0x010C
    " (vchrAlarm)",                // 0x010D
    " (vchrRonamatic)",            // 0x010E
    " (vchrGraffitiReference)",    // 0x010F
    " (vchrKeyboardAlpha)",        // 0x0110
    " (vchrKeyboardNumeric)",      // 0x0111
    " (vchrLock)",                 // 0x0112
    " (vchrBacklight)",            // 0x0113
    " (vchrAutoOff)",              // 0x0114
    " (vchrExgTest)",              // 0x0115
    " (vchrSendData)",             // 0x0116
    " (vchrIrReceive)",            // 0x0117
    " (vchrTsm1)",                 // 0x0118
    " (vchrTsm2)",                 // 0x0119
    " (vchrTsm3)",                 // 0x011A
    " (vchrTsm4)",                 // 0x011B
    " (vchrRadioCoverageOK)",      // 0x011C
    " (vchrRadioCoverageFail)",    // 0x011D
    " (vchrPowerOff)",             // 0x011E
    " (vchrResumeSleep)",          // 0x011F
    " (vchrLateWakeup)",           // 0x0120
    " (vchrTsmMode)",              // 0x0121
    " (vchrBrightness)",           // 0x0122
    " (vchrContrast)",             // 0x0123
    " (vchrExpCardInserted)",      // 0x0124
    " (vchrExpCardRemoved)",       // 0x0125
    " (vchrAttnStateChanged)",     // 0x0126
    " (vchrAttnUnsnooze)",         // 0x0127
    " (vchrAttnIndicatorTapped)",  // 0x0128
    " (vchrAttnAllowClose)",       // 0x0129
    " (vchrAttnReopen)",           // 0x012A
    " (vchrCardCloseMenu)",        // 0x012B
    " (vchrIrGotData)"             // 0x012C
};

const size_t gVirtualKeyDescriptionsCount = countof(kVirtualKeyDescriptions);

const char* kHardKeyDescriptions[] = {
    " (irObCloseChr)",       // 0x01FB
    " (irGotDataChr)",       // 0x01FC - vchrIrGotData in 4.0
    " (irInitLibChr)",       // 0x01FD
    " (irSerialChr)",        // 0x01FE
    " (vchrExgIntData)",     // 0x01FF
    "",                      // 0x0200
    "",                      // 0x0201
    "",                      // 0x0202
    "",                      // 0x0203
    " (vchrHard1)",          // 0x0204
    " (vchrHard2)",          // 0x0205
    " (vchrHard3)",          // 0x0206
    " (vchrHard4)",          // 0x0207
    " (vchrHardPower)",      // 0x0208
    " (vchrHardCradle)",     // 0x0209
    " (vchrHardCradle2)",    // 0x020A
    " (vchrHardContrast)",   // 0x020B
    " (vchrHardAntenna)",    // 0x020C
    " (vchrHardBrightness)"  // 0x020D
    " (vchrPageUpWhenOff)"   // 0x020E
    " (vchrHardEarbud)"      // 0x020F
};

const size_t gHardKeyDescriptionsCount = countof(kHardKeyDescriptions);
