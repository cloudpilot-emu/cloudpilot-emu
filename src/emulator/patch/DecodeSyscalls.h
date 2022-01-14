#ifndef _DECODE_SYSCALLS_H_
#define _DECODE_SYSCALLS_H_

static const char* trapWordToString(int trapWord) {
#define _ENTRY(x, y) \
    case y:          \
        return #x;

    switch (trapWord) {
        _ENTRY(sysTrapMemInit, 0xA000)
        _ENTRY(sysTrapMemInitHeapTable, 0xA001)
        _ENTRY(sysTrapMemStoreInit, 0xA002)
        _ENTRY(sysTrapMemCardFormat, 0xA003)
        _ENTRY(sysTrapMemCardInfo, 0xA004)
        _ENTRY(sysTrapMemStoreInfo, 0xA005)
        _ENTRY(sysTrapMemStoreSetInfo, 0xA006)
        _ENTRY(sysTrapMemNumHeaps, 0xA007)
        _ENTRY(sysTrapMemNumRAMHeaps, 0xA008)
        _ENTRY(sysTrapMemHeapID, 0xA009)
        _ENTRY(sysTrapMemHeapPtr, 0xA00A)
        _ENTRY(sysTrapMemHeapFreeBytes, 0xA00B)
        _ENTRY(sysTrapMemHeapSize, 0xA00C)
        _ENTRY(sysTrapMemHeapFlags, 0xA00D)
        _ENTRY(sysTrapMemHeapCompact, 0xA00E)
        _ENTRY(sysTrapMemHeapInit, 0xA00F)
        _ENTRY(sysTrapMemHeapFreeByOwnerID, 0xA010)
        _ENTRY(sysTrapMemChunkNew, 0xA011)
        _ENTRY(sysTrapMemChunkFree, 0xA012)
        _ENTRY(sysTrapMemPtrNew, 0xA013)
        _ENTRY(sysTrapMemPtrRecoverHandle, 0xA014)
        _ENTRY(sysTrapMemPtrFlags, 0xA015)
        _ENTRY(sysTrapMemPtrSize, 0xA016)
        _ENTRY(sysTrapMemPtrOwner, 0xA017)
        _ENTRY(sysTrapMemPtrHeapID, 0xA018)
        _ENTRY(sysTrapMemPtrCardNo, 0xA019)
        _ENTRY(sysTrapMemPtrToLocalID, 0xA01A)
        _ENTRY(sysTrapMemPtrSetOwner, 0xA01B)
        _ENTRY(sysTrapMemPtrResize, 0xA01C)
        _ENTRY(sysTrapMemPtrResetLock, 0xA01D)
        _ENTRY(sysTrapMemHandleNew, 0xA01E)
        _ENTRY(sysTrapMemHandleLockCount, 0xA01F)
        _ENTRY(sysTrapMemHandleToLocalID, 0xA020)
        _ENTRY(sysTrapMemHandleLock, 0xA021)
        _ENTRY(sysTrapMemHandleUnlock, 0xA022)
        _ENTRY(sysTrapMemLocalIDToGlobal, 0xA023)
        _ENTRY(sysTrapMemLocalIDKind, 0xA024)
        _ENTRY(sysTrapMemLocalIDToPtr, 0xA025)
        _ENTRY(sysTrapMemMove, 0xA026)
        _ENTRY(sysTrapMemSet, 0xA027)
        _ENTRY(sysTrapMemStoreSearch, 0xA028)
        _ENTRY(sysTrapSysReserved10Trap1,
               0xA029) /* "Reserved" trap in Palm OS 1.0 and later (was sysTrapMemPtrDataStorage) */

        _ENTRY(sysTrapMemKernelInit, 0xA02A)
        _ENTRY(sysTrapMemHandleFree, 0xA02B)
        _ENTRY(sysTrapMemHandleFlags, 0xA02C)
        _ENTRY(sysTrapMemHandleSize, 0xA02D)
        _ENTRY(sysTrapMemHandleOwner, 0xA02E)
        _ENTRY(sysTrapMemHandleHeapID, 0xA02F)
        _ENTRY(sysTrapMemHandleDataStorage, 0xA030)
        _ENTRY(sysTrapMemHandleCardNo, 0xA031)
        _ENTRY(sysTrapMemHandleSetOwner, 0xA032)
        _ENTRY(sysTrapMemHandleResize, 0xA033)
        _ENTRY(sysTrapMemHandleResetLock, 0xA034)
        _ENTRY(sysTrapMemPtrUnlock, 0xA035)
        _ENTRY(sysTrapMemLocalIDToLockedPtr, 0xA036)
        _ENTRY(sysTrapMemSetDebugMode, 0xA037)
        _ENTRY(sysTrapMemHeapScramble, 0xA038)
        _ENTRY(sysTrapMemHeapCheck, 0xA039)
        _ENTRY(sysTrapMemNumCards, 0xA03A)
        _ENTRY(sysTrapMemDebugMode, 0xA03B)
        _ENTRY(sysTrapMemSemaphoreReserve, 0xA03C)
        _ENTRY(sysTrapMemSemaphoreRelease, 0xA03D)
        _ENTRY(sysTrapMemHeapDynamic, 0xA03E)
        _ENTRY(sysTrapMemNVParams, 0xA03F)

        _ENTRY(sysTrapDmInit, 0xA040)
        _ENTRY(sysTrapDmCreateDatabase, 0xA041)
        _ENTRY(sysTrapDmDeleteDatabase, 0xA042)
        _ENTRY(sysTrapDmNumDatabases, 0xA043)
        _ENTRY(sysTrapDmGetDatabase, 0xA044)
        _ENTRY(sysTrapDmFindDatabase, 0xA045)
        _ENTRY(sysTrapDmDatabaseInfo, 0xA046)
        _ENTRY(sysTrapDmSetDatabaseInfo, 0xA047)
        _ENTRY(sysTrapDmDatabaseSize, 0xA048)
        _ENTRY(sysTrapDmOpenDatabase, 0xA049)
        _ENTRY(sysTrapDmCloseDatabase, 0xA04A)
        _ENTRY(sysTrapDmNextOpenDatabase, 0xA04B)
        _ENTRY(sysTrapDmOpenDatabaseInfo, 0xA04C)
        _ENTRY(sysTrapDmResetRecordStates, 0xA04D)
        _ENTRY(sysTrapDmGetLastErr, 0xA04E)
        _ENTRY(sysTrapDmNumRecords, 0xA04F)
        _ENTRY(sysTrapDmRecordInfo, 0xA050)
        _ENTRY(sysTrapDmSetRecordInfo, 0xA051)
        _ENTRY(sysTrapDmAttachRecord, 0xA052)
        _ENTRY(sysTrapDmDetachRecord, 0xA053)
        _ENTRY(sysTrapDmMoveRecord, 0xA054)
        _ENTRY(sysTrapDmNewRecord, 0xA055)
        _ENTRY(sysTrapDmRemoveRecord, 0xA056)
        _ENTRY(sysTrapDmDeleteRecord, 0xA057)
        _ENTRY(sysTrapDmArchiveRecord, 0xA058)
        _ENTRY(sysTrapDmNewHandle, 0xA059)
        _ENTRY(sysTrapDmRemoveSecretRecords, 0xA05A)
        _ENTRY(sysTrapDmQueryRecord, 0xA05B)
        _ENTRY(sysTrapDmGetRecord, 0xA05C)
        _ENTRY(sysTrapDmResizeRecord, 0xA05D)
        _ENTRY(sysTrapDmReleaseRecord, 0xA05E)
        _ENTRY(sysTrapDmGetResource, 0xA05F)
        _ENTRY(sysTrapDmGet1Resource, 0xA060)
        _ENTRY(sysTrapDmReleaseResource, 0xA061)
        _ENTRY(sysTrapDmResizeResource, 0xA062)
        _ENTRY(sysTrapDmNextOpenResDatabase, 0xA063)
        _ENTRY(sysTrapDmFindResourceType, 0xA064)
        _ENTRY(sysTrapDmFindResource, 0xA065)
        _ENTRY(sysTrapDmSearchResource, 0xA066)
        _ENTRY(sysTrapDmNumResources, 0xA067)
        _ENTRY(sysTrapDmResourceInfo, 0xA068)
        _ENTRY(sysTrapDmSetResourceInfo, 0xA069)
        _ENTRY(sysTrapDmAttachResource, 0xA06A)
        _ENTRY(sysTrapDmDetachResource, 0xA06B)
        _ENTRY(sysTrapDmNewResource, 0xA06C)
        _ENTRY(sysTrapDmRemoveResource, 0xA06D)
        _ENTRY(sysTrapDmGetResourceIndex, 0xA06E)
        _ENTRY(sysTrapDmQuickSort, 0xA06F)
        _ENTRY(sysTrapDmQueryNextInCategory, 0xA070)
        _ENTRY(sysTrapDmNumRecordsInCategory, 0xA071)
        _ENTRY(sysTrapDmPositionInCategory, 0xA072)
        _ENTRY(sysTrapDmSeekRecordInCategory, 0xA073)
        _ENTRY(sysTrapDmMoveCategory, 0xA074)
        _ENTRY(sysTrapDmOpenDatabaseByTypeCreator, 0xA075)
        _ENTRY(sysTrapDmWrite, 0xA076)
        _ENTRY(sysTrapDmStrCopy, 0xA077)
        _ENTRY(sysTrapDmGetNextDatabaseByTypeCreator, 0xA078)
        _ENTRY(sysTrapDmWriteCheck, 0xA079)
        _ENTRY(sysTrapDmMoveOpenDBContext, 0xA07A)
        _ENTRY(sysTrapDmFindRecordByID, 0xA07B)
        _ENTRY(sysTrapDmGetAppInfoID, 0xA07C)
        _ENTRY(sysTrapDmFindSortPositionV10, 0xA07D)
        _ENTRY(sysTrapDmSet, 0xA07E)
        _ENTRY(sysTrapDmCreateDatabaseFromImage, 0xA07F)

        _ENTRY(sysTrapDbgSrcMessage, 0xA080)
        _ENTRY(sysTrapDbgMessage, 0xA081)
        _ENTRY(sysTrapDbgGetMessage, 0xA082)
        _ENTRY(sysTrapDbgCommSettings, 0xA083)

        _ENTRY(sysTrapErrDisplayFileLineMsg, 0xA084)
        _ENTRY(sysTrapErrSetJump, 0xA085)
        _ENTRY(sysTrapErrLongJump, 0xA086)
        _ENTRY(sysTrapErrThrow, 0xA087)
        _ENTRY(sysTrapErrExceptionList, 0xA088)

        _ENTRY(sysTrapSysBroadcastActionCode, 0xA089)
        _ENTRY(sysTrapSysUnimplemented, 0xA08A)
        _ENTRY(sysTrapSysColdBoot, 0xA08B)
        _ENTRY(sysTrapSysReset, 0xA08C)
        _ENTRY(sysTrapSysDoze, 0xA08D)
        _ENTRY(sysTrapSysAppLaunch, 0xA08E)
        _ENTRY(sysTrapSysAppStartup, 0xA08F)
        _ENTRY(sysTrapSysAppExit, 0xA090)
        _ENTRY(sysTrapSysSetA5, 0xA091)
        _ENTRY(sysTrapSysSetTrapAddress, 0xA092)
        _ENTRY(sysTrapSysGetTrapAddress, 0xA093)
        _ENTRY(sysTrapSysTranslateKernelErr, 0xA094)
        _ENTRY(sysTrapSysSemaphoreCreate, 0xA095)
        _ENTRY(sysTrapSysSemaphoreDelete, 0xA096)
        _ENTRY(sysTrapSysSemaphoreWait, 0xA097)
        _ENTRY(sysTrapSysSemaphoreSignal, 0xA098)
        _ENTRY(sysTrapSysTimerCreate, 0xA099)
        _ENTRY(sysTrapSysTimerWrite, 0xA09A)
        _ENTRY(sysTrapSysTaskCreate, 0xA09B)
        _ENTRY(sysTrapSysTaskDelete, 0xA09C)
        _ENTRY(sysTrapSysTaskTrigger, 0xA09D)
        _ENTRY(sysTrapSysTaskID, 0xA09E)
        _ENTRY(sysTrapSysTaskUserInfoPtr, 0xA09F)
        _ENTRY(sysTrapSysTaskDelay, 0xA0A0)
        _ENTRY(sysTrapSysTaskSetTermProc, 0xA0A1)
        _ENTRY(sysTrapSysUILaunch, 0xA0A2)
        _ENTRY(sysTrapSysNewOwnerID, 0xA0A3)
        _ENTRY(sysTrapSysSemaphoreSet, 0xA0A4)
        _ENTRY(sysTrapSysDisableInts, 0xA0A5)
        _ENTRY(sysTrapSysRestoreStatus, 0xA0A6)
        _ENTRY(sysTrapSysUIAppSwitch, 0xA0A7)
        _ENTRY(sysTrapSysCurAppInfoPV20, 0xA0A8)
        _ENTRY(sysTrapSysHandleEvent, 0xA0A9)
        _ENTRY(sysTrapSysInit, 0xA0AA)
        _ENTRY(sysTrapSysQSort, 0xA0AB)
        _ENTRY(sysTrapSysCurAppDatabase, 0xA0AC)
        _ENTRY(sysTrapSysFatalAlert, 0xA0AD)
        _ENTRY(sysTrapSysResSemaphoreCreate, 0xA0AE)
        _ENTRY(sysTrapSysResSemaphoreDelete, 0xA0AF)
        _ENTRY(sysTrapSysResSemaphoreReserve, 0xA0B0)
        _ENTRY(sysTrapSysResSemaphoreRelease, 0xA0B1)
        _ENTRY(sysTrapSysSleep, 0xA0B2)
        _ENTRY(sysTrapSysKeyboardDialogV10, 0xA0B3)
        _ENTRY(sysTrapSysAppLauncherDialog, 0xA0B4)
        _ENTRY(sysTrapSysSetPerformance, 0xA0B5)
        _ENTRY(sysTrapSysBatteryInfoV20, 0xA0B6)
        _ENTRY(sysTrapSysLibInstall, 0xA0B7)
        _ENTRY(sysTrapSysLibRemove, 0xA0B8)
        _ENTRY(sysTrapSysLibTblEntry, 0xA0B9)
        _ENTRY(sysTrapSysLibFind, 0xA0BA)
        _ENTRY(sysTrapSysBatteryDialog, 0xA0BB)
        _ENTRY(sysTrapSysCopyStringResource, 0xA0BC)
        _ENTRY(sysTrapSysKernelInfo, 0xA0BD)
        _ENTRY(sysTrapSysLaunchConsole, 0xA0BE)
        _ENTRY(sysTrapSysTimerDelete, 0xA0BF)
        _ENTRY(sysTrapSysSetAutoOffTime, 0xA0C0)
        _ENTRY(sysTrapSysFormPointerArrayToStrings, 0xA0C1)
        _ENTRY(sysTrapSysRandom, 0xA0C2)
        _ENTRY(sysTrapSysTaskSwitching, 0xA0C3)
        _ENTRY(sysTrapSysTimerRead, 0xA0C4)

        _ENTRY(sysTrapStrCopy, 0xA0C5)
        _ENTRY(sysTrapStrCat, 0xA0C6)
        _ENTRY(sysTrapStrLen, 0xA0C7)
        _ENTRY(sysTrapStrCompare, 0xA0C8)
        _ENTRY(sysTrapStrIToA, 0xA0C9)
        _ENTRY(sysTrapStrCaselessCompare, 0xA0CA)
        _ENTRY(sysTrapStrIToH, 0xA0CB)
        _ENTRY(sysTrapStrChr, 0xA0CC)
        _ENTRY(sysTrapStrStr, 0xA0CD)
        _ENTRY(sysTrapStrAToI, 0xA0CE)
        _ENTRY(sysTrapStrToLower, 0xA0CF)

        _ENTRY(sysTrapSerReceiveISP, 0xA0D0)

        _ENTRY(sysTrapSlkOpen, 0xA0D1)
        _ENTRY(sysTrapSlkClose, 0xA0D2)
        _ENTRY(sysTrapSlkOpenSocket, 0xA0D3)
        _ENTRY(sysTrapSlkCloseSocket, 0xA0D4)
        _ENTRY(sysTrapSlkSocketRefNum, 0xA0D5)
        _ENTRY(sysTrapSlkSocketSetTimeout, 0xA0D6)
        _ENTRY(sysTrapSlkFlushSocket, 0xA0D7)
        _ENTRY(sysTrapSlkSetSocketListener, 0xA0D8)
        _ENTRY(sysTrapSlkSendPacket, 0xA0D9)
        _ENTRY(sysTrapSlkReceivePacket, 0xA0DA)
        _ENTRY(sysTrapSlkSysPktDefaultResponse, 0xA0DB)
        _ENTRY(sysTrapSlkProcessRPC, 0xA0DC)

        _ENTRY(sysTrapConPutS, 0xA0DD)
        _ENTRY(sysTrapConGetS, 0xA0DE)

        _ENTRY(sysTrapFplInit, 0xA0DF)         /* Obsolete, here for compatibilty only! */
        _ENTRY(sysTrapFplFree, 0xA0E0)         /* Obsolete, here for compatibilty only! */
        _ENTRY(sysTrapFplFToA, 0xA0E1)         /* Obsolete, here for compatibilty only! */
        _ENTRY(sysTrapFplAToF, 0xA0E2)         /* Obsolete, here for compatibilty only! */
        _ENTRY(sysTrapFplBase10Info, 0xA0E3)   /* Obsolete, here for compatibilty only! */
        _ENTRY(sysTrapFplLongToFloat, 0xA0E4)  /* Obsolete, here for compatibilty only! */
        _ENTRY(sysTrapFplFloatToLong, 0xA0E5)  /* Obsolete, here for compatibilty only! */
        _ENTRY(sysTrapFplFloatToULong, 0xA0E6) /* Obsolete, here for compatibilty only! */
        _ENTRY(sysTrapFplMul, 0xA0E7)          /* Obsolete, here for compatibilty only! */
        _ENTRY(sysTrapFplAdd, 0xA0E8)          /* Obsolete, here for compatibilty only! */
        _ENTRY(sysTrapFplSub, 0xA0E9)          /* Obsolete, here for compatibilty only! */
        _ENTRY(sysTrapFplDiv, 0xA0EA)          /* Obsolete, here for compatibilty only! */

        _ENTRY(sysTrapWinScreenInit, 0xA0EB) /* was sysTrapScrInit */
        _ENTRY(sysTrapScrCopyRectangle, 0xA0EC)
        _ENTRY(sysTrapScrDrawChars, 0xA0ED)
        _ENTRY(sysTrapScrLineRoutine, 0xA0EE)
        _ENTRY(sysTrapScrRectangleRoutine, 0xA0EF)
        _ENTRY(sysTrapScrScreenInfo, 0xA0F0)
        _ENTRY(sysTrapScrDrawNotify, 0xA0F1)
        _ENTRY(sysTrapScrSendUpdateArea, 0xA0F2)
        _ENTRY(sysTrapScrCompressScanLine, 0xA0F3)
        _ENTRY(sysTrapScrDeCompressScanLine, 0xA0F4)

        _ENTRY(sysTrapTimGetSeconds, 0xA0F5)
        _ENTRY(sysTrapTimSetSeconds, 0xA0F6)
        _ENTRY(sysTrapTimGetTicks, 0xA0F7)
        _ENTRY(sysTrapTimInit, 0xA0F8)
        _ENTRY(sysTrapTimSetAlarm, 0xA0F9)
        _ENTRY(sysTrapTimGetAlarm, 0xA0FA)
        _ENTRY(sysTrapTimHandleInterrupt, 0xA0FB)
        _ENTRY(sysTrapTimSecondsToDateTime, 0xA0FC)
        _ENTRY(sysTrapTimDateTimeToSeconds, 0xA0FD)
        _ENTRY(sysTrapTimAdjust, 0xA0FE)
        _ENTRY(sysTrapTimSleep, 0xA0FF)
        _ENTRY(sysTrapTimWake, 0xA100)

        _ENTRY(sysTrapCategoryCreateListV10, 0xA101)
        _ENTRY(sysTrapCategoryFreeListV10, 0xA102)
        _ENTRY(sysTrapCategoryFind, 0xA103)
        _ENTRY(sysTrapCategoryGetName, 0xA104)
        _ENTRY(sysTrapCategoryEditV10, 0xA105)
        _ENTRY(sysTrapCategorySelectV10, 0xA106)
        _ENTRY(sysTrapCategoryGetNext, 0xA107)
        _ENTRY(sysTrapCategorySetTriggerLabel, 0xA108)
        _ENTRY(sysTrapCategoryTruncateName, 0xA109)

        _ENTRY(sysTrapClipboardAddItem, 0xA10A)
        _ENTRY(sysTrapClipboardCheckIfItemExist, 0xA10B)
        _ENTRY(sysTrapClipboardGetItem, 0xA10C)

        _ENTRY(sysTrapCtlDrawControl, 0xA10D)
        _ENTRY(sysTrapCtlEraseControl, 0xA10E)
        _ENTRY(sysTrapCtlHideControl, 0xA10F)
        _ENTRY(sysTrapCtlShowControl, 0xA110)
        _ENTRY(sysTrapCtlGetValue, 0xA111)
        _ENTRY(sysTrapCtlSetValue, 0xA112)
        _ENTRY(sysTrapCtlGetLabel, 0xA113)
        _ENTRY(sysTrapCtlSetLabel, 0xA114)
        _ENTRY(sysTrapCtlHandleEvent, 0xA115)
        _ENTRY(sysTrapCtlHitControl, 0xA116)
        _ENTRY(sysTrapCtlSetEnabled, 0xA117)
        _ENTRY(sysTrapCtlSetUsable, 0xA118)
        _ENTRY(sysTrapCtlEnabled, 0xA119)

        _ENTRY(sysTrapEvtInitialize, 0xA11A)
        _ENTRY(sysTrapEvtAddEventToQueue, 0xA11B)
        _ENTRY(sysTrapEvtCopyEvent, 0xA11C)
        _ENTRY(sysTrapEvtGetEvent, 0xA11D)
        _ENTRY(sysTrapEvtGetPen, 0xA11E)
        _ENTRY(sysTrapEvtSysInit, 0xA11F)
        _ENTRY(sysTrapEvtGetSysEvent, 0xA120)
        _ENTRY(sysTrapEvtProcessSoftKeyStroke, 0xA121)
        _ENTRY(sysTrapEvtGetPenBtnList, 0xA122)
        _ENTRY(sysTrapEvtSetPenQueuePtr, 0xA123)
        _ENTRY(sysTrapEvtPenQueueSize, 0xA124)
        _ENTRY(sysTrapEvtFlushPenQueue, 0xA125)
        _ENTRY(sysTrapEvtEnqueuePenPoint, 0xA126)
        _ENTRY(sysTrapEvtDequeuePenStrokeInfo, 0xA127)
        _ENTRY(sysTrapEvtDequeuePenPoint, 0xA128)
        _ENTRY(sysTrapEvtFlushNextPenStroke, 0xA129)
        _ENTRY(sysTrapEvtSetKeyQueuePtr, 0xA12A)
        _ENTRY(sysTrapEvtKeyQueueSize, 0xA12B)
        _ENTRY(sysTrapEvtFlushKeyQueue, 0xA12C)
        _ENTRY(sysTrapEvtEnqueueKey, 0xA12D)
        _ENTRY(sysTrapEvtDequeueKeyEvent, 0xA12E)
        _ENTRY(sysTrapEvtWakeup, 0xA12F)
        _ENTRY(sysTrapEvtResetAutoOffTimer, 0xA130)
        _ENTRY(sysTrapEvtKeyQueueEmpty, 0xA131)
        _ENTRY(sysTrapEvtEnableGraffiti, 0xA132)

        _ENTRY(sysTrapFldCopy, 0xA133)
        _ENTRY(sysTrapFldCut, 0xA134)
        _ENTRY(sysTrapFldDrawField, 0xA135)
        _ENTRY(sysTrapFldEraseField, 0xA136)
        _ENTRY(sysTrapFldFreeMemory, 0xA137)
        _ENTRY(sysTrapFldGetBounds, 0xA138)
        _ENTRY(sysTrapFldGetTextPtr, 0xA139)
        _ENTRY(sysTrapFldGetSelection, 0xA13A)
        _ENTRY(sysTrapFldHandleEvent, 0xA13B)
        _ENTRY(sysTrapFldPaste, 0xA13C)
        _ENTRY(sysTrapFldRecalculateField, 0xA13D)
        _ENTRY(sysTrapFldSetBounds, 0xA13E)
        _ENTRY(sysTrapFldSetText, 0xA13F)
        _ENTRY(sysTrapFldGetFont, 0xA140)
        _ENTRY(sysTrapFldSetFont, 0xA141)
        _ENTRY(sysTrapFldSetSelection, 0xA142)
        _ENTRY(sysTrapFldGrabFocus, 0xA143)
        _ENTRY(sysTrapFldReleaseFocus, 0xA144)
        _ENTRY(sysTrapFldGetInsPtPosition, 0xA145)
        _ENTRY(sysTrapFldSetInsPtPosition, 0xA146)
        _ENTRY(sysTrapFldSetScrollPosition, 0xA147)
        _ENTRY(sysTrapFldGetScrollPosition, 0xA148)
        _ENTRY(sysTrapFldGetTextHeight, 0xA149)
        _ENTRY(sysTrapFldGetTextAllocatedSize, 0xA14A)
        _ENTRY(sysTrapFldGetTextLength, 0xA14B)
        _ENTRY(sysTrapFldScrollField, 0xA14C)
        _ENTRY(sysTrapFldScrollable, 0xA14D)
        _ENTRY(sysTrapFldGetVisibleLines, 0xA14E)
        _ENTRY(sysTrapFldGetAttributes, 0xA14F)
        _ENTRY(sysTrapFldSetAttributes, 0xA150)
        _ENTRY(sysTrapFldSendChangeNotification, 0xA151)
        _ENTRY(sysTrapFldCalcFieldHeight, 0xA152)
        _ENTRY(sysTrapFldGetTextHandle, 0xA153)
        _ENTRY(sysTrapFldCompactText, 0xA154)
        _ENTRY(sysTrapFldDirty, 0xA155)
        _ENTRY(sysTrapFldWordWrap, 0xA156)
        _ENTRY(sysTrapFldSetTextAllocatedSize, 0xA157)
        _ENTRY(sysTrapFldSetTextHandle, 0xA158)
        _ENTRY(sysTrapFldSetTextPtr, 0xA159)
        _ENTRY(sysTrapFldGetMaxChars, 0xA15A)
        _ENTRY(sysTrapFldSetMaxChars, 0xA15B)
        _ENTRY(sysTrapFldSetUsable, 0xA15C)
        _ENTRY(sysTrapFldInsert, 0xA15D)
        _ENTRY(sysTrapFldDelete, 0xA15E)
        _ENTRY(sysTrapFldUndo, 0xA15F)
        _ENTRY(sysTrapFldSetDirty, 0xA160)
        _ENTRY(sysTrapFldSendHeightChangeNotification, 0xA161)
        _ENTRY(sysTrapFldMakeFullyVisible, 0xA162)

        _ENTRY(sysTrapFntGetFont, 0xA163)
        _ENTRY(sysTrapFntSetFont, 0xA164)
        _ENTRY(sysTrapFntGetFontPtr, 0xA165)
        _ENTRY(sysTrapFntBaseLine, 0xA166)
        _ENTRY(sysTrapFntCharHeight, 0xA167)
        _ENTRY(sysTrapFntLineHeight, 0xA168)
        _ENTRY(sysTrapFntAverageCharWidth, 0xA169)
        _ENTRY(sysTrapFntCharWidth, 0xA16A)
        _ENTRY(sysTrapFntCharsWidth, 0xA16B)
        _ENTRY(sysTrapFntDescenderHeight, 0xA16C)
        _ENTRY(sysTrapFntCharsInWidth, 0xA16D)
        _ENTRY(sysTrapFntLineWidth, 0xA16E)

        _ENTRY(sysTrapFrmInitForm, 0xA16F)
        _ENTRY(sysTrapFrmDeleteForm, 0xA170)
        _ENTRY(sysTrapFrmDrawForm, 0xA171)
        _ENTRY(sysTrapFrmEraseForm, 0xA172)
        _ENTRY(sysTrapFrmGetActiveForm, 0xA173)
        _ENTRY(sysTrapFrmSetActiveForm, 0xA174)
        _ENTRY(sysTrapFrmGetActiveFormID, 0xA175)
        _ENTRY(sysTrapFrmGetUserModifiedState, 0xA176)
        _ENTRY(sysTrapFrmSetNotUserModified, 0xA177)
        _ENTRY(sysTrapFrmGetFocus, 0xA178)
        _ENTRY(sysTrapFrmSetFocus, 0xA179)
        _ENTRY(sysTrapFrmHandleEvent, 0xA17A)
        _ENTRY(sysTrapFrmGetFormBounds, 0xA17B)
        _ENTRY(sysTrapFrmGetWindowHandle, 0xA17C)
        _ENTRY(sysTrapFrmGetFormId, 0xA17D)
        _ENTRY(sysTrapFrmGetFormPtr, 0xA17E)
        _ENTRY(sysTrapFrmGetNumberOfObjects, 0xA17F)
        _ENTRY(sysTrapFrmGetObjectIndex, 0xA180)
        _ENTRY(sysTrapFrmGetObjectId, 0xA181)
        _ENTRY(sysTrapFrmGetObjectType, 0xA182)
        _ENTRY(sysTrapFrmGetObjectPtr, 0xA183)
        _ENTRY(sysTrapFrmHideObject, 0xA184)
        _ENTRY(sysTrapFrmShowObject, 0xA185)
        _ENTRY(sysTrapFrmGetObjectPosition, 0xA186)
        _ENTRY(sysTrapFrmSetObjectPosition, 0xA187)
        _ENTRY(sysTrapFrmGetControlValue, 0xA188)
        _ENTRY(sysTrapFrmSetControlValue, 0xA189)
        _ENTRY(sysTrapFrmGetControlGroupSelection, 0xA18A)
        _ENTRY(sysTrapFrmSetControlGroupSelection, 0xA18B)
        _ENTRY(sysTrapFrmCopyLabel, 0xA18C)
        _ENTRY(sysTrapFrmSetLabel, 0xA18D)
        _ENTRY(sysTrapFrmGetLabel, 0xA18E)
        _ENTRY(sysTrapFrmSetCategoryLabel, 0xA18F)
        _ENTRY(sysTrapFrmGetTitle, 0xA190)
        _ENTRY(sysTrapFrmSetTitle, 0xA191)
        _ENTRY(sysTrapFrmAlert, 0xA192)
        _ENTRY(sysTrapFrmDoDialog, 0xA193)
        _ENTRY(sysTrapFrmCustomAlert, 0xA194)
        _ENTRY(sysTrapFrmHelp, 0xA195)
        _ENTRY(sysTrapFrmUpdateScrollers, 0xA196)
        _ENTRY(sysTrapFrmGetFirstForm, 0xA197)
        _ENTRY(sysTrapFrmVisible, 0xA198)
        _ENTRY(sysTrapFrmGetObjectBounds, 0xA199)
        _ENTRY(sysTrapFrmCopyTitle, 0xA19A)
        _ENTRY(sysTrapFrmGotoForm, 0xA19B)
        _ENTRY(sysTrapFrmPopupForm, 0xA19C)
        _ENTRY(sysTrapFrmUpdateForm, 0xA19D)
        _ENTRY(sysTrapFrmReturnToForm, 0xA19E)
        _ENTRY(sysTrapFrmSetEventHandler, 0xA19F)
        _ENTRY(sysTrapFrmDispatchEvent, 0xA1A0)
        _ENTRY(sysTrapFrmCloseAllForms, 0xA1A1)
        _ENTRY(sysTrapFrmSaveAllForms, 0xA1A2)
        _ENTRY(sysTrapFrmGetGadgetData, 0xA1A3)
        _ENTRY(sysTrapFrmSetGadgetData, 0xA1A4)
        _ENTRY(sysTrapFrmSetCategoryTrigger, 0xA1A5)

        _ENTRY(sysTrapUIInitialize, 0xA1A6)
        _ENTRY(sysTrapUIReset, 0xA1A7)

        _ENTRY(sysTrapInsPtInitialize, 0xA1A8)
        _ENTRY(sysTrapInsPtSetLocation, 0xA1A9)
        _ENTRY(sysTrapInsPtGetLocation, 0xA1AA)
        _ENTRY(sysTrapInsPtEnable, 0xA1AB)
        _ENTRY(sysTrapInsPtEnabled, 0xA1AC)
        _ENTRY(sysTrapInsPtSetHeight, 0xA1AD)
        _ENTRY(sysTrapInsPtGetHeight, 0xA1AE)
        _ENTRY(sysTrapInsPtCheckBlink, 0xA1AF)

        _ENTRY(sysTrapLstSetDrawFunction, 0xA1B0)
        _ENTRY(sysTrapLstDrawList, 0xA1B1)
        _ENTRY(sysTrapLstEraseList, 0xA1B2)
        _ENTRY(sysTrapLstGetSelection, 0xA1B3)
        _ENTRY(sysTrapLstGetSelectionText, 0xA1B4)
        _ENTRY(sysTrapLstHandleEvent, 0xA1B5)
        _ENTRY(sysTrapLstSetHeight, 0xA1B6)
        _ENTRY(sysTrapLstSetSelection, 0xA1B7)
        _ENTRY(sysTrapLstSetListChoices, 0xA1B8)
        _ENTRY(sysTrapLstMakeItemVisible, 0xA1B9)
        _ENTRY(sysTrapLstGetNumberOfItems, 0xA1BA)
        _ENTRY(sysTrapLstPopupList, 0xA1BB)
        _ENTRY(sysTrapLstSetPosition, 0xA1BC)

        _ENTRY(sysTrapMenuInit, 0xA1BD)
        _ENTRY(sysTrapMenuDispose, 0xA1BE)
        _ENTRY(sysTrapMenuHandleEvent, 0xA1BF)
        _ENTRY(sysTrapMenuDrawMenu, 0xA1C0)
        _ENTRY(sysTrapMenuEraseStatus, 0xA1C1)
        _ENTRY(sysTrapMenuGetActiveMenu, 0xA1C2)
        _ENTRY(sysTrapMenuSetActiveMenu, 0xA1C3)

        _ENTRY(sysTrapRctSetRectangle, 0xA1C4)
        _ENTRY(sysTrapRctCopyRectangle, 0xA1C5)
        _ENTRY(sysTrapRctInsetRectangle, 0xA1C6)
        _ENTRY(sysTrapRctOffsetRectangle, 0xA1C7)
        _ENTRY(sysTrapRctPtInRectangle, 0xA1C8)
        _ENTRY(sysTrapRctGetIntersection, 0xA1C9)

        _ENTRY(sysTrapTblDrawTable, 0xA1CA)
        _ENTRY(sysTrapTblEraseTable, 0xA1CB)
        _ENTRY(sysTrapTblHandleEvent, 0xA1CC)
        _ENTRY(sysTrapTblGetItemBounds, 0xA1CD)
        _ENTRY(sysTrapTblSelectItem, 0xA1CE)
        _ENTRY(sysTrapTblGetItemInt, 0xA1CF)
        _ENTRY(sysTrapTblSetItemInt, 0xA1D0)
        _ENTRY(sysTrapTblSetItemStyle, 0xA1D1)
        _ENTRY(sysTrapTblUnhighlightSelection, 0xA1D2)
        _ENTRY(sysTrapTblSetRowUsable, 0xA1D3)
        _ENTRY(sysTrapTblGetNumberOfRows, 0xA1D4)
        _ENTRY(sysTrapTblSetCustomDrawProcedure, 0xA1D5)
        _ENTRY(sysTrapTblSetRowSelectable, 0xA1D6)
        _ENTRY(sysTrapTblRowSelectable, 0xA1D7)
        _ENTRY(sysTrapTblSetLoadDataProcedure, 0xA1D8)
        _ENTRY(sysTrapTblSetSaveDataProcedure, 0xA1D9)
        _ENTRY(sysTrapTblGetBounds, 0xA1DA)
        _ENTRY(sysTrapTblSetRowHeight, 0xA1DB)
        _ENTRY(sysTrapTblGetColumnWidth, 0xA1DC)
        _ENTRY(sysTrapTblGetRowID, 0xA1DD)
        _ENTRY(sysTrapTblSetRowID, 0xA1DE)
        _ENTRY(sysTrapTblMarkRowInvalid, 0xA1DF)
        _ENTRY(sysTrapTblMarkTableInvalid, 0xA1E0)
        _ENTRY(sysTrapTblGetSelection, 0xA1E1)
        _ENTRY(sysTrapTblInsertRow, 0xA1E2)
        _ENTRY(sysTrapTblRemoveRow, 0xA1E3)
        _ENTRY(sysTrapTblRowInvalid, 0xA1E4)
        _ENTRY(sysTrapTblRedrawTable, 0xA1E5)
        _ENTRY(sysTrapTblRowUsable, 0xA1E6)
        _ENTRY(sysTrapTblReleaseFocus, 0xA1E7)
        _ENTRY(sysTrapTblEditing, 0xA1E8)
        _ENTRY(sysTrapTblGetCurrentField, 0xA1E9)
        _ENTRY(sysTrapTblSetColumnUsable, 0xA1EA)
        _ENTRY(sysTrapTblGetRowHeight, 0xA1EB)
        _ENTRY(sysTrapTblSetColumnWidth, 0xA1EC)
        _ENTRY(sysTrapTblGrabFocus, 0xA1ED)
        _ENTRY(sysTrapTblSetItemPtr, 0xA1EE)
        _ENTRY(sysTrapTblFindRowID, 0xA1EF)
        _ENTRY(sysTrapTblGetLastUsableRow, 0xA1F0)
        _ENTRY(sysTrapTblGetColumnSpacing, 0xA1F1)
        _ENTRY(sysTrapTblFindRowData, 0xA1F2)
        _ENTRY(sysTrapTblGetRowData, 0xA1F3)
        _ENTRY(sysTrapTblSetRowData, 0xA1F4)
        _ENTRY(sysTrapTblSetColumnSpacing, 0xA1F5)

        _ENTRY(sysTrapWinCreateWindow, 0xA1F6)
        _ENTRY(sysTrapWinCreateOffscreenWindow, 0xA1F7)
        _ENTRY(sysTrapWinDeleteWindow, 0xA1F8)
        _ENTRY(sysTrapWinInitializeWindow, 0xA1F9)
        _ENTRY(sysTrapWinAddWindow, 0xA1FA)
        _ENTRY(sysTrapWinRemoveWindow, 0xA1FB)
        _ENTRY(sysTrapWinSetActiveWindow, 0xA1FC)
        _ENTRY(sysTrapWinSetDrawWindow, 0xA1FD)
        _ENTRY(sysTrapWinGetDrawWindow, 0xA1FE)
        _ENTRY(sysTrapWinGetActiveWindow, 0xA1FF)
        _ENTRY(sysTrapWinGetDisplayWindow, 0xA200)
        _ENTRY(sysTrapWinGetFirstWindow, 0xA201)
        _ENTRY(sysTrapWinEnableWindow, 0xA202)
        _ENTRY(sysTrapWinDisableWindow, 0xA203)
        _ENTRY(sysTrapWinGetWindowFrameRect, 0xA204)
        _ENTRY(sysTrapWinDrawWindowFrame, 0xA205)
        _ENTRY(sysTrapWinEraseWindow, 0xA206)
        _ENTRY(sysTrapWinSaveBits, 0xA207)
        _ENTRY(sysTrapWinRestoreBits, 0xA208)
        _ENTRY(sysTrapWinCopyRectangle, 0xA209)
        _ENTRY(sysTrapWinScrollRectangle, 0xA20A)
        _ENTRY(sysTrapWinGetDisplayExtent, 0xA20B)
        _ENTRY(sysTrapWinGetWindowExtent, 0xA20C)
        _ENTRY(sysTrapWinDisplayToWindowPt, 0xA20D)
        _ENTRY(sysTrapWinWindowToDisplayPt, 0xA20E)
        _ENTRY(sysTrapWinGetClip, 0xA20F)
        _ENTRY(sysTrapWinSetClip, 0xA210)
        _ENTRY(sysTrapWinResetClip, 0xA211)
        _ENTRY(sysTrapWinClipRectangle, 0xA212)
        _ENTRY(sysTrapWinDrawLine, 0xA213)
        _ENTRY(sysTrapWinDrawGrayLine, 0xA214)
        _ENTRY(sysTrapWinEraseLine, 0xA215)
        _ENTRY(sysTrapWinInvertLine, 0xA216)
        _ENTRY(sysTrapWinFillLine, 0xA217)
        _ENTRY(sysTrapWinDrawRectangle, 0xA218)
        _ENTRY(sysTrapWinEraseRectangle, 0xA219)
        _ENTRY(sysTrapWinInvertRectangle, 0xA21A)
        _ENTRY(sysTrapWinDrawRectangleFrame, 0xA21B)
        _ENTRY(sysTrapWinDrawGrayRectangleFrame, 0xA21C)
        _ENTRY(sysTrapWinEraseRectangleFrame, 0xA21D)
        _ENTRY(sysTrapWinInvertRectangleFrame, 0xA21E)
        _ENTRY(sysTrapWinGetFramesRectangle, 0xA21F)
        _ENTRY(sysTrapWinDrawChars, 0xA220)
        _ENTRY(sysTrapWinEraseChars, 0xA221)
        _ENTRY(sysTrapWinInvertChars, 0xA222)
        _ENTRY(sysTrapWinGetPattern, 0xA223)
        _ENTRY(sysTrapWinSetPattern, 0xA224)
        _ENTRY(sysTrapWinSetUnderlineMode, 0xA225)
        _ENTRY(sysTrapWinDrawBitmap, 0xA226)
        _ENTRY(sysTrapWinModal, 0xA227)
        _ENTRY(sysTrapWinGetDrawWindowBounds, 0xA228)
        _ENTRY(sysTrapWinFillRectangle, 0xA229)
        _ENTRY(sysTrapWinDrawInvertedChars, 0xA22A)

        _ENTRY(sysTrapPrefOpenPreferenceDBV10, 0xA22B)
        _ENTRY(sysTrapPrefGetPreferences, 0xA22C)
        _ENTRY(sysTrapPrefSetPreferences, 0xA22D)
        _ENTRY(sysTrapPrefGetAppPreferencesV10, 0xA22E)
        _ENTRY(sysTrapPrefSetAppPreferencesV10, 0xA22F)

        _ENTRY(sysTrapSndInit, 0xA230)
        _ENTRY(sysTrapSndSetDefaultVolume, 0xA231)
        _ENTRY(sysTrapSndGetDefaultVolume, 0xA232)
        _ENTRY(sysTrapSndDoCmd, 0xA233)
        _ENTRY(sysTrapSndPlaySystemSound, 0xA234)

        _ENTRY(sysTrapAlmInit, 0xA235)
        _ENTRY(sysTrapAlmCancelAll, 0xA236)
        _ENTRY(sysTrapAlmAlarmCallback, 0xA237)
        _ENTRY(sysTrapAlmSetAlarm, 0xA238)
        _ENTRY(sysTrapAlmGetAlarm, 0xA239)
        _ENTRY(sysTrapAlmDisplayAlarm, 0xA23A)
        _ENTRY(sysTrapAlmEnableNotification, 0xA23B)

        _ENTRY(sysTrapHwrGetRAMMapping, 0xA23C)
        _ENTRY(sysTrapHwrMemWritable, 0xA23D)
        _ENTRY(sysTrapHwrMemReadable, 0xA23E)
        _ENTRY(sysTrapHwrDoze, 0xA23F)
        _ENTRY(sysTrapHwrSleep, 0xA240)
        _ENTRY(sysTrapHwrWake, 0xA241)
        _ENTRY(sysTrapHwrSetSystemClock, 0xA242)
        _ENTRY(sysTrapHwrSetCPUDutyCycle, 0xA243)
        _ENTRY(sysTrapHwrDisplayInit,
               0xA244) /* Before OS 3.5, this trap a.k.a. sysTrapHwrLCDInit */
        _ENTRY(sysTrapHwrDisplaySleep,
               0xA245) /* Before OS 3.5, this trap a.k.a. sysTrapHwrLCDSleep, */
        _ENTRY(sysTrapHwrTimerInit, 0xA246)
        _ENTRY(sysTrapHwrCursorV33, 0xA247) /* This trap obsoleted for OS 3.5 and later */
        _ENTRY(sysTrapHwrBatteryLevel, 0xA248)
        _ENTRY(sysTrapHwrDelay, 0xA249)
        _ENTRY(sysTrapHwrEnableDataWrites, 0xA24A)
        _ENTRY(sysTrapHwrDisableDataWrites, 0xA24B)
        _ENTRY(sysTrapHwrLCDBaseAddrV33, 0xA24C) /* This trap obsoleted for OS 3.5 and later */
        _ENTRY(sysTrapHwrDisplayDrawBootScreen,
               0xA24D) /* Before OS 3.5, this trap a.k.a. sysTrapHwrLCDDrawBitmap */
        _ENTRY(sysTrapHwrTimerSleep, 0xA24E)
        _ENTRY(sysTrapHwrTimerWake, 0xA24F)
        _ENTRY(sysTrapHwrDisplayWake,
               0xA250) /* Before OS 3.5, this trap a.k.a. sysTrapHwrLCDWake */
        _ENTRY(sysTrapHwrIRQ1Handler, 0xA251)
        _ENTRY(sysTrapHwrIRQ2Handler, 0xA252)
        _ENTRY(sysTrapHwrIRQ3Handler, 0xA253)
        _ENTRY(sysTrapHwrIRQ4Handler, 0xA254)
        _ENTRY(sysTrapHwrIRQ5Handler, 0xA255)
        _ENTRY(sysTrapHwrIRQ6Handler, 0xA256)
        _ENTRY(sysTrapHwrDockSignals, 0xA257)
        _ENTRY(sysTrapHwrPluggedIn, 0xA258)

        _ENTRY(sysTrapCrc16CalcBlock, 0xA259)

        _ENTRY(sysTrapSelectDayV10, 0xA25A)
        _ENTRY(sysTrapSelectTimeV33, 0xA25B)

        _ENTRY(sysTrapDayDrawDaySelector, 0xA25C)
        _ENTRY(sysTrapDayHandleEvent, 0xA25D)
        _ENTRY(sysTrapDayDrawDays, 0xA25E)
        _ENTRY(sysTrapDayOfWeek, 0xA25F)
        _ENTRY(sysTrapDaysInMonth, 0xA260)
        _ENTRY(sysTrapDayOfMonth, 0xA261)

        _ENTRY(sysTrapDateDaysToDate, 0xA262)
        _ENTRY(sysTrapDateToDays, 0xA263)
        _ENTRY(sysTrapDateAdjust, 0xA264)
        _ENTRY(sysTrapDateSecondsToDate, 0xA265)
        _ENTRY(sysTrapDateToAscii, 0xA266)
        _ENTRY(sysTrapDateToDOWDMFormat, 0xA267)
        _ENTRY(sysTrapTimeToAscii, 0xA268)

        _ENTRY(sysTrapFind, 0xA269)
        _ENTRY(sysTrapFindStrInStr, 0xA26A)
        _ENTRY(sysTrapFindSaveMatch, 0xA26B)
        _ENTRY(sysTrapFindGetLineBounds, 0xA26C)
        _ENTRY(sysTrapFindDrawHeader, 0xA26D)

        _ENTRY(sysTrapPenOpen, 0xA26E)
        _ENTRY(sysTrapPenClose, 0xA26F)
        _ENTRY(sysTrapPenGetRawPen, 0xA270)
        _ENTRY(sysTrapPenCalibrate, 0xA271)
        _ENTRY(sysTrapPenRawToScreen, 0xA272)
        _ENTRY(sysTrapPenScreenToRaw, 0xA273)
        _ENTRY(sysTrapPenResetCalibration, 0xA274)
        _ENTRY(sysTrapPenSleep, 0xA275)
        _ENTRY(sysTrapPenWake, 0xA276)

        _ENTRY(sysTrapResLoadForm, 0xA277)
        _ENTRY(sysTrapResLoadMenu, 0xA278)

        _ENTRY(sysTrapFtrInit, 0xA279)
        _ENTRY(sysTrapFtrUnregister, 0xA27A)
        _ENTRY(sysTrapFtrGet, 0xA27B)
        _ENTRY(sysTrapFtrSet, 0xA27C)
        _ENTRY(sysTrapFtrGetByIndex, 0xA27D)

        _ENTRY(sysTrapGrfInit, 0xA27E)
        _ENTRY(sysTrapGrfFree, 0xA27F)
        _ENTRY(sysTrapGrfGetState, 0xA280)
        _ENTRY(sysTrapGrfSetState, 0xA281)
        _ENTRY(sysTrapGrfFlushPoints, 0xA282)
        _ENTRY(sysTrapGrfAddPoint, 0xA283)
        _ENTRY(sysTrapGrfInitState, 0xA284)
        _ENTRY(sysTrapGrfCleanState, 0xA285)
        _ENTRY(sysTrapGrfMatch, 0xA286)
        _ENTRY(sysTrapGrfGetMacro, 0xA287)
        _ENTRY(sysTrapGrfFilterPoints, 0xA288)
        _ENTRY(sysTrapGrfGetNumPoints, 0xA289)
        _ENTRY(sysTrapGrfGetPoint, 0xA28A)
        _ENTRY(sysTrapGrfFindBranch, 0xA28B)
        _ENTRY(sysTrapGrfMatchGlyph, 0xA28C)
        _ENTRY(sysTrapGrfGetGlyphMapping, 0xA28D)
        _ENTRY(sysTrapGrfGetMacroName, 0xA28E)
        _ENTRY(sysTrapGrfDeleteMacro, 0xA28F)
        _ENTRY(sysTrapGrfAddMacro, 0xA290)
        _ENTRY(sysTrapGrfGetAndExpandMacro, 0xA291)
        _ENTRY(sysTrapGrfProcessStroke, 0xA292)
        _ENTRY(sysTrapGrfFieldChange, 0xA293)

        _ENTRY(sysTrapGetCharSortValue, 0xA294)
        _ENTRY(sysTrapGetCharAttr, 0xA295)
        _ENTRY(sysTrapGetCharCaselessValue, 0xA296)

        _ENTRY(sysTrapPwdExists, 0xA297)
        _ENTRY(sysTrapPwdVerify, 0xA298)
        _ENTRY(sysTrapPwdSet, 0xA299)
        _ENTRY(sysTrapPwdRemove, 0xA29A)

        _ENTRY(sysTrapGsiInitialize, 0xA29B)
        _ENTRY(sysTrapGsiSetLocation, 0xA29C)
        _ENTRY(sysTrapGsiEnable, 0xA29D)
        _ENTRY(sysTrapGsiEnabled, 0xA29E)
        _ENTRY(sysTrapGsiSetShiftState, 0xA29F)

        _ENTRY(sysTrapKeyInit, 0xA2A0)
        _ENTRY(sysTrapKeyHandleInterrupt, 0xA2A1)
        _ENTRY(sysTrapKeyCurrentState, 0xA2A2)
        _ENTRY(sysTrapKeyResetDoubleTap, 0xA2A3)
        _ENTRY(sysTrapKeyRates, 0xA2A4)
        _ENTRY(sysTrapKeySleep, 0xA2A5)
        _ENTRY(sysTrapKeyWake, 0xA2A6)

        _ENTRY(sysTrapDlkControl, 0xA2A7) /* was sysTrapCmBroadcast */

        _ENTRY(sysTrapDlkStartServer, 0xA2A8)
        _ENTRY(sysTrapDlkGetSyncInfo, 0xA2A9)
        _ENTRY(sysTrapDlkSetLogEntry, 0xA2AA)

        _ENTRY(sysTrapIntlDispatch,
               0xA2AB) /* REUSED IN v3.1 (was sysTrapPsrInit in 1.0, removed in 2.0) */
        _ENTRY(sysTrapSysLibLoad, 0xA2AC) /* REUSED IN v2.0 (was sysTrapPsrClose) */
        _ENTRY(sysTrapSndPlaySmf,
               0xA2AD) /* REUSED IN v3.0 (was sysTrapPsrGetCommand in 1.0, removed in 2.0) */
        _ENTRY(sysTrapSndCreateMidiList,
               0xA2AE) /* REUSED IN v3.0 (was sysTrapPsrSendReply in 1.0, removed in 2.0) */

        _ENTRY(sysTrapAbtShowAbout, 0xA2AF)

        _ENTRY(sysTrapMdmDial, 0xA2B0)
        _ENTRY(sysTrapMdmHangUp, 0xA2B1)

        _ENTRY(sysTrapDmSearchRecord, 0xA2B2)

        _ENTRY(sysTrapSysInsertionSort, 0xA2B3)
        _ENTRY(sysTrapDmInsertionSort, 0xA2B4)

        _ENTRY(sysTrapLstSetTopItem, 0xA2B5)

        // Palm OS 2.X traps					Palm Pilot and 2.0 Upgrade Card

        _ENTRY(sysTrapSclSetScrollBar, 0xA2B6)
        _ENTRY(sysTrapSclDrawScrollBar, 0xA2B7)
        _ENTRY(sysTrapSclHandleEvent, 0xA2B8)

        _ENTRY(sysTrapSysMailboxCreate, 0xA2B9)
        _ENTRY(sysTrapSysMailboxDelete, 0xA2BA)
        _ENTRY(sysTrapSysMailboxFlush, 0xA2BB)
        _ENTRY(sysTrapSysMailboxSend, 0xA2BC)
        _ENTRY(sysTrapSysMailboxWait, 0xA2BD)

        _ENTRY(sysTrapSysTaskWait, 0xA2BE)
        _ENTRY(sysTrapSysTaskWake, 0xA2BF)
        _ENTRY(sysTrapSysTaskWaitClr, 0xA2C0)
        _ENTRY(sysTrapSysTaskSuspend, 0xA2C1)
        _ENTRY(sysTrapSysTaskResume, 0xA2C2)

        _ENTRY(sysTrapCategoryCreateList, 0xA2C3)
        _ENTRY(sysTrapCategoryFreeList, 0xA2C4)
        _ENTRY(sysTrapCategoryEditV20, 0xA2C5)
        _ENTRY(sysTrapCategorySelect, 0xA2C6)

        _ENTRY(sysTrapDmDeleteCategory, 0xA2C7)

        _ENTRY(sysTrapSysEvGroupCreate, 0xA2C8)
        _ENTRY(sysTrapSysEvGroupSignal, 0xA2C9)
        _ENTRY(sysTrapSysEvGroupRead, 0xA2CA)
        _ENTRY(sysTrapSysEvGroupWait, 0xA2CB)

        _ENTRY(sysTrapEvtEventAvail, 0xA2CC)
        _ENTRY(sysTrapEvtSysEventAvail, 0xA2CD)
        _ENTRY(sysTrapStrNCopy, 0xA2CE)

        _ENTRY(sysTrapKeySetMask, 0xA2CF)

        _ENTRY(sysTrapSelectDay, 0xA2D0)

        _ENTRY(sysTrapPrefGetPreference, 0xA2D1)
        _ENTRY(sysTrapPrefSetPreference, 0xA2D2)
        _ENTRY(sysTrapPrefGetAppPreferences, 0xA2D3)
        _ENTRY(sysTrapPrefSetAppPreferences, 0xA2D4)

        _ENTRY(sysTrapFrmPointInTitle, 0xA2D5)

        _ENTRY(sysTrapStrNCat, 0xA2D6)

        _ENTRY(sysTrapMemCmp, 0xA2D7)

        _ENTRY(sysTrapTblSetColumnEditIndicator, 0xA2D8)

        _ENTRY(sysTrapFntWordWrap, 0xA2D9)

        _ENTRY(sysTrapFldGetScrollValues, 0xA2DA)

        _ENTRY(sysTrapSysCreateDataBaseList, 0xA2DB)
        _ENTRY(sysTrapSysCreatePanelList, 0xA2DC)

        _ENTRY(sysTrapDlkDispatchRequest, 0xA2DD)

        _ENTRY(sysTrapStrPrintF, 0xA2DE)
        _ENTRY(sysTrapStrVPrintF, 0xA2DF)

        _ENTRY(sysTrapPrefOpenPreferenceDB, 0xA2E0)

        _ENTRY(sysTrapSysGraffitiReferenceDialog, 0xA2E1)

        _ENTRY(sysTrapSysKeyboardDialog, 0xA2E2)

        _ENTRY(sysTrapFntWordWrapReverseNLines, 0xA2E3)
        _ENTRY(sysTrapFntGetScrollValues, 0xA2E4)

        _ENTRY(sysTrapTblSetRowStaticHeight, 0xA2E5)
        _ENTRY(sysTrapTblHasScrollBar, 0xA2E6)

        _ENTRY(sysTrapSclGetScrollBar, 0xA2E7)

        _ENTRY(sysTrapFldGetNumberOfBlankLines, 0xA2E8)

        _ENTRY(sysTrapSysTicksPerSecond, 0xA2E9)
        _ENTRY(sysTrapHwrBacklightV33, 0xA2EA) /* This trap obsoleted for OS 3.5 and later */
        _ENTRY(sysTrapDmDatabaseProtect, 0xA2EB)

        _ENTRY(sysTrapTblSetBounds, 0xA2EC)

        _ENTRY(sysTrapStrNCompare, 0xA2ED)
        _ENTRY(sysTrapStrNCaselessCompare, 0xA2EE)

        _ENTRY(sysTrapPhoneNumberLookup, 0xA2EF)

        _ENTRY(sysTrapFrmSetMenu, 0xA2F0)

        _ENTRY(sysTrapEncDigestMD5, 0xA2F1)

        _ENTRY(sysTrapDmFindSortPosition, 0xA2F2)

        _ENTRY(sysTrapSysBinarySearch, 0xA2F3)
        _ENTRY(sysTrapSysErrString, 0xA2F4)
        _ENTRY(sysTrapSysStringByIndex, 0xA2F5)

        _ENTRY(sysTrapEvtAddUniqueEventToQueue, 0xA2F6)

        _ENTRY(sysTrapStrLocalizeNumber, 0xA2F7)
        _ENTRY(sysTrapStrDelocalizeNumber, 0xA2F8)
        _ENTRY(sysTrapLocGetNumberSeparators, 0xA2F9)

        _ENTRY(sysTrapMenuSetActiveMenuRscID, 0xA2FA)

        _ENTRY(sysTrapLstScrollList, 0xA2FB)

        _ENTRY(sysTrapCategoryInitialize, 0xA2FC)

        _ENTRY(sysTrapEncDigestMD4, 0xA2FD)
        _ENTRY(sysTrapEncDES, 0xA2FE)

        _ENTRY(sysTrapLstGetVisibleItems, 0xA2FF)

        _ENTRY(sysTrapWinSetBounds, 0xA300)

        _ENTRY(sysTrapCategorySetName, 0xA301)

        _ENTRY(sysTrapFldSetInsertionPoint, 0xA302)

        _ENTRY(sysTrapFrmSetObjectBounds, 0xA303)

        _ENTRY(sysTrapWinSetColors, 0xA304)

        _ENTRY(sysTrapFlpDispatch, 0xA305)
        _ENTRY(sysTrapFlpEmDispatch, 0xA306)

        // Palm OS 3.0 traps					Palm III and 3.0 Upgrade Card

        _ENTRY(sysTrapExgInit, 0xA307)
        _ENTRY(sysTrapExgConnect, 0xA308)
        _ENTRY(sysTrapExgPut, 0xA309)
        _ENTRY(sysTrapExgGet, 0xA30A)
        _ENTRY(sysTrapExgAccept, 0xA30B)
        _ENTRY(sysTrapExgDisconnect, 0xA30C)
        _ENTRY(sysTrapExgSend, 0xA30D)
        _ENTRY(sysTrapExgReceive, 0xA30E)
        _ENTRY(sysTrapExgRegisterData, 0xA30F)
        _ENTRY(sysTrapExgNotifyReceiveV35, 0xA310)
        _ENTRY(sysTrapSysReserved30Trap2,
               0xA311) /* "Reserved" trap in Palm OS 3.0 and later (was sysTrapExgControl) */

        _ENTRY(sysTrapPrgStartDialogV31, 0xA312) /* Updated in v3.2 */
        _ENTRY(sysTrapPrgStopDialog, 0xA313)
        _ENTRY(sysTrapPrgUpdateDialog, 0xA314)
        _ENTRY(sysTrapPrgHandleEvent, 0xA315)

        _ENTRY(sysTrapImcReadFieldNoSemicolon, 0xA316)
        _ENTRY(sysTrapImcReadFieldQuotablePrintable, 0xA317)
        _ENTRY(sysTrapImcReadPropertyParameter, 0xA318)
        _ENTRY(sysTrapImcSkipAllPropertyParameters, 0xA319)
        _ENTRY(sysTrapImcReadWhiteSpace, 0xA31A)
        _ENTRY(sysTrapImcWriteQuotedPrintable, 0xA31B)
        _ENTRY(sysTrapImcWriteNoSemicolon, 0xA31C)
        _ENTRY(sysTrapImcStringIsAscii, 0xA31D)

        _ENTRY(sysTrapTblGetItemFont, 0xA31E)
        _ENTRY(sysTrapTblSetItemFont, 0xA31F)

        _ENTRY(sysTrapFontSelect, 0xA320)
        _ENTRY(sysTrapFntDefineFont, 0xA321)

        _ENTRY(sysTrapCategoryEdit, 0xA322)

        _ENTRY(sysTrapSysGetOSVersionString, 0xA323)
        _ENTRY(sysTrapSysBatteryInfo, 0xA324)
        _ENTRY(sysTrapSysUIBusy, 0xA325)

        _ENTRY(sysTrapWinValidateHandle, 0xA326)
        _ENTRY(sysTrapFrmValidatePtr, 0xA327)
        _ENTRY(sysTrapCtlValidatePointer, 0xA328)
        _ENTRY(sysTrapWinMoveWindowAddr, 0xA329)
        _ENTRY(sysTrapFrmAddSpaceForObject, 0xA32A)
        _ENTRY(sysTrapFrmNewForm, 0xA32B)
        _ENTRY(sysTrapCtlNewControl, 0xA32C)
        _ENTRY(sysTrapFldNewField, 0xA32D)
        _ENTRY(sysTrapLstNewList, 0xA32E)
        _ENTRY(sysTrapFrmNewLabel, 0xA32F)
        _ENTRY(sysTrapFrmNewBitmap, 0xA330)
        _ENTRY(sysTrapFrmNewGadget, 0xA331)

        _ENTRY(sysTrapFileOpen, 0xA332)
        _ENTRY(sysTrapFileClose, 0xA333)
        _ENTRY(sysTrapFileDelete, 0xA334)
        _ENTRY(sysTrapFileReadLow, 0xA335)
        _ENTRY(sysTrapFileWrite, 0xA336)
        _ENTRY(sysTrapFileSeek, 0xA337)
        _ENTRY(sysTrapFileTell, 0xA338)
        _ENTRY(sysTrapFileTruncate, 0xA339)
        _ENTRY(sysTrapFileControl, 0xA33A)

        _ENTRY(sysTrapFrmActiveState, 0xA33B)

        _ENTRY(sysTrapSysGetAppInfo, 0xA33C)
        _ENTRY(sysTrapSysGetStackInfo, 0xA33D)

        _ENTRY(sysTrapWinScreenMode, 0xA33E)     /* was sysTrapScrDisplayMode */
        _ENTRY(sysTrapHwrLCDGetDepthV33, 0xA33F) /* This trap obsoleted for OS 3.5 and later */
        _ENTRY(sysTrapHwrGetROMToken, 0xA340)

        _ENTRY(sysTrapDbgControl, 0xA341)

        _ENTRY(sysTrapExgDBRead, 0xA342)
        _ENTRY(sysTrapExgDBWrite, 0xA343)

        _ENTRY(sysTrapHostControl,
               0xA344) /* Renamed from sysTrapSysGremlins, functionality generalized */
        _ENTRY(sysTrapFrmRemoveObject, 0xA345)

        _ENTRY(sysTrapSysReserved30Trap1,
               0xA346) /* "Reserved" trap in Palm OS 3.0 and later (was sysTrapSysReserved1) */

        // NOTE: The following two traps are reserved for future mgrs
        // that may or may not be present on any particular device.
        // They are NOT present by default; code must check first!
        _ENTRY(sysTrapExpansionDispatch,
               0xA347) /* Reserved for ExpansionMgr (was sysTrapSysReserved2) */
        _ENTRY(sysTrapFileSystemDispatch,
               0xA348) /* Reserved for FileSystemMgr (was sysTrapSysReserved3) */

        _ENTRY(sysTrapOEMDispatch, 0xA349) /* OEM trap in Palm OS 3.0 and later trap table (formerly
                                              sysTrapSysReserved4) */

        // Palm OS 3.1 traps					Palm IIIx and Palm V

        _ENTRY(sysTrapHwrLCDContrastV33, 0xA34A) /* This trap obsoleted for OS 3.5 and later */
        _ENTRY(sysTrapSysLCDContrast, 0xA34B)
        _ENTRY(sysTrapUIContrastAdjust, 0xA34C) /* Renamed from sysTrapContrastAdjust */
        _ENTRY(sysTrapHwrDockStatus, 0xA34D)

        _ENTRY(sysTrapFntWidthToOffset, 0xA34E)
        _ENTRY(sysTrapSelectOneTime, 0xA34F)
        _ENTRY(sysTrapWinDrawChar, 0xA350)
        _ENTRY(sysTrapWinDrawTruncChars, 0xA351)

        _ENTRY(sysTrapSysNotifyInit, 0xA352) /* Notification Manager traps */
        _ENTRY(sysTrapSysNotifyRegister, 0xA353)
        _ENTRY(sysTrapSysNotifyUnregister, 0xA354)
        _ENTRY(sysTrapSysNotifyBroadcast, 0xA355)
        _ENTRY(sysTrapSysNotifyBroadcastDeferred, 0xA356)
        _ENTRY(sysTrapSysNotifyDatabaseAdded, 0xA357)
        _ENTRY(sysTrapSysNotifyDatabaseRemoved, 0xA358)

        _ENTRY(sysTrapSysWantEvent, 0xA359)

        _ENTRY(sysTrapFtrPtrNew, 0xA35A)
        _ENTRY(sysTrapFtrPtrFree, 0xA35B)
        _ENTRY(sysTrapFtrPtrResize, 0xA35C)

        _ENTRY(sysTrapSysReserved31Trap1,
               0xA35D) /* "Reserved" trap in Palm OS 3.1 and later (was sysTrapSysReserved5) */

        // Palm OS 3.2 & 3.3 traps		Palm VII (3.2) and Fall '99 Palm OS Flash Update
        // (3.3)

        _ENTRY(sysTrapHwrNVPrefSet, 0xA35E) /* mapped to FlashParmsWrite */
        _ENTRY(sysTrapHwrNVPrefGet, 0xA35F) /* mapped to FlashParmsRead */
        _ENTRY(sysTrapFlashInit, 0xA360)
        _ENTRY(sysTrapFlashCompress, 0xA361)
        _ENTRY(sysTrapFlashErase, 0xA362)
        _ENTRY(sysTrapFlashProgram, 0xA363)

        _ENTRY(sysTrapAlmTimeChange, 0xA364)
        _ENTRY(sysTrapErrAlertCustom, 0xA365)
        _ENTRY(sysTrapPrgStartDialog, 0xA366) /* New version of sysTrapPrgStartDialogV31 */

        _ENTRY(sysTrapSerialDispatch, 0xA367)
        _ENTRY(sysTrapHwrBattery, 0xA368)
        _ENTRY(sysTrapDmGetDatabaseLockState, 0xA369)

        _ENTRY(sysTrapCncGetProfileList, 0xA36A)
        _ENTRY(sysTrapCncGetProfileInfo, 0xA36B)
        _ENTRY(sysTrapCncAddProfile, 0xA36C)
        _ENTRY(sysTrapCncDeleteProfile, 0xA36D)

        _ENTRY(sysTrapSndPlaySmfResource, 0xA36E)

        _ENTRY(sysTrapMemPtrDataStorage, 0xA36F) /* Never actually installed until now. */

        _ENTRY(sysTrapClipboardAppendItem, 0xA370)

        _ENTRY(sysTrapWiCmdV32, 0xA371) /* Code moved to INetLib; trap obsolete */

        // Palm OS 3.5 traps				Palm IIIc and other products

        // HAL Display-layer new traps
        _ENTRY(sysTrapHwrDisplayAttributes, 0xA372)
        _ENTRY(sysTrapHwrDisplayDoze, 0xA373)
        _ENTRY(sysTrapHwrDisplayPalette, 0xA374)

        // Screen driver new traps
        _ENTRY(sysTrapBltFindIndexes, 0xA375)
        _ENTRY(sysTrapBmpGetBits, 0xA376) /* was BltGetBitsAddr */
        _ENTRY(sysTrapBltCopyRectangle, 0xA377)
        _ENTRY(sysTrapBltDrawChars, 0xA378)
        _ENTRY(sysTrapBltLineRoutine, 0xA379)
        _ENTRY(sysTrapBltRectangleRoutine, 0xA37A)

        // ScrUtils new traps
        _ENTRY(sysTrapScrCompress, 0xA37B)
        _ENTRY(sysTrapScrDecompress, 0xA37C)

        // System Manager new traps
        _ENTRY(sysTrapSysLCDBrightness, 0xA37D)

        // WindowColor new traps
        _ENTRY(sysTrapWinPaintChar, 0xA37E)
        _ENTRY(sysTrapWinPaintChars, 0xA37F)
        _ENTRY(sysTrapWinPaintBitmap, 0xA380)
        _ENTRY(sysTrapWinGetPixel, 0xA381)
        _ENTRY(sysTrapWinPaintPixel, 0xA382)
        _ENTRY(sysTrapWinDrawPixel, 0xA383)
        _ENTRY(sysTrapWinErasePixel, 0xA384)
        _ENTRY(sysTrapWinInvertPixel, 0xA385)
        _ENTRY(sysTrapWinPaintPixels, 0xA386)
        _ENTRY(sysTrapWinPaintLines, 0xA387)
        _ENTRY(sysTrapWinPaintLine, 0xA388)
        _ENTRY(sysTrapWinPaintRectangle, 0xA389)
        _ENTRY(sysTrapWinPaintRectangleFrame, 0xA38A)
        _ENTRY(sysTrapWinPaintPolygon, 0xA38B)
        _ENTRY(sysTrapWinDrawPolygon, 0xA38C)
        _ENTRY(sysTrapWinErasePolygon, 0xA38D)
        _ENTRY(sysTrapWinInvertPolygon, 0xA38E)
        _ENTRY(sysTrapWinFillPolygon, 0xA38F)
        _ENTRY(sysTrapWinPaintArc, 0xA390)
        _ENTRY(sysTrapWinDrawArc, 0xA391)
        _ENTRY(sysTrapWinEraseArc, 0xA392)
        _ENTRY(sysTrapWinInvertArc, 0xA393)
        _ENTRY(sysTrapWinFillArc, 0xA394)
        _ENTRY(sysTrapWinPushDrawState, 0xA395)
        _ENTRY(sysTrapWinPopDrawState, 0xA396)
        _ENTRY(sysTrapWinSetDrawMode, 0xA397)
        _ENTRY(sysTrapWinSetForeColor, 0xA398)
        _ENTRY(sysTrapWinSetBackColor, 0xA399)
        _ENTRY(sysTrapWinSetTextColor, 0xA39A)
        _ENTRY(sysTrapWinGetPatternType, 0xA39B)
        _ENTRY(sysTrapWinSetPatternType, 0xA39C)
        _ENTRY(sysTrapWinPalette, 0xA39D)
        _ENTRY(sysTrapWinRGBToIndex, 0xA39E)
        _ENTRY(sysTrapWinIndexToRGB, 0xA39F)
        _ENTRY(sysTrapWinScreenLock, 0xA3A0)
        _ENTRY(sysTrapWinScreenUnlock, 0xA3A1)
        _ENTRY(sysTrapWinGetBitmap, 0xA3A2)

        // UIColor new traps
        _ENTRY(sysTrapUIColorInit, 0xA3A3)
        _ENTRY(sysTrapUIColorGetTableEntryIndex, 0xA3A4)
        _ENTRY(sysTrapUIColorGetTableEntryRGB, 0xA3A5)
        _ENTRY(sysTrapUIColorSetTableEntry, 0xA3A6)
        _ENTRY(sysTrapUIColorPushTable, 0xA3A7)
        _ENTRY(sysTrapUIColorPopTable, 0xA3A8)

        // misc cleanup and API additions

        _ENTRY(sysTrapCtlNewGraphicControl, 0xA3A9)

        _ENTRY(sysTrapTblGetItemPtr, 0xA3AA)

        _ENTRY(sysTrapUIBrightnessAdjust, 0xA3AB)
        _ENTRY(sysTrapUIPickColor, 0xA3AC)

        _ENTRY(sysTrapEvtSetAutoOffTimer, 0xA3AD)

        // Misc int'l/overlay support.
        _ENTRY(sysTrapTsmDispatch, 0xA3AE)
        _ENTRY(sysTrapOmDispatch, 0xA3AF)
        _ENTRY(sysTrapDmOpenDBNoOverlay, 0xA3B0)
        _ENTRY(sysTrapDmOpenDBWithLocale, 0xA3B1)
        _ENTRY(sysTrapResLoadConstant, 0xA3B2)

        // new boot-time SmallROM HAL additions
        _ENTRY(sysTrapHwrPreDebugInit, 0xA3B3)
        _ENTRY(sysTrapHwrResetNMI, 0xA3B4)
        _ENTRY(sysTrapHwrResetPWM, 0xA3B5)

        _ENTRY(sysTrapKeyBootKeys, 0xA3B6)

        _ENTRY(sysTrapDbgSerDrvOpen, 0xA3B7)
        _ENTRY(sysTrapDbgSerDrvClose, 0xA3B8)
        _ENTRY(sysTrapDbgSerDrvControl, 0xA3B9)
        _ENTRY(sysTrapDbgSerDrvStatus, 0xA3BA)
        _ENTRY(sysTrapDbgSerDrvWriteChar, 0xA3BB)
        _ENTRY(sysTrapDbgSerDrvReadChar, 0xA3BC)

        // new boot-time BigROM HAL additions
        _ENTRY(sysTrapHwrPostDebugInit, 0xA3BD)
        _ENTRY(sysTrapHwrIdentifyFeatures, 0xA3BE)
        _ENTRY(sysTrapHwrModelSpecificInit, 0xA3BF)
        _ENTRY(sysTrapHwrModelInitStage2, 0xA3C0)
        _ENTRY(sysTrapHwrInterruptsInit, 0xA3C1)

        _ENTRY(sysTrapHwrSoundOn, 0xA3C2)
        _ENTRY(sysTrapHwrSoundOff, 0xA3C3)

        // Kernel clock tick routine
        _ENTRY(sysTrapSysKernelClockTick, 0xA3C4)

        // MenuEraseMenu is exposed as of PalmOS 3.5, but there are
        // no public interfaces for it yet.	 Perhaps in a later release.
        _ENTRY(sysTrapMenuEraseMenu, 0xA3C5)

        _ENTRY(sysTrapSelectTime, 0xA3C6)

        // Menu Command Bar traps
        _ENTRY(sysTrapMenuCmdBarAddButton, 0xA3C7)
        _ENTRY(sysTrapMenuCmdBarGetButtonData, 0xA3C8)
        _ENTRY(sysTrapMenuCmdBarDisplay, 0xA3C9)

        // Silkscreen info
        _ENTRY(sysTrapHwrGetSilkscreenID, 0xA3CA)
        _ENTRY(sysTrapEvtGetSilkscreenAreaList, 0xA3CB)

        _ENTRY(sysTrapSysFatalAlertInit, 0xA3CC)
        _ENTRY(sysTrapDateTemplateToAscii, 0xA3CD)

        // New traps dealing with masking private records
        _ENTRY(sysTrapSecVerifyPW, 0xA3CE)
        _ENTRY(sysTrapSecSelectViewStatus, 0xA3CF)
        _ENTRY(sysTrapTblSetColumnMasked, 0xA3D0)
        _ENTRY(sysTrapTblSetRowMasked, 0xA3D1)
        _ENTRY(sysTrapTblRowMasked, 0xA3D2)

        // New form trap for dialogs with text entry field
        _ENTRY(sysTrapFrmCustomResponseAlert, 0xA3D3)
        _ENTRY(sysTrapFrmNewGsi, 0xA3D4)

        // New dynamic menu functions
        _ENTRY(sysTrapMenuShowItem, 0xA3D5)
        _ENTRY(sysTrapMenuHideItem, 0xA3D6)
        _ENTRY(sysTrapMenuAddItem, 0xA3D7)

        // New form traps for "smart gadgets"
        _ENTRY(sysTrapFrmSetGadgetHandler, 0xA3D8)

        // More new control functions
        _ENTRY(sysTrapCtlSetGraphics, 0xA3D9)
        _ENTRY(sysTrapCtlGetSliderValues, 0xA3DA)
        _ENTRY(sysTrapCtlSetSliderValues, 0xA3DB)
        _ENTRY(sysTrapCtlNewSliderControl, 0xA3DC)

        // Bitmap manager functions
        _ENTRY(sysTrapBmpCreate, 0xA3DD)
        _ENTRY(sysTrapBmpDelete, 0xA3DE)
        _ENTRY(sysTrapBmpCompress, 0xA3DF)
        // sysTrapBmpGetBits defined in Screen driver traps
        _ENTRY(sysTrapBmpGetColortable, 0xA3E0)
        _ENTRY(sysTrapBmpSize, 0xA3E1)
        _ENTRY(sysTrapBmpBitsSize, 0xA3E2)
        _ENTRY(sysTrapBmpColortableSize, 0xA3E3)
        // extra window namager
        _ENTRY(sysTrapWinCreateBitmapWindow, 0xA3E4)
        // Ask for a null event sooner (replaces a macro which Poser hated)
        _ENTRY(sysTrapEvtSetNullEventTick, 0xA3E5)

        // Exchange manager call to allow apps to select destination categories
        _ENTRY(sysTrapExgDoDialog, 0xA3E6)

        // this call will remove temporary UI like popup lists
        _ENTRY(sysTrapSysUICleanup, 0xA3E7)

        // The following 4 traps were "Reserved" traps, present only in SOME post-release builds of
        // Palm OS 3.5
        _ENTRY(sysTrapWinSetForeColorRGB, 0xA3E8)
        _ENTRY(sysTrapWinSetBackColorRGB, 0xA3E9)
        _ENTRY(sysTrapWinSetTextColorRGB, 0xA3EA)
        _ENTRY(sysTrapWinGetPixelRGB, 0xA3EB)

        // TRAPS ABOVE THIS POINT CAN NOT CHANGE BECAUSE THEY HAVE
        // BEEN RELEASED TO CUSTOMERS IN SHIPPING ROMS AND SDKS.
        // (MOVE THIS COMMENT DOWN WHENEVER THE "NEXT" RELEASE OCCURS.)

        // WARNING!!  The following are new traps for 4.0.  If this file is merged
        // with MAIN sources, new traps that are added for products that precede
        // 4.0 MUST insert their traps BEFORE this section.

        _ENTRY(sysTrapSysReserved40Trap1, 0xA3EC)
        _ENTRY(sysTrapSysReserved40Trap2, 0xA3ED)
        _ENTRY(sysTrapSysReserved40Trap3, 0xA3EE)
        _ENTRY(sysTrapSysReserved40Trap4, 0xA3EF)

        // DO NOT CHANGE TRAPS ABOVE THIS LINE
        // THESE TRAPS HAVE BEEN RELEASED IN THE 3.5 SDK
        // NEW TRAPS FOR PALM OS 4.0 CAN BE ADDED AFTER THIS
        // THE ORDER IS NOT IMPORTANT AND CAN BE CHANGED.

        // New Trap selector added for New Connection Mgr API
        _ENTRY(sysTrapCncMgrDispatch, 0xA3F0)

        // new trap for notify from interrupt, implemented in SysEvtMgr.c
        _ENTRY(sysTrapSysNotifyBroadcastFromInterrupt, 0xA3F1)

        // new trap for waking the UI without generating a null event
        _ENTRY(sysTrapEvtWakeupWithoutNilEvent, 0xA3F2)

        // new trap for doing stable, fast, 7-bit string compare
        _ENTRY(sysTrapStrCompareAscii, 0xA3F3)

        // New trap for accessors available thru PalmOS glue
        _ENTRY(sysTrapAccessorDispatch, 0xA3F4)

        _ENTRY(sysTrapBltGetPixel, 0xA3F5)
        _ENTRY(sysTrapBltPaintPixel, 0xA3F6)
        _ENTRY(sysTrapScrScreenInit, 0xA3F7)
        _ENTRY(sysTrapScrUpdateScreenBitmap, 0xA3F8)
        _ENTRY(sysTrapScrPalette, 0xA3F9)
        _ENTRY(sysTrapScrGetColortable, 0xA3FA)
        _ENTRY(sysTrapScrGetGrayPat, 0xA3FB)
        _ENTRY(sysTrapScrScreenLock, 0xA3FC)
        _ENTRY(sysTrapScrScreenUnlock, 0xA3FD)
        _ENTRY(sysTrapFntPrvGetFontList, 0xA3FE)

        // Exchange manager functions
        _ENTRY(sysTrapExgRegisterDatatype, 0xA3FF)
        _ENTRY(sysTrapExgNotifyReceive, 0xA400)
        _ENTRY(sysTrapExgNotifyGoto, 0xA401)
        _ENTRY(sysTrapExgRequest, 0xA402)
        _ENTRY(sysTrapExgSetDefaultApplication, 0xA403)
        _ENTRY(sysTrapExgGetDefaultApplication, 0xA404)
        _ENTRY(sysTrapExgGetTargetApplication, 0xA405)
        _ENTRY(sysTrapExgGetRegisteredApplications, 0xA406)
        _ENTRY(sysTrapExgGetRegisteredTypes, 0xA407)
        _ENTRY(sysTrapExgNotifyPreview, 0xA408)
        _ENTRY(sysTrapExgControl, 0xA409)

        // 04/30/00	CS - New Locale Manager handles access to region-specific info like date
        // formats
        _ENTRY(sysTrapLmDispatch, 0xA40A)

        // 05/10/00 kwk - New Memory Manager trap for retrieving ROM NVParam values (sys use only)
        _ENTRY(sysTrapMemGetRomNVParams, 0xA40B)

        // 05/12/00 kwk - Safe character width Font Mgr call
        _ENTRY(sysTrapFntWCharWidth, 0xA40C)

        // 05/17/00 kwk - Faster DmFindDatabase
        _ENTRY(sysTrapDmFindDatabaseWithTypeCreator, 0xA40D)

        // New Trap selectors added for time zone picker API
        _ENTRY(sysTrapSelectTimeZone, 0xA40E)
        _ENTRY(sysTrapTimeZoneToAscii, 0xA40F)

        // 08/18/00 kwk - trap for doing stable, fast, 7-bit string compare.
        // 08/21/00 kwk - moved here in place of sysTrapSelectDaylightSavingAdjustment.
        _ENTRY(sysTrapStrNCompareAscii, 0xA410)

        // New Trap selectors added for time zone conversion API
        _ENTRY(sysTrapTimTimeZoneToUTC, 0xA411)
        _ENTRY(sysTrapTimUTCToTimeZone, 0xA412)

        // New trap implemented in PhoneLookup.c
        _ENTRY(sysTrapPhoneNumberLookupCustom, 0xA413)

        // new trap for selecting debugger path.
        _ENTRY(sysTrapHwrDebugSelect, 0xA414)

        _ENTRY(sysTrapBltRoundedRectangle, 0xA415)
        _ENTRY(sysTrapBltRoundedRectangleFill, 0xA416)
        _ENTRY(sysTrapWinPrvInitCanvas, 0xA417)

        _ENTRY(sysTrapHwrCalcDynamicHeapSize, 0xA418)
        _ENTRY(sysTrapHwrDebuggerEnter, 0xA419)
        _ENTRY(sysTrapHwrDebuggerExit, 0xA41A)

        _ENTRY(sysTrapLstGetTopItem, 0xA41B)

        _ENTRY(sysTrapHwrModelInitStage3, 0xA41C)

        // 06/21/00 peter - New Attention Manager
        _ENTRY(sysTrapAttnIndicatorAllow, 0xA41D)
        _ENTRY(sysTrapAttnIndicatorAllowed, 0xA41E)
        _ENTRY(sysTrapAttnIndicatorEnable, 0xA41F)
        _ENTRY(sysTrapAttnIndicatorEnabled, 0xA420)
        _ENTRY(sysTrapAttnIndicatorSetBlinkPattern, 0xA421)
        _ENTRY(sysTrapAttnIndicatorGetBlinkPattern, 0xA422)
        _ENTRY(sysTrapAttnIndicatorTicksTillNextBlink, 0xA423)
        _ENTRY(sysTrapAttnIndicatorCheckBlink, 0xA424)
        _ENTRY(sysTrapAttnInitialize, 0xA425)
        _ENTRY(sysTrapAttnGetAttention, 0xA426)
        _ENTRY(sysTrapAttnUpdate, 0xA427)
        _ENTRY(sysTrapAttnForgetIt, 0xA428)
        _ENTRY(sysTrapAttnGetCounts, 0xA429)
        _ENTRY(sysTrapAttnListOpen, 0xA42A)
        _ENTRY(sysTrapAttnHandleEvent, 0xA42B)
        _ENTRY(sysTrapAttnEffectOfEvent, 0xA42C)
        _ENTRY(sysTrapAttnIterate, 0xA42D)
        _ENTRY(sysTrapAttnDoSpecialEffects, 0xA42E)
        _ENTRY(sysTrapAttnDoEmergencySpecialEffects, 0xA42F)
        _ENTRY(sysTrapAttnAllowClose, 0xA430)
        _ENTRY(sysTrapAttnReopen, 0xA431)
        _ENTRY(sysTrapAttnEnableNotification, 0xA432)
        _ENTRY(sysTrapHwrLEDAttributes, 0xA433)
        _ENTRY(sysTrapHwrVibrateAttributes, 0xA434)

        // Trap for getting and setting the device password hint.
        _ENTRY(sysTrapSecGetPwdHint, 0xA435)
        _ENTRY(sysTrapSecSetPwdHint, 0xA436)

        _ENTRY(sysTrapHwrFlashWrite, 0xA437)

        _ENTRY(sysTrapKeyboardStatusNew, 0xA438)
        _ENTRY(sysTrapKeyboardStatusFree, 0xA439)
        _ENTRY(sysTrapKbdSetLayout, 0xA43A)
        _ENTRY(sysTrapKbdGetLayout, 0xA43B)
        _ENTRY(sysTrapKbdSetPosition, 0xA43C)
        _ENTRY(sysTrapKbdGetPosition, 0xA43D)
        _ENTRY(sysTrapKbdSetShiftState, 0xA43E)
        _ENTRY(sysTrapKbdGetShiftState, 0xA43F)
        _ENTRY(sysTrapKbdDraw, 0xA440)
        _ENTRY(sysTrapKbdErase, 0xA441)
        _ENTRY(sysTrapKbdHandleEvent, 0xA442)

        _ENTRY(sysTrapOEMDispatch2, 0xA443)
        _ENTRY(sysTrapHwrCustom, 0xA444)

        // 08/28/00 kwk - Trap for getting form's active field.
        _ENTRY(sysTrapFrmGetActiveField, 0xA445)

        // 9/18/00 rkr - Added for playing sounds regardless of interruptible flag
        _ENTRY(sysTrapSndPlaySmfIrregardless, 0xA446)
        _ENTRY(sysTrapSndPlaySmfResourceIrregardless, 0xA447)
        _ENTRY(sysTrapSndInterruptSmfIrregardless, 0xA448)

        // 10/14/00 ABa: UDA manager
        _ENTRY(sysTrapUdaMgrDispatch, 0xA449)

        // WK: private traps for PalmOS
        _ENTRY(sysTrapPalmPrivate1, 0xA44A)
        _ENTRY(sysTrapPalmPrivate2, 0xA44B)
        _ENTRY(sysTrapPalmPrivate3, 0xA44C)
        _ENTRY(sysTrapPalmPrivate4, 0xA44D)

        // 11/07/00 tlw: Added accessors
        _ENTRY(sysTrapBmpGetDimensions, 0xA44E)
        _ENTRY(sysTrapBmpGetBitDepth, 0xA44F)
        _ENTRY(sysTrapBmpGetNextBitmap, 0xA450)
        _ENTRY(sysTrapTblGetNumberOfColumns, 0xA451)
        _ENTRY(sysTrapTblGetTopRow, 0xA452)
        _ENTRY(sysTrapTblSetSelection, 0xA453)
        _ENTRY(sysTrapFrmGetObjectIndexFromPtr, 0xA454)

        // 11/10/00 acs
        _ENTRY(sysTrapBmpGetSizes, 0xA455)
        _ENTRY(sysTrapWinGetBounds, 0xA456)

        _ENTRY(sysTrapBltPaintPixels, 0xA457)

        // 11/22/00 bob
        _ENTRY(sysTrapFldSetMaxVisibleLines, 0xA458)

        // 01/09/01 acs
        _ENTRY(sysTrapScrDefaultPaletteState, 0xA459)

        // WARNING!! LEAVE THIS AT THE END AND ALWAYS ADD NEW TRAPS TO
        // THE END OF THE TRAP TABLE BUT RIGHT BEFORE THIS TRAP, AND THEN
        // RENUMBER THIS ONE TO ONE MORE THAN THE ONE RIGHT BEFORE IT!!!!!!!!!

        _ENTRY(sysTrapLastTrapNumber, 0xA45A)

        default:
            return "unknown";

#undef _ENTTRY
    }
}

#endif  // _DECODE_SYSCALLS_H_
