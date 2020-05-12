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

#include "EmPalmStructs.h"

#include "EmCommon.h"

#if __profile__
    #pragma profile off
#endif

#undef INLINE_
#define INLINE_

#if !INLINE_SCALAR_IMPLEMENTATION

FOR_EACH_SCALAR_1(DEFINE_SCALAR_CLASSES)
FOR_EACH_SCALAR_2(DEFINE_SCALAR_CLASSES)
FOR_EACH_SCALAR_3(DEFINE_SCALAR_CLASSES)
FOR_EACH_SCALAR_4(DEFINE_SCALAR_CLASSES)

#endif

FOR_EACH_STRUCT_1(DEFINE_STRUCT_CLASSES)
FOR_EACH_STRUCT_2(DEFINE_STRUCT_CLASSES)
FOR_EACH_STRUCT_3(DEFINE_STRUCT_CLASSES)
FOR_EACH_STRUCT_4(DEFINE_STRUCT_CLASSES)
FOR_EACH_STRUCT_5(DEFINE_STRUCT_CLASSES)
FOR_EACH_STRUCT_6(DEFINE_STRUCT_CLASSES)
FOR_EACH_STRUCT_7(DEFINE_STRUCT_CLASSES)
FOR_EACH_STRUCT_8(DEFINE_STRUCT_CLASSES)
FOR_EACH_STRUCT_9(DEFINE_STRUCT_CLASSES)
FOR_EACH_STRUCT_10(DEFINE_STRUCT_CLASSES)
FOR_EACH_STRUCT_11(DEFINE_STRUCT_CLASSES)
FOR_EACH_STRUCT_12(DEFINE_STRUCT_CLASSES)
FOR_EACH_STRUCT_13(DEFINE_STRUCT_CLASSES)
FOR_EACH_STRUCT_14(DEFINE_STRUCT_CLASSES)

/*
        Note: Under VC++, the following INSTANTIATE macros cause the compiler to complain
        that the <LAS> types are already instantiated, possibly beccause of the following
        operators in the Proxy classes:

                EmAlias##type<LAS>	operator[] (int);
                const EmAlias##type<LAS>	operator[] (int) const;

        However, merely taking out the "template class EmAlias##type<LAS>" results in a
        lot of missing function errors during the link stage.  Therefore, we inhibit the
        compiler warnings with a #pragma in EmCommonWin.h.
*/

#define INSTANTIATE_SCALAR_CLASS(type, asType) \
    template class EmAlias##type<LAS>;         \
    template class EmAlias##type<PAS>;

#define INSTANTIATE_STRUCT_CLASS(type, size, FOR_EACH_FIELD) \
    template class EmAlias##type<LAS>;                       \
    template class EmAlias##type<PAS>;

FOR_EACH_SCALAR_1(INSTANTIATE_SCALAR_CLASS)
FOR_EACH_SCALAR_2(INSTANTIATE_SCALAR_CLASS)
FOR_EACH_SCALAR_3(INSTANTIATE_SCALAR_CLASS)
FOR_EACH_SCALAR_4(INSTANTIATE_SCALAR_CLASS)

FOR_EACH_STRUCT_1(INSTANTIATE_STRUCT_CLASS)
FOR_EACH_STRUCT_2(INSTANTIATE_STRUCT_CLASS)
FOR_EACH_STRUCT_3(INSTANTIATE_STRUCT_CLASS)
FOR_EACH_STRUCT_4(INSTANTIATE_STRUCT_CLASS)
FOR_EACH_STRUCT_5(INSTANTIATE_STRUCT_CLASS)
FOR_EACH_STRUCT_6(INSTANTIATE_STRUCT_CLASS)
FOR_EACH_STRUCT_7(INSTANTIATE_STRUCT_CLASS)
FOR_EACH_STRUCT_8(INSTANTIATE_STRUCT_CLASS)
FOR_EACH_STRUCT_9(INSTANTIATE_STRUCT_CLASS)
FOR_EACH_STRUCT_10(INSTANTIATE_STRUCT_CLASS)
FOR_EACH_STRUCT_11(INSTANTIATE_STRUCT_CLASS)
FOR_EACH_STRUCT_12(INSTANTIATE_STRUCT_CLASS)
FOR_EACH_STRUCT_13(INSTANTIATE_STRUCT_CLASS)
FOR_EACH_STRUCT_14(INSTANTIATE_STRUCT_CLASS)

int BadGetter(void) {
    EmAssert(false);
    return 0;
}

void BadSetter(void) { EmAssert(false); }
