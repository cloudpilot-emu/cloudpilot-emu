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

#include "Byteswapping.h"

#include "EmCommon.h"

#if BYTESWAP

    #include "UAE.h"  // regstruct

void Byteswap(HwrM68328Type& regs) {
    Byteswap(regs.csAGroupBase);
    Byteswap(regs.csBGroupBase);
    Byteswap(regs.csCGroupBase);
    Byteswap(regs.csDGroupBase);

    Byteswap(regs.csAGroupMask);
    Byteswap(regs.csBGroupMask);
    Byteswap(regs.csCGroupMask);
    Byteswap(regs.csDGroupMask);

    Byteswap(regs.csASelect0);
    Byteswap(regs.csASelect1);
    Byteswap(regs.csASelect2);
    Byteswap(regs.csASelect3);

    Byteswap(regs.csBSelect0);
    Byteswap(regs.csBSelect1);
    Byteswap(regs.csBSelect2);
    Byteswap(regs.csBSelect3);

    Byteswap(regs.csCSelect0);
    Byteswap(regs.csCSelect1);
    Byteswap(regs.csCSelect2);
    Byteswap(regs.csCSelect3);

    Byteswap(regs.csDSelect0);
    Byteswap(regs.csDSelect1);
    Byteswap(regs.csDSelect2);
    Byteswap(regs.csDSelect3);

    Byteswap(regs.csDebug);

    Byteswap(regs.pllControl);
    Byteswap(regs.pllFreqSel);
    Byteswap(regs.pllTest);

    Byteswap(regs.intControl);
    Byteswap(regs.intMaskHi);
    Byteswap(regs.intMaskLo);
    Byteswap(regs.intWakeupEnHi);
    Byteswap(regs.intWakeupEnLo);
    Byteswap(regs.intStatusHi);
    Byteswap(regs.intStatusLo);
    Byteswap(regs.intPendingHi);
    Byteswap(regs.intPendingLo);

    Byteswap(regs.pwmControl);
    Byteswap(regs.pwmPeriod);
    Byteswap(regs.pwmWidth);
    Byteswap(regs.pwmCounter);

    Byteswap(regs.tmr1Control);
    Byteswap(regs.tmr1Prescaler);
    Byteswap(regs.tmr1Compare);
    Byteswap(regs.tmr1Capture);
    Byteswap(regs.tmr1Counter);
    Byteswap(regs.tmr1Status);

    Byteswap(regs.tmr2Control);
    Byteswap(regs.tmr2Prescaler);
    Byteswap(regs.tmr2Compare);
    Byteswap(regs.tmr2Capture);
    Byteswap(regs.tmr2Counter);
    Byteswap(regs.tmr2Status);

    Byteswap(regs.wdControl);
    Byteswap(regs.wdReference);
    Byteswap(regs.wdCounter);

    Byteswap(regs.spiSlave);

    Byteswap(regs.spiMasterData);
    Byteswap(regs.spiMasterControl);

    Byteswap(regs.uControl);
    Byteswap(regs.uBaud);
    Byteswap(regs.uReceive);
    Byteswap(regs.uTransmit);
    Byteswap(regs.uMisc);

    Byteswap(regs.lcdStartAddr);
    Byteswap(regs.lcdScreenWidth);
    Byteswap(regs.lcdScreenHeight);
    Byteswap(regs.lcdCursorXPos);
    Byteswap(regs.lcdCursorYPos);
    Byteswap(regs.lcdCursorWidthHeight);
    Byteswap(regs.lcdGrayPalette);

    Byteswap(regs.rtcHourMinSec);
    Byteswap(regs.rtcAlarm);
    Byteswap(regs.rtcReserved);
    Byteswap(regs.rtcControl);
    Byteswap(regs.rtcIntStatus);
    Byteswap(regs.rtcIntEnable);
    Byteswap(regs.stopWatch);
}

void Byteswap(HwrM68EZ328Type& regs) {
    Byteswap(regs.swID);

    Byteswap(regs.csAGroupBase);
    Byteswap(regs.csBGroupBase);
    Byteswap(regs.csCGroupBase);
    Byteswap(regs.csDGroupBase);

    Byteswap(regs.csASelect);
    Byteswap(regs.csBSelect);
    Byteswap(regs.csCSelect);
    Byteswap(regs.csDSelect);

    Byteswap(regs.emuCS);

    Byteswap(regs.pllControl);
    Byteswap(regs.pllFreqSel);
    Byteswap(regs.pllTest);

    Byteswap(regs.intControl);
    Byteswap(regs.intMaskHi);
    Byteswap(regs.intMaskLo);
    Byteswap(regs.intStatusHi);
    Byteswap(regs.intStatusLo);
    Byteswap(regs.intPendingHi);
    Byteswap(regs.intPendingLo);

    Byteswap(regs.pwmControl);

    Byteswap(regs.tmr1Control);
    Byteswap(regs.tmr1Prescaler);
    Byteswap(regs.tmr1Compare);
    Byteswap(regs.tmr1Capture);
    Byteswap(regs.tmr1Counter);
    Byteswap(regs.tmr1Status);

    Byteswap(regs.spiMasterData);
    Byteswap(regs.spiMasterControl);

    Byteswap(regs.uControl);
    Byteswap(regs.uBaud);
    Byteswap(regs.uReceive);
    Byteswap(regs.uTransmit);
    Byteswap(regs.uMisc);
    Byteswap(regs.uNonIntPresc);

    Byteswap(regs.lcdStartAddr);
    Byteswap(regs.lcdScreenWidth);
    Byteswap(regs.lcdScreenHeight);
    Byteswap(regs.lcdCursorXPos);
    Byteswap(regs.lcdCursorYPos);
    Byteswap(regs.lcdCursorWidthHeight);
    Byteswap(regs.lcdContrastControlPWM);

    Byteswap(regs.rtcHourMinSec);
    Byteswap(regs.rtcAlarm);
    Byteswap(regs.rtcWatchDog);
    Byteswap(regs.rtcControl);
    Byteswap(regs.rtcIntStatus);
    Byteswap(regs.rtcIntEnable);
    Byteswap(regs.stopWatch);
    Byteswap(regs.rtcDay);
    Byteswap(regs.rtcDayAlarm);

    Byteswap(regs.dramConfig);
    Byteswap(regs.dramControl);

    Byteswap(regs.emuAddrCompare);
    Byteswap(regs.emuAddrMask);
    Byteswap(regs.emuControlCompare);
    Byteswap(regs.emuControlMask);
    Byteswap(regs.emuControl);
    Byteswap(regs.emuStatus);
}

void Byteswap(HwrM68VZ328Type& regs) {
    Byteswap(regs.scr);

    Byteswap(regs.pcr);
    Byteswap(regs.chipID);
    Byteswap(regs.maskID);
    Byteswap(regs.swID);
    Byteswap(regs.ioDriveControl);

    Byteswap(regs.csAGroupBase);
    Byteswap(regs.csBGroupBase);
    Byteswap(regs.csCGroupBase);
    Byteswap(regs.csDGroupBase);

    Byteswap(regs.csUGroupBase);

    Byteswap(regs.csControl1);
    Byteswap(regs.csControl2);
    Byteswap(regs.csControl3);

    Byteswap(regs.csASelect);
    Byteswap(regs.csBSelect);
    Byteswap(regs.csCSelect);
    Byteswap(regs.csDSelect);

    Byteswap(regs.emuCS);

    Byteswap(regs.pllControl);
    Byteswap(regs.pllFreqSel);

    Byteswap(regs.pwrControl);

    Byteswap(regs.intVector);
    Byteswap(regs.intControl);
    Byteswap(regs.intMaskHi);
    Byteswap(regs.intMaskLo);
    Byteswap(regs.intStatusHi);
    Byteswap(regs.intStatusLo);
    Byteswap(regs.intPendingHi);
    Byteswap(regs.intPendingLo);
    Byteswap(regs.intLevelControl);

    Byteswap(regs.portADir);
    Byteswap(regs.portAData);
    Byteswap(regs.portAPullupEn);

    Byteswap(regs.portBDir);
    Byteswap(regs.portBData);
    Byteswap(regs.portBPullupEn);
    Byteswap(regs.portBSelect);

    Byteswap(regs.portCDir);
    Byteswap(regs.portCData);
    Byteswap(regs.portCPulldnEn);
    Byteswap(regs.portCSelect);

    Byteswap(regs.portDDir);
    Byteswap(regs.portDData);
    Byteswap(regs.portDPullupEn);
    Byteswap(regs.portDSelect);
    Byteswap(regs.portDPolarity);
    Byteswap(regs.portDIntReqEn);
    Byteswap(regs.portDKbdIntEn);
    Byteswap(regs.portDIntEdge);

    Byteswap(regs.portEDir);
    Byteswap(regs.portEData);
    Byteswap(regs.portEPullupEn);
    Byteswap(regs.portESelect);

    Byteswap(regs.portFDir);
    Byteswap(regs.portFData);
    Byteswap(regs.portFPullupdnEn);
    Byteswap(regs.portFSelect);

    Byteswap(regs.portGDir);
    Byteswap(regs.portGData);
    Byteswap(regs.portGPullupEn);
    Byteswap(regs.portGSelect);

    Byteswap(regs.portJDir);
    Byteswap(regs.portJData);
    Byteswap(regs.portJPullupEn);
    Byteswap(regs.portJSelect);

    Byteswap(regs.portKDir);
    Byteswap(regs.portKData);
    Byteswap(regs.portKPullupdnEn);
    Byteswap(regs.portKSelect);

    Byteswap(regs.portMDir);
    Byteswap(regs.portMData);
    Byteswap(regs.portMPullupdnEn);
    Byteswap(regs.portMSelect);

    Byteswap(regs.pwmControl);
    Byteswap(regs.pwmSampleHi);
    Byteswap(regs.pwmSampleLo);
    Byteswap(regs.pwmPeriod);
    Byteswap(regs.pwmCounter);

    Byteswap(regs.pwm2Control);
    Byteswap(regs.pwm2Period);
    Byteswap(regs.pwm2Width);
    Byteswap(regs.pwm2Counter);

    Byteswap(regs.tmr1Control);
    Byteswap(regs.tmr1Prescaler);
    Byteswap(regs.tmr1Compare);
    Byteswap(regs.tmr1Capture);
    Byteswap(regs.tmr1Counter);
    Byteswap(regs.tmr1Status);

    Byteswap(regs.tmr2Control);
    Byteswap(regs.tmr2Prescaler);
    Byteswap(regs.tmr2Compare);
    Byteswap(regs.tmr2Capture);
    Byteswap(regs.tmr2Counter);
    Byteswap(regs.tmr2Status);

    Byteswap(regs.spiRxD);
    Byteswap(regs.spiTxD);
    Byteswap(regs.spiCont1);
    Byteswap(regs.spiIntCS);
    Byteswap(regs.spiTest);
    Byteswap(regs.spiSpc);

    Byteswap(regs.spiMasterData);
    Byteswap(regs.spiMasterControl);

    Byteswap(regs.uControl);
    Byteswap(regs.uBaud);
    Byteswap(regs.uReceive);
    Byteswap(regs.uTransmit);
    Byteswap(regs.uMisc);
    Byteswap(regs.uNonIntPresc);

    Byteswap(regs.u2Control);
    Byteswap(regs.u2Baud);
    Byteswap(regs.u2Receive);
    Byteswap(regs.u2Transmit);
    Byteswap(regs.u2Misc);
    Byteswap(regs.u2NonIntPresc);
    Byteswap(regs.u2FIFOHMark);

    Byteswap(regs.lcdStartAddr);
    Byteswap(regs.lcdPageWidth);
    Byteswap(regs.lcdScreenWidth);
    Byteswap(regs.lcdScreenHeight);
    Byteswap(regs.lcdCursorXPos);
    Byteswap(regs.lcdCursorYPos);
    Byteswap(regs.lcdCursorWidthHeight);
    Byteswap(regs.lcdBlinkControl);
    Byteswap(regs.lcdPanelControl);
    Byteswap(regs.lcdPolarity);
    Byteswap(regs.lcdACDRate);
    Byteswap(regs.lcdPixelClock);
    Byteswap(regs.lcdClockControl);
    Byteswap(regs.lcdRefreshRateAdj);
    Byteswap(regs.lcdReserved1);
    Byteswap(regs.lcdPanningOffset);
    Byteswap(regs.lcdFrameRate);
    Byteswap(regs.lcdGrayPalette);
    Byteswap(regs.lcdReserved2);
    Byteswap(regs.lcdContrastControlPWM);
    Byteswap(regs.lcdRefreshModeControl);
    Byteswap(regs.lcdDMAControl);

    Byteswap(regs.rtcHourMinSec);
    Byteswap(regs.rtcAlarm);
    Byteswap(regs.rtcWatchDog);
    Byteswap(regs.rtcControl);
    Byteswap(regs.rtcIntStatus);
    Byteswap(regs.rtcIntEnable);
    Byteswap(regs.stopWatch);
    Byteswap(regs.rtcDay);
    Byteswap(regs.rtcDayAlarm);

    Byteswap(regs.dramConfig);
    Byteswap(regs.dramControl);
    Byteswap(regs.sdramControl);
    Byteswap(regs.sdramPwDn);

    Byteswap(regs.emuAddrCompare);
    Byteswap(regs.emuAddrMask);
    Byteswap(regs.emuControlCompare);
    Byteswap(regs.emuControlMask);
    Byteswap(regs.emuControl);
    Byteswap(regs.emuStatus);
}

void Byteswap(regstruct& p) {
    Byteswap(p.regs[0]);
    Byteswap(p.regs[1]);
    Byteswap(p.regs[2]);
    Byteswap(p.regs[3]);
    Byteswap(p.regs[4]);
    Byteswap(p.regs[5]);
    Byteswap(p.regs[6]);
    Byteswap(p.regs[7]);

    Byteswap(p.regs[8]);
    Byteswap(p.regs[9]);
    Byteswap(p.regs[10]);
    Byteswap(p.regs[11]);
    Byteswap(p.regs[12]);
    Byteswap(p.regs[13]);
    Byteswap(p.regs[14]);
    Byteswap(p.regs[15]);

    Byteswap(p.usp);
    Byteswap(p.isp);
    Byteswap(p.msp);
    Byteswap(p.sr);
    Byteswap(p.t1);
    Byteswap(p.t0);
    Byteswap(p.s);
    Byteswap(p.m);
    Byteswap(p.stopped);
    Byteswap(p.intmask);
    Byteswap(p.pc);
    Byteswap(p.vbr);
    Byteswap(p.sfc);
    Byteswap(p.dfc);

    #if 0  // we don't support FP
	Byteswap (fp.regs[0]);
	Byteswap (fp.regs[1]);
	Byteswap (fp.regs[2]);
	Byteswap (fp.regs[3]);
	Byteswap (fp.regs[4]);
	Byteswap (fp.regs[5]);
	Byteswap (fp.regs[6]);
	Byteswap (fp.regs[7]);

	Byteswap (p.fpcr);
	Byteswap (p.fpsr);
	Byteswap (p.fpiar);
    #endif

    Byteswap(p.spcflags);
    Byteswap(p.kick_mask);
    Byteswap(p.prefetch);
}

void Byteswap(SED1375RegsType& p) {
    Byteswap(p.productRevisionCode);
    Byteswap(p.mode0);
    Byteswap(p.mode1);
    Byteswap(p.mode2);
    Byteswap(p.horizontalPanelSize);
    Byteswap(p.verticalPanelSizeLSB);
    Byteswap(p.verticalPanelSizeMSB);
    Byteswap(p.FPLineStartPosition);
    Byteswap(p.horizontalNonDisplayPeriod);
    Byteswap(p.FPFRAMEStartPosition);
    Byteswap(p.verticalNonDisplayPeriod);
    Byteswap(p.MODRate);
    Byteswap(p.screen1StartAddressLSB);
    Byteswap(p.screen1StartAddressMSB);
    Byteswap(p.screen1StartAddressMSBit);
    Byteswap(p.screen2StartAddressLSB);
    Byteswap(p.screen2StartAddressMSB);
    Byteswap(p.screen1StartAddressMSBit);
    Byteswap(p.memoryAddressOffset);
    Byteswap(p.screen1VerticalSizeLSB);
    Byteswap(p.screen1VerticalSizeMSB);
    Byteswap(p.lookUpTableAddress);
    Byteswap(p.unused1);
    Byteswap(p.lookUpTableData);
    Byteswap(p.GPIOConfigurationControl);
    Byteswap(p.GPIOStatusControl);
    Byteswap(p.scratchPad);
    Byteswap(p.portraitMode);
    Byteswap(p.lineByteCountRegister);
    Byteswap(p.unused2);
    Byteswap(p.unused3);
    Byteswap(p.unused4);
}

#endif  // BYTESWAP
