// Taken from CoreTraps.h

#define syscall68kMemInit 0xA000
#define syscall68kMemInitHeapTable 0xA001
#define syscall68kMemStoreInit 0xA002
#define syscall68kMemCardFormat 0xA003
#define syscall68kMemCardInfo 0xA004
#define syscall68kMemStoreInfo 0xA005
#define syscall68kMemStoreSetInfo 0xA006
#define syscall68kMemNumHeaps 0xA007
#define syscall68kMemNumRAMHeaps 0xA008
#define syscall68kMemHeapID 0xA009
#define syscall68kMemHeapPtr 0xA00A
#define syscall68kMemHeapFreeBytes 0xA00B
#define syscall68kMemHeapSize 0xA00C
#define syscall68kMemHeapFlags 0xA00D
#define syscall68kMemHeapCompact 0xA00E
#define syscall68kMemHeapInit 0xA00F
#define syscall68kMemHeapFreeByOwnerID 0xA010
#define syscall68kMemChunkNew 0xA011
#define syscall68kMemChunkFree 0xA012
#define syscall68kMemPtrNew 0xA013
#define syscall68kMemPtrRecoverHandle 0xA014
#define syscall68kMemPtrFlags 0xA015
#define syscall68kMemPtrSize 0xA016
#define syscall68kMemPtrOwner 0xA017
#define syscall68kMemPtrHeapID 0xA018
#define syscall68kMemPtrCardNo 0xA019
#define syscall68kMemPtrToLocalID 0xA01A
#define syscall68kMemPtrSetOwner 0xA01B
#define syscall68kMemPtrResize 0xA01C
#define syscall68kMemPtrResetLock 0xA01D
#define syscall68kMemHandleNew 0xA01E
#define syscall68kMemHandleLockCount 0xA01F
#define syscall68kMemHandleToLocalID 0xA020
#define syscall68kMemHandleLock 0xA021
#define syscall68kMemHandleUnlock 0xA022
#define syscall68kMemLocalIDToGlobal 0xA023
#define syscall68kMemLocalIDKind 0xA024
#define syscall68kMemLocalIDToPtr 0xA025
#define syscall68kMemMove 0xA026
#define syscall68kMemSet 0xA027
#define syscall68kMemStoreSearch 0xA028
#define syscall68kSysReserved10Trap1 \
    0xA029 /* "Reserved" trap in Palm OS 1.0 and later (was syscall68kMemPtrDataStorage) */

#define syscall68kMemKernelInit 0xA02A
#define syscall68kMemHandleFree 0xA02B
#define syscall68kMemHandleFlags 0xA02C
#define syscall68kMemHandleSize 0xA02D
#define syscall68kMemHandleOwner 0xA02E
#define syscall68kMemHandleHeapID 0xA02F
#define syscall68kMemHandleDataStorage 0xA030
#define syscall68kMemHandleCardNo 0xA031
#define syscall68kMemHandleSetOwner 0xA032
#define syscall68kMemHandleResize 0xA033
#define syscall68kMemHandleResetLock 0xA034
#define syscall68kMemPtrUnlock 0xA035
#define syscall68kMemLocalIDToLockedPtr 0xA036
#define syscall68kMemSetDebugMode 0xA037
#define syscall68kMemHeapScramble 0xA038
#define syscall68kMemHeapCheck 0xA039
#define syscall68kMemNumCards 0xA03A
#define syscall68kMemDebugMode 0xA03B
#define syscall68kMemSemaphoreReserve 0xA03C
#define syscall68kMemSemaphoreRelease 0xA03D
#define syscall68kMemHeapDynamic 0xA03E
#define syscall68kMemNVParams 0xA03F

#define syscall68kDmInit 0xA040
#define syscall68kDmCreateDatabase 0xA041
#define syscall68kDmDeleteDatabase 0xA042
#define syscall68kDmNumDatabases 0xA043
#define syscall68kDmGetDatabase 0xA044
#define syscall68kDmFindDatabase 0xA045
#define syscall68kDmDatabaseInfo 0xA046
#define syscall68kDmSetDatabaseInfo 0xA047
#define syscall68kDmDatabaseSize 0xA048
#define syscall68kDmOpenDatabase 0xA049
#define syscall68kDmCloseDatabase 0xA04A
#define syscall68kDmNextOpenDatabase 0xA04B
#define syscall68kDmOpenDatabaseInfo 0xA04C
#define syscall68kDmResetRecordStates 0xA04D
#define syscall68kDmGetLastErr 0xA04E
#define syscall68kDmNumRecords 0xA04F
#define syscall68kDmRecordInfo 0xA050
#define syscall68kDmSetRecordInfo 0xA051
#define syscall68kDmAttachRecord 0xA052
#define syscall68kDmDetachRecord 0xA053
#define syscall68kDmMoveRecord 0xA054
#define syscall68kDmNewRecord 0xA055
#define syscall68kDmRemoveRecord 0xA056
#define syscall68kDmDeleteRecord 0xA057
#define syscall68kDmArchiveRecord 0xA058
#define syscall68kDmNewHandle 0xA059
#define syscall68kDmRemoveSecretRecords 0xA05A
#define syscall68kDmQueryRecord 0xA05B
#define syscall68kDmGetRecord 0xA05C
#define syscall68kDmResizeRecord 0xA05D
#define syscall68kDmReleaseRecord 0xA05E
#define syscall68kDmGetResource 0xA05F
#define syscall68kDmGet1Resource 0xA060
#define syscall68kDmReleaseResource 0xA061
#define syscall68kDmResizeResource 0xA062
#define syscall68kDmNextOpenResDatabase 0xA063
#define syscall68kDmFindResourceType 0xA064
#define syscall68kDmFindResource 0xA065
#define syscall68kDmSearchResource 0xA066
#define syscall68kDmNumResources 0xA067
#define syscall68kDmResourceInfo 0xA068
#define syscall68kDmSetResourceInfo 0xA069
#define syscall68kDmAttachResource 0xA06A
#define syscall68kDmDetachResource 0xA06B
#define syscall68kDmNewResource 0xA06C
#define syscall68kDmRemoveResource 0xA06D
#define syscall68kDmGetResourceIndex 0xA06E
#define syscall68kDmQuickSort 0xA06F
#define syscall68kDmQueryNextInCategory 0xA070
#define syscall68kDmNumRecordsInCategory 0xA071
#define syscall68kDmPositionInCategory 0xA072
#define syscall68kDmSeekRecordInCategory 0xA073
#define syscall68kDmMoveCategory 0xA074
#define syscall68kDmOpenDatabaseByTypeCreator 0xA075
#define syscall68kDmWrite 0xA076
#define syscall68kDmStrCopy 0xA077
#define syscall68kDmGetNextDatabaseByTypeCreator 0xA078
#define syscall68kDmWriteCheck 0xA079
#define syscall68kDmMoveOpenDBContext 0xA07A
#define syscall68kDmFindRecordByID 0xA07B
#define syscall68kDmGetAppInfoID 0xA07C
#define syscall68kDmFindSortPositionV10 0xA07D
#define syscall68kDmSet 0xA07E
#define syscall68kDmCreateDatabaseFromImage 0xA07F

#define syscall68kDbgSrcMessage 0xA080
#define syscall68kDbgMessage 0xA081
#define syscall68kDbgGetMessage 0xA082
#define syscall68kDbgCommSettings 0xA083

#define syscall68kErrDisplayFileLineMsg 0xA084
#define syscall68kErrSetJump 0xA085
#define syscall68kErrLongJump 0xA086
#define syscall68kErrThrow 0xA087
#define syscall68kErrExceptionList 0xA088

#define syscall68kSysBroadcastActionCode 0xA089
#define syscall68kSysUnimplemented 0xA08A
#define syscall68kSysColdBoot 0xA08B
#define syscall68kSysReset 0xA08C
#define syscall68kSysDoze 0xA08D
#define syscall68kSysAppLaunch 0xA08E
#define syscall68kSysAppStartup 0xA08F
#define syscall68kSysAppExit 0xA090
#define syscall68kSysSetA5 0xA091
#define syscall68kSysSetTrapAddress 0xA092
#define syscall68kSysGetTrapAddress 0xA093
#define syscall68kSysTranslateKernelErr 0xA094
#define syscall68kSysSemaphoreCreate 0xA095
#define syscall68kSysSemaphoreDelete 0xA096
#define syscall68kSysSemaphoreWait 0xA097
#define syscall68kSysSemaphoreSignal 0xA098
#define syscall68kSysTimerCreate 0xA099
#define syscall68kSysTimerWrite 0xA09A
#define syscall68kSysTaskCreate 0xA09B
#define syscall68kSysTaskDelete 0xA09C
#define syscall68kSysTaskTrigger 0xA09D
#define syscall68kSysTaskID 0xA09E
#define syscall68kSysTaskUserInfoPtr 0xA09F
#define syscall68kSysTaskDelay 0xA0A0
#define syscall68kSysTaskSetTermProc 0xA0A1
#define syscall68kSysUILaunch 0xA0A2
#define syscall68kSysNewOwnerID 0xA0A3
#define syscall68kSysSemaphoreSet 0xA0A4
#define syscall68kSysDisableInts 0xA0A5
#define syscall68kSysRestoreStatus 0xA0A6
#define syscall68kSysUIAppSwitch 0xA0A7
#define syscall68kSysCurAppInfoPV20 0xA0A8
#define syscall68kSysHandleEvent 0xA0A9
#define syscall68kSysInit 0xA0AA
#define syscall68kSysQSort 0xA0AB
#define syscall68kSysCurAppDatabase 0xA0AC
#define syscall68kSysFatalAlert 0xA0AD
#define syscall68kSysResSemaphoreCreate 0xA0AE
#define syscall68kSysResSemaphoreDelete 0xA0AF
#define syscall68kSysResSemaphoreReserve 0xA0B0
#define syscall68kSysResSemaphoreRelease 0xA0B1
#define syscall68kSysSleep 0xA0B2
#define syscall68kSysKeyboardDialogV10 0xA0B3
#define syscall68kSysAppLauncherDialog 0xA0B4
#define syscall68kSysSetPerformance 0xA0B5
#define syscall68kSysBatteryInfoV20 0xA0B6
#define syscall68kSysLibInstall 0xA0B7
#define syscall68kSysLibRemove 0xA0B8
#define syscall68kSysLibTblEntry 0xA0B9
#define syscall68kSysLibFind 0xA0BA
#define syscall68kSysBatteryDialog 0xA0BB
#define syscall68kSysCopyStringResource 0xA0BC
#define syscall68kSysKernelInfo 0xA0BD
#define syscall68kSysLaunchConsole 0xA0BE
#define syscall68kSysTimerDelete 0xA0BF
#define syscall68kSysSetAutoOffTime 0xA0C0
#define syscall68kSysFormPointerArrayToStrings 0xA0C1
#define syscall68kSysRandom 0xA0C2
#define syscall68kSysTaskSwitching 0xA0C3
#define syscall68kSysTimerRead 0xA0C4

#define syscall68kStrCopy 0xA0C5
#define syscall68kStrCat 0xA0C6
#define syscall68kStrLen 0xA0C7
#define syscall68kStrCompare 0xA0C8
#define syscall68kStrIToA 0xA0C9
#define syscall68kStrCaselessCompare 0xA0CA
#define syscall68kStrIToH 0xA0CB
#define syscall68kStrChr 0xA0CC
#define syscall68kStrStr 0xA0CD
#define syscall68kStrAToI 0xA0CE
#define syscall68kStrToLower 0xA0CF

#define syscall68kSerReceiveISP 0xA0D0

#define syscall68kSlkOpen 0xA0D1
#define syscall68kSlkClose 0xA0D2
#define syscall68kSlkOpenSocket 0xA0D3
#define syscall68kSlkCloseSocket 0xA0D4
#define syscall68kSlkSocketRefNum 0xA0D5
#define syscall68kSlkSocketSetTimeout 0xA0D6
#define syscall68kSlkFlushSocket 0xA0D7
#define syscall68kSlkSetSocketListener 0xA0D8
#define syscall68kSlkSendPacket 0xA0D9
#define syscall68kSlkReceivePacket 0xA0DA
#define syscall68kSlkSysPktDefaultResponse 0xA0DB
#define syscall68kSlkProcessRPC 0xA0DC

#define syscall68kConPutS 0xA0DD
#define syscall68kConGetS 0xA0DE

#define syscall68kFplInit 0xA0DF         /* Obsolete, here for compatibilty only! */
#define syscall68kFplFree 0xA0E0         /* Obsolete, here for compatibilty only! */
#define syscall68kFplFToA 0xA0E1         /* Obsolete, here for compatibilty only! */
#define syscall68kFplAToF 0xA0E2         /* Obsolete, here for compatibilty only! */
#define syscall68kFplBase10Info 0xA0E3   /* Obsolete, here for compatibilty only! */
#define syscall68kFplLongToFloat 0xA0E4  /* Obsolete, here for compatibilty only! */
#define syscall68kFplFloatToLong 0xA0E5  /* Obsolete, here for compatibilty only! */
#define syscall68kFplFloatToULong 0xA0E6 /* Obsolete, here for compatibilty only! */
#define syscall68kFplMul 0xA0E7          /* Obsolete, here for compatibilty only! */
#define syscall68kFplAdd 0xA0E8          /* Obsolete, here for compatibilty only! */
#define syscall68kFplSub 0xA0E9          /* Obsolete, here for compatibilty only! */
#define syscall68kFplDiv 0xA0EA          /* Obsolete, here for compatibilty only! */

#define syscall68kWinScreenInit 0xA0EB /* was syscall68kScrInit */
#define syscall68kScrCopyRectangle 0xA0EC
#define syscall68kScrDrawChars 0xA0ED
#define syscall68kScrLineRoutine 0xA0EE
#define syscall68kScrRectangleRoutine 0xA0EF
#define syscall68kScrScreenInfo 0xA0F0
#define syscall68kScrDrawNotify 0xA0F1
#define syscall68kScrSendUpdateArea 0xA0F2
#define syscall68kScrCompressScanLine 0xA0F3
#define syscall68kScrDeCompressScanLine 0xA0F4

#define syscall68kTimGetSeconds 0xA0F5
#define syscall68kTimSetSeconds 0xA0F6
#define syscall68kTimGetTicks 0xA0F7
#define syscall68kTimInit 0xA0F8
#define syscall68kTimSetAlarm 0xA0F9
#define syscall68kTimGetAlarm 0xA0FA
#define syscall68kTimHandleInterrupt 0xA0FB
#define syscall68kTimSecondsToDateTime 0xA0FC
#define syscall68kTimDateTimeToSeconds 0xA0FD
#define syscall68kTimAdjust 0xA0FE
#define syscall68kTimSleep 0xA0FF
#define syscall68kTimWake 0xA100

#define syscall68kCategoryCreateListV10 0xA101
#define syscall68kCategoryFreeListV10 0xA102
#define syscall68kCategoryFind 0xA103
#define syscall68kCategoryGetName 0xA104
#define syscall68kCategoryEditV10 0xA105
#define syscall68kCategorySelectV10 0xA106
#define syscall68kCategoryGetNext 0xA107
#define syscall68kCategorySetTriggerLabel 0xA108
#define syscall68kCategoryTruncateName 0xA109

#define syscall68kClipboardAddItem 0xA10A
#define syscall68kClipboardCheckIfItemExist 0xA10B
#define syscall68kClipboardGetItem 0xA10C

#define syscall68kCtlDrawControl 0xA10D
#define syscall68kCtlEraseControl 0xA10E
#define syscall68kCtlHideControl 0xA10F
#define syscall68kCtlShowControl 0xA110
#define syscall68kCtlGetValue 0xA111
#define syscall68kCtlSetValue 0xA112
#define syscall68kCtlGetLabel 0xA113
#define syscall68kCtlSetLabel 0xA114
#define syscall68kCtlHandleEvent 0xA115
#define syscall68kCtlHitControl 0xA116
#define syscall68kCtlSetEnabled 0xA117
#define syscall68kCtlSetUsable 0xA118
#define syscall68kCtlEnabled 0xA119

#define syscall68kEvtInitialize 0xA11A
#define syscall68kEvtAddEventToQueue 0xA11B
#define syscall68kEvtCopyEvent 0xA11C
#define syscall68kEvtGetEvent 0xA11D
#define syscall68kEvtGetPen 0xA11E
#define syscall68kEvtSysInit 0xA11F
#define syscall68kEvtGetSysEvent 0xA120
#define syscall68kEvtProcessSoftKeyStroke 0xA121
#define syscall68kEvtGetPenBtnList 0xA122
#define syscall68kEvtSetPenQueuePtr 0xA123
#define syscall68kEvtPenQueueSize 0xA124
#define syscall68kEvtFlushPenQueue 0xA125
#define syscall68kEvtEnqueuePenPoint 0xA126
#define syscall68kEvtDequeuePenStrokeInfo 0xA127
#define syscall68kEvtDequeuePenPoint 0xA128
#define syscall68kEvtFlushNextPenStroke 0xA129
#define syscall68kEvtSetKeyQueuePtr 0xA12A
#define syscall68kEvtKeyQueueSize 0xA12B
#define syscall68kEvtFlushKeyQueue 0xA12C
#define syscall68kEvtEnqueueKey 0xA12D
#define syscall68kEvtDequeueKeyEvent 0xA12E
#define syscall68kEvtWakeup 0xA12F
#define syscall68kEvtResetAutoOffTimer 0xA130
#define syscall68kEvtKeyQueueEmpty 0xA131
#define syscall68kEvtEnableGraffiti 0xA132

#define syscall68kFldCopy 0xA133
#define syscall68kFldCut 0xA134
#define syscall68kFldDrawField 0xA135
#define syscall68kFldEraseField 0xA136
#define syscall68kFldFreeMemory 0xA137
#define syscall68kFldGetBounds 0xA138
#define syscall68kFldGetTextPtr 0xA139
#define syscall68kFldGetSelection 0xA13A
#define syscall68kFldHandleEvent 0xA13B
#define syscall68kFldPaste 0xA13C
#define syscall68kFldRecalculateField 0xA13D
#define syscall68kFldSetBounds 0xA13E
#define syscall68kFldSetText 0xA13F
#define syscall68kFldGetFont 0xA140
#define syscall68kFldSetFont 0xA141
#define syscall68kFldSetSelection 0xA142
#define syscall68kFldGrabFocus 0xA143
#define syscall68kFldReleaseFocus 0xA144
#define syscall68kFldGetInsPtPosition 0xA145
#define syscall68kFldSetInsPtPosition 0xA146
#define syscall68kFldSetScrollPosition 0xA147
#define syscall68kFldGetScrollPosition 0xA148
#define syscall68kFldGetTextHeight 0xA149
#define syscall68kFldGetTextAllocatedSize 0xA14A
#define syscall68kFldGetTextLength 0xA14B
#define syscall68kFldScrollField 0xA14C
#define syscall68kFldScrollable 0xA14D
#define syscall68kFldGetVisibleLines 0xA14E
#define syscall68kFldGetAttributes 0xA14F
#define syscall68kFldSetAttributes 0xA150
#define syscall68kFldSendChangeNotification 0xA151
#define syscall68kFldCalcFieldHeight 0xA152
#define syscall68kFldGetTextHandle 0xA153
#define syscall68kFldCompactText 0xA154
#define syscall68kFldDirty 0xA155
#define syscall68kFldWordWrap 0xA156
#define syscall68kFldSetTextAllocatedSize 0xA157
#define syscall68kFldSetTextHandle 0xA158
#define syscall68kFldSetTextPtr 0xA159
#define syscall68kFldGetMaxChars 0xA15A
#define syscall68kFldSetMaxChars 0xA15B
#define syscall68kFldSetUsable 0xA15C
#define syscall68kFldInsert 0xA15D
#define syscall68kFldDelete 0xA15E
#define syscall68kFldUndo 0xA15F
#define syscall68kFldSetDirty 0xA160
#define syscall68kFldSendHeightChangeNotification 0xA161
#define syscall68kFldMakeFullyVisible 0xA162

#define syscall68kFntGetFont 0xA163
#define syscall68kFntSetFont 0xA164
#define syscall68kFntGetFontPtr 0xA165
#define syscall68kFntBaseLine 0xA166
#define syscall68kFntCharHeight 0xA167
#define syscall68kFntLineHeight 0xA168
#define syscall68kFntAverageCharWidth 0xA169
#define syscall68kFntCharWidth 0xA16A
#define syscall68kFntCharsWidth 0xA16B
#define syscall68kFntDescenderHeight 0xA16C
#define syscall68kFntCharsInWidth 0xA16D
#define syscall68kFntLineWidth 0xA16E

#define syscall68kFrmInitForm 0xA16F
#define syscall68kFrmDeleteForm 0xA170
#define syscall68kFrmDrawForm 0xA171
#define syscall68kFrmEraseForm 0xA172
#define syscall68kFrmGetActiveForm 0xA173
#define syscall68kFrmSetActiveForm 0xA174
#define syscall68kFrmGetActiveFormID 0xA175
#define syscall68kFrmGetUserModifiedState 0xA176
#define syscall68kFrmSetNotUserModified 0xA177
#define syscall68kFrmGetFocus 0xA178
#define syscall68kFrmSetFocus 0xA179
#define syscall68kFrmHandleEvent 0xA17A
#define syscall68kFrmGetFormBounds 0xA17B
#define syscall68kFrmGetWindowHandle 0xA17C
#define syscall68kFrmGetFormId 0xA17D
#define syscall68kFrmGetFormPtr 0xA17E
#define syscall68kFrmGetNumberOfObjects 0xA17F
#define syscall68kFrmGetObjectIndex 0xA180
#define syscall68kFrmGetObjectId 0xA181
#define syscall68kFrmGetObjectType 0xA182
#define syscall68kFrmGetObjectPtr 0xA183
#define syscall68kFrmHideObject 0xA184
#define syscall68kFrmShowObject 0xA185
#define syscall68kFrmGetObjectPosition 0xA186
#define syscall68kFrmSetObjectPosition 0xA187
#define syscall68kFrmGetControlValue 0xA188
#define syscall68kFrmSetControlValue 0xA189
#define syscall68kFrmGetControlGroupSelection 0xA18A
#define syscall68kFrmSetControlGroupSelection 0xA18B
#define syscall68kFrmCopyLabel 0xA18C
#define syscall68kFrmSetLabel 0xA18D
#define syscall68kFrmGetLabel 0xA18E
#define syscall68kFrmSetCategoryLabel 0xA18F
#define syscall68kFrmGetTitle 0xA190
#define syscall68kFrmSetTitle 0xA191
#define syscall68kFrmAlert 0xA192
#define syscall68kFrmDoDialog 0xA193
#define syscall68kFrmCustomAlert 0xA194
#define syscall68kFrmHelp 0xA195
#define syscall68kFrmUpdateScrollers 0xA196
#define syscall68kFrmGetFirstForm 0xA197
#define syscall68kFrmVisible 0xA198
#define syscall68kFrmGetObjectBounds 0xA199
#define syscall68kFrmCopyTitle 0xA19A
#define syscall68kFrmGotoForm 0xA19B
#define syscall68kFrmPopupForm 0xA19C
#define syscall68kFrmUpdateForm 0xA19D
#define syscall68kFrmReturnToForm 0xA19E
#define syscall68kFrmSetEventHandler 0xA19F
#define syscall68kFrmDispatchEvent 0xA1A0
#define syscall68kFrmCloseAllForms 0xA1A1
#define syscall68kFrmSaveAllForms 0xA1A2
#define syscall68kFrmGetGadgetData 0xA1A3
#define syscall68kFrmSetGadgetData 0xA1A4
#define syscall68kFrmSetCategoryTrigger 0xA1A5

#define syscall68kUIInitialize 0xA1A6
#define syscall68kUIReset 0xA1A7

#define syscall68kInsPtInitialize 0xA1A8
#define syscall68kInsPtSetLocation 0xA1A9
#define syscall68kInsPtGetLocation 0xA1AA
#define syscall68kInsPtEnable 0xA1AB
#define syscall68kInsPtEnabled 0xA1AC
#define syscall68kInsPtSetHeight 0xA1AD
#define syscall68kInsPtGetHeight 0xA1AE
#define syscall68kInsPtCheckBlink 0xA1AF

#define syscall68kLstSetDrawFunction 0xA1B0
#define syscall68kLstDrawList 0xA1B1
#define syscall68kLstEraseList 0xA1B2
#define syscall68kLstGetSelection 0xA1B3
#define syscall68kLstGetSelectionText 0xA1B4
#define syscall68kLstHandleEvent 0xA1B5
#define syscall68kLstSetHeight 0xA1B6
#define syscall68kLstSetSelection 0xA1B7
#define syscall68kLstSetListChoices 0xA1B8
#define syscall68kLstMakeItemVisible 0xA1B9
#define syscall68kLstGetNumberOfItems 0xA1BA
#define syscall68kLstPopupList 0xA1BB
#define syscall68kLstSetPosition 0xA1BC

#define syscall68kMenuInit 0xA1BD
#define syscall68kMenuDispose 0xA1BE
#define syscall68kMenuHandleEvent 0xA1BF
#define syscall68kMenuDrawMenu 0xA1C0
#define syscall68kMenuEraseStatus 0xA1C1
#define syscall68kMenuGetActiveMenu 0xA1C2
#define syscall68kMenuSetActiveMenu 0xA1C3

#define syscall68kRctSetRectangle 0xA1C4
#define syscall68kRctCopyRectangle 0xA1C5
#define syscall68kRctInsetRectangle 0xA1C6
#define syscall68kRctOffsetRectangle 0xA1C7
#define syscall68kRctPtInRectangle 0xA1C8
#define syscall68kRctGetIntersection 0xA1C9

#define syscall68kTblDrawTable 0xA1CA
#define syscall68kTblEraseTable 0xA1CB
#define syscall68kTblHandleEvent 0xA1CC
#define syscall68kTblGetItemBounds 0xA1CD
#define syscall68kTblSelectItem 0xA1CE
#define syscall68kTblGetItemInt 0xA1CF
#define syscall68kTblSetItemInt 0xA1D0
#define syscall68kTblSetItemStyle 0xA1D1
#define syscall68kTblUnhighlightSelection 0xA1D2
#define syscall68kTblSetRowUsable 0xA1D3
#define syscall68kTblGetNumberOfRows 0xA1D4
#define syscall68kTblSetCustomDrawProcedure 0xA1D5
#define syscall68kTblSetRowSelectable 0xA1D6
#define syscall68kTblRowSelectable 0xA1D7
#define syscall68kTblSetLoadDataProcedure 0xA1D8
#define syscall68kTblSetSaveDataProcedure 0xA1D9
#define syscall68kTblGetBounds 0xA1DA
#define syscall68kTblSetRowHeight 0xA1DB
#define syscall68kTblGetColumnWidth 0xA1DC
#define syscall68kTblGetRowID 0xA1DD
#define syscall68kTblSetRowID 0xA1DE
#define syscall68kTblMarkRowInvalid 0xA1DF
#define syscall68kTblMarkTableInvalid 0xA1E0
#define syscall68kTblGetSelection 0xA1E1
#define syscall68kTblInsertRow 0xA1E2
#define syscall68kTblRemoveRow 0xA1E3
#define syscall68kTblRowInvalid 0xA1E4
#define syscall68kTblRedrawTable 0xA1E5
#define syscall68kTblRowUsable 0xA1E6
#define syscall68kTblReleaseFocus 0xA1E7
#define syscall68kTblEditing 0xA1E8
#define syscall68kTblGetCurrentField 0xA1E9
#define syscall68kTblSetColumnUsable 0xA1EA
#define syscall68kTblGetRowHeight 0xA1EB
#define syscall68kTblSetColumnWidth 0xA1EC
#define syscall68kTblGrabFocus 0xA1ED
#define syscall68kTblSetItemPtr 0xA1EE
#define syscall68kTblFindRowID 0xA1EF
#define syscall68kTblGetLastUsableRow 0xA1F0
#define syscall68kTblGetColumnSpacing 0xA1F1
#define syscall68kTblFindRowData 0xA1F2
#define syscall68kTblGetRowData 0xA1F3
#define syscall68kTblSetRowData 0xA1F4
#define syscall68kTblSetColumnSpacing 0xA1F5

#define syscall68kWinCreateWindow 0xA1F6
#define syscall68kWinCreateOffscreenWindow 0xA1F7
#define syscall68kWinDeleteWindow 0xA1F8
#define syscall68kWinInitializeWindow 0xA1F9
#define syscall68kWinAddWindow 0xA1FA
#define syscall68kWinRemoveWindow 0xA1FB
#define syscall68kWinSetActiveWindow 0xA1FC
#define syscall68kWinSetDrawWindow 0xA1FD
#define syscall68kWinGetDrawWindow 0xA1FE
#define syscall68kWinGetActiveWindow 0xA1FF
#define syscall68kWinGetDisplayWindow 0xA200
#define syscall68kWinGetFirstWindow 0xA201
#define syscall68kWinEnableWindow 0xA202
#define syscall68kWinDisableWindow 0xA203
#define syscall68kWinGetWindowFrameRect 0xA204
#define syscall68kWinDrawWindowFrame 0xA205
#define syscall68kWinEraseWindow 0xA206
#define syscall68kWinSaveBits 0xA207
#define syscall68kWinRestoreBits 0xA208
#define syscall68kWinCopyRectangle 0xA209
#define syscall68kWinScrollRectangle 0xA20A
#define syscall68kWinGetDisplayExtent 0xA20B
#define syscall68kWinGetWindowExtent 0xA20C
#define syscall68kWinDisplayToWindowPt 0xA20D
#define syscall68kWinWindowToDisplayPt 0xA20E
#define syscall68kWinGetClip 0xA20F
#define syscall68kWinSetClip 0xA210
#define syscall68kWinResetClip 0xA211
#define syscall68kWinClipRectangle 0xA212
#define syscall68kWinDrawLine 0xA213
#define syscall68kWinDrawGrayLine 0xA214
#define syscall68kWinEraseLine 0xA215
#define syscall68kWinInvertLine 0xA216
#define syscall68kWinFillLine 0xA217
#define syscall68kWinDrawRectangle 0xA218
#define syscall68kWinEraseRectangle 0xA219
#define syscall68kWinInvertRectangle 0xA21A
#define syscall68kWinDrawRectangleFrame 0xA21B
#define syscall68kWinDrawGrayRectangleFrame 0xA21C
#define syscall68kWinEraseRectangleFrame 0xA21D
#define syscall68kWinInvertRectangleFrame 0xA21E
#define syscall68kWinGetFramesRectangle 0xA21F
#define syscall68kWinDrawChars 0xA220
#define syscall68kWinEraseChars 0xA221
#define syscall68kWinInvertChars 0xA222
#define syscall68kWinGetPattern 0xA223
#define syscall68kWinSetPattern 0xA224
#define syscall68kWinSetUnderlineMode 0xA225
#define syscall68kWinDrawBitmap 0xA226
#define syscall68kWinModal 0xA227
#define syscall68kWinGetDrawWindowBounds 0xA228
#define syscall68kWinFillRectangle 0xA229
#define syscall68kWinDrawInvertedChars 0xA22A

#define syscall68kPrefOpenPreferenceDBV10 0xA22B
#define syscall68kPrefGetPreferences 0xA22C
#define syscall68kPrefSetPreferences 0xA22D
#define syscall68kPrefGetAppPreferencesV10 0xA22E
#define syscall68kPrefSetAppPreferencesV10 0xA22F

#define syscall68kSndInit 0xA230
#define syscall68kSndSetDefaultVolume 0xA231
#define syscall68kSndGetDefaultVolume 0xA232
#define syscall68kSndDoCmd 0xA233
#define syscall68kSndPlaySystemSound 0xA234

#define syscall68kAlmInit 0xA235
#define syscall68kAlmCancelAll 0xA236
#define syscall68kAlmAlarmCallback 0xA237
#define syscall68kAlmSetAlarm 0xA238
#define syscall68kAlmGetAlarm 0xA239
#define syscall68kAlmDisplayAlarm 0xA23A
#define syscall68kAlmEnableNotification 0xA23B

#define syscall68kHwrGetRAMMapping 0xA23C
#define syscall68kHwrMemWritable 0xA23D
#define syscall68kHwrMemReadable 0xA23E
#define syscall68kHwrDoze 0xA23F
#define syscall68kHwrSleep 0xA240
#define syscall68kHwrWake 0xA241
#define syscall68kHwrSetSystemClock 0xA242
#define syscall68kHwrSetCPUDutyCycle 0xA243
#define syscall68kHwrDisplayInit 0xA244 /* Before OS 3.5, this trap a.k.a. syscall68kHwrLCDInit */
#define syscall68kHwrDisplaySleep \
    0xA245 /* Before OS 3.5, this trap a.k.a. syscall68kHwrLCDSleep, */
#define syscall68kHwrTimerInit 0xA246
#define syscall68kHwrCursorV33 0xA247 /* This trap obsoleted for OS 3.5 and later */
#define syscall68kHwrBatteryLevel 0xA248
#define syscall68kHwrDelay 0xA249
#define syscall68kHwrEnableDataWrites 0xA24A
#define syscall68kHwrDisableDataWrites 0xA24B
#define syscall68kHwrLCDBaseAddrV33 0xA24C /* This trap obsoleted for OS 3.5 and later */
#define syscall68kHwrDisplayDrawBootScreen \
    0xA24D /* Before OS 3.5, this trap a.k.a. syscall68kHwrLCDDrawBitmap */
#define syscall68kHwrTimerSleep 0xA24E
#define syscall68kHwrTimerWake 0xA24F
#define syscall68kHwrDisplayWake 0xA250 /* Before OS 3.5, this trap a.k.a. syscall68kHwrLCDWake */
#define syscall68kHwrIRQ1Handler 0xA251
#define syscall68kHwrIRQ2Handler 0xA252
#define syscall68kHwrIRQ3Handler 0xA253
#define syscall68kHwrIRQ4Handler 0xA254
#define syscall68kHwrIRQ5Handler 0xA255
#define syscall68kHwrIRQ6Handler 0xA256
#define syscall68kHwrDockSignals 0xA257
#define syscall68kHwrPluggedIn 0xA258

#define syscall68kCrc16CalcBlock 0xA259

#define syscall68kSelectDayV10 0xA25A
#define syscall68kSelectTimeV33 0xA25B

#define syscall68kDayDrawDaySelector 0xA25C
#define syscall68kDayHandleEvent 0xA25D
#define syscall68kDayDrawDays 0xA25E
#define syscall68kDayOfWeek 0xA25F
#define syscall68kDaysInMonth 0xA260
#define syscall68kDayOfMonth 0xA261

#define syscall68kDateDaysToDate 0xA262
#define syscall68kDateToDays 0xA263
#define syscall68kDateAdjust 0xA264
#define syscall68kDateSecondsToDate 0xA265
#define syscall68kDateToAscii 0xA266
#define syscall68kDateToDOWDMFormat 0xA267
#define syscall68kTimeToAscii 0xA268

#define syscall68kFind 0xA269
#define syscall68kFindStrInStr 0xA26A
#define syscall68kFindSaveMatch 0xA26B
#define syscall68kFindGetLineBounds 0xA26C
#define syscall68kFindDrawHeader 0xA26D

#define syscall68kPenOpen 0xA26E
#define syscall68kPenClose 0xA26F
#define syscall68kPenGetRawPen 0xA270
#define syscall68kPenCalibrate 0xA271
#define syscall68kPenRawToScreen 0xA272
#define syscall68kPenScreenToRaw 0xA273
#define syscall68kPenResetCalibration 0xA274
#define syscall68kPenSleep 0xA275
#define syscall68kPenWake 0xA276

#define syscall68kResLoadForm 0xA277
#define syscall68kResLoadMenu 0xA278

#define syscall68kFtrInit 0xA279
#define syscall68kFtrUnregister 0xA27A
#define syscall68kFtrGet 0xA27B
#define syscall68kFtrSet 0xA27C
#define syscall68kFtrGetByIndex 0xA27D

#define syscall68kGrfInit 0xA27E
#define syscall68kGrfFree 0xA27F
#define syscall68kGrfGetState 0xA280
#define syscall68kGrfSetState 0xA281
#define syscall68kGrfFlushPoints 0xA282
#define syscall68kGrfAddPoint 0xA283
#define syscall68kGrfInitState 0xA284
#define syscall68kGrfCleanState 0xA285
#define syscall68kGrfMatch 0xA286
#define syscall68kGrfGetMacro 0xA287
#define syscall68kGrfFilterPoints 0xA288
#define syscall68kGrfGetNumPoints 0xA289
#define syscall68kGrfGetPoint 0xA28A
#define syscall68kGrfFindBranch 0xA28B
#define syscall68kGrfMatchGlyph 0xA28C
#define syscall68kGrfGetGlyphMapping 0xA28D
#define syscall68kGrfGetMacroName 0xA28E
#define syscall68kGrfDeleteMacro 0xA28F
#define syscall68kGrfAddMacro 0xA290
#define syscall68kGrfGetAndExpandMacro 0xA291
#define syscall68kGrfProcessStroke 0xA292
#define syscall68kGrfFieldChange 0xA293

#define syscall68kGetCharSortValue 0xA294
#define syscall68kGetCharAttr 0xA295
#define syscall68kGetCharCaselessValue 0xA296

#define syscall68kPwdExists 0xA297
#define syscall68kPwdVerify 0xA298
#define syscall68kPwdSet 0xA299
#define syscall68kPwdRemove 0xA29A

#define syscall68kGsiInitialize 0xA29B
#define syscall68kGsiSetLocation 0xA29C
#define syscall68kGsiEnable 0xA29D
#define syscall68kGsiEnabled 0xA29E
#define syscall68kGsiSetShiftState 0xA29F

#define syscall68kKeyInit 0xA2A0
#define syscall68kKeyHandleInterrupt 0xA2A1
#define syscall68kKeyCurrentState 0xA2A2
#define syscall68kKeyResetDoubleTap 0xA2A3
#define syscall68kKeyRates 0xA2A4
#define syscall68kKeySleep 0xA2A5
#define syscall68kKeyWake 0xA2A6

#define syscall68kDlkControl 0xA2A7 /* was syscall68kCmBroadcast */

#define syscall68kDlkStartServer 0xA2A8
#define syscall68kDlkGetSyncInfo 0xA2A9
#define syscall68kDlkSetLogEntry 0xA2AA

#define syscall68kIntlDispatch                                              \
    0xA2AB /* REUSED IN v3.1 (was syscall68kPsrInit in 1.0, removed in 2.0) \
            */
#define syscall68kSysLibLoad 0xA2AC /* REUSED IN v2.0 (was syscall68kPsrClose) */
#define syscall68kSndPlaySmf \
    0xA2AD /* REUSED IN v3.0 (was syscall68kPsrGetCommand in 1.0, removed in 2.0) */
#define syscall68kSndCreateMidiList \
    0xA2AE /* REUSED IN v3.0 (was syscall68kPsrSendReply in 1.0, removed in 2.0) */

#define syscall68kAbtShowAbout 0xA2AF

#define syscall68kMdmDial 0xA2B0
#define syscall68kMdmHangUp 0xA2B1

#define syscall68kDmSearchRecord 0xA2B2

#define syscall68kSysInsertionSort 0xA2B3
#define syscall68kDmInsertionSort 0xA2B4

#define syscall68kLstSetTopItem 0xA2B5

// Palm OS 2.X traps					Palm Pilot and 2.0 Upgrade Card

#define syscall68kSclSetScrollBar 0xA2B6
#define syscall68kSclDrawScrollBar 0xA2B7
#define syscall68kSclHandleEvent 0xA2B8

#define syscall68kSysMailboxCreate 0xA2B9
#define syscall68kSysMailboxDelete 0xA2BA
#define syscall68kSysMailboxFlush 0xA2BB
#define syscall68kSysMailboxSend 0xA2BC
#define syscall68kSysMailboxWait 0xA2BD

#define syscall68kSysTaskWait 0xA2BE
#define syscall68kSysTaskWake 0xA2BF
#define syscall68kSysTaskWaitClr 0xA2C0
#define syscall68kSysTaskSuspend 0xA2C1
#define syscall68kSysTaskResume 0xA2C2

#define syscall68kCategoryCreateList 0xA2C3
#define syscall68kCategoryFreeList 0xA2C4
#define syscall68kCategoryEditV20 0xA2C5
#define syscall68kCategorySelect 0xA2C6

#define syscall68kDmDeleteCategory 0xA2C7

#define syscall68kSysEvGroupCreate 0xA2C8
#define syscall68kSysEvGroupSignal 0xA2C9
#define syscall68kSysEvGroupRead 0xA2CA
#define syscall68kSysEvGroupWait 0xA2CB

#define syscall68kEvtEventAvail 0xA2CC
#define syscall68kEvtSysEventAvail 0xA2CD
#define syscall68kStrNCopy 0xA2CE

#define syscall68kKeySetMask 0xA2CF

#define syscall68kSelectDay 0xA2D0

#define syscall68kPrefGetPreference 0xA2D1
#define syscall68kPrefSetPreference 0xA2D2
#define syscall68kPrefGetAppPreferences 0xA2D3
#define syscall68kPrefSetAppPreferences 0xA2D4

#define syscall68kFrmPointInTitle 0xA2D5

#define syscall68kStrNCat 0xA2D6

#define syscall68kMemCmp 0xA2D7

#define syscall68kTblSetColumnEditIndicator 0xA2D8

#define syscall68kFntWordWrap 0xA2D9

#define syscall68kFldGetScrollValues 0xA2DA

#define syscall68kSysCreateDataBaseList 0xA2DB
#define syscall68kSysCreatePanelList 0xA2DC

#define syscall68kDlkDispatchRequest 0xA2DD

#define syscall68kStrPrintF 0xA2DE
#define syscall68kStrVPrintF 0xA2DF

#define syscall68kPrefOpenPreferenceDB 0xA2E0

#define syscall68kSysGraffitiReferenceDialog 0xA2E1

#define syscall68kSysKeyboardDialog 0xA2E2

#define syscall68kFntWordWrapReverseNLines 0xA2E3
#define syscall68kFntGetScrollValues 0xA2E4

#define syscall68kTblSetRowStaticHeight 0xA2E5
#define syscall68kTblHasScrollBar 0xA2E6

#define syscall68kSclGetScrollBar 0xA2E7

#define syscall68kFldGetNumberOfBlankLines 0xA2E8

#define syscall68kSysTicksPerSecond 0xA2E9
#define syscall68kHwrBacklightV33 0xA2EA /* This trap obsoleted for OS 3.5 and later */
#define syscall68kDmDatabaseProtect 0xA2EB

#define syscall68kTblSetBounds 0xA2EC

#define syscall68kStrNCompare 0xA2ED
#define syscall68kStrNCaselessCompare 0xA2EE

#define syscall68kPhoneNumberLookup 0xA2EF

#define syscall68kFrmSetMenu 0xA2F0

#define syscall68kEncDigestMD5 0xA2F1

#define syscall68kDmFindSortPosition 0xA2F2

#define syscall68kSysBinarySearch 0xA2F3
#define syscall68kSysErrString 0xA2F4
#define syscall68kSysStringByIndex 0xA2F5

#define syscall68kEvtAddUniqueEventToQueue 0xA2F6

#define syscall68kStrLocalizeNumber 0xA2F7
#define syscall68kStrDelocalizeNumber 0xA2F8
#define syscall68kLocGetNumberSeparators 0xA2F9

#define syscall68kMenuSetActiveMenuRscID 0xA2FA

#define syscall68kLstScrollList 0xA2FB

#define syscall68kCategoryInitialize 0xA2FC

#define syscall68kEncDigestMD4 0xA2FD
#define syscall68kEncDES 0xA2FE

#define syscall68kLstGetVisibleItems 0xA2FF

#define syscall68kWinSetBounds 0xA300

#define syscall68kCategorySetName 0xA301

#define syscall68kFldSetInsertionPoint 0xA302

#define syscall68kFrmSetObjectBounds 0xA303

#define syscall68kWinSetColors 0xA304

#define syscall68kFlpDispatch 0xA305
#define syscall68kFlpEmDispatch 0xA306

// Palm OS 3.0 traps					Palm III and 3.0 Upgrade Card

#define syscall68kExgInit 0xA307
#define syscall68kExgConnect 0xA308
#define syscall68kExgPut 0xA309
#define syscall68kExgGet 0xA30A
#define syscall68kExgAccept 0xA30B
#define syscall68kExgDisconnect 0xA30C
#define syscall68kExgSend 0xA30D
#define syscall68kExgReceive 0xA30E
#define syscall68kExgRegisterData 0xA30F
#define syscall68kExgNotifyReceiveV35 0xA310
#define syscall68kSysReserved30Trap2 \
    0xA311 /* "Reserved" trap in Palm OS 3.0 and later (was syscall68kExgControl) */

#define syscall68kPrgStartDialogV31 0xA312 /* Updated in v3.2 */
#define syscall68kPrgStopDialog 0xA313
#define syscall68kPrgUpdateDialog 0xA314
#define syscall68kPrgHandleEvent 0xA315

#define syscall68kImcReadFieldNoSemicolon 0xA316
#define syscall68kImcReadFieldQuotablePrintable 0xA317
#define syscall68kImcReadPropertyParameter 0xA318
#define syscall68kImcSkipAllPropertyParameters 0xA319
#define syscall68kImcReadWhiteSpace 0xA31A
#define syscall68kImcWriteQuotedPrintable 0xA31B
#define syscall68kImcWriteNoSemicolon 0xA31C
#define syscall68kImcStringIsAscii 0xA31D

#define syscall68kTblGetItemFont 0xA31E
#define syscall68kTblSetItemFont 0xA31F

#define syscall68kFontSelect 0xA320
#define syscall68kFntDefineFont 0xA321

#define syscall68kCategoryEdit 0xA322

#define syscall68kSysGetOSVersionString 0xA323
#define syscall68kSysBatteryInfo 0xA324
#define syscall68kSysUIBusy 0xA325

#define syscall68kWinValidateHandle 0xA326
#define syscall68kFrmValidatePtr 0xA327
#define syscall68kCtlValidatePointer 0xA328
#define syscall68kWinMoveWindowAddr 0xA329
#define syscall68kFrmAddSpaceForObject 0xA32A
#define syscall68kFrmNewForm 0xA32B
#define syscall68kCtlNewControl 0xA32C
#define syscall68kFldNewField 0xA32D
#define syscall68kLstNewList 0xA32E
#define syscall68kFrmNewLabel 0xA32F
#define syscall68kFrmNewBitmap 0xA330
#define syscall68kFrmNewGadget 0xA331

#define syscall68kFileOpen 0xA332
#define syscall68kFileClose 0xA333
#define syscall68kFileDelete 0xA334
#define syscall68kFileReadLow 0xA335
#define syscall68kFileWrite 0xA336
#define syscall68kFileSeek 0xA337
#define syscall68kFileTell 0xA338
#define syscall68kFileTruncate 0xA339
#define syscall68kFileControl 0xA33A

#define syscall68kFrmActiveState 0xA33B

#define syscall68kSysGetAppInfo 0xA33C
#define syscall68kSysGetStackInfo 0xA33D

#define syscall68kWinScreenMode 0xA33E     /* was syscall68kScrDisplayMode */
#define syscall68kHwrLCDGetDepthV33 0xA33F /* This trap obsoleted for OS 3.5 and later */
#define syscall68kHwrGetROMToken 0xA340

#define syscall68kDbgControl 0xA341

#define syscall68kExgDBRead 0xA342
#define syscall68kExgDBWrite 0xA343

#define syscall68kHostControl \
    0xA344 /* Renamed from syscall68kSysGremlins, functionality generalized */
#define syscall68kFrmRemoveObject 0xA345

#define syscall68kSysReserved30Trap1 \
    0xA346 /* "Reserved" trap in Palm OS 3.0 and later (was syscall68kSysReserved1) */

// NOTE: The following two traps are reserved for future mgrs
// that may or may not be present on any particular device.
// They are NOT present by default; code must check first!
#define syscall68kExpansionDispatch \
    0xA347 /* Reserved for ExpansionMgr (was syscall68kSysReserved2) */
#define syscall68kFileSystemDispatch                                  \
    0xA348 /* Reserved for FileSystemMgr (was syscall68kSysReserved3) \
            */

#define syscall68kOEMDispatch \
    0xA349 /* OEM trap in Palm OS 3.0 and later trap table (formerly syscall68kSysReserved4) */

// Palm OS 3.1 traps					Palm IIIx and Palm V

#define syscall68kHwrLCDContrastV33 0xA34A /* This trap obsoleted for OS 3.5 and later */
#define syscall68kSysLCDContrast 0xA34B
#define syscall68kUIContrastAdjust 0xA34C /* Renamed from syscall68kContrastAdjust */
#define syscall68kHwrDockStatus 0xA34D

#define syscall68kFntWidthToOffset 0xA34E
#define syscall68kSelectOneTime 0xA34F
#define syscall68kWinDrawChar 0xA350
#define syscall68kWinDrawTruncChars 0xA351

#define syscall68kSysNotifyInit 0xA352 /* Notification Manager traps */
#define syscall68kSysNotifyRegister 0xA353
#define syscall68kSysNotifyUnregister 0xA354
#define syscall68kSysNotifyBroadcast 0xA355
#define syscall68kSysNotifyBroadcastDeferred 0xA356
#define syscall68kSysNotifyDatabaseAdded 0xA357
#define syscall68kSysNotifyDatabaseRemoved 0xA358

#define syscall68kSysWantEvent 0xA359

#define syscall68kFtrPtrNew 0xA35A
#define syscall68kFtrPtrFree 0xA35B
#define syscall68kFtrPtrResize 0xA35C

#define syscall68kSysReserved31Trap1 \
    0xA35D /* "Reserved" trap in Palm OS 3.1 and later (was syscall68kSysReserved5) */

// Palm OS 3.2 & 3.3 traps		Palm VII (3.2) and Fall '99 Palm OS Flash Update (3.3)

#define syscall68kHwrNVPrefSet 0xA35E /* mapped to FlashParmsWrite */
#define syscall68kHwrNVPrefGet 0xA35F /* mapped to FlashParmsRead */
#define syscall68kFlashInit 0xA360
#define syscall68kFlashCompress 0xA361
#define syscall68kFlashErase 0xA362
#define syscall68kFlashProgram 0xA363

#define syscall68kAlmTimeChange 0xA364
#define syscall68kErrAlertCustom 0xA365
#define syscall68kPrgStartDialog 0xA366 /* New version of syscall68kPrgStartDialogV31 */

#define syscall68kSerialDispatch 0xA367
#define syscall68kHwrBattery 0xA368
#define syscall68kDmGetDatabaseLockState 0xA369

#define syscall68kCncGetProfileList 0xA36A
#define syscall68kCncGetProfileInfo 0xA36B
#define syscall68kCncAddProfile 0xA36C
#define syscall68kCncDeleteProfile 0xA36D

#define syscall68kSndPlaySmfResource 0xA36E

#define syscall68kMemPtrDataStorage 0xA36F /* Never actually installed until now. */

#define syscall68kClipboardAppendItem 0xA370

#define syscall68kWiCmdV32 0xA371 /* Code moved to INetLib; trap obsolete */

// Palm OS 3.5 traps				Palm IIIc and other products

// HAL Display-layer new traps
#define syscall68kHwrDisplayAttributes 0xA372
#define syscall68kHwrDisplayDoze 0xA373
#define syscall68kHwrDisplayPalette 0xA374

// Screen driver new traps
#define syscall68kBltFindIndexes 0xA375
#define syscall68kBmpGetBits 0xA376 /* was BltGetBitsAddr */
#define syscall68kBltCopyRectangle 0xA377
#define syscall68kBltDrawChars 0xA378
#define syscall68kBltLineRoutine 0xA379
#define syscall68kBltRectangleRoutine 0xA37A

// ScrUtils new traps
#define syscall68kScrCompress 0xA37B
#define syscall68kScrDecompress 0xA37C

// System Manager new traps
#define syscall68kSysLCDBrightness 0xA37D

// WindowColor new traps
#define syscall68kWinPaintChar 0xA37E
#define syscall68kWinPaintChars 0xA37F
#define syscall68kWinPaintBitmap 0xA380
#define syscall68kWinGetPixel 0xA381
#define syscall68kWinPaintPixel 0xA382
#define syscall68kWinDrawPixel 0xA383
#define syscall68kWinErasePixel 0xA384
#define syscall68kWinInvertPixel 0xA385
#define syscall68kWinPaintPixels 0xA386
#define syscall68kWinPaintLines 0xA387
#define syscall68kWinPaintLine 0xA388
#define syscall68kWinPaintRectangle 0xA389
#define syscall68kWinPaintRectangleFrame 0xA38A
#define syscall68kWinPaintPolygon 0xA38B
#define syscall68kWinDrawPolygon 0xA38C
#define syscall68kWinErasePolygon 0xA38D
#define syscall68kWinInvertPolygon 0xA38E
#define syscall68kWinFillPolygon 0xA38F
#define syscall68kWinPaintArc 0xA390
#define syscall68kWinDrawArc 0xA391
#define syscall68kWinEraseArc 0xA392
#define syscall68kWinInvertArc 0xA393
#define syscall68kWinFillArc 0xA394
#define syscall68kWinPushDrawState 0xA395
#define syscall68kWinPopDrawState 0xA396
#define syscall68kWinSetDrawMode 0xA397
#define syscall68kWinSetForeColor 0xA398
#define syscall68kWinSetBackColor 0xA399
#define syscall68kWinSetTextColor 0xA39A
#define syscall68kWinGetPatternType 0xA39B
#define syscall68kWinSetPatternType 0xA39C
#define syscall68kWinPalette 0xA39D
#define syscall68kWinRGBToIndex 0xA39E
#define syscall68kWinIndexToRGB 0xA39F
#define syscall68kWinScreenLock 0xA3A0
#define syscall68kWinScreenUnlock 0xA3A1
#define syscall68kWinGetBitmap 0xA3A2

// UIColor new traps
#define syscall68kUIColorInit 0xA3A3
#define syscall68kUIColorGetTableEntryIndex 0xA3A4
#define syscall68kUIColorGetTableEntryRGB 0xA3A5
#define syscall68kUIColorSetTableEntry 0xA3A6
#define syscall68kUIColorPushTable 0xA3A7
#define syscall68kUIColorPopTable 0xA3A8

// misc cleanup and API additions

#define syscall68kCtlNewGraphicControl 0xA3A9

#define syscall68kTblGetItemPtr 0xA3AA

#define syscall68kUIBrightnessAdjust 0xA3AB
#define syscall68kUIPickColor 0xA3AC

#define syscall68kEvtSetAutoOffTimer 0xA3AD

// Misc int'l/overlay support.
#define syscall68kTsmDispatch 0xA3AE
#define syscall68kOmDispatch 0xA3AF
#define syscall68kDmOpenDBNoOverlay 0xA3B0
#define syscall68kDmOpenDBWithLocale 0xA3B1
#define syscall68kResLoadConstant 0xA3B2

// new boot-time SmallROM HAL additions
#define syscall68kHwrPreDebugInit 0xA3B3
#define syscall68kHwrResetNMI 0xA3B4
#define syscall68kHwrResetPWM 0xA3B5

#define syscall68kKeyBootKeys 0xA3B6

#define syscall68kDbgSerDrvOpen 0xA3B7
#define syscall68kDbgSerDrvClose 0xA3B8
#define syscall68kDbgSerDrvControl 0xA3B9
#define syscall68kDbgSerDrvStatus 0xA3BA
#define syscall68kDbgSerDrvWriteChar 0xA3BB
#define syscall68kDbgSerDrvReadChar 0xA3BC

// new boot-time BigROM HAL additions
#define syscall68kHwrPostDebugInit 0xA3BD
#define syscall68kHwrIdentifyFeatures 0xA3BE
#define syscall68kHwrModelSpecificInit 0xA3BF
#define syscall68kHwrModelInitStage2 0xA3C0
#define syscall68kHwrInterruptsInit 0xA3C1

#define syscall68kHwrSoundOn 0xA3C2
#define syscall68kHwrSoundOff 0xA3C3

// Kernel clock tick routine
#define syscall68kSysKernelClockTick 0xA3C4

// MenuEraseMenu is exposed as of PalmOS 3.5, but there are
// no public interfaces for it yet.	 Perhaps in a later release.
#define syscall68kMenuEraseMenu 0xA3C5

#define syscall68kSelectTime 0xA3C6

// Menu Command Bar traps
#define syscall68kMenuCmdBarAddButton 0xA3C7
#define syscall68kMenuCmdBarGetButtonData 0xA3C8
#define syscall68kMenuCmdBarDisplay 0xA3C9

// Silkscreen info
#define syscall68kHwrGetSilkscreenID 0xA3CA
#define syscall68kEvtGetSilkscreenAreaList 0xA3CB

#define syscall68kSysFatalAlertInit 0xA3CC
#define syscall68kDateTemplateToAscii 0xA3CD

// New traps dealing with masking private records
#define syscall68kSecVerifyPW 0xA3CE
#define syscall68kSecSelectViewStatus 0xA3CF
#define syscall68kTblSetColumnMasked 0xA3D0
#define syscall68kTblSetRowMasked 0xA3D1
#define syscall68kTblRowMasked 0xA3D2

// New form trap for dialogs with text entry field
#define syscall68kFrmCustomResponseAlert 0xA3D3
#define syscall68kFrmNewGsi 0xA3D4

// New dynamic menu functions
#define syscall68kMenuShowItem 0xA3D5
#define syscall68kMenuHideItem 0xA3D6
#define syscall68kMenuAddItem 0xA3D7

// New form traps for "smart gadgets"
#define syscall68kFrmSetGadgetHandler 0xA3D8

// More new control functions
#define syscall68kCtlSetGraphics 0xA3D9
#define syscall68kCtlGetSliderValues 0xA3DA
#define syscall68kCtlSetSliderValues 0xA3DB
#define syscall68kCtlNewSliderControl 0xA3DC

// Bitmap manager functions
#define syscall68kBmpCreate 0xA3DD
#define syscall68kBmpDelete 0xA3DE
#define syscall68kBmpCompress 0xA3DF
// syscall68kBmpGetBits defined in Screen driver traps
#define syscall68kBmpGetColortable 0xA3E0
#define syscall68kBmpSize 0xA3E1
#define syscall68kBmpBitsSize 0xA3E2
#define syscall68kBmpColortableSize 0xA3E3
// extra window namager
#define syscall68kWinCreateBitmapWindow 0xA3E4
// Ask for a null event sooner (replaces a macro which Poser hated)
#define syscall68kEvtSetNullEventTick 0xA3E5

// Exchange manager call to allow apps to select destination categories
#define syscall68kExgDoDialog 0xA3E6

// this call will remove temporary UI like popup lists
#define syscall68kSysUICleanup 0xA3E7

// The following 4 traps were "Reserved" traps, present only in SOME post-release builds of Palm
// OS 3.5
#define syscall68kWinSetForeColorRGB 0xA3E8
#define syscall68kWinSetBackColorRGB 0xA3E9
#define syscall68kWinSetTextColorRGB 0xA3EA
#define syscall68kWinGetPixelRGB 0xA3EB

// TRAPS ABOVE THIS POINT CAN NOT CHANGE BECAUSE THEY HAVE
// BEEN RELEASED TO CUSTOMERS IN SHIPPING ROMS AND SDKS.
// (MOVE THIS COMMENT DOWN WHENEVER THE "NEXT" RELEASE OCCURS.)

// WARNING!!  The following are new traps for 4.0.  If this file is merged
// with MAIN sources, new traps that are added for products that precede
// 4.0 MUST insert their traps BEFORE this section.

#define syscall68kSysReserved40Trap1 0xA3EC
#define syscall68kSysReserved40Trap2 0xA3ED
#define syscall68kSysReserved40Trap3 0xA3EE
#define syscall68kSysReserved40Trap4 0xA3EF

// DO NOT CHANGE TRAPS ABOVE THIS LINE
// THESE TRAPS HAVE BEEN RELEASED IN THE 3.5 SDK
// NEW TRAPS FOR PALM OS 4.0 CAN BE ADDED AFTER THIS
// THE ORDER IS NOT IMPORTANT AND CAN BE CHANGED.

// New Trap selector added for New Connection Mgr API
#define syscall68kCncMgrDispatch 0xA3F0

// new trap for notify from interrupt, implemented in SysEvtMgr.c
#define syscall68kSysNotifyBroadcastFromInterrupt 0xA3F1

// new trap for waking the UI without generating a null event
#define syscall68kEvtWakeupWithoutNilEvent 0xA3F2

// new trap for doing stable, fast, 7-bit string compare
#define syscall68kStrCompareAscii 0xA3F3

// New trap for accessors available thru PalmOS glue
#define syscall68kAccessorDispatch 0xA3F4

#define syscall68kBltGetPixel 0xA3F5
#define syscall68kBltPaintPixel 0xA3F6
#define syscall68kScrScreenInit 0xA3F7
#define syscall68kScrUpdateScreenBitmap 0xA3F8
#define syscall68kScrPalette 0xA3F9
#define syscall68kScrGetColortable 0xA3FA
#define syscall68kScrGetGrayPat 0xA3FB
#define syscall68kScrScreenLock 0xA3FC
#define syscall68kScrScreenUnlock 0xA3FD
#define syscall68kFntPrvGetFontList 0xA3FE

// Exchange manager functions
#define syscall68kExgRegisterDatatype 0xA3FF
#define syscall68kExgNotifyReceive 0xA400
#define syscall68kExgNotifyGoto 0xA401
#define syscall68kExgRequest 0xA402
#define syscall68kExgSetDefaultApplication 0xA403
#define syscall68kExgGetDefaultApplication 0xA404
#define syscall68kExgGetTargetApplication 0xA405
#define syscall68kExgGetRegisteredApplications 0xA406
#define syscall68kExgGetRegisteredTypes 0xA407
#define syscall68kExgNotifyPreview 0xA408
#define syscall68kExgControl 0xA409

// 04/30/00	CS - New Locale Manager handles access to region-specific info like date formats
#define syscall68kLmDispatch 0xA40A

// 05/10/00 kwk - New Memory Manager trap for retrieving ROM NVParam values (sys use only)
#define syscall68kMemGetRomNVParams 0xA40B

// 05/12/00 kwk - Safe character width Font Mgr call
#define syscall68kFntWCharWidth 0xA40C

// 05/17/00 kwk - Faster DmFindDatabase
#define syscall68kDmFindDatabaseWithTypeCreator 0xA40D

// New Trap selectors added for time zone picker API
#define syscall68kSelectTimeZone 0xA40E
#define syscall68kTimeZoneToAscii 0xA40F

// 08/18/00 kwk - trap for doing stable, fast, 7-bit string compare.
// 08/21/00 kwk - moved here in place of syscall68kSelectDaylightSavingAdjustment.
#define syscall68kStrNCompareAscii 0xA410

// New Trap selectors added for time zone conversion API
#define syscall68kTimTimeZoneToUTC 0xA411
#define syscall68kTimUTCToTimeZone 0xA412

// New trap implemented in PhoneLookup.c
#define syscall68kPhoneNumberLookupCustom 0xA413

// new trap for selecting debugger path.
#define syscall68kHwrDebugSelect 0xA414

#define syscall68kBltRoundedRectangle 0xA415
#define syscall68kBltRoundedRectangleFill 0xA416
#define syscall68kWinPrvInitCanvas 0xA417

#define syscall68kHwrCalcDynamicHeapSize 0xA418
#define syscall68kHwrDebuggerEnter 0xA419
#define syscall68kHwrDebuggerExit 0xA41A

#define syscall68kLstGetTopItem 0xA41B

#define syscall68kHwrModelInitStage3 0xA41C

// 06/21/00 peter - New Attention Manager
#define syscall68kAttnIndicatorAllow 0xA41D
#define syscall68kAttnIndicatorAllowed 0xA41E
#define syscall68kAttnIndicatorEnable 0xA41F
#define syscall68kAttnIndicatorEnabled 0xA420
#define syscall68kAttnIndicatorSetBlinkPattern 0xA421
#define syscall68kAttnIndicatorGetBlinkPattern 0xA422
#define syscall68kAttnIndicatorTicksTillNextBlink 0xA423
#define syscall68kAttnIndicatorCheckBlink 0xA424
#define syscall68kAttnInitialize 0xA425
#define syscall68kAttnGetAttention 0xA426
#define syscall68kAttnUpdate 0xA427
#define syscall68kAttnForgetIt 0xA428
#define syscall68kAttnGetCounts 0xA429
#define syscall68kAttnListOpen 0xA42A
#define syscall68kAttnHandleEvent 0xA42B
#define syscall68kAttnEffectOfEvent 0xA42C
#define syscall68kAttnIterate 0xA42D
#define syscall68kAttnDoSpecialEffects 0xA42E
#define syscall68kAttnDoEmergencySpecialEffects 0xA42F
#define syscall68kAttnAllowClose 0xA430
#define syscall68kAttnReopen 0xA431
#define syscall68kAttnEnableNotification 0xA432
#define syscall68kHwrLEDAttributes 0xA433
#define syscall68kHwrVibrateAttributes 0xA434

// Trap for getting and setting the device password hint.
#define syscall68kSecGetPwdHint 0xA435
#define syscall68kSecSetPwdHint 0xA436

#define syscall68kHwrFlashWrite 0xA437

#define syscall68kKeyboardStatusNew 0xA438
#define syscall68kKeyboardStatusFree 0xA439
#define syscall68kKbdSetLayout 0xA43A
#define syscall68kKbdGetLayout 0xA43B
#define syscall68kKbdSetPosition 0xA43C
#define syscall68kKbdGetPosition 0xA43D
#define syscall68kKbdSetShiftState 0xA43E
#define syscall68kKbdGetShiftState 0xA43F
#define syscall68kKbdDraw 0xA440
#define syscall68kKbdErase 0xA441
#define syscall68kKbdHandleEvent 0xA442

#define syscall68kOEMDispatch2 0xA443
#define syscall68kHwrCustom 0xA444

// 08/28/00 kwk - Trap for getting form's active field.
#define syscall68kFrmGetActiveField 0xA445

// 9/18/00 rkr - Added for playing sounds regardless of interruptible flag
#define syscall68kSndPlaySmfIrregardless 0xA446
#define syscall68kSndPlaySmfResourceIrregardless 0xA447
#define syscall68kSndInterruptSmfIrregardless 0xA448

// 10/14/00 ABa: UDA manager
#define syscall68kUdaMgrDispatch 0xA449

// WK: private traps for PalmOS
#define syscall68kPalmPrivate1 0xA44A
#define syscall68kPalmPrivate2 0xA44B
#define syscall68kPalmPrivate3 0xA44C
#define syscall68kPalmPrivate4 0xA44D

// 11/07/00 tlw: Added accessors
#define syscall68kBmpGetDimensions 0xA44E
#define syscall68kBmpGetBitDepth 0xA44F
#define syscall68kBmpGetNextBitmap 0xA450
#define syscall68kTblGetNumberOfColumns 0xA451
#define syscall68kTblGetTopRow 0xA452
#define syscall68kTblSetSelection 0xA453
#define syscall68kFrmGetObjectIndexFromPtr 0xA454

// 11/10/00 acs
#define syscall68kBmpGetSizes 0xA455
#define syscall68kWinGetBounds 0xA456

#define syscall68kBltPaintPixels 0xA457

// 11/22/00 bob
#define syscall68kFldSetMaxVisibleLines 0xA458

// 01/09/01 acs
#define syscall68kScrDefaultPaletteState 0xA459