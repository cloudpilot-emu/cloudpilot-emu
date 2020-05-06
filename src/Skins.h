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

#ifndef _SKINS_H_
#define _SKINS_H_

#include <string>
#include <vector>

#include "EmCommon.h"

#if 0                       // CSTODO
    #include "EmDevice.h"   // EmDevice
    #include "EmRect.h"     // EmRect
    #include "EmStructs.h"  // RGBType
#endif

class EmStream;

enum SkinElementType {
    kElement_First,

    kElement_PowerButton = kElement_First,
    kElement_UpButton,
    kElement_DownButton,
    kElement_App1Button,
    kElement_App2Button,
    kElement_App3Button,
    kElement_App4Button,
    kElement_CradleButton,
    kElement_Antenna,
    kElement_ContrastButton,

    // Symbol-specific
    kElement_TriggerLeft,
    kElement_TriggerCenter,
    kElement_TriggerRight,
    kElement_UpButtonLeft,
    kElement_UpButtonRight,
    kElement_DownButtonLeft,
    kElement_DownButtonRight,

    kElement_Touchscreen,
    kElement_LCD,
    kElement_LED,

    kElement_NumElements,

    kElement_Frame = -1,
    kElement_None = -2
};

DEFINE_SCALAR_MODIFIERS(SkinElementType)

typedef string SkinName;
typedef vector<SkinName> SkinNameList;

#if 0  // CSTODO

SkinName SkinGetSkinName(const EmDevice&);
SkinName SkinGetDefaultSkinName(const EmDevice&);
void SkinGetSkinNames(const EmDevice&, SkinNameList&);

void SkinSetSkin(void);
void SkinSetSkin(const EmDevice&, ScaleType scale, const SkinName&);
void SkinSetSkinName(const EmDevice&, const SkinName&);

EmFileRef SkinGetSkinfoFile(void);
EmFileRef SkinGetSkinFile(void);
EmFileRef SkinGetSkinFile(ScaleType scale);

EmStream* SkinGetSkinStream(void);
EmStream* SkinGetSkinStream(ScaleType scale);

Bool SkinValidSkin(const EmDevice&, const SkinName&);

RGBType SkinGetBackgroundColor(void);
RGBType SkinGetHighlightColor(void);

SkinElementType SkinTestPoint(const EmPoint& windowPt);
EmPoint SkinWindowToTouchscreen(const EmPoint& windowPt);
EmPoint SkinTouchscreenToWindow(const EmPoint& lcdPt);
Bool SkinGetElementInfo(int index, SkinElementType&, EmRect&);

EmPoint SkinScaleDown(const EmPoint&);
EmRect SkinScaleDown(const EmRect&);

EmPoint SkinScaleUp(const EmPoint&);
EmRect SkinScaleUp(const EmRect&);

#endif

#endif  // _SKINS_H_
