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

#ifndef _ROMSTUBS_H_
#define _ROMSTUBS_H_

#include "EmCommon.h"

Err FtrGet(UInt32 creator, UInt16 featureNum, UInt32* valueP);
Err FtrSet(UInt32 creator, UInt16 featureNum, UInt32 newValue);

UInt16 SysSetAutoOffTime(UInt16 seconds);

Err PenScreenToRaw(PointType* penP);

Err EvtEnqueuePenPoint(PointType* ptP);

Err EvtWakeup(void);

Err EvtEnqueueKey(UInt16 ascii, UInt16 keycode, UInt16 modifiers);

Err SysKernelInfo(MemPtr p);

LocalID DmFindDatabase(UInt16 cardNo, const Char* nameP);
Err DmDeleteDatabase(UInt16 cardNo, LocalID dbID);
Err DmCreateDatabase(UInt16 cardNo, const Char* const nameP, UInt32 creator, UInt32 type,
                     Boolean resDB);
Err DmGetLastErr(void);
Err DmSetDatabaseInfo(UInt16 cardNo, LocalID dbID, const Char* nameP, UInt16* attributesP,
                      UInt16* versionP, UInt32* crDateP, UInt32* modDateP, UInt32* bckUpDateP,
                      UInt32* modNumP, LocalID* appInfoIDP, LocalID* sortInfoIDP, UInt32* typeP,
                      UInt32* creatorP);
emuptr DmOpenDatabase(UInt16 cardNo, LocalID dbID, UInt16 mode);
emuptr DmNewHandle(emuptr dbR, UInt32 size);
Err DmWrite(emuptr recordP, UInt32 offset, const void* const srcP, UInt32 bytes);
emuptr DmNewResource(emuptr dbR, DmResType resType, DmResID resID, UInt32 size);
Err DmReleaseResource(emuptr resourceH);
emuptr DmNewRecord(emuptr dbR, UInt16* atP, UInt32 size);
Err DmSetRecordInfo(emuptr dbR, UInt16 index, UInt16* attrP, UInt32* uniqueIDP);
Err DmReleaseRecord(emuptr dbR, UInt16 index, Boolean dirty);
Err DmCloseDatabase(emuptr dbR);

emuptr MemHandleLock(emuptr h);
Err MemHandleUnlock(emuptr h);
LocalID MemHandleToLocalID(emuptr h);

#endif /* _ROMSTUBS_H_ */
