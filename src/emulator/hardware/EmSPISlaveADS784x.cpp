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

#include "EmSPISlaveADS784x.h"

#include "EmCommon.h"
#include "EmLowMem.h"       // EmLowMem_GetGlobal
#include "EmMemory.h"       // CEnableFullAccess
#include "EmPalmStructs.h"  // EmAliasSysBatteryDataStruct
#include "Logging.h"

// #define LOGGING 1
#ifdef LOGGING
    #define PRINTF logging::printf
#else
    #define PRINTF(...) ;
#endif

/********************************************************************
 * Single-Ended Mode Channel Constants (adcSERDFR = 1)
 ********************************************************************/
#define adcSERTemp0 0x04  // Temperature 0
#define adcSERYPos 0x14   // Y-Position
#define adcSERBat 0x24    // Battery
#define adcSERZ1Pos 0x34  // Z1-Position
#define adcSERZ2Pos 0x44  // Z2-Position
#define adcSERXPos 0x54   // X-Position
#define adcSERAux 0x64    // Auxilliary
#define adcSERTemp1 0x74  // Temperature 1

/********************************************************************
 * Differential Mode Channel Constants (adcSERDFR = 0)
 ********************************************************************/
#define adcDFRYPos 0x10   // Y-Position
#define adcDFRZ1Pos 0x30  // Z1-Position
#define adcDFRZ2Pos 0x40  // Z2-Position
#define adcDFRXPos 0x50   // X-Position

/********************************************************************
 * Conversion Mode Resolution
 ********************************************************************/
#define adcMode12Bit 0x00  // 12-bit conversion
#define adcMode8Bit 0x08   // 8-bit conversion

/********************************************************************
 * Power Down Bit Use (for Burr-Brown ADS7846)
 ********************************************************************/
#define adcPDADCOn 0x01        // If set, turn on the ADC converter
#define adcPDReferenceOn 0x02  // If set, the internal Vref reference is turned on

#define kChannelBits 0x70

// ---------------------------------------------------------------------------
//		� EmSPISlaveADS784x::EmSPISlaveADS784x
// ---------------------------------------------------------------------------

EmSPISlaveADS784x::EmSPISlaveADS784x(EmADSChannelType ch0, EmADSChannelType ch1,
                                     EmADSChannelType ch2, EmADSChannelType ch3,
                                     EmADSChannelType ch4, EmADSChannelType ch5,
                                     EmADSChannelType ch6, EmADSChannelType ch7)
    : EmSPISlave(),
      fBitBufferIn(0),
      fBitBufferOut(0),
      fNumBitsIn(0),
      fPendingResult(0),
      fHavePending(false),
      fCommandBitsSeen(0) {
    fChannelUse[0] = ch0;
    fChannelUse[1] = ch1;
    fChannelUse[2] = ch2;
    fChannelUse[3] = ch3;
    fChannelUse[4] = ch4;
    fChannelUse[5] = ch5;
    fChannelUse[6] = ch6;
    fChannelUse[7] = ch7;

    // If there's a 7846-style battery, it *must* be on channel 2.

    for (int ii = 0; ii < 8; ++ii) {
        if (fChannelUse[ii] == kChannelBattery7846) {
            EmAssert(ii == 2);
        }
    }
}

// ---------------------------------------------------------------------------
//		� EmSPISlaveADS784x::~EmSPISlaveADS784x
// ---------------------------------------------------------------------------

EmSPISlaveADS784x::~EmSPISlaveADS784x(void) {}

// ---------------------------------------------------------------------------
//		� EmSPISlaveADS784x::DoExchange
// ---------------------------------------------------------------------------

uint16 EmSPISlaveADS784x::DoExchange(uint16 control, uint16 data) {
    PRINTF("");
    PRINTF("EmSPISlaveADS784x::DoExchange");
    PRINTF("control = 0x%04X, data = 0x%04X", control, data);

    // -----------------------------------------------------------------------
    // Merge the incoming bits with our current buffer.
    // -----------------------------------------------------------------------

    uint16 numBits = (control & hwrVZ328SPIMControlBitsMask) + 1;
    uint32 oldBitsMask = ~0u << numBits;
    uint32 newBitsMask = ~oldBitsMask;

    PRINTF("Before merging input:   fBitBufferIn = 0x%04X, fNumBitsIn = 0x%04X", fBitBufferIn,
           fNumBitsIn);

    fBitBufferIn = ((fBitBufferIn << numBits) & oldBitsMask) | (data & newBitsMask);
    fNumBitsIn += numBits;

    PRINTF("After merging input:    fBitBufferIn = 0x%04X, fNumBitsIn = 0x%04X", fBitBufferIn,
           fNumBitsIn);

    // -----------------------------------------------------------------------
    // Start processing the command bits.
    // -----------------------------------------------------------------------

    EmAssert(fNumBitsIn - fCommandBitsSeen - 1 >= 0);

    uint16 result = 0;
    uint32 mask = 1 << (fNumBitsIn - fCommandBitsSeen - 1);

    while (mask) {
        // Shift out a bit.

        {
            result = (result << 1) | (fBitBufferOut >> 15);
            fBitBufferOut <<= 1;
        }

        // If we haven't seen the Start bit yet, look for it.

        if (fCommandBitsSeen == 0) {
            // If we found the Start bit, start counting the
            // number of command bits as we stream through them.

            if ((mask & fBitBufferIn) != 0) {
                fCommandBitsSeen++;
            }

            // Otherwise, adjust fNumBitsIn so that when we *do*
            // find the Start bit, we know where it is.

            else {
                fNumBitsIn--;
            }

            // If there's a pending conversion, load it into the
            // output shift register after receiving the first
            // bit after the last bit of the previous command.

            this->LoadPendingConversion();
        } else {
            fCommandBitsSeen++;

            // If we've seen 8 bits, process the command, and then
            // prepare for the next one.

            if (fCommandBitsSeen == 8) {
                fNumBitsIn -= 8;
                fCommandBitsSeen = 0;

                uint8 command = fBitBufferIn >> fNumBitsIn;
                this->ProcessCommand(command);

                PRINTF("After ProcessCommand:   fPendingResult = 0x%04X", fPendingResult);
            }
        }

        mask >>= 1;
    }

    // ----------------------------------------------------------------------
    // Return the result.
    // ----------------------------------------------------------------------

    PRINTF("result = 0x%04X", result);

    return result;
}

// ---------------------------------------------------------------------------
//		� EmSPISlaveADS784x::ProcessCommand
// ---------------------------------------------------------------------------

void EmSPISlaveADS784x::ProcessCommand(uint8 command) {
    uint16 result = 0;

    /*
            Command format is:

                    +-----+-----+-----+-----+-----+-----+-----+-----+
                    |  S  |  A2 |  A1 |  A0 |  M  | Ref | PD1 | PD0 |
                    +-----+-----+-----+-----+-----+-----+-----+-----+

                    S		= Start Bit.  Must be 1.
                    A2:0	= Channel select bits
                    M		= 12/8 bit mode (1 = 8-bit)
                    Ref		= Single-ended/Differential reference select bit
                    PD1:0	= Power down mode select bits.

            We care about the A2:0 bits, since they tell us what data
            is being asked for.  We also care about the M bit, as that
            determines how many bits we load into fBitBufferOut.
    */

    int channel = (command & kChannelBits) >> 4;

    EmAssert(channel >= 0 && channel <= 7);

    switch (fChannelUse[channel]) {
        case kChannelPenX:
        case kChannelPenY: {
            result = 0;
            break;
        }

        case kChannelBattery7843:
        case kChannelBattery7846: {
            /*
                    The current batterly level is read with the following:

                            currentLevel = PrvAverageBattery ( );

                    PrvAverageBattery() reads the ADC battery value and then
                    uses a 7/8 averaging method to merge the new value into
                    previously averaged values.

                    "currentLevel" is then used to determine "batteryLevel"
                    as follows:

                            batteryLevel = (((UInt16)currentLevel +
               (battDataP->sysBattVoltageStepOffset))
                                                    * 100 + (battDataP->sysBattStepsPerVolt)/2)
                                                    / (battDataP->sysBattStepsPerVolt);

                    "batteryLevel" is then used to search through the sysBattVoltageCurve
                    array to determine the percent charge.

                    Thus, in order to determine the "currentLevel" for a desired
                    "batteryLevel", we solve for it, getting:

                            currentLevel = (batteryLevel * battDataP->sysBattStepsPerVolt + 50) /
               100 - battDataP->sysBattVoltageStepOffset;

                    We can then get the "batteryLevel" from the voltage curve
                    array in the battery table.  For example, for a 100% charge,
                    read sysBattVoltageCurve[10].
            */

            CEnableFullAccess munge;

            // Get a pointer to the system battery globals and determine
            // what version table we're using.

            emuptr sysBatteryDataP = EmLowMem_GetGlobal(sysBatteryDataP);

            if (sysBatteryDataP) {
                EmAliasSysBatteryDataStructV1<PAS> sysBatteryData(sysBatteryDataP);

                UInt16 sysBattDataStructVersion = sysBatteryData.sysBattDataStructVersion;

                if (sysBattDataStructVersion <= 3) {
                    UInt16 voltageCurve = 0;
                    UInt16 sysBattStepsPerVolt = 0;
                    Int16 sysBattVoltageStepOffset = 0;

                    // Fill out the above variables from the system battery globals,
                    // sorting out table version differences.

                    if (sysBattDataStructVersion == 1) {
                        EmAliasSysBatteryDataStructV1<PAS> sysBatteryDataV1(sysBatteryDataP);

                        voltageCurve = sysBatteryDataV1.sysBattVoltageCurve[10];
                        sysBattStepsPerVolt = sysBatteryDataV1.sysBattStepsPerVolt;
                        sysBattVoltageStepOffset = sysBatteryDataV1.sysBattVoltageStepOffset;
                    } else if (sysBattDataStructVersion == 2) {
                        EmAliasSysBatteryDataStructV2<PAS> sysBatteryDataV2(sysBatteryDataP);

                        voltageCurve = sysBatteryDataV2.sysBattVoltageCurve[10];
                        sysBattStepsPerVolt = sysBatteryDataV2.sysBattStepsPerVolt;
                        sysBattVoltageStepOffset = sysBatteryDataV2.sysBattVoltageStepOffset;
                    } else if (sysBattDataStructVersion == 3) {
                        EmAliasSysBatteryDataStructV3<PAS> sysBatteryDataV3(sysBatteryDataP);

                        voltageCurve = sysBatteryDataV3.sysBattVoltageCurve[10];
                        sysBattStepsPerVolt = sysBatteryDataV3.sysBattStepsPerVolt;
                        sysBattVoltageStepOffset = sysBatteryDataV3.sysBattVoltageStepOffset;
                    } else {
                        EmAssert(false);
                    }

                    PRINTF("voltageCurve = %d", voltageCurve);
                    PRINTF("sysBattStepsPerVolt = %d", sysBattStepsPerVolt);
                    PRINTF("sysBattVoltageStepOffset = %d", sysBattVoltageStepOffset);

                    // Determine the result based on the formula in the comments above.

                    result =
                        (voltageCurve * sysBattStepsPerVolt + 50) / 100 - sysBattVoltageStepOffset;

                    // Turn this into a 12 bit result.

                    result <<= 4;

                    if (fChannelUse[channel] == kChannelBattery7846) {
                        result *= 2;  // Account for the fact that the Palm OS battery
                                      // tables assume a 5.0V reference voltage, while
                                      // the 7846 uses 2.5V. (?)
                        result /= 4;  // Account for 1:4 voltage divider
                    }
                } else {
                    result = 0x0FFF;
                }
            } else {
                result = 0x0FFF;
            }

            PRINTF("result = 0x%04X", result);
            break;
        }

        case kChannelDockSerial: {
            // Say that we're undocked.

            result = 0x0000;
            break;
        }

        case kChannelDockTwister: {
            // Say that we're undocked.

            result = 0x0FFF;
            break;
        }

        case kChannelTemp0: {
            result = 0x0FFF;  // !!! Dummy value; need to determine real value
            break;
        }

        case kChannelTemp1: {
            result = 0x0FFF;  // !!! Dummy value; need to determine real value
            break;
        }

        default:
            EmAssert(false);
            break;
    }

    fPendingResult = result << 4;
    fHavePending = true;
}

// ---------------------------------------------------------------------------
//		� EmSPISlaveADS784x::LoadPendingConversion
// ---------------------------------------------------------------------------

void EmSPISlaveADS784x::LoadPendingConversion(void) {
    // -----------------------------------------------------------------------
    // If there's a pending conversion, move it into the output shift register.
    // -----------------------------------------------------------------------

    if (fHavePending) {
        PRINTF("Before merging pending: fBitBufferOut = 0x%04X", fBitBufferOut);

        fHavePending = false;
        fBitBufferOut = fPendingResult;

        PRINTF("After merging pending:  fBitBufferOut = 0x%04X", fBitBufferOut);
    }
}
