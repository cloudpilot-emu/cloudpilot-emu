#include "syscall.h"

#include <stdlib.h>

const char* getSyscallName(uint32_t syscall) {
    switch (syscall) {
        case (4 << 12) | 0x000:
            return "DALGetGlobalsPtr";
        case (4 << 12) | 0x004:
            return "HALDbgBreak";
        case (4 << 12) | 0x008:
            return "HALDbgMessage";
        case (4 << 12) | 0x00c:
            return "HALAttnGetLEDAttributes";
        case (4 << 12) | 0x010:
            return "HALAttnGetVibrateAttributes";
        case (4 << 12) | 0x014:
            return "HALAttnSetLEDAttributes";
        case (4 << 12) | 0x018:
            return "HALAttnSetVibrateAttributes";
        case (4 << 12) | 0x01c:
            return "HALBatteryGetInfo";
        case (4 << 12) | 0x020:
            return "HALBatteryGetValidKinds";
        case (4 << 12) | 0x024:
            return "HALBatterySetInfo";
        case (4 << 12) | 0x028:
            return "HALDelay";
        case (4 << 12) | 0x02c:
            return "HALDisplayGetAttributes";
        case (4 << 12) | 0x030:
            return "HALDisplayGetPalette";
        case (4 << 12) | 0x034:
            return "HALDisplaySetAttributes";
        case (4 << 12) | 0x038:
            return "HALDisplaySetPalette";
        case (4 << 12) | 0x03c:
            return "HALDisplayDoze";
        case (4 << 12) | 0x040:
            return "HALDisplayWake";
        case (4 << 12) | 0x044:
            return "HALDisplayLock";
        case (4 << 12) | 0x048:
            return "HALDisplayUnlock";
        case (4 << 12) | 0x04c:
            return "HALDockStatus";
        case (4 << 12) | 0x050:
            return "HALDrawInit";
        case (4 << 12) | 0x074:
            return "HALErrDisplay";
        case (4 << 12) | 0x078:
            return "HALEventPost";
        case (4 << 12) | 0x07c:
            return "HALEventRegisterCallBack";
        case (4 << 12) | 0x080:
            return "HALFlpAddDouble";
        case (4 << 12) | 0x084:
            return "HALFlpAddFloat";
        case (4 << 12) | 0x088:
            return "HALFlpCompareDoubleEqual";
        case (4 << 12) | 0x08c:
            return "HALFlpCompareDoubleLessThan";
        case (4 << 12) | 0x090:
            return "HALFlpCompareFloatEqual";
        case (4 << 12) | 0x094:
            return "HALFlpCompareFloatLessThan";
        case (4 << 12) | 0x098:
            return "HALFlpDivDouble";
        case (4 << 12) | 0x09c:
            return "HALFlpDivFloat";
        case (4 << 12) | 0x0a0:
            return "HALFlpDoubleToFloat";
        case (4 << 12) | 0x0a4:
            return "HALFlpDoubleToInt32";
        case (4 << 12) | 0x0a8:
            return "HALFlpDoubleToLongDouble";
        case (4 << 12) | 0x0ac:
            return "HALFlpDoubleToLongLong";
        case (4 << 12) | 0x0b0:
            return "HALFlpDoubleToUInt32";
        case (4 << 12) | 0x0b4:
            return "HALFlpDoubleToULongLong";
        case (4 << 12) | 0x0b8:
            return "HALFlpFloatToDouble";
        case (4 << 12) | 0x0bc:
            return "HALFlpFloatToInt32";
        case (4 << 12) | 0x0c0:
            return "HALFlpFloatToLongDouble";
        case (4 << 12) | 0x0c4:
            return "HALFlpFloatToLongLong";
        case (4 << 12) | 0x0c8:
            return "HALFlpFloatToUInt32";
        case (4 << 12) | 0x0cc:
            return "HALFlpFloatToULongLong";
        case (4 << 12) | 0x0d0:
            return "HALFlpInt32ToDouble";
        case (4 << 12) | 0x0d4:
            return "HALFlpInt32ToFloat";
        case (4 << 12) | 0x0d8:
            return "HALFlpLongDoubleToDouble";
        case (4 << 12) | 0x0dc:
            return "HALFlpLongDoubleToFloat";
        case (4 << 12) | 0x0e0:
            return "HALFlpLongLongToDouble";
        case (4 << 12) | 0x0e4:
            return "HALFlpLongLongToFloat";
        case (4 << 12) | 0x0e8:
            return "HALFlpMulDouble";
        case (4 << 12) | 0x0ec:
            return "HALFlpMulFloat";
        case (4 << 12) | 0x0f0:
            return "HALFlpNegDouble";
        case (4 << 12) | 0x0f4:
            return "HALFlpNegFloat";
        case (4 << 12) | 0x0f8:
            return "HALFlpSubDouble";
        case (4 << 12) | 0x0fc:
            return "HALFlpSubFloat";
        case (4 << 12) | 0x100:
            return "HALFlpUInt32ToDouble";
        case (4 << 12) | 0x104:
            return "HALFlpUInt32ToFloat";
        case (4 << 12) | 0x108:
            return "HALFlpULongLongToDouble";
        case (4 << 12) | 0x10c:
            return "HALFlpULongLongToFloat";
        case (4 << 12) | 0x110:
            return "HALGetHwrMiscFlags";
        case (4 << 12) | 0x114:
            return "HALGetHwrMiscFlagsExt";
        case (4 << 12) | 0x118:
            return "HALGetHwrWakeUp";
        case (4 << 12) | 0x11c:
            return "HALInterruptAllSetStatus";
        case (4 << 12) | 0x120:
            return "HALInterruptAllGetStatus";
        case (4 << 12) | 0x124:
            return "HALInterruptGetHandler";
        case (4 << 12) | 0x128:
            return "HALInterruptSetHandler";
        case (4 << 12) | 0x12c:
            return "HALInterruptSetState";
        case (4 << 12) | 0x130:
            return "HALKeyGetRates";
        case (4 << 12) | 0x134:
            return "HALKeyGetState";
        case (4 << 12) | 0x138:
            return "HALKeyResetDoubleTap";
        case (4 << 12) | 0x13c:
            return "HALKeySetMask";
        case (4 << 12) | 0x140:
            return "HALKeySetRates";
        case (4 << 12) | 0x144:
            return "HALMemoryGetMemoryMap";
        case (4 << 12) | 0x148:
            return "HALMemoryGetStorageAreaProtectionState";
        case (4 << 12) | 0x14c:
            return "HALMemorySetStorageAreaProtectionState";
        case (4 << 12) | 0x150:
            return "HALMemoryValidateArea";
        case (4 << 12) | 0x154:
            return "HALMemoryGetAreaSize";
        case (4 << 12) | 0x158:
            return "HALPenCalibrate";
        case (4 << 12) | 0x15c:
            return "HALPenRawToScreen";
        case (4 << 12) | 0x160:
            return "HALPenResetCalibration";
        case (4 << 12) | 0x164:
            return "HALPenGetPosition";
        case (4 << 12) | 0x168:
            return "HALPenScreenToRaw";
        case (4 << 12) | 0x16c:
            return "HALPowerGetAutoOffEvtTime";
        case (4 << 12) | 0x170:
            return "HALPowerGetAutoOffSeconds";
        case (4 << 12) | 0x174:
            return "HALPowerSetAutoOffEvtTime";
        case (4 << 12) | 0x178:
            return "HALPowerSetAutoOffSeconds";
        case (4 << 12) | 0x17c:
            return "HALPowerSleepReady";
        case (4 << 12) | 0x180:
            return "HALProcessorID";
        case (4 << 12) | 0x184:
            return "HALRandomGetBits";
        case (4 << 12) | 0x188:
            return "HALRandomInitializeSeed";
        case (4 << 12) | 0x18c:
            return "HALReset";
        case (4 << 12) | 0x190:
            return "HALScreenDefaultPalette";
        case (4 << 12) | 0x194:
            return "HALScreenDrawNotify";
        case (4 << 12) | 0x198:
            return "HALScreenGetColortable";
        case (4 << 12) | 0x19c:
            return "HALScreenInit";
        case (4 << 12) | 0x1a0:
            return "HALScreenLock";
        case (4 << 12) | 0x1a4:
            return "HALScreenPalette";
        case (4 << 12) | 0x1a8:
            return "HALScreenSendUpdateArea";
        case (4 << 12) | 0x1ac:
            return "HALScreenUnlock";
        case (4 << 12) | 0x1b0:
            return "HALScreenUpdateBitmap";
        case (4 << 12) | 0x1b4:
            return "HALSetInitStage";
        case (4 << 12) | 0x1b8:
            return "HALSetHwrMiscFlags";
        case (4 << 12) | 0x1bc:
            return "HALSilkscreenGetID";
        case (4 << 12) | 0x1c0:
            return "HALSoundOff";
        case (4 << 12) | 0x1c4:
            return "HALSoundPlay";
        case (4 << 12) | 0x1c8:
            return "HALTimeGetAlarm";
        case (4 << 12) | 0x1cc:
            return "HALTimeGetSeconds";
        case (4 << 12) | 0x1d0:
            return "HALTimeGetSystemTime";
        case (4 << 12) | 0x1d4:
            return "HALTimeGetSystemTimerInterval";
        case (4 << 12) | 0x1d8:
            return "HALTimeSetAlarm";
        case (4 << 12) | 0x1dc:
            return "HALTimeSetSeconds";
        case (4 << 12) | 0x1e0:
            return "HALTraceInit";
        case (4 << 12) | 0x1e4:
            return "HALTraceClose";
        case (4 << 12) | 0x1e8:
            return "HALTraceOutputT";
        case (4 << 12) | 0x1ec:
            return "HALTraceOutputTL";
        case (4 << 12) | 0x1f0:
            return "HALTraceOutputVT";
        case (4 << 12) | 0x1f4:
            return "HALTraceOutputVTL";
        case (4 << 12) | 0x1f8:
            return "HALTraceOutputB";
        case (4 << 12) | 0x1fc:
            return "HALOEMGetCompanyID";
        case (4 << 12) | 0x200:
            return "HALOEMGetHALID";
        case (4 << 12) | 0x204:
            return "HALOEMGetDeviceID";
        case (4 << 12) | 0x208:
            return "HwrCalcDynamicHeapSize";
        case (4 << 12) | 0x20c:
            return "HwrCalcDynamicRAMSize";
        case (4 << 12) | 0x210:
            return "HALDisplayDrawBootScreen";
        case (4 << 12) | 0x214:
            return "HALGetROMToken";
        case (4 << 12) | 0x218:
            return "KALEventGroupClear";
        case (4 << 12) | 0x21c:
            return "KALEventGroupCreate";
        case (4 << 12) | 0x220:
            return "KALEventGroupDelete";
        case (4 << 12) | 0x224:
            return "KALEventGroupRead";
        case (4 << 12) | 0x228:
            return "KALEventGroupSignal";
        case (4 << 12) | 0x22c:
            return "KALEventGroupWait";
        case (4 << 12) | 0x230:
            return "KALEventGroupGetInfo";
        case (4 << 12) | 0x234:
            return "KALMailboxCreate";
        case (4 << 12) | 0x238:
            return "KALMailboxDelete";
        case (4 << 12) | 0x23c:
            return "KALMailboxSend";
        case (4 << 12) | 0x240:
            return "KALMailboxWait";
        case (4 << 12) | 0x244:
            return "KALMailboxGetInfo";
        case (4 << 12) | 0x248:
            return "KALMutexCreate";
        case (4 << 12) | 0x24c:
            return "KALMutexDelete";
        case (4 << 12) | 0x250:
            return "KALMutexRelease";
        case (4 << 12) | 0x254:
            return "KALMutexReserve";
        case (4 << 12) | 0x258:
            return "KALMutexGetInfo";
        case (4 << 12) | 0x25c:
            return "KALSemaphoreCreate";
        case (4 << 12) | 0x260:
            return "KALSemaphoreDelete";
        case (4 << 12) | 0x264:
            return "KALSemaphoreSignal";
        case (4 << 12) | 0x268:
            return "KALSemaphoreWait";
        case (4 << 12) | 0x26c:
            return "KALSemaphoreGetInfo";
        case (4 << 12) | 0x270:
            return "KALTaskCreate";
        case (4 << 12) | 0x274:
            return "KALTaskDelay";
        case (4 << 12) | 0x278:
            return "KALTaskDelete";
        case (4 << 12) | 0x27c:
            return "KALTaskGetCurrentID";
        case (4 << 12) | 0x280:
            return "KALTaskGetInfo";
        case (4 << 12) | 0x284:
            return "KALTaskResume";
        case (4 << 12) | 0x288:
            return "KALTaskSuspend";
        case (4 << 12) | 0x28c:
            return "KALTaskStart";
        case (4 << 12) | 0x290:
            return "KALTaskSwitching";
        case (4 << 12) | 0x294:
            return "KALTaskWait";
        case (4 << 12) | 0x298:
            return "KALTaskWaitClr";
        case (4 << 12) | 0x29c:
            return "KALTaskWake";
        case (4 << 12) | 0x2a0:
            return "KALTaskExit";
        case (4 << 12) | 0x2a4:
            return "KALTimerCreate";
        case (4 << 12) | 0x2a8:
            return "KALTimerDelete";
        case (4 << 12) | 0x2ac:
            return "KALTimerSet";
        case (4 << 12) | 0x2b0:
            return "KALTimerGetInfo";
        case (4 << 12) | 0x2b4:
            return "RALCallWithNewStack";
        case (4 << 12) | 0x2b8:
            return "RALGetEntryAddresses";
        case (4 << 12) | 0x2bc:
            return "RALGetModuleInfo";
        case (4 << 12) | 0x2c0:
            return "RALGetStaticBase";
        case (4 << 12) | 0x2c4:
            return "RALLinkClient";
        case (4 << 12) | 0x2c8:
            return "RALLoadModule";
        case (4 << 12) | 0x2cc:
            return "RALPatchEntry";
        case (4 << 12) | 0x2d0:
            return "RALSetA5";
        case (4 << 12) | 0x2d4:
            return "RALUnloadModule";
        case (4 << 12) | 0x2d8:
            return "RALUnloadNext";
        case (4 << 12) | 0x2dc:
            return "DALSetDbgLockout";
        case (4 << 12) | 0x2e0:
            return "HALKeyGetDoubleTapDelay";
        case (4 << 12) | 0x2e4:
            return "HALKeySetDoubleTapDelay";
        case (4 << 12) | 0x2e8:
            return "HALInvalidateICache";
        case (4 << 12) | 0x2ec:
            return "HALHandleReset";
        case (4 << 12) | 0x2f8:
            return "HALDraw_Bitmap";
        case (4 << 12) | 0x2fc:
            return "HALDraw_FindIndexes";
        case (4 << 12) | 0x300:
            return "HALDraw_GetPixel";
        case (4 << 12) | 0x304:
            return "HALDraw_Chars";
        case (4 << 12) | 0x308:
            return "HALDraw_Line";
        case (4 << 12) | 0x30c:
            return "HALDraw_Rectangle";
        case (4 << 12) | 0x310:
            return "HALDraw_Pixels";
        case (4 << 12) | 0x314:
            return "HALSoundOpen";
        case (4 << 12) | 0x318:
            return "HALSoundClose";
        case (4 << 12) | 0x31c:
            return "HALSoundRead";
        case (4 << 12) | 0x320:
            return "HALSoundWrite";
        case (4 << 12) | 0x324:
            return "HALSoundIoctl";
        case (4 << 12) | 0x328:
            return "HALSoundInitialize";
        case (4 << 12) | 0x32c:
            return "HALSoundDispose";
        case (4 << 12) | 0x330:
            return "dprintf";
        case (4 << 12) | 0x334:
            return "HALDraw_GetSupportedDensity";
        case (4 << 12) | 0x338:
            return "HALPlaySmf";
        case (4 << 12) | 0x358:
            return "HALDisplaySleep";
        case (4 << 12) | 0x33c:
            return "OEMReserved1";
        case (4 << 12) | 0x340:
            return "OEMReserved2";
        case (4 << 12) | 0x344:
            return "OEMReserved3";
        case (4 << 12) | 0x348:
            return "OEMReserved4";
        case (4 << 12) | 0x34c:
            return "OEMReserved5";
        case (4 << 12) | 0x350:
            return "OEMReserved6";
        case (4 << 12) | 0x354:
            return "OEMReserved7";
        case (4 << 12) | 0x35c:
            return "OEMReserved8";
        case (4 << 12) | 0x360:
            return "OEMReserved9";
        case (4 << 12) | 0x364:
            return "OEMReserved10";
        case (4 << 12) | 0x368:
            return "OEMReserved11";
        case (4 << 12) | 0x36c:
            return "OEMReserved12";
        case (4 << 12) | 0x370:
            return "OEMReserved13";
        case (4 << 12) | 0x374:
            return "OEMReserved14";
        case (4 << 12) | 0x378:
            return "OEMReserved15";
        case (4 << 12) | 0x37c:
            return "OEMReserved16";
        case (4 << 12) | 0x380:
            return "OEMReserved17";
        case (4 << 12) | 0x384:
            return "OEMReserved18";
        case (4 << 12) | 0x388:
            return "OEMReserved19";
        case (4 << 12) | 0x38c:
            return "OEMReserved20";
        case (4 << 12) | 0x390:
            return "OEMReserved21";
        case (4 << 12) | 0x394:
            return "OEMReserved22";
        case (4 << 12) | 0x398:
            return "OEMReserved23";
        case (4 << 12) | 0x39c:
            return "OEMReserved24";
        case (4 << 12) | 0x3a0:
            return "OEMReserved25";
        case (4 << 12) | 0x3a4:
            return "OEMReserved26";
        case (4 << 12) | 0x3a8:
            return "OEMReserved27";
        case (4 << 12) | 0x3ac:
            return "OEMReserved28";
        case (4 << 12) | 0x3b0:
            return "OEMReserved29";
        case (4 << 12) | 0x3b4:
            return "OEMReserved30";
        case (4 << 12) | 0x3b8:
            return "OEMReserved31";
        case (4 << 12) | 0x3bc:
            return "OEMReserved32";
        case (8 << 12) | 0x000:
            return "PalmOSMain";
        case (8 << 12) | 0x004:
            return "PalmOSGetGlobalsPtr";
        case (8 << 12) | 0x008:
            return "AlmAlarmCallback";
        case (8 << 12) | 0x00c:
            return "AlmCancelAll";
        case (8 << 12) | 0x010:
            return "AlmDisplayAlarm";
        case (8 << 12) | 0x014:
            return "AlmEnableNotification";
        case (8 << 12) | 0x018:
            return "AlmGetAlarm";
        case (8 << 12) | 0x01c:
            return "AlmGetAlarmV40";
        case (8 << 12) | 0x020:
            return "AlmInit";
        case (8 << 12) | 0x024:
            return "AlmSetAlarm";
        case (8 << 12) | 0x028:
            return "AlmSetAlarmV40";
        case (8 << 12) | 0x02c:
            return "AlmTimeChange";
        case (8 << 12) | 0x030:
            return "AttnGetAttentionV40";
        case (8 << 12) | 0x034:
            return "AttnUpdateV40";
        case (8 << 12) | 0x038:
            return "AttnForgetItV40";
        case (8 << 12) | 0x03c:
            return "AttnGetCountsV40";
        case (8 << 12) | 0x040:
            return "AttnIterateV40";
        case (8 << 12) | 0x044:
            return "BmpBitsSize";
        case (8 << 12) | 0x048:
            return "BmpColortableSize";
        case (8 << 12) | 0x04c:
            return "BmpCompress";
        case (8 << 12) | 0x050:
            return "BmpCreate";
        case (8 << 12) | 0x054:
            return "BmpDelete";
        case (8 << 12) | 0x058:
            return "BmpGetBitDepth";
        case (8 << 12) | 0x05c:
            return "BmpGetBits";
        case (8 << 12) | 0x060:
            return "BmpGetColortable";
        case (8 << 12) | 0x064:
            return "BmpGetDimensions";
        case (8 << 12) | 0x068:
            return "BmpGetNextBitmap";
        case (8 << 12) | 0x06c:
            return "BmpGetSizes";
        case (8 << 12) | 0x070:
            return "BmpSize";
        case (8 << 12) | 0x074:
            return "CncAddProfile";
        case (8 << 12) | 0x078:
            return "CncDeleteProfile";
        case (8 << 12) | 0x07c:
            return "CncGetProfileInfo";
        case (8 << 12) | 0x080:
            return "CncGetProfileList";
        case (8 << 12) | 0x084:
            return "CncProfileCloseDB";
        case (8 << 12) | 0x088:
            return "CncProfileCount";
        case (8 << 12) | 0x08c:
            return "CncProfileCreate";
        case (8 << 12) | 0x090:
            return "CncProfileDelete";
        case (8 << 12) | 0x094:
            return "CncProfileGetCurrent";
        case (8 << 12) | 0x098:
            return "CncProfileGetIDFromIndex";
        case (8 << 12) | 0x09c:
            return "CncProfileGetIDFromName";
        case (8 << 12) | 0x0a0:
            return "CncProfileGetIndex";
        case (8 << 12) | 0x0a4:
            return "CncProfileOpenDB";
        case (8 << 12) | 0x0a8:
            return "CncProfileSetCurrent";
        case (8 << 12) | 0x0ac:
            return "CncProfileSettingGet";
        case (8 << 12) | 0x0b0:
            return "CncProfileSettingSet";
        case (8 << 12) | 0x0b4:
            return "CnvInPlaceSwapNBytes";
        case (8 << 12) | 0x0b8:
            return "CnvReadSwappedNBytes";
        case (8 << 12) | 0x0bc:
            return "CnvWriteSwappedNBytes";
        case (8 << 12) | 0x0c0:
            return "CnvReadARMStructureFrom68KFormat";
        case (8 << 12) | 0x0c4:
            return "CnvWriteARMStructureTo68KFormat";
        case (8 << 12) | 0x0c8:
            return "Crc16CalcBlock";
        case (8 << 12) | 0x0cc:
            return "DateAdjust";
        case (8 << 12) | 0x0d0:
            return "DateDaysToDate";
        case (8 << 12) | 0x0d4:
            return "DateSecondsToDate";
        case (8 << 12) | 0x0d8:
            return "DateTemplateToAscii";
        case (8 << 12) | 0x0dc:
            return "DateToAscii";
        case (8 << 12) | 0x0e0:
            return "DateToDays";
        case (8 << 12) | 0x0e4:
            return "DateToDOWDMFormat";
        case (8 << 12) | 0x0e8:
            return "DayOfMonth";
        case (8 << 12) | 0x0ec:
            return "DayOfWeek";
        case (8 << 12) | 0x0f0:
            return "DaysInMonth";
        case (8 << 12) | 0x0f4:
            return "DbgMessage";
        case (8 << 12) | 0x0f8:
            return "DbgBreak";
        case (8 << 12) | 0x0fc:
            return "DlkControl";
        case (8 << 12) | 0x100:
            return "DlkDispatchRequest";
        case (8 << 12) | 0x104:
            return "DlkGetSyncInfo";
        case (8 << 12) | 0x108:
            return "DlkSetLogEntry";
        case (8 << 12) | 0x10c:
            return "DlkStartServer";
        case (8 << 12) | 0x110:
            return "DmArchiveRecord";
        case (8 << 12) | 0x114:
            return "DmAttachRecord";
        case (8 << 12) | 0x118:
            return "DmAttachResource";
        case (8 << 12) | 0x11c:
            return "DmCloseDatabase";
        case (8 << 12) | 0x120:
            return "DmCreateDatabase";
        case (8 << 12) | 0x124:
            return "DmCreateDatabaseFromImage";
        case (8 << 12) | 0x128:
            return "DmCreateDatabaseV40";
        case (8 << 12) | 0x12c:
            return "DmDatabaseInfo";
        case (8 << 12) | 0x130:
            return "DmDatabaseInfoV40";
        case (8 << 12) | 0x134:
            return "DmDatabaseProtect";
        case (8 << 12) | 0x138:
            return "DmDatabaseProtectV40";
        case (8 << 12) | 0x13c:
            return "DmDatabaseSize";
        case (8 << 12) | 0x140:
            return "DmDatabaseSizeV40";
        case (8 << 12) | 0x144:
            return "DmDeleteCategory";
        case (8 << 12) | 0x148:
            return "DmDeleteDatabase";
        case (8 << 12) | 0x14c:
            return "DmDeleteDatabaseV40";
        case (8 << 12) | 0x150:
            return "DmDeleteRecord";
        case (8 << 12) | 0x154:
            return "DmDetachRecord";
        case (8 << 12) | 0x158:
            return "DmDetachResource";
        case (8 << 12) | 0x15c:
            return "DmFindDatabase";
        case (8 << 12) | 0x160:
            return "DmFindDatabaseV40";
        case (8 << 12) | 0x164:
            return "DmFindDatabaseWithTypeCreatorV40";
        case (8 << 12) | 0x168:
            return "DmFindDatabaseWithTypeCreator";
        case (8 << 12) | 0x16c:
            return "DmFindRecordByID";
        case (8 << 12) | 0x170:
            return "DmFindResource";
        case (8 << 12) | 0x174:
            return "DmFindResourceType";
        case (8 << 12) | 0x178:
            return "DmFindSortPosition";
        case (8 << 12) | 0x17c:
            return "DmFindSortPositionV10";
        case (8 << 12) | 0x180:
            return "DmGet1Resource";
        case (8 << 12) | 0x184:
            return "DmGetAppInfoH";
        case (8 << 12) | 0x188:
            return "DmGetDatabase";
        case (8 << 12) | 0x18c:
            return "DmGetDatabaseLockState";
        case (8 << 12) | 0x190:
            return "DmGetDatabaseV40";
        case (8 << 12) | 0x194:
            return "DmGetLastErr";
        case (8 << 12) | 0x198:
            return "DmGetNextDatabaseByTypeCreator";
        case (8 << 12) | 0x19c:
            return "DmGetNextDatabaseByTypeCreatorV40";
        case (8 << 12) | 0x1a0:
            return "DmGetRecord";
        case (8 << 12) | 0x1a4:
            return "DmGetResource";
        case (8 << 12) | 0x1a8:
            return "DmGetResourceIndex";
        case (8 << 12) | 0x1ac:
            return "DmInit";
        case (8 << 12) | 0x1b0:
            return "DmInsertionSort";
        case (8 << 12) | 0x1b4:
            return "DmMoveCategory";
        case (8 << 12) | 0x1b8:
            return "DmMoveOpenDBContext";
        case (8 << 12) | 0x1bc:
            return "DmMoveRecord";
        case (8 << 12) | 0x1c0:
            return "DmNewHandle";
        case (8 << 12) | 0x1c4:
            return "DmNewRecord";
        case (8 << 12) | 0x1c8:
            return "DmNewResource";
        case (8 << 12) | 0x1cc:
            return "DmNextOpenDatabase";
        case (8 << 12) | 0x1d0:
            return "DmNextOpenResDatabase";
        case (8 << 12) | 0x1d4:
            return "DmNumDatabases";
        case (8 << 12) | 0x1d8:
            return "DmNumDatabasesV40";
        case (8 << 12) | 0x1dc:
            return "DmNumRecords";
        case (8 << 12) | 0x1e0:
            return "DmNumRecordsInCategory";
        case (8 << 12) | 0x1e4:
            return "DmNumResources";
        case (8 << 12) | 0x1e8:
            return "DmOpenDatabase";
        case (8 << 12) | 0x1ec:
            return "DmOpenDatabaseByTypeCreator";
        case (8 << 12) | 0x1f0:
            return "DmOpenDatabaseInfo";
        case (8 << 12) | 0x1f4:
            return "DmOpenDatabaseInfoV40";
        case (8 << 12) | 0x1f8:
            return "DmOpenDatabaseV40";
        case (8 << 12) | 0x1fc:
            return "DmOpenDBNoOverlay";
        case (8 << 12) | 0x200:
            return "DmOpenDBNoOverlayV40";
        case (8 << 12) | 0x204:
            return "DmOpenDBWithLocale";
        case (8 << 12) | 0x208:
            return "DmPositionInCategory";
        case (8 << 12) | 0x20c:
            return "DmQueryNextInCategory";
        case (8 << 12) | 0x210:
            return "DmQueryRecord";
        case (8 << 12) | 0x214:
            return "DmQuickSort";
        case (8 << 12) | 0x218:
            return "DmRecordInfo";
        case (8 << 12) | 0x21c:
            return "DmReleaseRecord";
        case (8 << 12) | 0x220:
            return "DmReleaseResource";
        case (8 << 12) | 0x224:
            return "DmRemoveRecord";
        case (8 << 12) | 0x228:
            return "DmRemoveResource";
        case (8 << 12) | 0x22c:
            return "DmRemoveSecretRecords";
        case (8 << 12) | 0x230:
            return "DmResetRecordStates";
        case (8 << 12) | 0x234:
            return "DmResizeRecord";
        case (8 << 12) | 0x238:
            return "DmResizeResource";
        case (8 << 12) | 0x23c:
            return "DmResourceInfo";
        case (8 << 12) | 0x240:
            return "DmSearchRecord";
        case (8 << 12) | 0x244:
            return "DmSearchResource";
        case (8 << 12) | 0x248:
            return "DmSeekRecordInCategory";
        case (8 << 12) | 0x24c:
            return "DmSet";
        case (8 << 12) | 0x250:
            return "DmSetDatabaseInfo";
        case (8 << 12) | 0x254:
            return "DmSetDatabaseInfoV40";
        case (8 << 12) | 0x258:
            return "DmSetRecordInfo";
        case (8 << 12) | 0x25c:
            return "DmSetResourceInfo";
        case (8 << 12) | 0x260:
            return "DmStrCopy";
        case (8 << 12) | 0x264:
            return "DmWrite";
        case (8 << 12) | 0x268:
            return "DmWriteCheck";
        case (8 << 12) | 0x26c:
            return "EncDES";
        case (8 << 12) | 0x270:
            return "EncDigestMD4";
        case (8 << 12) | 0x274:
            return "EncDigestMD5";
        case (8 << 12) | 0x278:
            return "ErrAlertCustom";
        case (8 << 12) | 0x27c:
            return "ErrDisplayFileLineMsg";
        case (8 << 12) | 0x280:
            return "ErrExceptionList";
        case (8 << 12) | 0x284:
            return "ErrThrow";
        case (8 << 12) | 0x288:
            return "EvtDequeueKeyEvent";
        case (8 << 12) | 0x28c:
            return "EvtDequeuePenPoint";
        case (8 << 12) | 0x290:
            return "EvtDequeuePenStrokeInfo";
        case (8 << 12) | 0x294:
            return "EvtEnableGraffiti";
        case (8 << 12) | 0x298:
            return "EvtEnqueueKey";
        case (8 << 12) | 0x29c:
            return "EvtEnqueuePenPoint";
        case (8 << 12) | 0x2a0:
            return "EvtFlushKeyQueue";
        case (8 << 12) | 0x2a4:
            return "EvtFlushNextPenStroke";
        case (8 << 12) | 0x2a8:
            return "EvtFlushPenQueue";
        case (8 << 12) | 0x2ac:
            return "EvtGetPen";
        case (8 << 12) | 0x2b0:
            return "EvtGetPenBtnList";
        case (8 << 12) | 0x2b4:
            return "EvtGetSilkscreenAreaList";
        case (8 << 12) | 0x2b8:
            return "EvtGetSysEvent";
        case (8 << 12) | 0x2bc:
            return "EvtKeyQueueEmpty";
        case (8 << 12) | 0x2c0:
            return "EvtKeyQueueSize";
        case (8 << 12) | 0x2c4:
            return "EvtPenQueueSize";
        case (8 << 12) | 0x2c8:
            return "EvtProcessSoftKeyStroke";
        case (8 << 12) | 0x2cc:
            return "EvtResetAutoOffTimer";
        case (8 << 12) | 0x2d0:
            return "EvtSetAutoOffTimer";
        case (8 << 12) | 0x2d4:
            return "EvtSetKeyQueuePtr";
        case (8 << 12) | 0x2d8:
            return "EvtSetNullEventTick";
        case (8 << 12) | 0x2dc:
            return "EvtSetPenQueuePtr";
        case (8 << 12) | 0x2e0:
            return "EvtSysEventAvail";
        case (8 << 12) | 0x2e4:
            return "EvtSysInit";
        case (8 << 12) | 0x2e8:
            return "EvtWakeup";
        case (8 << 12) | 0x2ec:
            return "EvtWakeupWithoutNilEvent";
        case (8 << 12) | 0x2f0:
            return "ExgAccept";
        case (8 << 12) | 0x2f4:
            return "ExgConnect";
        case (8 << 12) | 0x2f8:
            return "ExgControl";
        case (8 << 12) | 0x2fc:
            return "ExgDBRead";
        case (8 << 12) | 0x300:
            return "ExgDBReadV40";
        case (8 << 12) | 0x304:
            return "ExgDBWrite";
        case (8 << 12) | 0x308:
            return "ExgDBWriteV40";
        case (8 << 12) | 0x30c:
            return "ExgDisconnect";
        case (8 << 12) | 0x310:
            return "ExgDoDialog";
        case (8 << 12) | 0x314:
            return "ExgGet";
        case (8 << 12) | 0x318:
            return "ExgGetDefaultApplication";
        case (8 << 12) | 0x31c:
            return "ExgGetRegisteredApplications";
        case (8 << 12) | 0x320:
            return "ExgGetRegisteredTypes";
        case (8 << 12) | 0x324:
            return "ExgGetTargetApplication";
        case (8 << 12) | 0x328:
            return "ExgInit";
        case (8 << 12) | 0x32c:
            return "ExgNotifyGoto";
        case (8 << 12) | 0x330:
            return "ExgNotifyPreview";
        case (8 << 12) | 0x334:
            return "ExgNotifyReceive";
        case (8 << 12) | 0x338:
            return "ExgNotifyReceiveV35";
        case (8 << 12) | 0x33c:
            return "ExgPut";
        case (8 << 12) | 0x340:
            return "ExgReceive";
        case (8 << 12) | 0x344:
            return "ExgRegisterData";
        case (8 << 12) | 0x348:
            return "ExgRegisterDatatype";
        case (8 << 12) | 0x34c:
            return "ExgRequest";
        case (8 << 12) | 0x350:
            return "ExgSend";
        case (8 << 12) | 0x354:
            return "ExgSetDefaultApplication";
        case (8 << 12) | 0x358:
            return "ExgLibControl";
        case (8 << 12) | 0x35c:
            return "ExgLibGetLibraryRef";
        case (8 << 12) | 0x360:
            return "ExgLibHandleEvent";
        case (8 << 12) | 0x364:
            return "ExpCardGetSerialPort";
        case (8 << 12) | 0x368:
            return "ExpCardInfo";
        case (8 << 12) | 0x36c:
            return "ExpCardInserted";
        case (8 << 12) | 0x370:
            return "ExpCardPresent";
        case (8 << 12) | 0x374:
            return "ExpCardRemoved";
        case (8 << 12) | 0x378:
            return "ExpInit";
        case (8 << 12) | 0x37c:
            return "ExpSlotDriverInstall";
        case (8 << 12) | 0x380:
            return "ExpSlotDriverRemove";
        case (8 << 12) | 0x384:
            return "ExpSlotEnumerate";
        case (8 << 12) | 0x388:
            return "ExpSlotLibFind";
        case (8 << 12) | 0x38c:
            return "ExpSlotRegister";
        case (8 << 12) | 0x390:
            return "ExpSlotUnregister";
        case (8 << 12) | 0x394:
            return "FileClose";
        case (8 << 12) | 0x398:
            return "FileControl";
        case (8 << 12) | 0x39c:
            return "FileDelete";
        case (8 << 12) | 0x3a0:
            return "FileDeleteV40";
        case (8 << 12) | 0x3a4:
            return "FileOpen";
        case (8 << 12) | 0x3a8:
            return "FileOpenV40";
        case (8 << 12) | 0x3ac:
            return "FileReadLow";
        case (8 << 12) | 0x3b0:
            return "FileSeek";
        case (8 << 12) | 0x3b4:
            return "FileTell";
        case (8 << 12) | 0x3b8:
            return "FileTruncate";
        case (8 << 12) | 0x3bc:
            return "FileWrite";
        case (8 << 12) | 0x3c0:
            return "FindSaveMatchV40";
        case (8 << 12) | 0x3c4:
            return "FlpAToF";
        case (8 << 12) | 0x3c8:
            return "FlpBase10Info";
        case (8 << 12) | 0x3cc:
            return "FlpCorrectedAdd";
        case (8 << 12) | 0x3d0:
            return "FlpCorrectedSub";
        case (8 << 12) | 0x3d4:
            return "FlpFToA";
        case (8 << 12) | 0x3d8:
            return "FlpVersion";
        case (8 << 12) | 0x3dc:
            return "FntAverageCharWidth";
        case (8 << 12) | 0x3e0:
            return "FntBaseLine";
        case (8 << 12) | 0x3e4:
            return "FntCharHeight";
        case (8 << 12) | 0x3e8:
            return "FntCharsInWidth";
        case (8 << 12) | 0x3ec:
            return "FntCharsWidth";
        case (8 << 12) | 0x3f0:
            return "FntCharWidth";
        case (8 << 12) | 0x3f4:
            return "FntDefineFont";
        case (8 << 12) | 0x3f8:
            return "FntDescenderHeight";
        case (8 << 12) | 0x3fc:
            return "FntGetFont";
        case (8 << 12) | 0x400:
            return "FntGetFontPtr";
        case (8 << 12) | 0x404:
            return "FntGetScrollValues";
        case (8 << 12) | 0x408:
            return "FntLineHeight";
        case (8 << 12) | 0x40c:
            return "FntLineWidth";
        case (8 << 12) | 0x410:
            return "FntSetFont";
        case (8 << 12) | 0x414:
            return "FntWCharWidth";
        case (8 << 12) | 0x418:
            return "FntWidthToOffset";
        case (8 << 12) | 0x41c:
            return "FntWordWrap";
        case (8 << 12) | 0x420:
            return "FntWordWrapReverseNLines";
        case (8 << 12) | 0x424:
            return "FtrGet";
        case (8 << 12) | 0x428:
            return "FtrGetByIndex";
        case (8 << 12) | 0x42c:
            return "FtrInit";
        case (8 << 12) | 0x430:
            return "FtrPtrFree";
        case (8 << 12) | 0x434:
            return "FtrPtrNew";
        case (8 << 12) | 0x438:
            return "FtrPtrResize";
        case (8 << 12) | 0x43c:
            return "FtrSet";
        case (8 << 12) | 0x440:
            return "FtrUnregister";
        case (8 << 12) | 0x444:
            return "GetCharAttr";
        case (8 << 12) | 0x448:
            return "GetCharCaselessValue";
        case (8 << 12) | 0x44c:
            return "GetCharSortValue";
        case (8 << 12) | 0x450:
            return "TmOutputB";
        case (8 << 12) | 0x454:
            return "TmOutputT";
        case (8 << 12) | 0x458:
            return "TmOutputTL";
        case (8 << 12) | 0x45c:
            return "TmOutputVT";
        case (8 << 12) | 0x460:
            return "TmOutputVTL";
        case (8 << 12) | 0x464:
            return "HSUtilReverseDWord";
        case (8 << 12) | 0x468:
            return "HSUtilReverseWord";
        case (8 << 12) | 0x46c:
            return "ImcReadFieldNoSemicolon";
        case (8 << 12) | 0x470:
            return "ImcReadFieldQuotablePrintable";
        case (8 << 12) | 0x474:
            return "ImcReadPropertyParameter";
        case (8 << 12) | 0x478:
            return "ImcReadWhiteSpace";
        case (8 << 12) | 0x47c:
            return "ImcSkipAllPropertyParameters";
        case (8 << 12) | 0x480:
            return "ImcStringIsAscii";
        case (8 << 12) | 0x484:
            return "ImcWriteNoSemicolon";
        case (8 << 12) | 0x488:
            return "ImcWriteQuotedPrintable";
        case (8 << 12) | 0x48c:
            return "KeyCurrentState";
        case (8 << 12) | 0x490:
            return "KeyRates";
        case (8 << 12) | 0x494:
            return "KeySetMask";
        case (8 << 12) | 0x498:
            return "LmGetLocaleSetting";
        case (8 << 12) | 0x49c:
            return "LmGetNumLocales";
        case (8 << 12) | 0x4a0:
            return "LmLocaleToIndex";
        case (8 << 12) | 0x4a4:
            return "LocGetNumberSeparators";
        case (8 << 12) | 0x4a8:
            return "LstSetDrawFunctionV40";
        case (8 << 12) | 0x4ac:
            return "MdmDial";
        case (8 << 12) | 0x4b0:
            return "MdmHangUp";
        case (8 << 12) | 0x4b4:
            return "MemCardInfoV40";
        case (8 << 12) | 0x4b8:
            return "MemChunkFree";
        case (8 << 12) | 0x4bc:
            return "MemChunkNew";
        case (8 << 12) | 0x4c0:
            return "MemCmp";
        case (8 << 12) | 0x4c4:
            return "MemDebugMode";
        case (8 << 12) | 0x4c8:
            return "MemFormat";
        case (8 << 12) | 0x4cc:
            return "MemGetRomNVParams";
        case (8 << 12) | 0x4d0:
            return "MemHandleCardNoV40";
        case (8 << 12) | 0x4d4:
            return "MemHandleDataStorage";
        case (8 << 12) | 0x4d8:
            return "MemHandleFlags";
        case (8 << 12) | 0x4dc:
            return "MemHandleFree";
        case (8 << 12) | 0x4e0:
            return "MemHandleHeapID";
        case (8 << 12) | 0x4e4:
            return "MemHandleLock";
        case (8 << 12) | 0x4e8:
            return "MemHandleLockCount";
        case (8 << 12) | 0x4ec:
            return "MemHandleNew";
        case (8 << 12) | 0x4f0:
            return "MemHandleOwner";
        case (8 << 12) | 0x4f4:
            return "MemHandleResetLock";
        case (8 << 12) | 0x4f8:
            return "MemHandleResize";
        case (8 << 12) | 0x4fc:
            return "MemHandleSetOwner";
        case (8 << 12) | 0x500:
            return "MemHandleSize";
        case (8 << 12) | 0x504:
            return "MemHandleToLocalIDV40";
        case (8 << 12) | 0x508:
            return "MemHandleUnlock";
        case (8 << 12) | 0x50c:
            return "MemHeapCheck";
        case (8 << 12) | 0x510:
            return "MemHeapCompact";
        case (8 << 12) | 0x514:
            return "MemHeapDynamic";
        case (8 << 12) | 0x518:
            return "MemHeapFlags";
        case (8 << 12) | 0x51c:
            return "MemHeapFreeByOwnerID";
        case (8 << 12) | 0x520:
            return "MemHeapFreeBytes";
        case (8 << 12) | 0x524:
            return "MemHeapID";
        case (8 << 12) | 0x528:
            return "MemHeapIDV40";
        case (8 << 12) | 0x52c:
            return "MemHeapInit";
        case (8 << 12) | 0x530:
            return "MemHeapPtr";
        case (8 << 12) | 0x534:
            return "MemHeapScramble";
        case (8 << 12) | 0x538:
            return "MemHeapSize";
        case (8 << 12) | 0x53c:
            return "MemInfo";
        case (8 << 12) | 0x540:
            return "MemInit";
        case (8 << 12) | 0x544:
            return "MemKernelInit";
        case (8 << 12) | 0x548:
            return "MemLocalIDKindV40";
        case (8 << 12) | 0x54c:
            return "MemLocalIDToGlobalV40";
        case (8 << 12) | 0x550:
            return "MemLocalIDToLockedPtrV40";
        case (8 << 12) | 0x554:
            return "MemLocalIDToPtrV40";
        case (8 << 12) | 0x558:
            return "MemMove";
        case (8 << 12) | 0x55c:
            return "MemNumCardsV40";
        case (8 << 12) | 0x560:
            return "MemNumHeaps";
        case (8 << 12) | 0x564:
            return "MemNumHeapsV40";
        case (8 << 12) | 0x568:
            return "MemNumRAMHeaps";
        case (8 << 12) | 0x56c:
            return "MemNumRAMHeapsV40";
        case (8 << 12) | 0x570:
            return "MemNVParams";
        case (8 << 12) | 0x574:
            return "MemPtrCardNoV40";
        case (8 << 12) | 0x578:
            return "MemPtrDataStorage";
        case (8 << 12) | 0x57c:
            return "MemPtrFlags";
        case (8 << 12) | 0x580:
            return "MemPtrHeapID";
        case (8 << 12) | 0x584:
            return "MemPtrNew";
        case (8 << 12) | 0x588:
            return "MemPtrOwner";
        case (8 << 12) | 0x58c:
            return "MemPtrRecoverHandle";
        case (8 << 12) | 0x590:
            return "MemPtrResetLock";
        case (8 << 12) | 0x594:
            return "MemPtrResize";
        case (8 << 12) | 0x598:
            return "MemPtrSetOwner";
        case (8 << 12) | 0x59c:
            return "MemPtrSize";
        case (8 << 12) | 0x5a0:
            return "MemPtrToLocalIDV40";
        case (8 << 12) | 0x5a4:
            return "MemPtrUnlock";
        case (8 << 12) | 0x5a8:
            return "MemSemaphoreRelease";
        case (8 << 12) | 0x5ac:
            return "MemSemaphoreReserve";
        case (8 << 12) | 0x5b0:
            return "MemSet";
        case (8 << 12) | 0x5b4:
            return "MemSetDebugMode";
        case (8 << 12) | 0x5b8:
            return "MemStoreInfo";
        case (8 << 12) | 0x5bc:
            return "MemStoreInfoV40";
        case (8 << 12) | 0x5c0:
            return "MemStoreSearch";
        case (8 << 12) | 0x5c4:
            return "MemStoreSetInfo";
        case (8 << 12) | 0x5c8:
            return "MemStoreSetInfoV40";
        case (8 << 12) | 0x5cc:
            return "OmGetCurrentLocale";
        case (8 << 12) | 0x5d0:
            return "OmGetIndexedLocale";
        case (8 << 12) | 0x5d4:
            return "OmGetNextSystemLocale";
        case (8 << 12) | 0x5d8:
            return "OmGetSystemLocale";
        case (8 << 12) | 0x5dc:
            return "OmLocaleToOverlayDBName";
        case (8 << 12) | 0x5e0:
            return "OmOverlayDBNameToLocale";
        case (8 << 12) | 0x5e4:
            return "OmSetSystemLocale";
        case (8 << 12) | 0x5e8:
            return "PenCalibrate";
        case (8 << 12) | 0x5ec:
            return "PenResetCalibration";
        case (8 << 12) | 0x5f0:
            return "PrefGetAppPreferences";
        case (8 << 12) | 0x5f4:
            return "PrefGetAppPreferencesV10";
        case (8 << 12) | 0x5f8:
            return "PrefGetPreference";
        case (8 << 12) | 0x5fc:
            return "PrefGetPreferences";
        case (8 << 12) | 0x600:
            return "PrefOpenPreferenceDB";
        case (8 << 12) | 0x604:
            return "PrefOpenPreferenceDBV10";
        case (8 << 12) | 0x608:
            return "PrefSetAppPreferences";
        case (8 << 12) | 0x60c:
            return "PrefSetAppPreferencesV10";
        case (8 << 12) | 0x610:
            return "PrefSetPreference";
        case (8 << 12) | 0x614:
            return "PrefSetPreferences";
        case (8 << 12) | 0x618:
            return "PrvCncCreateDatabaseFromDefault";
        case (8 << 12) | 0x61c:
            return "PrvCncMgrInit";
        case (8 << 12) | 0x620:
            return "PrvCncPostHotSyncDBCleanup";
        case (8 << 12) | 0x624:
            return "PrvCncProfileBuildUniqueName";
        case (8 << 12) | 0x628:
            return "PrvCncProfileDuplicate";
        case (8 << 12) | 0x62c:
            return "PwdExists";
        case (8 << 12) | 0x630:
            return "PwdRemove";
        case (8 << 12) | 0x634:
            return "PwdSet";
        case (8 << 12) | 0x638:
            return "PwdVerify";
        case (8 << 12) | 0x63c:
            return "RctCopyRectangle";
        case (8 << 12) | 0x640:
            return "RctGetIntersection";
        case (8 << 12) | 0x644:
            return "RctInsetRectangle";
        case (8 << 12) | 0x648:
            return "RctOffsetRectangle";
        case (8 << 12) | 0x64c:
            return "RctPtInRectangle";
        case (8 << 12) | 0x650:
            return "RctSetRectangle";
        case (8 << 12) | 0x654:
            return "ScrCompress";
        case (8 << 12) | 0x658:
            return "ScrCompressScanLine";
        case (8 << 12) | 0x65c:
            return "ScrDecompress";
        case (8 << 12) | 0x660:
            return "ScrDeCompressScanLine";
        case (8 << 12) | 0x664:
            return "SecGetPwdHint";
        case (8 << 12) | 0x668:
            return "SecSetPwdHint";
        case (8 << 12) | 0x66c:
            return "SerialMgrInstall";
        case (8 << 12) | 0x670:
            return "SlkClose";
        case (8 << 12) | 0x674:
            return "SlkCloseSocket";
        case (8 << 12) | 0x678:
            return "SlkFlushSocket";
        case (8 << 12) | 0x67c:
            return "SlkOpen";
        case (8 << 12) | 0x680:
            return "SlkOpenSocket";
        case (8 << 12) | 0x684:
            return "SlkProcessRPC";
        case (8 << 12) | 0x688:
            return "SlkReceivePacket";
        case (8 << 12) | 0x68c:
            return "SlkSendPacket";
        case (8 << 12) | 0x690:
            return "SlkSetSocketListener";
        case (8 << 12) | 0x694:
            return "SlkSocketPortID";
        case (8 << 12) | 0x698:
            return "SlkSocketSetTimeout";
        case (8 << 12) | 0x69c:
            return "SlkSysPktDefaultResponse";
        case (8 << 12) | 0x6a0:
            return "SlotCardGetSerialPort";
        case (8 << 12) | 0x6a4:
            return "SlotCardInfo";
        case (8 << 12) | 0x6a8:
            return "SlotCardIsFilesystemSupported";
        case (8 << 12) | 0x6ac:
            return "SlotCardLowLevelFormat";
        case (8 << 12) | 0x6b0:
            return "SlotCardMediaType";
        case (8 << 12) | 0x6b4:
            return "SlotCardMetrics";
        case (8 << 12) | 0x6b8:
            return "SlotCardPresent";
        case (8 << 12) | 0x6bc:
            return "SlotCardRelease";
        case (8 << 12) | 0x6c0:
            return "SlotCardReserve";
        case (8 << 12) | 0x6c4:
            return "SlotCardSectorRead";
        case (8 << 12) | 0x6c8:
            return "SlotCardSectorWrite";
        case (8 << 12) | 0x6cc:
            return "SlotClose";
        case (8 << 12) | 0x6d0:
            return "SlotCustomControl";
        case (8 << 12) | 0x6d4:
            return "SlotOpen";
        case (8 << 12) | 0x6d8:
            return "SlotLibAPIVersion";
        case (8 << 12) | 0x6dc:
            return "SlotPowerCheck";
        case (8 << 12) | 0x6e0:
            return "SlotSleep";
        case (8 << 12) | 0x6e4:
            return "SlotWake";
        case (8 << 12) | 0x6e8:
            return "SndCreateMidiList";
        case (8 << 12) | 0x6ec:
            return "SndDoCmd";
        case (8 << 12) | 0x6f0:
            return "SndGetDefaultVolume";
        case (8 << 12) | 0x6f4:
            return "SndInit";
        case (8 << 12) | 0x6f8:
            return "SndPlaySmf";
        case (8 << 12) | 0x6fc:
            return "SndPlaySmfResource";
        case (8 << 12) | 0x700:
            return "SndPlaySystemSound";
        case (8 << 12) | 0x704:
            return "SndSetDefaultVolume";
        case (8 << 12) | 0x708:
            return "SrmClearErr";
        case (8 << 12) | 0x70c:
            return "SrmClose";
        case (8 << 12) | 0x710:
            return "SrmControl";
        case (8 << 12) | 0x714:
            return "SrmCustomControl";
        case (8 << 12) | 0x718:
            return "SrmExtOpen";
        case (8 << 12) | 0x71c:
            return "SrmExtOpenBackground";
        case (8 << 12) | 0x720:
            return "SrmGetDeviceCount";
        case (8 << 12) | 0x724:
            return "SrmGetDeviceInfo";
        case (8 << 12) | 0x728:
            return "SrmGetStatus";
        case (8 << 12) | 0x72c:
            return "SrmOpen";
        case (8 << 12) | 0x730:
            return "SrmOpenBackground";
        case (8 << 12) | 0x734:
            return "SrmPrimeWakeupHandler";
        case (8 << 12) | 0x738:
            return "SrmReceive";
        case (8 << 12) | 0x73c:
            return "SrmReceiveCheck";
        case (8 << 12) | 0x740:
            return "SrmReceiveFlush";
        case (8 << 12) | 0x744:
            return "SrmReceiveWait";
        case (8 << 12) | 0x748:
            return "SrmReceiveWindowClose";
        case (8 << 12) | 0x74c:
            return "SrmReceiveWindowOpen";
        case (8 << 12) | 0x750:
            return "SrmSend";
        case (8 << 12) | 0x754:
            return "SrmSendCheck";
        case (8 << 12) | 0x758:
            return "SrmSendFlush";
        case (8 << 12) | 0x75c:
            return "SrmSendWait";
        case (8 << 12) | 0x760:
            return "SrmSetReceiveBuffer";
        case (8 << 12) | 0x764:
            return "SrmSetWakeupHandler";
        case (8 << 12) | 0x768:
            return "SrmSleep";
        case (8 << 12) | 0x76c:
            return "SrmWake";
        case (8 << 12) | 0x770:
            return "StrAToI";
        case (8 << 12) | 0x774:
            return "StrCaselessCompare";
        case (8 << 12) | 0x778:
            return "StrCat";
        case (8 << 12) | 0x77c:
            return "StrChr";
        case (8 << 12) | 0x780:
            return "StrCompare";
        case (8 << 12) | 0x784:
            return "StrCompareAscii";
        case (8 << 12) | 0x788:
            return "StrCopy";
        case (8 << 12) | 0x78c:
            return "StrDelocalizeNumber";
        case (8 << 12) | 0x790:
            return "StrIToA";
        case (8 << 12) | 0x794:
            return "StrIToH";
        case (8 << 12) | 0x798:
            return "StrLen";
        case (8 << 12) | 0x79c:
            return "StrLocalizeNumber";
        case (8 << 12) | 0x7a0:
            return "StrNCaselessCompare";
        case (8 << 12) | 0x7a4:
            return "StrNCat";
        case (8 << 12) | 0x7a8:
            return "StrNCompare";
        case (8 << 12) | 0x7ac:
            return "StrNCompareAscii";
        case (8 << 12) | 0x7b0:
            return "StrNCopy";
        case (8 << 12) | 0x7b4:
            return "StrPrintF";
        case (8 << 12) | 0x7b8:
            return "StrStr";
        case (8 << 12) | 0x7bc:
            return "StrToLower";
        case (8 << 12) | 0x7c0:
            return "StrVPrintF";
        case (8 << 12) | 0x7c4:
            return "SysAppExit";
        case (8 << 12) | 0x7c8:
            return "SysAppLaunch";
        case (8 << 12) | 0x7cc:
            return "SysAppLaunchV40";
        case (8 << 12) | 0x7d0:
            return "SysAppStartup";
        case (8 << 12) | 0x7d4:
            return "SysAreWeUIThread";
        case (8 << 12) | 0x7d8:
            return "SysBatteryInfo";
        case (8 << 12) | 0x7dc:
            return "SysBatteryInfoV20";
        case (8 << 12) | 0x7e0:
            return "SysBatteryInfoV40";
        case (8 << 12) | 0x7e4:
            return "SysBinarySearch";
        case (8 << 12) | 0x7e8:
            return "SysBroadcastActionCode";
        case (8 << 12) | 0x7ec:
            return "SysCheckPwdTimeout";
        case (8 << 12) | 0x7f0:
            return "SysColdBoot";
        case (8 << 12) | 0x7f4:
            return "SysCopyStringResource";
        case (8 << 12) | 0x7f8:
            return "SysCreateDataBaseList";
        case (8 << 12) | 0x7fc:
            return "SysCreatePanelList";
        case (8 << 12) | 0x800:
            return "SysCurAppDatabase";
        case (8 << 12) | 0x804:
            return "SysCurAppDatabaseV40";
        case (8 << 12) | 0x808:
            return "SysCurAppInfoPV20";
        case (8 << 12) | 0x80c:
            return "SysDisableInts";
        case (8 << 12) | 0x810:
            return "SysErrString";
        case (8 << 12) | 0x814:
            return "SysEventAddToQueue";
        case (8 << 12) | 0x818:
            return "SysEventAddUniqueToQueue";
        case (8 << 12) | 0x81c:
            return "SysEventAvail";
        case (8 << 12) | 0x820:
            return "SysEventCopy";
        case (8 << 12) | 0x824:
            return "SysEventGet";
        case (8 << 12) | 0x828:
            return "SysEventInitialize";
        case (8 << 12) | 0x82c:
            return "SysFindModule";
        case (8 << 12) | 0x830:
            return "SysFormPointerArrayToStrings";
        case (8 << 12) | 0x834:
            return "SysGetAppInfo";
        case (8 << 12) | 0x838:
            return "SysGetEntryAddresses";
        case (8 << 12) | 0x83c:
            return "SysGetOSVersionString";
        case (8 << 12) | 0x840:
            return "SysGetROMToken";
        case (8 << 12) | 0x844:
            return "SysGetROMTokenV40";
        case (8 << 12) | 0x848:
            return "SysGetStackInfo";
        case (8 << 12) | 0x84c:
            return "SysHandleEvent";
        case (8 << 12) | 0x850:
            return "SysInit";
        case (8 << 12) | 0x854:
            return "SysInsertionSort";
        case (8 << 12) | 0x858:
            return "SysLCDBrightness";
        case (8 << 12) | 0x85c:
            return "SysLCDContrast";
        case (8 << 12) | 0x860:
            return "SysLibLoad";
        case (8 << 12) | 0x864:
            return "SysLibRemove";
        case (8 << 12) | 0x868:
            return "SysLibFindV40";
        case (8 << 12) | 0x86c:
            return "SysLibInstallV40";
        case (8 << 12) | 0x870:
            return "SysLibLoadV40";
        case (8 << 12) | 0x874:
            return "SysLibRemoveV40";
        case (8 << 12) | 0x878:
            return "SysLibTblEntry";
        case (8 << 12) | 0x87c:
            return "SysLinkerStub";
        case (8 << 12) | 0x880:
            return "SysLoadModule";
        case (8 << 12) | 0x884:
            return "SysNewOwnerID";
        case (8 << 12) | 0x888:
            return "SysNotifyBroadcast";
        case (8 << 12) | 0x88c:
            return "SysNotifyBroadcastDeferred";
        case (8 << 12) | 0x890:
            return "SysNotifyDatabaseAdded";
        case (8 << 12) | 0x894:
            return "SysNotifyDatabaseRemoved";
        case (8 << 12) | 0x898:
            return "SysNotifyInit";
        case (8 << 12) | 0x89c:
            return "SysNotifyRegister";
        case (8 << 12) | 0x8a0:
            return "SysNotifyRegisterV40";
        case (8 << 12) | 0x8a4:
            return "SysNotifyUnregister";
        case (8 << 12) | 0x8a8:
            return "SysNotifyUnregisterV40";
        case (8 << 12) | 0x8ac:
            return "SysNotifyBroadcastFromInterrupt";
        case (8 << 12) | 0x8b0:
            return "SysPatchEntry";
        case (8 << 12) | 0x8b4:
            return "SysQSort";
        case (8 << 12) | 0x8b8:
            return "SysRandom";
        case (8 << 12) | 0x8bc:
            return "SysReset";
        case (8 << 12) | 0x8c0:
            return "SysRestoreStatus";
        case (8 << 12) | 0x8c4:
            return "SysSetA5";
        case (8 << 12) | 0x8c8:
            return "SysSetAutoOffTime";
        case (8 << 12) | 0x8cc:
            return "SysSetDbgLockout";
        case (8 << 12) | 0x8d0:
            return "SysSetDevAutoLockTime";
        case (8 << 12) | 0x8d4:
            return "SysSetPwdTimeout";
        case (8 << 12) | 0x8d8:
            return "SysSleep";
        case (8 << 12) | 0x8dc:
            return "SysSleepV40";
        case (8 << 12) | 0x8e0:
            return "SysStringByIndex";
        case (8 << 12) | 0x8e4:
            return "SysTaskCreate";
        case (8 << 12) | 0x8e8:
            return "SysTaskDelay";
        case (8 << 12) | 0x8ec:
            return "SysTaskDelayV40";
        case (8 << 12) | 0x8f0:
            return "SysTaskDelete";
        case (8 << 12) | 0x8f4:
            return "SysTicksPerSecond";
        case (8 << 12) | 0x8f8:
            return "SysUIAppSwitch";
        case (8 << 12) | 0x8fc:
            return "SysUIAppSwitchV40";
        case (8 << 12) | 0x900:
            return "SysUIBusy";
        case (8 << 12) | 0x904:
            return "SysUILaunch";
        case (8 << 12) | 0x908:
            return "SysUnloadModule";
        case (8 << 12) | 0x90c:
            return "SysUnimplemented";
        case (8 << 12) | 0x910:
            return "SysWantEvent";
        case (8 << 12) | 0x914:
            return "TimAdjust";
        case (8 << 12) | 0x918:
            return "TimDateTimeToSeconds";
        case (8 << 12) | 0x91c:
            return "TimeToAscii";
        case (8 << 12) | 0x920:
            return "TimGetAlarm";
        case (8 << 12) | 0x924:
            return "TimGetSeconds";
        case (8 << 12) | 0x928:
            return "TimGetTicks";
        case (8 << 12) | 0x92c:
            return "TimSecondsToDateTime";
        case (8 << 12) | 0x930:
            return "TimSetAlarm";
        case (8 << 12) | 0x934:
            return "TimSetSeconds";
        case (8 << 12) | 0x938:
            return "TimTimeZoneToUTC";
        case (8 << 12) | 0x93c:
            return "TimUTCToTimeZone";
        case (8 << 12) | 0x940:
            return "TlsSetSystemData";
        case (8 << 12) | 0x944:
            return "TsmDrawMode";
        case (8 << 12) | 0x948:
            return "TsmFepCommitAction";
        case (8 << 12) | 0x94c:
            return "TsmFepHandleEvent";
        case (8 << 12) | 0x950:
            return "TsmFepMapEvent";
        case (8 << 12) | 0x954:
            return "TsmFepReset";
        case (8 << 12) | 0x958:
            return "TsmFepTerminate";
        case (8 << 12) | 0x95c:
            return "TsmGetCurrentFepCreator";
        case (8 << 12) | 0x960:
            return "TsmGetFepMode";
        case (8 << 12) | 0x964:
            return "TsmGetSystemFepCreator";
        case (8 << 12) | 0x968:
            return "TsmHandleEvent";
        case (8 << 12) | 0x96c:
            return "TsmInit";
        case (8 << 12) | 0x970:
            return "TsmSetCurrentFepCreator";
        case (8 << 12) | 0x974:
            return "TsmSetFepMode";
        case (8 << 12) | 0x978:
            return "TsmSetSystemFepCreator";
        case (8 << 12) | 0x97c:
            return "TxtByteAttr";
        case (8 << 12) | 0x980:
            return "TxtCaselessCompare";
        case (8 << 12) | 0x984:
            return "TxtCharAttr";
        case (8 << 12) | 0x988:
            return "TxtCharEncoding";
        case (8 << 12) | 0x98c:
            return "TxtCharIsValid";
        case (8 << 12) | 0x990:
            return "TxtCharWidth";
        case (8 << 12) | 0x994:
            return "TxtCharXAttr";
        case (8 << 12) | 0x998:
            return "TxtCharBounds";
        case (8 << 12) | 0x99c:
            return "TxtCharSize";
        case (8 << 12) | 0x9a0:
            return "TxtCompare";
        case (8 << 12) | 0x9a4:
            return "TxtConvertEncoding";
        case (8 << 12) | 0x9a8:
            return "TxtEncodingName";
        case (8 << 12) | 0x9ac:
            return "TxtFindString";
        case (8 << 12) | 0x9b0:
            return "TxtGetChar";
        case (8 << 12) | 0x9b4:
            return "TxtGetNextChar";
        case (8 << 12) | 0x9b8:
            return "TxtGetPreviousChar";
        case (8 << 12) | 0x9bc:
            return "TxtGetTruncationOffset";
        case (8 << 12) | 0x9c0:
            return "TxtGetWordWrapOffset";
        case (8 << 12) | 0x9c4:
            return "TxtMaxEncoding";
        case (8 << 12) | 0x9c8:
            return "TxtNameToEncoding";
        case (8 << 12) | 0x9cc:
            return "TxtParamString";
        case (8 << 12) | 0x9d0:
            return "TxtPrepFindString";
        case (8 << 12) | 0x9d4:
            return "TxtReplaceStr";
        case (8 << 12) | 0x9d8:
            return "TxtSetNextChar";
        case (8 << 12) | 0x9dc:
            return "TxtStrEncoding";
        case (8 << 12) | 0x9e0:
            return "TxtTransliterate";
        case (8 << 12) | 0x9e4:
            return "TxtWordBounds";
        case (8 << 12) | 0x9e8:
            return "UDAControl";
        case (8 << 12) | 0x9ec:
            return "UDAExchangeReaderNew";
        case (8 << 12) | 0x9f0:
            return "UDAExchangeWriterNew";
        case (8 << 12) | 0x9f4:
            return "UDAMemoryReaderNew";
        case (8 << 12) | 0x9f8:
            return "UDAFoldFilterNew";
        case (8 << 12) | 0x9fc:
            return "UDAUnfoldFilterNew";
        case (8 << 12) | 0xa00:
            return "UDAQPEncoderFilterNew";
        case (8 << 12) | 0xa04:
            return "UDAQPDecoderFilterNew";
        case (8 << 12) | 0xa08:
            return "UDAB64DecoderFilterNew";
        case (8 << 12) | 0xa0c:
            return "UDAB64EncoderFilterNew";
        case (8 << 12) | 0xa10:
            return "UDAEscapeFilterNew";
        case (8 << 12) | 0xa14:
            return "UDAUnEscapeFilterNew";
        case (8 << 12) | 0xa18:
            return "UDAPartFilterNew";
        case (8 << 12) | 0xa1c:
            return "UDAMemoryReaderNewTo68K";
        case (8 << 12) | 0xa20:
            return "UDAExchangeReaderNewTo68K";
        case (8 << 12) | 0xa24:
            return "UDAExchangeWriterNewTo68K";
        case (8 << 12) | 0xa28:
            return "UDAReaderToARM";
        case (8 << 12) | 0xa2c:
            return "UDAReaderTo68K";
        case (8 << 12) | 0xa30:
            return "UDAWriterToARM";
        case (8 << 12) | 0xa34:
            return "UDAWriterTo68K";
        case (8 << 12) | 0xa38:
            return "VFSCustomControl";
        case (8 << 12) | 0xa3c:
            return "VFSDirCreate";
        case (8 << 12) | 0xa40:
            return "VFSDirEntryEnumerate";
        case (8 << 12) | 0xa44:
            return "VFSExportDatabaseToFile";
        case (8 << 12) | 0xa48:
            return "VFSExportDatabaseToFileV40";
        case (8 << 12) | 0xa4c:
            return "VFSExportDatabaseToFileCustom";
        case (8 << 12) | 0xa50:
            return "VFSExportDatabaseToFileCustomV40";
        case (8 << 12) | 0xa54:
            return "VFSFileClose";
        case (8 << 12) | 0xa58:
            return "VFSFileCreate";
        case (8 << 12) | 0xa5c:
            return "VFSFileDBGetRecord";
        case (8 << 12) | 0xa60:
            return "VFSFileDBGetResource";
        case (8 << 12) | 0xa64:
            return "VFSFileDBInfo";
        case (8 << 12) | 0xa68:
            return "VFSFileDelete";
        case (8 << 12) | 0xa6c:
            return "VFSFileEOF";
        case (8 << 12) | 0xa70:
            return "VFSFileGetAttributes";
        case (8 << 12) | 0xa74:
            return "VFSFileGetDate";
        case (8 << 12) | 0xa78:
            return "VFSFileOpen";
        case (8 << 12) | 0xa7c:
            return "VFSFileRead";
        case (8 << 12) | 0xa80:
            return "VFSFileReadData";
        case (8 << 12) | 0xa84:
            return "VFSFileRename";
        case (8 << 12) | 0xa88:
            return "VFSFileResize";
        case (8 << 12) | 0xa8c:
            return "VFSFileSeek";
        case (8 << 12) | 0xa90:
            return "VFSFileSetAttributes";
        case (8 << 12) | 0xa94:
            return "VFSFileSetDate";
        case (8 << 12) | 0xa98:
            return "VFSFileSize";
        case (8 << 12) | 0xa9c:
            return "VFSFileTell";
        case (8 << 12) | 0xaa0:
            return "VFSFileWrite";
        case (8 << 12) | 0xaa4:
            return "VFSGetDefaultDirectory";
        case (8 << 12) | 0xaa8:
            return "VFSImportDatabaseFromFile";
        case (8 << 12) | 0xaac:
            return "VFSImportDatabaseFromFileV40";
        case (8 << 12) | 0xab0:
            return "VFSImportDatabaseFromFileCustom";
        case (8 << 12) | 0xab4:
            return "VFSImportDatabaseFromFileCustomV40";
        case (8 << 12) | 0xab8:
            return "VFSInit";
        case (8 << 12) | 0xabc:
            return "VFSInstallFSLib";
        case (8 << 12) | 0xac0:
            return "VFSRegisterDefaultDirectory";
        case (8 << 12) | 0xac4:
            return "VFSRemoveFSLib";
        case (8 << 12) | 0xac8:
            return "VFSUnregisterDefaultDirectory";
        case (8 << 12) | 0xacc:
            return "VFSVolumeEnumerate";
        case (8 << 12) | 0xad0:
            return "VFSVolumeFormat";
        case (8 << 12) | 0xad4:
            return "VFSVolumeGetLabel";
        case (8 << 12) | 0xad8:
            return "VFSVolumeInfo";
        case (8 << 12) | 0xadc:
            return "VFSVolumeMount";
        case (8 << 12) | 0xae0:
            return "VFSVolumeSetLabel";
        case (8 << 12) | 0xae4:
            return "VFSVolumeSize";
        case (8 << 12) | 0xae8:
            return "VFSVolumeUnmount";
        case (8 << 12) | 0xaec:
            return "WinAddWindow";
        case (8 << 12) | 0xaf0:
            return "WinClipRectangle";
        case (8 << 12) | 0xaf4:
            return "WinCopyRectangle";
        case (8 << 12) | 0xaf8:
            return "WinCreateBitmapWindow";
        case (8 << 12) | 0xafc:
            return "WinCreateOffscreenWindow";
        case (8 << 12) | 0xb00:
            return "WinCreateWindow";
        case (8 << 12) | 0xb04:
            return "WinDeleteWindow";
        case (8 << 12) | 0xb08:
            return "WinDisableWindow";
        case (8 << 12) | 0xb0c:
            return "WinDisplayToWindowPt";
        case (8 << 12) | 0xb10:
            return "WinDrawBitmap";
        case (8 << 12) | 0xb14:
            return "WinDrawChar";
        case (8 << 12) | 0xb18:
            return "WinDrawChars";
        case (8 << 12) | 0xb1c:
            return "WinDrawGrayLine";
        case (8 << 12) | 0xb20:
            return "WinDrawGrayRectangleFrame";
        case (8 << 12) | 0xb24:
            return "WinDrawInvertedChars";
        case (8 << 12) | 0xb28:
            return "WinDrawLine";
        case (8 << 12) | 0xb2c:
            return "WinDrawPixel";
        case (8 << 12) | 0xb30:
            return "WinDrawRectangle";
        case (8 << 12) | 0xb34:
            return "WinDrawRectangleFrame";
        case (8 << 12) | 0xb38:
            return "WinDrawTruncChars";
        case (8 << 12) | 0xb3c:
            return "WinDrawWindowFrame";
        case (8 << 12) | 0xb40:
            return "WinEnableWindow";
        case (8 << 12) | 0xb44:
            return "WinEraseChars";
        case (8 << 12) | 0xb48:
            return "WinEraseLine";
        case (8 << 12) | 0xb4c:
            return "WinErasePixel";
        case (8 << 12) | 0xb50:
            return "WinEraseRectangle";
        case (8 << 12) | 0xb54:
            return "WinEraseRectangleFrame";
        case (8 << 12) | 0xb58:
            return "WinEraseWindow";
        case (8 << 12) | 0xb5c:
            return "WinFillLine";
        case (8 << 12) | 0xb60:
            return "WinFillRectangle";
        case (8 << 12) | 0xb64:
            return "WinGetActiveWindow";
        case (8 << 12) | 0xb68:
            return "WinGetBitmap";
        case (8 << 12) | 0xb6c:
            return "WinGetClip";
        case (8 << 12) | 0xb70:
            return "WinGetDisplayExtent";
        case (8 << 12) | 0xb74:
            return "WinGetDisplayWindow";
        case (8 << 12) | 0xb78:
            return "WinGetDrawWindow";
        case (8 << 12) | 0xb7c:
            return "WinGetDrawWindowBounds";
        case (8 << 12) | 0xb80:
            return "WinGetFirstWindow";
        case (8 << 12) | 0xb84:
            return "WinGetFramesRectangle";
        case (8 << 12) | 0xb88:
            return "WinGetPattern";
        case (8 << 12) | 0xb8c:
            return "WinGetPatternType";
        case (8 << 12) | 0xb90:
            return "WinGetPixel";
        case (8 << 12) | 0xb94:
            return "WinGetPixelRGB";
        case (8 << 12) | 0xb98:
            return "WinGetBounds";
        case (8 << 12) | 0xb9c:
            return "WinGetWindowExtent";
        case (8 << 12) | 0xba0:
            return "WinGetWindowFrameRect";
        case (8 << 12) | 0xba4:
            return "WinIndexToRGB";
        case (8 << 12) | 0xba8:
            return "WinInitializeWindow";
        case (8 << 12) | 0xbac:
            return "WinInvertChars";
        case (8 << 12) | 0xbb0:
            return "WinInvertLine";
        case (8 << 12) | 0xbb4:
            return "WinInvertPixel";
        case (8 << 12) | 0xbb8:
            return "WinInvertRectangle";
        case (8 << 12) | 0xbbc:
            return "WinInvertRectangleFrame";
        case (8 << 12) | 0xbc0:
            return "WinModal";
        case (8 << 12) | 0xbc4:
            return "WinMoveWindowAddr";
        case (8 << 12) | 0xbc8:
            return "WinPaintBitmap";
        case (8 << 12) | 0xbcc:
            return "WinPaintChar";
        case (8 << 12) | 0xbd0:
            return "WinPaintChars";
        case (8 << 12) | 0xbd4:
            return "WinPaintLine";
        case (8 << 12) | 0xbd8:
            return "WinPaintLines";
        case (8 << 12) | 0xbdc:
            return "WinPaintPixel";
        case (8 << 12) | 0xbe0:
            return "WinPaintPixels";
        case (8 << 12) | 0xbe4:
            return "WinPaintRectangle";
        case (8 << 12) | 0xbe8:
            return "WinPaintRectangleFrame";
        case (8 << 12) | 0xbec:
            return "WinPalette";
        case (8 << 12) | 0xbf0:
            return "WinPopDrawState";
        case (8 << 12) | 0xbf4:
            return "WinPushDrawState";
        case (8 << 12) | 0xbf8:
            return "WinRemoveWindow";
        case (8 << 12) | 0xbfc:
            return "WinResetClip";
        case (8 << 12) | 0xc00:
            return "WinRestoreBits";
        case (8 << 12) | 0xc04:
            return "WinRGBToIndex";
        case (8 << 12) | 0xc08:
            return "WinSaveBits";
        case (8 << 12) | 0xc0c:
            return "WinScreenInit";
        case (8 << 12) | 0xc10:
            return "WinScreenLock";
        case (8 << 12) | 0xc14:
            return "WinScreenMode";
        case (8 << 12) | 0xc18:
            return "WinScreenUnlock";
        case (8 << 12) | 0xc1c:
            return "WinScrollRectangle";
        case (8 << 12) | 0xc20:
            return "WinSetActiveWindow";
        case (8 << 12) | 0xc24:
            return "WinSetBackColor";
        case (8 << 12) | 0xc28:
            return "WinSetBackColorRGB";
        case (8 << 12) | 0xc2c:
            return "WinSetClip";
        case (8 << 12) | 0xc30:
            return "WinSetColors";
        case (8 << 12) | 0xc34:
            return "WinSetDrawMode";
        case (8 << 12) | 0xc38:
            return "WinSetDrawWindow";
        case (8 << 12) | 0xc3c:
            return "WinSetForeColor";
        case (8 << 12) | 0xc40:
            return "WinSetForeColorRGB";
        case (8 << 12) | 0xc44:
            return "WinSetPattern";
        case (8 << 12) | 0xc48:
            return "WinSetPatternType";
        case (8 << 12) | 0xc4c:
            return "WinSetTextColor";
        case (8 << 12) | 0xc50:
            return "WinSetTextColorRGB";
        case (8 << 12) | 0xc54:
            return "WinSetUnderlineMode";
        case (8 << 12) | 0xc58:
            return "WinSetBounds";
        case (8 << 12) | 0xc5c:
            return "WinValidateHandle";
        case (8 << 12) | 0xc60:
            return "WinWindowToDisplayPt";
        case (8 << 12) | 0xc64:
            return "IntlSetStrictChecks";
        case (8 << 12) | 0xc68:
            return "BmpCreateBitmapV3";
        case (8 << 12) | 0xc6c:
            return "BmpGetCompressionType";
        case (8 << 12) | 0xc70:
            return "BmpGetDensity";
        case (8 << 12) | 0xc74:
            return "BmpGetNextBitmapAnyDensity";
        case (8 << 12) | 0xc78:
            return "BmpGetTransparentValue";
        case (8 << 12) | 0xc7c:
            return "BmpGetVersion";
        case (8 << 12) | 0xc80:
            return "BmpSetDensity";
        case (8 << 12) | 0xc84:
            return "BmpSetTransparentValue";
        case (8 << 12) | 0xc88:
            return "EvtGetPenNative";
        case (8 << 12) | 0xc8c:
            return "WinGetCoordinateSystem";
        case (8 << 12) | 0xc90:
            return "WinGetSupportedDensity";
        case (8 << 12) | 0xc94:
            return "WinPaintTiledBitmap";
        case (8 << 12) | 0xc98:
            return "WinSetCoordinateSystem";
        case (8 << 12) | 0xc9c:
            return "WinScalePoint";
        case (8 << 12) | 0xca0:
            return "WinScaleRectangle";
        case (8 << 12) | 0xca4:
            return "WinScreenGetAttribute";
        case (8 << 12) | 0xca8:
            return "WinUnscalePoint";
        case (8 << 12) | 0xcac:
            return "WinUnscaleRectangle";
        case (8 << 12) | 0xcb0:
            return "SysSleepWakeNotificationCallback";
        case (8 << 12) | 0xcb4:
            return "WinScaleCoord";
        case (8 << 12) | 0xcb8:
            return "WinUnscaleCoord";
        case (8 << 12) | 0xcbc:
            return "WinPaintRoundedRectangleFrame";
        case (8 << 12) | 0xcc0:
            return "SndPlaySmfIrregardless";
        case (8 << 12) | 0xcc4:
            return "SndPlaySmfResourceIrregardless";
        case (8 << 12) | 0xcc8:
            return "SndInterruptSmfIrregardless";
        case (8 << 12) | 0xccc:
            return "Crc16CalcBigBlock";
        case (8 << 12) | 0xcd0:
            return "SlotMediaType";
        case (8 << 12) | 0xcd4:
            return "IntlInit";
        case (8 << 12) | 0xcd8:
            return "IntlHandleEvent";
        case (8 << 12) | 0xcdc:
            return "IntlGetRoutineAddress";
        case (8 << 12) | 0xce0:
            return "OmGetRoutineAddress";
        case (8 << 12) | 0xce4:
            return "OmInit";
        case (8 << 12) | 0xce8:
            return "OmOpenOverlayDatabase";
        case (8 << 12) | 0xcec:
            return "PenRestoreSavedCalibration";
        case (8 << 12) | 0xcf0:
            return "ScrCompressRLE";
        case (8 << 12) | 0xcf4:
            return "ScrDecompressRLE";
        case (8 << 12) | 0xcf8:
            return "ScrCompressPackBits";
        case (8 << 12) | 0xcfc:
            return "ScrCompressPackBits16";
        case (8 << 12) | 0xd00:
            return "ScrDecompressPackBits";
        case (8 << 12) | 0xd04:
            return "ScrDecompressPackBits16";
        case (8 << 12) | 0xd08:
            return "VFSPrvCleanupFiles";
        case (8 << 12) | 0xd0c:
            return "SndStreamCreate";
        case (8 << 12) | 0xd10:
            return "SndStreamDelete";
        case (8 << 12) | 0xd14:
            return "SndStreamStart";
        case (8 << 12) | 0xd18:
            return "SndStreamPause";
        case (8 << 12) | 0xd1c:
            return "SndStreamStop";
        case (8 << 12) | 0xd20:
            return "SndStreamSetVolume";
        case (8 << 12) | 0xd24:
            return "SndStreamGetVolume";
        case (8 << 12) | 0xd28:
            return "SndStreamSetPan";
        case (8 << 12) | 0xd2c:
            return "SndStreamGetPan";
        case (8 << 12) | 0xd30:
            return "SndPlayResource";
        case (8 << 12) | 0xd34:
            return "WinPrvInitCanvas";
        case (8 << 12) | 0xd38:
            return "WinPrvInitCanvasWithBitmap";
        case (8 << 12) | 0xd3c:
            return "WinPrvInitDrawState";
        case (8 << 12) | 0xd40:
            return "BmpPrvConvertBitmap";
        case (8 << 12) | 0xd44:
            return "BmpGetPixelFormat";
        case (8 << 12) | 0xd48:
            return "BmpCreateVersion3";
        case (8 << 12) | 0xd4c:
            return "WinGetBitmapDimensions";
        case (8 << 12) | 0xd50:
            return "SndStreamCreateExtended";
        case (8 << 12) | 0xd54:
            return "SndStreamDeviceControl";
        case (8 << 12) | 0xd58:
            return "WinSetScalingMode";
        case (8 << 12) | 0xd5c:
            return "WinGetScalingMode";
        case (8 << 12) | 0xd60:
            return "GrfAddMacro";
        case (8 << 12) | 0xd64:
            return "GrfAddPoint";
        case (8 << 12) | 0xd68:
            return "GrfCleanState";
        case (8 << 12) | 0xd6c:
            return "GrfDeleteMacro";
        case (8 << 12) | 0xd70:
            return "GrfFieldChange";
        case (8 << 12) | 0xd74:
            return "GrfFilterPoints";
        case (8 << 12) | 0xd78:
            return "GrfFindBranch";
        case (8 << 12) | 0xd7c:
            return "GrfFlushPoints";
        case (8 << 12) | 0xd80:
            return "GrfFree";
        case (8 << 12) | 0xd84:
            return "GrfGetAndExpandMacro";
        case (8 << 12) | 0xd88:
            return "GrfGetGlyphMapping";
        case (8 << 12) | 0xd8c:
            return "GrfGetMacro";
        case (8 << 12) | 0xd90:
            return "GrfGetMacroName";
        case (8 << 12) | 0xd94:
            return "GrfGetNumPoints";
        case (8 << 12) | 0xd98:
            return "GrfGetPoint";
        case (8 << 12) | 0xd9c:
            return "GrfGetState";
        case (8 << 12) | 0xda0:
            return "GrfInit";
        case (8 << 12) | 0xda4:
            return "GrfInitState";
        case (8 << 12) | 0xda8:
            return "GrfMatch";
        case (8 << 12) | 0xdac:
            return "GrfMatchGlyph";
        case (8 << 12) | 0xdb0:
            return "GrfProcessStroke";
        case (8 << 12) | 0xdb4:
            return "GrfSetState";
        case (8 << 12) | 0xdb8:
            return "GrfBeginStroke";
        case (8 << 12) | 0xdbc:
            return "SysGraffitiReferenceDialog";
        case (8 << 12) | 0xdc0:
            return "PINGetInputAreaState";
        case (8 << 12) | 0xdc4:
            return "PINSetInputAreaState";
        case (8 << 12) | 0xdc8:
            return "PINGetInputTriggerState";
        case (8 << 12) | 0xdcc:
            return "PINSetInputTriggerState";
        case (8 << 12) | 0xdd0:
            return "PINAltInputSystemEnabled";
        case (8 << 12) | 0xdd4:
            return "PINGetCurrentPinletName";
        case (8 << 12) | 0xdd8:
            return "PINSwitchToPinlet";
        case (8 << 12) | 0xddc:
            return "PINCountPinlets";
        case (8 << 12) | 0xde0:
            return "PINGetPinletInfo";
        case (8 << 12) | 0xde4:
            return "PINSetInputMode";
        case (8 << 12) | 0xde8:
            return "PINGetInputMode";
        case (8 << 12) | 0xdec:
            return "PINClearPinletState";
        case (8 << 12) | 0xdf0:
            return "PINShowReferenceDialog";
        case (8 << 12) | 0xdf4:
            return "StatGetAttribute";
        case (8 << 12) | 0xdf8:
            return "StatShow";
        case (8 << 12) | 0xdfc:
            return "StatHide";
        case (8 << 12) | 0xe00:
            return "WinSetConstraintsSize";
        case (8 << 12) | 0xe04:
            return "PINPrvInit";
        case (8 << 12) | 0xe08:
            return "PINPrvGetInputUserState";
        case (8 << 12) | 0xe0c:
            return "PINPrvSetInputUserState";
        case (8 << 12) | 0xe10:
            return "PINPrvGetInputAreaInitialized";
        case (8 << 12) | 0xe14:
            return "PINPrvResetInputAreaInitialized";
        case (8 << 12) | 0xe18:
            return "LmGetLocaleSettingSize";
        case (8 << 12) | 0xe1c:
            return "WinPrvGetInputAreaBounds";
        case (8 << 12) | 0xe20:
            return "WinPrvScalePoint";
        case (8 << 12) | 0xe24:
            return "SysGetOrientation";
        case (8 << 12) | 0xe28:
            return "SysSetOrientation";
        case (8 << 12) | 0xe2c:
            return "SysGetOrientationTriggerState";
        case (8 << 12) | 0xe30:
            return "SysSetOrientationTriggerState";
        case (12 << 12) | 0x000:
            return "AbtShowAbout";
        case (12 << 12) | 0x004:
            return "AttnAllowClose";
        case (12 << 12) | 0x008:
            return "AttnDoEmergencySpecialEffects";
        case (12 << 12) | 0x00c:
            return "AttnDoSpecialEffects";
        case (12 << 12) | 0x010:
            return "AttnEffectOfEvent";
        case (12 << 12) | 0x014:
            return "AttnEnableNotification";
        case (12 << 12) | 0x018:
            return "AttnForgetIt";
        case (12 << 12) | 0x01c:
            return "AttnGetAttention";
        case (12 << 12) | 0x020:
            return "AttnGetCounts";
        case (12 << 12) | 0x024:
            return "AttnHandleEvent";
        case (12 << 12) | 0x028:
            return "AttnIterate";
        case (12 << 12) | 0x02c:
            return "AttnListOpen";
        case (12 << 12) | 0x030:
            return "AttnIndicatorAllow";
        case (12 << 12) | 0x034:
            return "AttnIndicatorAllowed";
        case (12 << 12) | 0x038:
            return "AttnIndicatorCheckBlink";
        case (12 << 12) | 0x03c:
            return "AttnIndicatorEnable";
        case (12 << 12) | 0x040:
            return "AttnIndicatorEnabled";
        case (12 << 12) | 0x044:
            return "AttnIndicatorGetBlinkPattern";
        case (12 << 12) | 0x048:
            return "AttnIndicatorSetBlinkPattern";
        case (12 << 12) | 0x04c:
            return "AttnIndicatorTicksTillNextBlink";
        case (12 << 12) | 0x050:
            return "AttnReopen";
        case (12 << 12) | 0x054:
            return "AttnUpdate";
        case (12 << 12) | 0x058:
            return "CategoryCreateList";
        case (12 << 12) | 0x05c:
            return "CategoryCreateListV10";
        case (12 << 12) | 0x060:
            return "CategoryEdit";
        case (12 << 12) | 0x064:
            return "CategoryEditV10";
        case (12 << 12) | 0x068:
            return "CategoryEditV20";
        case (12 << 12) | 0x06c:
            return "CategoryFind";
        case (12 << 12) | 0x070:
            return "CategoryFreeList";
        case (12 << 12) | 0x074:
            return "CategoryFreeListV10";
        case (12 << 12) | 0x078:
            return "CategoryGetName";
        case (12 << 12) | 0x07c:
            return "CategoryGetNext";
        case (12 << 12) | 0x080:
            return "CategoryInitialize";
        case (12 << 12) | 0x084:
            return "CategorySelect";
        case (12 << 12) | 0x088:
            return "CategorySelectV10";
        case (12 << 12) | 0x08c:
            return "CategorySetName";
        case (12 << 12) | 0x090:
            return "CategorySetTriggerLabel";
        case (12 << 12) | 0x094:
            return "CategoryTruncateName";
        case (12 << 12) | 0x098:
            return "ClipboardAddItem";
        case (12 << 12) | 0x09c:
            return "ClipboardAppendItem";
        case (12 << 12) | 0x0a0:
            return "ClipboardGetItem";
        case (12 << 12) | 0x0a4:
            return "CtlDrawControl";
        case (12 << 12) | 0x0a8:
            return "CtlEnabled";
        case (12 << 12) | 0x0ac:
            return "CtlEraseControl";
        case (12 << 12) | 0x0b0:
            return "CtlGetLabel";
        case (12 << 12) | 0x0b4:
            return "CtlGetSliderValues";
        case (12 << 12) | 0x0b8:
            return "CtlGetValue";
        case (12 << 12) | 0x0bc:
            return "CtlHandleEvent";
        case (12 << 12) | 0x0c0:
            return "CtlHideControl";
        case (12 << 12) | 0x0c4:
            return "CtlHitControl";
        case (12 << 12) | 0x0c8:
            return "CtlNewControl";
        case (12 << 12) | 0x0cc:
            return "CtlNewGraphicControl";
        case (12 << 12) | 0x0d0:
            return "CtlNewSliderControl";
        case (12 << 12) | 0x0d4:
            return "CtlSetEnabled";
        case (12 << 12) | 0x0d8:
            return "CtlSetGraphics";
        case (12 << 12) | 0x0dc:
            return "CtlSetLabel";
        case (12 << 12) | 0x0e0:
            return "CtlSetSliderValues";
        case (12 << 12) | 0x0e4:
            return "CtlSetUsable";
        case (12 << 12) | 0x0e8:
            return "CtlSetValue";
        case (12 << 12) | 0x0ec:
            return "CtlShowControl";
        case (12 << 12) | 0x0f0:
            return "CtlValidatePointer";
        case (12 << 12) | 0x0f4:
            return "DayDrawDays";
        case (12 << 12) | 0x0f8:
            return "DayDrawDaySelector";
        case (12 << 12) | 0x0fc:
            return "DayHandleEvent";
        case (12 << 12) | 0x100:
            return "Find";
        case (12 << 12) | 0x104:
            return "FindDrawHeader";
        case (12 << 12) | 0x108:
            return "FindGetLineBounds";
        case (12 << 12) | 0x10c:
            return "FindSaveMatch";
        case (12 << 12) | 0x110:
            return "FindStrInStr";
        case (12 << 12) | 0x114:
            return "FldCalcFieldHeight";
        case (12 << 12) | 0x118:
            return "FldCompactText";
        case (12 << 12) | 0x11c:
            return "FldCopy";
        case (12 << 12) | 0x120:
            return "FldCut";
        case (12 << 12) | 0x124:
            return "FldDelete";
        case (12 << 12) | 0x128:
            return "FldDirty";
        case (12 << 12) | 0x12c:
            return "FldDrawField";
        case (12 << 12) | 0x130:
            return "FldEraseField";
        case (12 << 12) | 0x134:
            return "FldFreeMemory";
        case (12 << 12) | 0x138:
            return "FldGetAttributes";
        case (12 << 12) | 0x13c:
            return "FldGetBounds";
        case (12 << 12) | 0x140:
            return "FldGetFont";
        case (12 << 12) | 0x144:
            return "FldGetInsPtPosition";
        case (12 << 12) | 0x148:
            return "FldGetMaxChars";
        case (12 << 12) | 0x14c:
            return "FldGetNumberOfBlankLines";
        case (12 << 12) | 0x150:
            return "FldGetScrollPosition";
        case (12 << 12) | 0x154:
            return "FldGetScrollValues";
        case (12 << 12) | 0x158:
            return "FldGetSelection";
        case (12 << 12) | 0x15c:
            return "FldGetTextAllocatedSize";
        case (12 << 12) | 0x160:
            return "FldGetTextHandle";
        case (12 << 12) | 0x164:
            return "FldGetTextHeight";
        case (12 << 12) | 0x168:
            return "FldGetTextLength";
        case (12 << 12) | 0x16c:
            return "FldGetTextPtr";
        case (12 << 12) | 0x170:
            return "FldGetVisibleLines";
        case (12 << 12) | 0x174:
            return "FldGrabFocus";
        case (12 << 12) | 0x178:
            return "FldHandleEvent";
        case (12 << 12) | 0x17c:
            return "FldInsert";
        case (12 << 12) | 0x180:
            return "FldMakeFullyVisible";
        case (12 << 12) | 0x184:
            return "FldNewField";
        case (12 << 12) | 0x188:
            return "FldPaste";
        case (12 << 12) | 0x18c:
            return "FldRecalculateField";
        case (12 << 12) | 0x190:
            return "FldReleaseFocus";
        case (12 << 12) | 0x194:
            return "FldScrollable";
        case (12 << 12) | 0x198:
            return "FldScrollField";
        case (12 << 12) | 0x19c:
            return "FldSendChangeNotification";
        case (12 << 12) | 0x1a0:
            return "FldSendHeightChangeNotification";
        case (12 << 12) | 0x1a4:
            return "FldSetAttributes";
        case (12 << 12) | 0x1a8:
            return "FldSetBounds";
        case (12 << 12) | 0x1ac:
            return "FldSetDirty";
        case (12 << 12) | 0x1b0:
            return "FldSetFont";
        case (12 << 12) | 0x1b4:
            return "FldSetInsertionPoint";
        case (12 << 12) | 0x1b8:
            return "FldSetInsPtPosition";
        case (12 << 12) | 0x1bc:
            return "FldSetMaxChars";
        case (12 << 12) | 0x1c0:
            return "FldSetMaxVisibleLines";
        case (12 << 12) | 0x1c4:
            return "FldSetScrollPosition";
        case (12 << 12) | 0x1c8:
            return "FldSetSelection";
        case (12 << 12) | 0x1cc:
            return "FldSetText";
        case (12 << 12) | 0x1d0:
            return "FldSetTextAllocatedSize";
        case (12 << 12) | 0x1d4:
            return "FldSetTextHandle";
        case (12 << 12) | 0x1d8:
            return "FldSetTextPtr";
        case (12 << 12) | 0x1dc:
            return "FldSetUsable";
        case (12 << 12) | 0x1e0:
            return "FldUndo";
        case (12 << 12) | 0x1e4:
            return "FldWordWrap";
        case (12 << 12) | 0x1e8:
            return "FontSelect";
        case (12 << 12) | 0x1ec:
            return "FrmActiveState";
        case (12 << 12) | 0x1f0:
            return "FrmAddSpaceForObject";
        case (12 << 12) | 0x1f4:
            return "FrmAlert";
        case (12 << 12) | 0x1f8:
            return "FrmCloseAllForms";
        case (12 << 12) | 0x1fc:
            return "FrmCopyLabel";
        case (12 << 12) | 0x200:
            return "FrmCopyTitle";
        case (12 << 12) | 0x204:
            return "FrmCustomAlert";
        case (12 << 12) | 0x208:
            return "FrmCustomResponseAlert";
        case (12 << 12) | 0x20c:
            return "FrmDeleteForm";
        case (12 << 12) | 0x210:
            return "FrmDispatchEvent";
        case (12 << 12) | 0x214:
            return "FrmDoDialog";
        case (12 << 12) | 0x218:
            return "FrmDrawForm";
        case (12 << 12) | 0x21c:
            return "FrmEraseForm";
        case (12 << 12) | 0x220:
            return "FrmGetActiveField";
        case (12 << 12) | 0x224:
            return "FrmGetActiveForm";
        case (12 << 12) | 0x228:
            return "FrmGetActiveFormID";
        case (12 << 12) | 0x22c:
            return "FrmGetControlGroupSelection";
        case (12 << 12) | 0x230:
            return "FrmGetControlValue";
        case (12 << 12) | 0x234:
            return "FrmGetFirstForm";
        case (12 << 12) | 0x238:
            return "FrmGetFocus";
        case (12 << 12) | 0x23c:
            return "FrmGetFormBounds";
        case (12 << 12) | 0x240:
            return "FrmGetFormId";
        case (12 << 12) | 0x244:
            return "FrmGetFormPtr";
        case (12 << 12) | 0x248:
            return "FrmGetGadgetData";
        case (12 << 12) | 0x24c:
            return "FrmGetLabel";
        case (12 << 12) | 0x250:
            return "FrmGetNumberOfObjects";
        case (12 << 12) | 0x254:
            return "FrmGetObjectBounds";
        case (12 << 12) | 0x258:
            return "FrmGetObjectId";
        case (12 << 12) | 0x25c:
            return "FrmGetObjectIndex";
        case (12 << 12) | 0x260:
            return "FrmGetObjectIndexFromPtr";
        case (12 << 12) | 0x264:
            return "FrmGetObjectPosition";
        case (12 << 12) | 0x268:
            return "FrmGetObjectPtr";
        case (12 << 12) | 0x26c:
            return "FrmGetObjectType";
        case (12 << 12) | 0x270:
            return "FrmGetTitle";
        case (12 << 12) | 0x274:
            return "FrmGetUserModifiedState";
        case (12 << 12) | 0x278:
            return "FrmGetWindowHandle";
        case (12 << 12) | 0x27c:
            return "FrmGotoForm";
        case (12 << 12) | 0x280:
            return "FrmHandleEvent";
        case (12 << 12) | 0x284:
            return "FrmHelp";
        case (12 << 12) | 0x288:
            return "FrmHideObject";
        case (12 << 12) | 0x28c:
            return "FrmInitForm";
        case (12 << 12) | 0x290:
            return "FrmNewBitmap";
        case (12 << 12) | 0x294:
            return "FrmNewForm";
        case (12 << 12) | 0x298:
            return "FrmNewGadget";
        case (12 << 12) | 0x29c:
            return "FrmNewGsi";
        case (12 << 12) | 0x2a0:
            return "FrmNewLabel";
        case (12 << 12) | 0x2a4:
            return "FrmPointInTitle";
        case (12 << 12) | 0x2a8:
            return "FrmPopupForm";
        case (12 << 12) | 0x2ac:
            return "FrmRemoveObject";
        case (12 << 12) | 0x2b0:
            return "FrmReturnToForm";
        case (12 << 12) | 0x2b4:
            return "FrmSaveAllForms";
        case (12 << 12) | 0x2b8:
            return "FrmSetActiveForm";
        case (12 << 12) | 0x2bc:
            return "FrmSetCategoryLabel";
        case (12 << 12) | 0x2c0:
            return "FrmSetControlGroupSelection";
        case (12 << 12) | 0x2c4:
            return "FrmSetControlValue";
        case (12 << 12) | 0x2c8:
            return "FrmSetEventHandler";
        case (12 << 12) | 0x2cc:
            return "FrmSetFocus";
        case (12 << 12) | 0x2d0:
            return "FrmSetGadgetData";
        case (12 << 12) | 0x2d4:
            return "FrmSetGadgetHandler";
        case (12 << 12) | 0x2d8:
            return "FrmSetMenu";
        case (12 << 12) | 0x2dc:
            return "FrmSetNotUserModified";
        case (12 << 12) | 0x2e0:
            return "FrmSetObjectBounds";
        case (12 << 12) | 0x2e4:
            return "FrmSetObjectPosition";
        case (12 << 12) | 0x2e8:
            return "FrmSetTitle";
        case (12 << 12) | 0x2ec:
            return "FrmShowObject";
        case (12 << 12) | 0x2f0:
            return "FrmUpdateForm";
        case (12 << 12) | 0x2f4:
            return "FrmUpdateScrollers";
        case (12 << 12) | 0x2f8:
            return "FrmValidatePtr";
        case (12 << 12) | 0x2fc:
            return "FrmVisible";
        case (12 << 12) | 0x300:
            return "GsiEnable";
        case (12 << 12) | 0x304:
            return "GsiEnabled";
        case (12 << 12) | 0x308:
            return "GsiInitialize";
        case (12 << 12) | 0x30c:
            return "GsiSetLocation";
        case (12 << 12) | 0x310:
            return "GsiSetShiftState";
        case (12 << 12) | 0x314:
            return "InsPtCheckBlink";
        case (12 << 12) | 0x318:
            return "InsPtEnable";
        case (12 << 12) | 0x31c:
            return "InsPtEnabled";
        case (12 << 12) | 0x320:
            return "InsPtGetHeight";
        case (12 << 12) | 0x324:
            return "InsPtGetLocation";
        case (12 << 12) | 0x328:
            return "InsPtInitialize";
        case (12 << 12) | 0x32c:
            return "InsPtSetHeight";
        case (12 << 12) | 0x330:
            return "InsPtSetLocation";
        case (12 << 12) | 0x334:
            return "KbdDraw";
        case (12 << 12) | 0x338:
            return "KbdErase";
        case (12 << 12) | 0x33c:
            return "KbdGetLayout";
        case (12 << 12) | 0x340:
            return "KbdGetPosition";
        case (12 << 12) | 0x344:
            return "KbdGetShiftState";
        case (12 << 12) | 0x348:
            return "KbdHandleEvent";
        case (12 << 12) | 0x34c:
            return "KbdSetLayout";
        case (12 << 12) | 0x350:
            return "KbdSetPosition";
        case (12 << 12) | 0x354:
            return "KbdSetShiftState";
        case (12 << 12) | 0x358:
            return "KeyboardStatusFree";
        case (12 << 12) | 0x35c:
            return "KeyboardStatusNew";
        case (12 << 12) | 0x360:
            return "LstDrawList";
        case (12 << 12) | 0x364:
            return "LstEraseList";
        case (12 << 12) | 0x368:
            return "LstGetNumberOfItems";
        case (12 << 12) | 0x36c:
            return "LstGetSelection";
        case (12 << 12) | 0x370:
            return "LstGetSelectionText";
        case (12 << 12) | 0x374:
            return "LstGetTopItem";
        case (12 << 12) | 0x378:
            return "LstGetVisibleItems";
        case (12 << 12) | 0x37c:
            return "LstHandleEvent";
        case (12 << 12) | 0x380:
            return "LstMakeItemVisible";
        case (12 << 12) | 0x384:
            return "LstNewList";
        case (12 << 12) | 0x388:
            return "LstPopupList";
        case (12 << 12) | 0x38c:
            return "LstScrollList";
        case (12 << 12) | 0x390:
            return "LstSetDrawFunction";
        case (12 << 12) | 0x394:
            return "LstSetHeight";
        case (12 << 12) | 0x398:
            return "LstSetListChoices";
        case (12 << 12) | 0x39c:
            return "LstSetPosition";
        case (12 << 12) | 0x3a0:
            return "LstSetSelection";
        case (12 << 12) | 0x3a4:
            return "LstSetTopItem";
        case (12 << 12) | 0x3a8:
            return "MenuAddItem";
        case (12 << 12) | 0x3ac:
            return "MenuCmdBarAddButton";
        case (12 << 12) | 0x3b0:
            return "MenuCmdBarDisplay";
        case (12 << 12) | 0x3b4:
            return "MenuCmdBarGetButtonData";
        case (12 << 12) | 0x3b8:
            return "MenuDispose";
        case (12 << 12) | 0x3bc:
            return "MenuDrawMenu";
        case (12 << 12) | 0x3c0:
            return "MenuEraseMenu";
        case (12 << 12) | 0x3c4:
            return "MenuEraseStatus";
        case (12 << 12) | 0x3c8:
            return "MenuGetActiveMenu";
        case (12 << 12) | 0x3cc:
            return "MenuHandleEvent";
        case (12 << 12) | 0x3d0:
            return "MenuHideItem";
        case (12 << 12) | 0x3d4:
            return "MenuInit";
        case (12 << 12) | 0x3d8:
            return "MenuSetActiveMenu";
        case (12 << 12) | 0x3dc:
            return "MenuSetActiveMenuRscID";
        case (12 << 12) | 0x3e0:
            return "MenuShowItem";
        case (12 << 12) | 0x3e4:
            return "PhoneNumberLookup";
        case (12 << 12) | 0x3e8:
            return "PhoneNumberLookupCustom";
        case (12 << 12) | 0x3ec:
            return "PrgHandleEvent";
        case (12 << 12) | 0x3f0:
            return "PrgStartDialog";
        case (12 << 12) | 0x3f4:
            return "PrgStartDialogV31";
        case (12 << 12) | 0x3f8:
            return "PrgStopDialog";
        case (12 << 12) | 0x3fc:
            return "PrgUpdateDialog";
        case (12 << 12) | 0x400:
            return "ResLoadConstant";
        case (12 << 12) | 0x404:
            return "ResLoadForm";
        case (12 << 12) | 0x408:
            return "ResLoadMenu";
        case (12 << 12) | 0x40c:
            return "SclDrawScrollBar";
        case (12 << 12) | 0x410:
            return "SclGetScrollBar";
        case (12 << 12) | 0x414:
            return "SclHandleEvent";
        case (12 << 12) | 0x418:
            return "SclSetScrollBar";
        case (12 << 12) | 0x41c:
            return "SecSelectViewStatus";
        case (12 << 12) | 0x420:
            return "SecVerifyPW";
        case (12 << 12) | 0x424:
            return "SelectDay";
        case (12 << 12) | 0x428:
            return "SelectDayV10";
        case (12 << 12) | 0x42c:
            return "SelectOneTime";
        case (12 << 12) | 0x430:
            return "SelectTime";
        case (12 << 12) | 0x434:
            return "SelectTimeV33";
        case (12 << 12) | 0x438:
            return "SelectTimeZone";
        case (12 << 12) | 0x43c:
            return "SysAppLauncherDialog";
        case (12 << 12) | 0x440:
            return "SysBatteryDialog";
        case (12 << 12) | 0x444:
            return "SysFatalAlert";
        case (12 << 12) | 0x448:
            return "SysFatalAlertInit";
        case (12 << 12) | 0x44c:
            return "SysKeyboardDialog";
        case (12 << 12) | 0x450:
            return "SysKeyboardDialogV10";
        case (12 << 12) | 0x454:
            return "TblDrawTable";
        case (12 << 12) | 0x458:
            return "TblEditing";
        case (12 << 12) | 0x45c:
            return "TblEraseTable";
        case (12 << 12) | 0x460:
            return "TblFindRowData";
        case (12 << 12) | 0x464:
            return "TblFindRowID";
        case (12 << 12) | 0x468:
            return "TblGetBounds";
        case (12 << 12) | 0x46c:
            return "TblGetColumnSpacing";
        case (12 << 12) | 0x470:
            return "TblGetColumnWidth";
        case (12 << 12) | 0x474:
            return "TblGetCurrentField";
        case (12 << 12) | 0x478:
            return "TblGetItemBounds";
        case (12 << 12) | 0x47c:
            return "TblGetItemFont";
        case (12 << 12) | 0x480:
            return "TblGetItemInt";
        case (12 << 12) | 0x484:
            return "TblGetItemPtr";
        case (12 << 12) | 0x488:
            return "TblGetLastUsableRow";
        case (12 << 12) | 0x48c:
            return "TblGetNumberOfRows";
        case (12 << 12) | 0x490:
            return "TblGetNumberOfColumns";
        case (12 << 12) | 0x494:
            return "TblGetRowData";
        case (12 << 12) | 0x498:
            return "TblGetRowHeight";
        case (12 << 12) | 0x49c:
            return "TblGetRowID";
        case (12 << 12) | 0x4a0:
            return "TblGetSelection";
        case (12 << 12) | 0x4a4:
            return "TblGetTopRow";
        case (12 << 12) | 0x4a8:
            return "TblGrabFocus";
        case (12 << 12) | 0x4ac:
            return "TblHandleEvent";
        case (12 << 12) | 0x4b0:
            return "TblHasScrollBar";
        case (12 << 12) | 0x4b4:
            return "TblInsertRow";
        case (12 << 12) | 0x4b8:
            return "TblMarkRowInvalid";
        case (12 << 12) | 0x4bc:
            return "TblMarkTableInvalid";
        case (12 << 12) | 0x4c0:
            return "TblRedrawTable";
        case (12 << 12) | 0x4c4:
            return "TblReleaseFocus";
        case (12 << 12) | 0x4c8:
            return "TblRemoveRow";
        case (12 << 12) | 0x4cc:
            return "TblRowInvalid";
        case (12 << 12) | 0x4d0:
            return "TblRowMasked";
        case (12 << 12) | 0x4d4:
            return "TblRowSelectable";
        case (12 << 12) | 0x4d8:
            return "TblRowUsable";
        case (12 << 12) | 0x4dc:
            return "TblSelectItem";
        case (12 << 12) | 0x4e0:
            return "TblSetBounds";
        case (12 << 12) | 0x4e4:
            return "TblSetColumnEditIndicator";
        case (12 << 12) | 0x4e8:
            return "TblSetColumnMasked";
        case (12 << 12) | 0x4ec:
            return "TblSetColumnSpacing";
        case (12 << 12) | 0x4f0:
            return "TblSetColumnUsable";
        case (12 << 12) | 0x4f4:
            return "TblSetColumnWidth";
        case (12 << 12) | 0x4f8:
            return "TblSetCustomDrawProcedure";
        case (12 << 12) | 0x4fc:
            return "TblSetItemFont";
        case (12 << 12) | 0x500:
            return "TblSetItemInt";
        case (12 << 12) | 0x504:
            return "TblSetItemPtr";
        case (12 << 12) | 0x508:
            return "TblSetItemStyle";
        case (12 << 12) | 0x50c:
            return "TblSetLoadDataProcedure";
        case (12 << 12) | 0x510:
            return "TblSetRowData";
        case (12 << 12) | 0x514:
            return "TblSetRowHeight";
        case (12 << 12) | 0x518:
            return "TblSetRowID";
        case (12 << 12) | 0x51c:
            return "TblSetRowMasked";
        case (12 << 12) | 0x520:
            return "TblSetRowSelectable";
        case (12 << 12) | 0x524:
            return "TblSetRowStaticHeight";
        case (12 << 12) | 0x528:
            return "TblSetRowUsable";
        case (12 << 12) | 0x52c:
            return "TblSetSaveDataProcedure";
        case (12 << 12) | 0x530:
            return "TblSetSelection";
        case (12 << 12) | 0x534:
            return "TblUnhighlightSelection";
        case (12 << 12) | 0x538:
            return "TimeZoneToAscii";
        case (12 << 12) | 0x53c:
            return "UIBrightnessAdjust";
        case (12 << 12) | 0x540:
            return "UIColorGetTableEntryIndex";
        case (12 << 12) | 0x544:
            return "UIColorGetTableEntryRGB";
        case (12 << 12) | 0x548:
            return "UIColorInit";
        case (12 << 12) | 0x54c:
            return "UIColorPopTable";
        case (12 << 12) | 0x550:
            return "UIColorPushTable";
        case (12 << 12) | 0x554:
            return "UIColorSetTableEntry";
        case (12 << 12) | 0x558:
            return "UIContrastAdjust";
        case (12 << 12) | 0x55c:
            return "UIInitialize";
        case (12 << 12) | 0x560:
            return "UIPickColor";
        case (12 << 12) | 0x564:
            return "UIGetGlobalsPtr";
        case (12 << 12) | 0x568:
            return "UIReset";
        case (12 << 12) | 0x56c:
            return "AttnInitialize";
        case (12 << 12) | 0x570:
            return "FrmGetDIAPolicyAttr";
        case (12 << 12) | 0x574:
            return "FrmSetDIAPolicyAttr";
        case (12 << 12) | 0x578:
            return "FrmPrvSetActiveFormPINAttributes";
        case (12 << 12) | 0x57c:
            return "FrmPrvRedrawDisplay";
        default:
            return NULL;
    }
}
