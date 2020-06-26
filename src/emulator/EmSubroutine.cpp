/* -*- mode: C++; tab-width: 4 -*- */
/* ===================================================================== *\
        Copyright (c) 2001 Palm, Inc. or its subsidiaries.
        All rights reserved.

        This file is part of the Palm OS Emulator.

        This program is free software; you can redistribute it and/or modify
        it under the terms of the GNU General Public License as published by
        the Free Software Foundation; either version 2 of the License, or
        (at your option) any later version.
\* ===================================================================== */

#include "EmSubroutine.h"

#include <ctype.h>  // isspace

#include "Byteswapping.h"  // Canonical
#include "EmBankMapped.h"  // EmBankMapped::GetEmulatedAddress
#include "EmCPU68K.h"
#include "EmCommon.h"
#include "EmMemory.h"       // EmMemPut8, etc.
#include "EmPalmStructs.h"  // EmAlias
#include "EmStructs.h"      // StringList
#include "Miscellaneous.h"  // SeparateList
#include "Platform.h"       // Platform::AllocateMemory

static EmParamInfo kEmParamInfo[] = {
    // These are the base integral types.

    {"Int8", kEm_SI1, false},
    {"Int16", kEm_SI2, false},
    {"Int32", kEm_SI4, false},
    //	{ "Int64",					kEm_SI8,	false },

    {"UInt8", kEm_UI1, false},
    {"UInt16", kEm_UI2, false},
    {"UInt32", kEm_UI4, false},
    //	{ "UInt64",					kEm_UI8,	false },

    //	{ "Float",					kEm_FP4,	false },
    //	{ "Double",					kEm_FP8,	false },
    //	{ "LongDouble",				kEm_FP16,	false },

    // These types are synonyms for the simple integral values.

    {"char", kEm_SI1, false},
    {"short", kEm_SI2, false},
    {"long", kEm_SI4, false},

    {"Char", kEm_UI1, false},
    {"WChar", kEm_UI2, false},

    {"Boolean", kEm_UI1, false},
    {"ClipboardFormatType", kEm_UI1, false},
    {"DlkSyncStateType", kEm_UI1, false},
    {"FormObjectKind", kEm_UI1, false},
    {"LocalIDKind", kEm_UI1, false},
    {"NetSocketAddrEnum", kEm_UI1, false},
    {"NetSocketTypeEnum", kEm_UI1, false},
    {"SystemPreferencesChoice", kEm_UI1, false},

    {"DmResID", kEm_UI2, false},
    {"Err", kEm_UI2, false},
    {"HostControlSelectorType", kEm_UI2, false},

    {"Coord", kEm_SI2, false},
    {"NetSocketRef", kEm_SI2, false},

    {"DmResType", kEm_UI4, false},
    {"LocalID", kEm_UI4, false},
    {"NetFDSetType", kEm_UI4, false},
    {"NetIPAddr", kEm_UI4, false},

    {"HostBoolType", kEm_SI4, false},
    {"HostClockType", kEm_SI4, false},
    {"HostErrType", kEm_SI4, false},
    {"HostIDType", kEm_SI4, false},
    {"HostPlatformType", kEm_SI4, false},
    {"HostSignalType", kEm_SI4, false},
    {"HostSizeType", kEm_SI4, false},
    {"HostTimeType", kEm_SI4, false},

    // These types are pointer types, but are mostly treated as integral values.
    // They get pushed onto the stack as a 4-byte value, but the stuff they
    // point to is not affected or adjusted in any way.  Also, when used as a
    // return type on the 68K, they are treated as pointers, not integers.

    {"DmOpenRef", kEm_Void, true},
    {"HostFILEType", kEm_Void, true},

    // The void type can be used as a return type ("void foo (int);"), or as an
    // empty parameter list ("int foo (void);").  It can also be used as a
    // generic pointer type ("void foo (void*);").

    {"void", kEm_Void, false}

};

class EmSubroutineCPU {
   public:
    EmSubroutineCPU(void);
    virtual ~EmSubroutineCPU(void);

    virtual long FormatStack(EmParamList&) = 0;
    virtual Err PrepareStack(Bool forCalling, Bool forStdArg, long stackSize) = 0;
    virtual Err PrepareStack(emuptr) = 0;

    virtual Err Call(uint16) = 0;
    virtual Err CallSelector(uint16, uint16) = 0;

    virtual void GetParamVal(EmParam&, int8&);
    virtual void GetParamVal(EmParam&, int16&);
    virtual void GetParamVal(EmParam&, int32&);
    virtual void GetParamVal(EmParam&, int64&);
    virtual void GetParamVal(EmParam&, uint8&);
    virtual void GetParamVal(EmParam&, uint16&);
    virtual void GetParamVal(EmParam&, uint32&);
    virtual void GetParamVal(EmParam&, uint64&);
    virtual void GetParamVal(EmParam&, float&);
    virtual void GetParamVal(EmParam&, double&);
    virtual void GetParamVal(EmParam&, long double&);

    virtual void SetParamVal(EmParam&, int8);
    virtual void SetParamVal(EmParam&, int16);
    virtual void SetParamVal(EmParam&, int32);
    virtual void SetParamVal(EmParam&, int64);
    virtual void SetParamVal(EmParam&, uint8);
    virtual void SetParamVal(EmParam&, uint16);
    virtual void SetParamVal(EmParam&, uint32);
    virtual void SetParamVal(EmParam&, uint64);
    virtual void SetParamVal(EmParam&, float);
    virtual void SetParamVal(EmParam&, double);
    virtual void SetParamVal(EmParam&, long double);

    virtual uint32 GetReturnRegInteger(void) = 0;
    virtual emuptr GetReturnRegPointer(void) = 0;

    virtual void SetReturnRegInteger(uint32) = 0;
    virtual void SetReturnRegPointer(uint32) = 0;

   protected:
    emuptr fStackPtr;
};

class EmSubroutineCPU68K : public EmSubroutineCPU {
   public:
    EmSubroutineCPU68K(void);
    virtual ~EmSubroutineCPU68K(void);

    virtual long FormatStack(EmParamList&);
    virtual Err PrepareStack(Bool forCalling, Bool forStdArg, long stackSize);
    virtual Err PrepareStack(emuptr);

    virtual Err Call(uint16);
    virtual Err CallSelector(uint16, uint16);

    virtual uint32 GetReturnRegInteger(void);
    virtual emuptr GetReturnRegPointer(void);

    virtual void SetReturnRegInteger(uint32);
    virtual void SetReturnRegPointer(uint32);

   private:
    Err DoCall(uint16 trapWord);
    char* GetStackBase(void);

    static Bool HandleTrap12(ExceptionNumber);

    enum { kStackSize = 4096 };
    char fStack[kStackSize + 3];

    uint32 fReturnedA0;
    uint32 fReturnedD0;
};

#if NDEBUG
    #define ERROR_CHECKING 0
#else
    #define ERROR_CHECKING 1
#endif

// ---------------------------------------------------------------------------
//		� EmSubroutine constructor
// ---------------------------------------------------------------------------

EmSubroutine::EmSubroutine(void) : fCPU(NULL), fParams(), fReturnType(), fStackSize(-1) {}

// ---------------------------------------------------------------------------
//		� EmSubroutine destructor
// ---------------------------------------------------------------------------

EmSubroutine::~EmSubroutine(void) { delete fCPU; }

// ---------------------------------------------------------------------------
//		� EmSubroutine::DescribeDecl
// ---------------------------------------------------------------------------

Err EmSubroutine::DescribeDecl(EmParamDecl returnType, EmParamListDecl decl) {
    // Determine the return type.

    Err err = this->ParseParamDecl(returnType, fReturnType);
#if ERROR_CHECKING
    if (err) {
        this->Reset();
        return err;
    }
#endif

    // Determine the parameter types.  Start by breaking up the parameter
    // list at the commas.

    StringList paramDecls;
    ::SeparateList(paramDecls, decl, ',');

    // Iterate over all the stuff we found between the commas.

    StringList::iterator iter = paramDecls.begin();
    while (iter != paramDecls.end()) {
        // For each intra-comma chunk, parse it up and determine the
        // type and name information it specifies.

        err = this->AddParam(iter->c_str());
#if ERROR_CHECKING
        if (err) {
            this->Reset();
            return err;
        }
#endif

        ++iter;
    }

    // After all the parameters have been parsed up, determine where they
    // should appear on the stack (specified as offsets from the stack
    // pointer).

    fStackSize = this->GetCPU()->FormatStack(fParams);

    return errNone;
}

// ---------------------------------------------------------------------------
//		� EmSubroutine::AddParam
// ---------------------------------------------------------------------------

Err EmSubroutine::AddParam(EmParamDecl decl) {
    EmParam param;

#if ERROR_CHECKING
    Err err = this->ParseParamDecl(decl, param);
    if (err) {
        this->Reset();
        return err;
    }
#else
    this->ParseParamDecl(decl, param);
#endif

    // If it's not just a bare "(void)", push it onto our collection
    // of parsed parameter information.

    if (param.fByRef || param.fType != kEm_Void) {
        fParams.push_back(param);
    }

    // After all the parameters have been parsed up, determine where they
    // should appear on the stack (specified as offsets from the stack
    // pointer).

    fStackSize = this->GetCPU()->FormatStack(fParams);

    return errNone;
}

// ---------------------------------------------------------------------------
//		� EmSubroutine::PrepareStack
// ---------------------------------------------------------------------------

Err EmSubroutine::PrepareStack(Bool forCalling, Bool forStdArg) {
    return this->GetCPU()->PrepareStack(forCalling, forStdArg, fStackSize);
}

// ---------------------------------------------------------------------------
//		� EmSubroutine::PrepareStack
// ---------------------------------------------------------------------------

Err EmSubroutine::PrepareStack(emuptr stackAddr) { return this->GetCPU()->PrepareStack(stackAddr); }

// ---------------------------------------------------------------------------
//		� EmSubroutine::Reset
// ---------------------------------------------------------------------------

Err EmSubroutine::Reset(void) {
    fParams.clear();
    fReturnType = EmParam();

    delete fCPU;
    fCPU = NULL;

    fStackSize = -1;

    return errNone;
}

#pragma mark -

// ---------------------------------------------------------------------------
//		� EmSubroutine::GetParamVal
// ---------------------------------------------------------------------------

Err EmSubroutine::GetParamVal(EmParamNameArg name, int8& result) {
    EmParamList::iterator iter = this->FindParam(name);

#if ERROR_CHECKING
    if (iter == fParams.end()) {
        EmAssert(false);
        return kEmErrUnknownParameter;
    }

    if (iter->fType != kEm_SI1) {
        EmAssert(false);
        return kEmErrTypeMismatch;
    }

    if (iter->fByRef) {
        EmAssert(false);
        return kEmErrTypeMismatch;
    }
#endif

    fCPU->GetParamVal(*iter, result);

    return errNone;
}

// ---------------------------------------------------------------------------
//		� EmSubroutine::GetParamVal
// ---------------------------------------------------------------------------

Err EmSubroutine::GetParamVal(EmParamNameArg name, int16& result) {
    EmParamList::iterator iter = this->FindParam(name);

#if ERROR_CHECKING
    if (iter == fParams.end()) {
        EmAssert(false);
        return kEmErrUnknownParameter;
    }

    if (iter->fType != kEm_SI2) {
        EmAssert(false);
        return kEmErrTypeMismatch;
    }

    if (iter->fByRef) {
        EmAssert(false);
        return kEmErrTypeMismatch;
    }
#endif

    fCPU->GetParamVal(*iter, result);

    return errNone;
}

// ---------------------------------------------------------------------------
//		� EmSubroutine::GetParamVal
// ---------------------------------------------------------------------------

Err EmSubroutine::GetParamVal(EmParamNameArg name, int32& result) {
    EmParamList::iterator iter = this->FindParam(name);

#if ERROR_CHECKING
    if (iter == fParams.end()) {
        EmAssert(false);
        return kEmErrUnknownParameter;
    }

    if (iter->fType != kEm_SI4) {
        EmAssert(false);
        return kEmErrTypeMismatch;
    }

    if (iter->fByRef) {
        EmAssert(false);
        return kEmErrTypeMismatch;
    }
#endif

    fCPU->GetParamVal(*iter, result);

    return errNone;
}

// ---------------------------------------------------------------------------
//		� EmSubroutine::GetParamVal
// ---------------------------------------------------------------------------

Err EmSubroutine::GetParamVal(EmParamNameArg name, int64& result) {
    EmParamList::iterator iter = this->FindParam(name);

#if ERROR_CHECKING
    if (iter == fParams.end()) {
        EmAssert(false);
        return kEmErrUnknownParameter;
    }

    if (iter->fType != kEm_SI8) {
        EmAssert(false);
        return kEmErrTypeMismatch;
    }

    if (iter->fByRef) {
        EmAssert(false);
        return kEmErrTypeMismatch;
    }
#endif

    fCPU->GetParamVal(*iter, result);

    return errNone;
}

// ---------------------------------------------------------------------------
//		� EmSubroutine::GetParamVal
// ---------------------------------------------------------------------------

Err EmSubroutine::GetParamVal(EmParamNameArg name, uint8& result) {
    EmParamList::iterator iter = this->FindParam(name);

#if ERROR_CHECKING
    if (iter == fParams.end()) {
        EmAssert(false);
        return kEmErrUnknownParameter;
    }

    if (iter->fType != kEm_UI1) {
        EmAssert(false);
        return kEmErrTypeMismatch;
    }

    if (iter->fByRef) {
        EmAssert(false);
        return kEmErrTypeMismatch;
    }
#endif

    fCPU->GetParamVal(*iter, result);

    return errNone;
}

// ---------------------------------------------------------------------------
//		� EmSubroutine::GetParamVal
// ---------------------------------------------------------------------------

Err EmSubroutine::GetParamVal(EmParamNameArg name, uint16& result) {
    EmParamList::iterator iter = this->FindParam(name);

#if ERROR_CHECKING
    if (iter == fParams.end()) {
        EmAssert(false);
        return kEmErrUnknownParameter;
    }

    if (iter->fType != kEm_UI2) {
        EmAssert(false);
        return kEmErrTypeMismatch;
    }

    if (iter->fByRef) {
        EmAssert(false);
        return kEmErrTypeMismatch;
    }
#endif

    fCPU->GetParamVal(*iter, result);

    return errNone;
}

// ---------------------------------------------------------------------------
//		� EmSubroutine::GetParamVal
// ---------------------------------------------------------------------------

Err EmSubroutine::GetParamVal(EmParamNameArg name, uint32& result) {
    EmParamList::iterator iter = this->FindParam(name);

#if ERROR_CHECKING
    if (iter == fParams.end()) {
        EmAssert(false);
        return kEmErrUnknownParameter;
    }

    if (!iter->fByRef && iter->fType != kEm_UI4) {
        EmAssert(false);
        return kEmErrTypeMismatch;
    }

//	if (iter->fByRef)
//	{
//		EmAssert (false);
//		return kEmErrTypeMismatch;
//	}
#endif

    fCPU->GetParamVal(*iter, result);

    return errNone;
}

// ---------------------------------------------------------------------------
//		� EmSubroutine::GetParamVal
// ---------------------------------------------------------------------------

Err EmSubroutine::GetParamVal(EmParamNameArg name, uint64& result) {
    EmParamList::iterator iter = this->FindParam(name);

#if ERROR_CHECKING
    if (iter == fParams.end()) {
        EmAssert(false);
        return kEmErrUnknownParameter;
    }

    if (iter->fType != kEm_UI8) {
        EmAssert(false);
        return kEmErrTypeMismatch;
    }

    if (iter->fByRef) {
        EmAssert(false);
        return kEmErrTypeMismatch;
    }
#endif

    fCPU->GetParamVal(*iter, result);

    return errNone;
}

// ---------------------------------------------------------------------------
//		� EmSubroutine::GetParamVal
// ---------------------------------------------------------------------------

Err EmSubroutine::GetParamVal(EmParamNameArg name, float& result) {
    EmParamList::iterator iter = this->FindParam(name);

#if ERROR_CHECKING
    if (iter == fParams.end()) {
        EmAssert(false);
        return kEmErrUnknownParameter;
    }

    if (iter->fType != kEm_FP4) {
        EmAssert(false);
        return kEmErrTypeMismatch;
    }

    if (iter->fByRef) {
        EmAssert(false);
        return kEmErrTypeMismatch;
    }
#endif

    fCPU->GetParamVal(*iter, result);

    return errNone;
}

// ---------------------------------------------------------------------------
//		� EmSubroutine::GetParamVal
// ---------------------------------------------------------------------------

Err EmSubroutine::GetParamVal(EmParamNameArg name, double& result) {
    EmParamList::iterator iter = this->FindParam(name);

#if ERROR_CHECKING
    if (iter == fParams.end()) {
        EmAssert(false);
        return kEmErrUnknownParameter;
    }

    if (iter->fType != kEm_FP8) {
        EmAssert(false);
        return kEmErrTypeMismatch;
    }

    if (iter->fByRef) {
        EmAssert(false);
        return kEmErrTypeMismatch;
    }
#endif

    fCPU->GetParamVal(*iter, result);

    return errNone;
}

// ---------------------------------------------------------------------------
//		� EmSubroutine::GetParamVal
// ---------------------------------------------------------------------------

Err EmSubroutine::GetParamVal(EmParamNameArg name, long double& result) {
    EmParamList::iterator iter = this->FindParam(name);

#if ERROR_CHECKING
    if (iter == fParams.end()) {
        EmAssert(false);
        return kEmErrUnknownParameter;
    }

    if (iter->fType != kEm_FP16) {
        EmAssert(false);
        return kEmErrTypeMismatch;
    }

    if (iter->fByRef) {
        EmAssert(false);
        return kEmErrTypeMismatch;
    }
#endif

    fCPU->GetParamVal(*iter, result);

    return errNone;
}

#pragma mark -

// ---------------------------------------------------------------------------
//		� EmSubroutine::GetReturnVal
// ---------------------------------------------------------------------------

Err EmSubroutine::GetReturnVal(int8& result) {
#if ERROR_CHECKING
    if (fReturnType.fType != kEm_SI1) {
        EmAssert(false);
        return kEmErrTypeMismatch;
    }

    if (fReturnType.fByRef) {
        EmAssert(false);
        return kEmErrTypeMismatch;
    }
#endif

    uint32 reg = this->GetReturnRegInteger();
    result = (int8)reg;

    return errNone;
}

// ---------------------------------------------------------------------------
//		� EmSubroutine::GetReturnVal
// ---------------------------------------------------------------------------

Err EmSubroutine::GetReturnVal(int16& result) {
#if ERROR_CHECKING
    if (fReturnType.fType != kEm_SI2) {
        EmAssert(false);
        return kEmErrTypeMismatch;
    }

    if (fReturnType.fByRef) {
        EmAssert(false);
        return kEmErrTypeMismatch;
    }
#endif

    uint32 reg = this->GetReturnRegInteger();
    result = (int16)reg;

    return errNone;
}

// ---------------------------------------------------------------------------
//		� EmSubroutine::GetReturnVal
// ---------------------------------------------------------------------------

Err EmSubroutine::GetReturnVal(int32& result) {
#if ERROR_CHECKING
    if (fReturnType.fType != kEm_SI4) {
        EmAssert(false);
        return kEmErrTypeMismatch;
    }

    if (fReturnType.fByRef) {
        EmAssert(false);
        return kEmErrTypeMismatch;
    }
#endif

    uint32 reg = this->GetReturnRegInteger();
    result = (int32)reg;

    return errNone;
}

// ---------------------------------------------------------------------------
//		� EmSubroutine::GetReturnVal
// ---------------------------------------------------------------------------

Err EmSubroutine::GetReturnVal(int64& /*result*/) {
#if ERROR_CHECKING
    if (fReturnType.fType != kEm_SI8) {
        EmAssert(false);
        return kEmErrTypeMismatch;
    }

    if (fReturnType.fByRef) {
        EmAssert(false);
        return kEmErrTypeMismatch;
    }
#endif

    EmAssert(false);

    return errNone;
}

// ---------------------------------------------------------------------------
//		� EmSubroutine::GetReturnVal
// ---------------------------------------------------------------------------

Err EmSubroutine::GetReturnVal(uint8& result) {
#if ERROR_CHECKING
    if (fReturnType.fType != kEm_UI1) {
        EmAssert(false);
        return kEmErrTypeMismatch;
    }

    if (fReturnType.fByRef) {
        EmAssert(false);
        return kEmErrTypeMismatch;
    }
#endif

    uint32 reg = this->GetReturnRegInteger();
    result = (uint8)reg;

    return errNone;
}

// ---------------------------------------------------------------------------
//		� EmSubroutine::GetReturnVal
// ---------------------------------------------------------------------------

Err EmSubroutine::GetReturnVal(uint16& result) {
#if ERROR_CHECKING
    if (fReturnType.fType != kEm_UI2) {
        EmAssert(false);
        return kEmErrTypeMismatch;
    }

    if (fReturnType.fByRef) {
        EmAssert(false);
        return kEmErrTypeMismatch;
    }
#endif

    uint32 reg = this->GetReturnRegInteger();
    result = (uint16)reg;

    return errNone;
}

// ---------------------------------------------------------------------------
//		� EmSubroutine::GetReturnVal
// ---------------------------------------------------------------------------

Err EmSubroutine::GetReturnVal(uint32& result) {
#if ERROR_CHECKING
    if (!fReturnType.fByRef && fReturnType.fType != kEm_UI4) {
        EmAssert(false);
        return kEmErrTypeMismatch;
    }

//	if (fReturnType.fByRef)
//	{
//		EmAssert (false);
//		return kEmErrTypeMismatch;
//	}
#endif

    uint32 reg;

    if (fReturnType.fByRef)
        reg = this->GetReturnRegPointer();
    else
        reg = this->GetReturnRegInteger();

    result = reg;

    return errNone;
}

// ---------------------------------------------------------------------------
//		� EmSubroutine::GetReturnVal
// ---------------------------------------------------------------------------

Err EmSubroutine::GetReturnVal(uint64& /*result*/) {
#if ERROR_CHECKING
    if (fReturnType.fType != kEm_UI8) {
        EmAssert(false);
        return kEmErrTypeMismatch;
    }

    if (fReturnType.fByRef) {
        EmAssert(false);
        return kEmErrTypeMismatch;
    }
#endif

    EmAssert(false);

    return errNone;
}

// ---------------------------------------------------------------------------
//		� EmSubroutine::GetReturnVal
// ---------------------------------------------------------------------------

Err EmSubroutine::GetReturnVal(float& /*result*/) {
#if ERROR_CHECKING
    if (fReturnType.fType != kEm_FP4) {
        EmAssert(false);
        return kEmErrTypeMismatch;
    }

    if (fReturnType.fByRef) {
        EmAssert(false);
        return kEmErrTypeMismatch;
    }
#endif

    EmAssert(false);

    return errNone;
}

// ---------------------------------------------------------------------------
//		� EmSubroutine::GetReturnVal
// ---------------------------------------------------------------------------

Err EmSubroutine::GetReturnVal(double& /*result*/) {
#if ERROR_CHECKING
    if (fReturnType.fType != kEm_FP8) {
        EmAssert(false);
        return kEmErrTypeMismatch;
    }

    if (fReturnType.fByRef) {
        EmAssert(false);
        return kEmErrTypeMismatch;
    }
#endif

    EmAssert(false);

    return errNone;
}

// ---------------------------------------------------------------------------
//		� EmSubroutine::GetReturnVal
// ---------------------------------------------------------------------------

Err EmSubroutine::GetReturnVal(long double&) {
#if ERROR_CHECKING
    if (fReturnType.fType != kEm_FP16) {
        EmAssert(false);
        return kEmErrTypeMismatch;
    }

    if (fReturnType.fByRef) {
        EmAssert(false);
        return kEmErrTypeMismatch;
    }
#endif

    EmAssert(false);

    return errNone;
}

#pragma mark -

// ---------------------------------------------------------------------------
//		� EmSubroutine::SetParamVal
// ---------------------------------------------------------------------------

Err EmSubroutine::SetParamVal(EmParamNameArg name, int8 val) {
    EmParamList::iterator iter = this->FindParam(name);

#if ERROR_CHECKING
    if (iter == fParams.end()) {
        EmAssert(false);
        return kEmErrUnknownParameter;
    }

    if (iter->fType != kEm_SI1) {
        EmAssert(false);
        return kEmErrTypeMismatch;
    }

    if (iter->fByRef) {
        EmAssert(false);
        return kEmErrTypeMismatch;
    }
#endif

    fCPU->SetParamVal(*iter, val);

    return errNone;
}

// ---------------------------------------------------------------------------
//		� EmSubroutine::SetParamVal
// ---------------------------------------------------------------------------

Err EmSubroutine::SetParamVal(EmParamNameArg name, int16 val) {
    EmParamList::iterator iter = this->FindParam(name);

#if ERROR_CHECKING
    if (iter == fParams.end()) {
        EmAssert(false);
        return kEmErrUnknownParameter;
    }

    if (iter->fType != kEm_SI2) {
        EmAssert(false);
        return kEmErrTypeMismatch;
    }

    if (iter->fByRef) {
        EmAssert(false);
        return kEmErrTypeMismatch;
    }
#endif

    fCPU->SetParamVal(*iter, val);

    return errNone;
}

// ---------------------------------------------------------------------------
//		� EmSubroutine::SetParamVal
// ---------------------------------------------------------------------------

Err EmSubroutine::SetParamVal(EmParamNameArg name, int32 val) {
    EmParamList::iterator iter = this->FindParam(name);

#if ERROR_CHECKING
    if (iter == fParams.end()) {
        EmAssert(false);
        return kEmErrUnknownParameter;
    }

    if (iter->fType != kEm_SI4) {
        EmAssert(false);
        return kEmErrTypeMismatch;
    }

    if (iter->fByRef) {
        EmAssert(false);
        return kEmErrTypeMismatch;
    }
#endif

    fCPU->SetParamVal(*iter, val);

    return errNone;
}

// ---------------------------------------------------------------------------
//		� EmSubroutine::SetParamVal
// ---------------------------------------------------------------------------

Err EmSubroutine::SetParamVal(EmParamNameArg name, int64 val) {
    EmParamList::iterator iter = this->FindParam(name);

#if ERROR_CHECKING
    if (iter == fParams.end()) {
        EmAssert(false);
        return kEmErrUnknownParameter;
    }

    if (iter->fType != kEm_SI8) {
        EmAssert(false);
        return kEmErrTypeMismatch;
    }

    if (iter->fByRef) {
        EmAssert(false);
        return kEmErrTypeMismatch;
    }
#endif

    fCPU->SetParamVal(*iter, val);

    return errNone;
}

// ---------------------------------------------------------------------------
//		� EmSubroutine::SetParamVal
// ---------------------------------------------------------------------------

Err EmSubroutine::SetParamVal(EmParamNameArg name, uint8 val) {
    EmParamList::iterator iter = this->FindParam(name);

#if ERROR_CHECKING
    if (iter == fParams.end()) {
        EmAssert(false);
        return kEmErrUnknownParameter;
    }

    if (iter->fType != kEm_UI1) {
        EmAssert(false);
        return kEmErrTypeMismatch;
    }

    if (iter->fByRef) {
        EmAssert(false);
        return kEmErrTypeMismatch;
    }
#endif

    fCPU->SetParamVal(*iter, val);

    return errNone;
}

// ---------------------------------------------------------------------------
//		� EmSubroutine::SetParamVal
// ---------------------------------------------------------------------------

Err EmSubroutine::SetParamVal(EmParamNameArg name, uint16 val) {
    EmParamList::iterator iter = this->FindParam(name);

#if ERROR_CHECKING
    if (iter == fParams.end()) {
        EmAssert(false);
        return kEmErrUnknownParameter;
    }

    if (iter->fType != kEm_UI2) {
        EmAssert(false);
        return kEmErrTypeMismatch;
    }

    if (iter->fByRef) {
        EmAssert(false);
        return kEmErrTypeMismatch;
    }
#endif

    fCPU->SetParamVal(*iter, val);

    return errNone;
}

// ---------------------------------------------------------------------------
//		� EmSubroutine::SetParamVal
// ---------------------------------------------------------------------------

Err EmSubroutine::SetParamVal(EmParamNameArg name, uint32 val) {
    EmParamList::iterator iter = this->FindParam(name);

#if ERROR_CHECKING
    if (iter == fParams.end()) {
        EmAssert(false);
        return kEmErrUnknownParameter;
    }

    if (!iter->fByRef && iter->fType != kEm_UI4) {
        EmAssert(false);
        return kEmErrTypeMismatch;
    }

//	if (iter->fByRef)
//	{
//		EmAssert (false);
//		return kEmErrTypeMismatch;
//	}
#endif

    fCPU->SetParamVal(*iter, val);

    return errNone;
}

// ---------------------------------------------------------------------------
//		� EmSubroutine::SetParamVal
// ---------------------------------------------------------------------------

Err EmSubroutine::SetParamVal(EmParamNameArg name, uint64 val) {
    EmParamList::iterator iter = this->FindParam(name);

#if ERROR_CHECKING
    if (iter == fParams.end()) {
        EmAssert(false);
        return kEmErrUnknownParameter;
    }

    if (iter->fType != kEm_UI8) {
        EmAssert(false);
        return kEmErrTypeMismatch;
    }

    if (iter->fByRef) {
        EmAssert(false);
        return kEmErrTypeMismatch;
    }
#endif

    fCPU->SetParamVal(*iter, val);

    return errNone;
}

// ---------------------------------------------------------------------------
//		� EmSubroutine::SetParamVal
// ---------------------------------------------------------------------------

Err EmSubroutine::SetParamVal(EmParamNameArg name, float val) {
    EmParamList::iterator iter = this->FindParam(name);

#if ERROR_CHECKING
    if (iter == fParams.end()) {
        EmAssert(false);
        return kEmErrUnknownParameter;
    }

    if (iter->fType != kEm_FP4) {
        EmAssert(false);
        return kEmErrTypeMismatch;
    }

    if (iter->fByRef) {
        EmAssert(false);
        return kEmErrTypeMismatch;
    }
#endif

    fCPU->SetParamVal(*iter, val);

    return errNone;
}

// ---------------------------------------------------------------------------
//		� EmSubroutine::SetParamVal
// ---------------------------------------------------------------------------

Err EmSubroutine::SetParamVal(EmParamNameArg name, double val) {
    EmParamList::iterator iter = this->FindParam(name);

#if ERROR_CHECKING
    if (iter == fParams.end()) {
        EmAssert(false);
        return kEmErrUnknownParameter;
    }

    if (iter->fType != kEm_FP8) {
        EmAssert(false);
        return kEmErrTypeMismatch;
    }

    if (iter->fByRef) {
        EmAssert(false);
        return kEmErrTypeMismatch;
    }
#endif

    fCPU->SetParamVal(*iter, val);

    return errNone;
}

// ---------------------------------------------------------------------------
//		� EmSubroutine::SetParamVal
// ---------------------------------------------------------------------------

Err EmSubroutine::SetParamVal(EmParamNameArg name, long double val) {
    EmParamList::iterator iter = this->FindParam(name);

#if ERROR_CHECKING
    if (iter == fParams.end()) {
        EmAssert(false);
        return kEmErrUnknownParameter;
    }

    if (iter->fType != kEm_FP16) {
        EmAssert(false);
        return kEmErrTypeMismatch;
    }

    if (iter->fByRef) {
        EmAssert(false);
        return kEmErrTypeMismatch;
    }
#endif

    fCPU->SetParamVal(*iter, val);

    return errNone;
}

#pragma mark -

// ---------------------------------------------------------------------------
//		� EmSubroutine::SetReturnVal
// ---------------------------------------------------------------------------

Err EmSubroutine::SetReturnVal(int8 val) {
#if ERROR_CHECKING
    if (fReturnType.fType != kEm_SI1) {
        EmAssert(false);
        return kEmErrTypeMismatch;
    }

    if (fReturnType.fByRef) {
        EmAssert(false);
        return kEmErrTypeMismatch;
    }
#endif

    this->SetReturnRegInteger((uint32)(uint8)val);

    return errNone;
}

// ---------------------------------------------------------------------------
//		� EmSubroutine::SetReturnVal
// ---------------------------------------------------------------------------

Err EmSubroutine::SetReturnVal(int16 val) {
#if ERROR_CHECKING
    if (fReturnType.fType != kEm_SI2) {
        EmAssert(false);
        return kEmErrTypeMismatch;
    }

    if (fReturnType.fByRef) {
        EmAssert(false);
        return kEmErrTypeMismatch;
    }
#endif

    this->SetReturnRegInteger((uint32)(uint16)val);

    return errNone;
}

// ---------------------------------------------------------------------------
//		� EmSubroutine::SetReturnVal
// ---------------------------------------------------------------------------

Err EmSubroutine::SetReturnVal(int32 val) {
#if ERROR_CHECKING
    if (fReturnType.fType != kEm_SI4) {
        EmAssert(false);
        return kEmErrTypeMismatch;
    }

    if (fReturnType.fByRef) {
        EmAssert(false);
        return kEmErrTypeMismatch;
    }
#endif

    this->SetReturnRegInteger((uint32)val);

    return errNone;
}

// ---------------------------------------------------------------------------
//		� EmSubroutine::SetReturnVal
// ---------------------------------------------------------------------------

Err EmSubroutine::SetReturnVal(int64) {
#if ERROR_CHECKING
    if (fReturnType.fType != kEm_SI8) {
        EmAssert(false);
        return kEmErrTypeMismatch;
    }

    if (fReturnType.fByRef) {
        EmAssert(false);
        return kEmErrTypeMismatch;
    }
#endif

    //	this->SetReturnRegInteger ((uint32) val);
    EmAssert(false);

    return errNone;
}

// ---------------------------------------------------------------------------
//		� EmSubroutine::SetReturnVal
// ---------------------------------------------------------------------------

Err EmSubroutine::SetReturnVal(uint8 val) {
#if ERROR_CHECKING
    if (fReturnType.fType != kEm_UI1) {
        EmAssert(false);
        return kEmErrTypeMismatch;
    }

    if (fReturnType.fByRef) {
        EmAssert(false);
        return kEmErrTypeMismatch;
    }
#endif

    this->SetReturnRegInteger(val);

    return errNone;
}

// ---------------------------------------------------------------------------
//		� EmSubroutine::SetReturnVal
// ---------------------------------------------------------------------------

Err EmSubroutine::SetReturnVal(uint16 val) {
#if ERROR_CHECKING
    if (fReturnType.fType != kEm_UI2) {
        EmAssert(false);
        return kEmErrTypeMismatch;
    }

    if (fReturnType.fByRef) {
        EmAssert(false);
        return kEmErrTypeMismatch;
    }
#endif

    this->SetReturnRegInteger(val);

    return errNone;
}

// ---------------------------------------------------------------------------
//		� EmSubroutine::SetReturnVal
// ---------------------------------------------------------------------------

Err EmSubroutine::SetReturnVal(uint32 val) {
#if ERROR_CHECKING
    if (!fReturnType.fByRef && fReturnType.fType != kEm_UI4) {
        EmAssert(false);
        return kEmErrTypeMismatch;
    }

//	if (fReturnType.fByRef)
//	{
//		EmAssert (false);
//		return kEmErrTypeMismatch;
//	}
#endif

    if (fReturnType.fByRef)
        this->SetReturnRegPointer(val);
    else
        this->SetReturnRegInteger(val);

    return errNone;
}

// ---------------------------------------------------------------------------
//		� EmSubroutine::SetReturnVal
// ---------------------------------------------------------------------------

Err EmSubroutine::SetReturnVal(uint64) {
#if ERROR_CHECKING
    if (fReturnType.fType != kEm_UI8) {
        EmAssert(false);
        return kEmErrTypeMismatch;
    }

    if (fReturnType.fByRef) {
        EmAssert(false);
        return kEmErrTypeMismatch;
    }
#endif

    //	this->SetReturnRegInteger (val);
    EmAssert(false);

    return errNone;
}

// ---------------------------------------------------------------------------
//		� EmSubroutine::SetReturnVal
// ---------------------------------------------------------------------------

Err EmSubroutine::SetReturnVal(float) {
#if ERROR_CHECKING
    if (fReturnType.fType != kEm_FP4) {
        EmAssert(false);
        return kEmErrTypeMismatch;
    }

    if (fReturnType.fByRef) {
        EmAssert(false);
        return kEmErrTypeMismatch;
    }
#endif

    //	this->SetReturnRegInteger (val);
    EmAssert(false);

    return errNone;
}

// ---------------------------------------------------------------------------
//		� EmSubroutine::SetReturnVal
// ---------------------------------------------------------------------------

Err EmSubroutine::SetReturnVal(double) {
#if ERROR_CHECKING
    if (fReturnType.fType != kEm_FP8) {
        EmAssert(false);
        return kEmErrTypeMismatch;
    }

    if (fReturnType.fByRef) {
        EmAssert(false);
        return kEmErrTypeMismatch;
    }
#endif

    //	this->SetReturnRegInteger (val);
    EmAssert(false);

    return errNone;
}

// ---------------------------------------------------------------------------
//		� EmSubroutine::SetReturnVal
// ---------------------------------------------------------------------------

Err EmSubroutine::SetReturnVal(long double) {
#if ERROR_CHECKING
    if (fReturnType.fType != kEm_FP16) {
        EmAssert(false);
        return kEmErrTypeMismatch;
    }

    if (fReturnType.fByRef) {
        EmAssert(false);
        return kEmErrTypeMismatch;
    }
#endif

    //	this->SetReturnRegInteger (val);
    EmAssert(false);

    return errNone;
}

#pragma mark -

// ---------------------------------------------------------------------------
//		� EmSubroutine::Call
// ---------------------------------------------------------------------------

Err EmSubroutine::Call(uint16 trapWord) { return this->GetCPU()->Call(trapWord); }

// ---------------------------------------------------------------------------
//		� EmSubroutine::CallSelector
// ---------------------------------------------------------------------------

Err EmSubroutine::CallSelector(uint16 trapWord, uint16 selector) {
    return this->GetCPU()->CallSelector(trapWord, selector);
}

#pragma mark -

// ---------------------------------------------------------------------------
//		� EmSubroutine::GetReturnRegInteger
// ---------------------------------------------------------------------------

uint32 EmSubroutine::GetReturnRegInteger(void) { return this->GetCPU()->GetReturnRegInteger(); }

// ---------------------------------------------------------------------------
//		� EmSubroutine::GetReturnRegPointer
// ---------------------------------------------------------------------------

emuptr EmSubroutine::GetReturnRegPointer(void) { return this->GetCPU()->GetReturnRegPointer(); }

// ---------------------------------------------------------------------------
//		� EmSubroutine::SetReturnRegInteger
// ---------------------------------------------------------------------------

void EmSubroutine::SetReturnRegInteger(uint32 val) { this->GetCPU()->SetReturnRegInteger(val); }

// ---------------------------------------------------------------------------
//		� EmSubroutine::SetReturnRegPointer
// ---------------------------------------------------------------------------

void EmSubroutine::SetReturnRegPointer(uint32 val) { this->GetCPU()->SetReturnRegPointer(val); }

#pragma mark -

// ---------------------------------------------------------------------------
//		� EmSubroutine::GetCPU
// ---------------------------------------------------------------------------

EmSubroutineCPU* EmSubroutine::GetCPU(void) {
    if (fCPU == NULL) {
        fCPU = new EmSubroutineCPU68K;
    }

    return fCPU;
}

// ---------------------------------------------------------------------------
//		� EmSubroutine::Is68K
// ---------------------------------------------------------------------------

Bool EmSubroutine::Is68K(void) { return true; }

// ---------------------------------------------------------------------------
//		� EmSubroutine::IsARM
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
//		� EmSubroutine::ParseParamDecl
// ---------------------------------------------------------------------------

Err EmSubroutine::ParseParamDecl(EmParamDecl decl, EmParam& result) {
    string token;
    string::size_type offset = 0;

    // Get the type.

    token = this->GetToken(decl, offset);

    // See if we recognize it.

    size_t ii;
    for (ii = 0; ii < countof(kEmParamInfo); ++ii) {
        if (token == kEmParamInfo[ii].fTypeName) {
            result.fType = kEmParamInfo[ii].fType;
            result.fByRef = kEmParamInfo[ii].fByRef;
            break;
        }
    }

    // If not, try some default processing.

    if (ii >= countof(kEmParamInfo)) {
        // Set it up as an unknown type.  Hopefully, it will be
        // followed by a "*", so that we can treat it as a pointer type.

        result.fType = kEm_Unknown;
        result.fByRef = false;

        // Check the name to see if it ends in "Ptr" or "Handle.  If so,
        // treat it as a pointer type.

        if (::EndsWith(token.c_str(), "Ptr") || ::EndsWith(token.c_str(), "Handle")) {
            result.fByRef = true;
        }
    }

    // Get what follows the type.  This is either the parameter name,
    // or a "*" to indicate a reference parameter.

    token = this->GetToken(decl, offset);

    if (token == "*") {
        result.fByRef = true;

        // OK, *now* get the parameter name.

        token = this->GetToken(decl, offset);

        // Whoops...it's another "*".  So make the param a ptr to a ptr.

        while (token == "*") {
            result.fType = kEm_Void;

            // OK, *now* get the parameter name.

            token = this->GetToken(decl, offset);
        }
    }

    result.fName = token;

    return errNone;
}

// ---------------------------------------------------------------------------
//		� EmSubroutine::GetToken
// ---------------------------------------------------------------------------

string EmSubroutine::GetToken(EmParamDecl decl, string::size_type& offset) {
    string result;

    // Get a C++ string so that we can use the STL routines.

    string str(decl);

    do {
        // Skip whitespace.

        while (isspace(str[offset])) ++offset;

        // Nothing but whitespace.  This could happen if the
        // declaration has a type but no name.

        if (offset >= str.size()) {
            result.erase();
        }

        // If this is a "*", return it.

        else if (str[offset] == '*') {
            ++offset;
            result = "*";
        }

        // Otherwise, scoop of the identifer

        else {
            string::size_type begin = offset;

            while (isalnum(str[offset]) || str[offset] == '_') ++offset;

            result = str.substr(begin, offset - begin);
        }

        // If it's "const", "signed" or "unsigned", filter it out.
    } while (result == "const" || result == "signed" || result == "unsigned");

    return result;
}

// ---------------------------------------------------------------------------
//		� EmSubroutine::FindParam
// ---------------------------------------------------------------------------

EmParamList::iterator EmSubroutine::FindParam(EmParamNameArg name) {
    EmParamList::iterator iter = fParams.begin();

    while (iter != fParams.end()) {
        if (iter->fName == name) {
            break;
        }

        ++iter;
    }

    return iter;
}

#pragma mark------------------------------

// ---------------------------------------------------------------------------
//		� EmSubroutineCPU::EmSubroutineCPU
// ---------------------------------------------------------------------------

EmSubroutineCPU::EmSubroutineCPU(void) : fStackPtr(EmMemNULL) {}

// ---------------------------------------------------------------------------
//		� EmSubroutineCPU::~EmSubroutineCPU
// ---------------------------------------------------------------------------

EmSubroutineCPU::~EmSubroutineCPU(void) {}

#pragma mark -

// ---------------------------------------------------------------------------
//		� EmSubroutineCPU::GetParamVal
// ---------------------------------------------------------------------------

void EmSubroutineCPU::GetParamVal(EmParam& param, int8& result) {
#if ERROR_CHECKING
    EmAssert(fStackPtr);
    EmAssert(param.fStackOffset >= 0);
#endif

    result = EmMemGet8(fStackPtr + param.fStackOffset);
}

// ---------------------------------------------------------------------------
//		� EmSubroutineCPU::GetParamVal
// ---------------------------------------------------------------------------

void EmSubroutineCPU::GetParamVal(EmParam& param, int16& result) {
#if ERROR_CHECKING
    EmAssert(fStackPtr);
    EmAssert(param.fStackOffset >= 0);
#endif

    result = EmMemGet16(fStackPtr + param.fStackOffset);
}

// ---------------------------------------------------------------------------
//		� EmSubroutineCPU::GetParamVal
// ---------------------------------------------------------------------------

void EmSubroutineCPU::GetParamVal(EmParam& param, int32& result) {
#if ERROR_CHECKING
    EmAssert(fStackPtr);
    EmAssert(param.fStackOffset >= 0);
#endif

    result = EmMemGet32(fStackPtr + param.fStackOffset);
}

// ---------------------------------------------------------------------------
//		� EmSubroutineCPU::GetParamVal
// ---------------------------------------------------------------------------

void EmSubroutineCPU::GetParamVal(EmParam& param, int64& /*result*/) {
#if ERROR_CHECKING
    EmAssert(fStackPtr);
    EmAssert(param.fStackOffset >= 0);
#endif

    EmAssert(false);
}

// ---------------------------------------------------------------------------
//		� EmSubroutineCPU::GetParamVal
// ---------------------------------------------------------------------------

void EmSubroutineCPU::GetParamVal(EmParam& param, uint8& result) {
#if ERROR_CHECKING
    EmAssert(fStackPtr);
    EmAssert(param.fStackOffset >= 0);
#endif

    result = EmMemGet8(fStackPtr + param.fStackOffset);
}

// ---------------------------------------------------------------------------
//		� EmSubroutineCPU::GetParamVal
// ---------------------------------------------------------------------------

void EmSubroutineCPU::GetParamVal(EmParam& param, uint16& result) {
#if ERROR_CHECKING
    EmAssert(fStackPtr);
    EmAssert(param.fStackOffset >= 0);
#endif

    result = EmMemGet16(fStackPtr + param.fStackOffset);
}

// ---------------------------------------------------------------------------
//		� EmSubroutineCPU::GetParamVal
// ---------------------------------------------------------------------------

void EmSubroutineCPU::GetParamVal(EmParam& param, uint32& result) {
#if ERROR_CHECKING
    EmAssert(fStackPtr);
    EmAssert(param.fStackOffset >= 0);
#endif

    result = EmMemGet32(fStackPtr + param.fStackOffset);
}

// ---------------------------------------------------------------------------
//		� EmSubroutineCPU::GetParamVal
// ---------------------------------------------------------------------------

void EmSubroutineCPU::GetParamVal(EmParam& param, uint64& /*result*/) {
#if ERROR_CHECKING
    EmAssert(fStackPtr);
    EmAssert(param.fStackOffset >= 0);
#endif

    EmAssert(false);
}

// ---------------------------------------------------------------------------
//		� EmSubroutineCPU::GetParamVal
// ---------------------------------------------------------------------------

void EmSubroutineCPU::GetParamVal(EmParam& param, float& /*result*/) {
#if ERROR_CHECKING
    EmAssert(fStackPtr);
    EmAssert(param.fStackOffset >= 0);
#endif

    EmAssert(false);
}

// ---------------------------------------------------------------------------
//		� EmSubroutineCPU::GetParamVal
// ---------------------------------------------------------------------------

void EmSubroutineCPU::GetParamVal(EmParam& param, double& /*result*/) {
#if ERROR_CHECKING
    EmAssert(fStackPtr);
    EmAssert(param.fStackOffset >= 0);
#endif

    EmAssert(false);
}

// ---------------------------------------------------------------------------
//		� EmSubroutineCPU::GetParamVal
// ---------------------------------------------------------------------------

void EmSubroutineCPU::GetParamVal(EmParam& param, long double& /*result*/) {
#if ERROR_CHECKING
    EmAssert(fStackPtr);
    EmAssert(param.fStackOffset >= 0);
#endif

    EmAssert(false);
}

#pragma mark -

// ---------------------------------------------------------------------------
//		� EmSubroutineCPU::SetParamVal
// ---------------------------------------------------------------------------

void EmSubroutineCPU::SetParamVal(EmParam& param, int8 val) {
#if ERROR_CHECKING
    EmAssert(fStackPtr);
    EmAssert(param.fStackOffset >= 0);
#endif

    EmMemPut8(fStackPtr + param.fStackOffset, val);
}

// ---------------------------------------------------------------------------
//		� EmSubroutineCPU::SetParamVal
// ---------------------------------------------------------------------------

void EmSubroutineCPU::SetParamVal(EmParam& param, int16 val) {
#if ERROR_CHECKING
    EmAssert(fStackPtr);
    EmAssert(param.fStackOffset >= 0);
#endif

    EmMemPut16(fStackPtr + param.fStackOffset, val);
}

// ---------------------------------------------------------------------------
//		� EmSubroutineCPU::SetParamVal
// ---------------------------------------------------------------------------

void EmSubroutineCPU::SetParamVal(EmParam& param, int32 val) {
#if ERROR_CHECKING
    EmAssert(fStackPtr);
    EmAssert(param.fStackOffset >= 0);
#endif

    EmMemPut32(fStackPtr + param.fStackOffset, val);
}

// ---------------------------------------------------------------------------
//		� EmSubroutineCPU::SetParamVal
// ---------------------------------------------------------------------------

void EmSubroutineCPU::SetParamVal(EmParam& param, int64 /*val*/) {
#if ERROR_CHECKING
    EmAssert(fStackPtr);
    EmAssert(param.fStackOffset >= 0);
#endif

    EmAssert(false);
}

// ---------------------------------------------------------------------------
//		� EmSubroutineCPU::SetParamVal
// ---------------------------------------------------------------------------

void EmSubroutineCPU::SetParamVal(EmParam& param, uint8 val) {
#if ERROR_CHECKING
    EmAssert(fStackPtr);
    EmAssert(param.fStackOffset >= 0);
#endif

    EmMemPut8(fStackPtr + param.fStackOffset, val);
}

// ---------------------------------------------------------------------------
//		� EmSubroutineCPU::SetParamVal
// ---------------------------------------------------------------------------

void EmSubroutineCPU::SetParamVal(EmParam& param, uint16 val) {
#if ERROR_CHECKING
    EmAssert(fStackPtr);
    EmAssert(param.fStackOffset >= 0);
#endif

    EmMemPut16(fStackPtr + param.fStackOffset, val);
}

// ---------------------------------------------------------------------------
//		� EmSubroutineCPU::SetParamVal
// ---------------------------------------------------------------------------

void EmSubroutineCPU::SetParamVal(EmParam& param, uint32 val) {
#if ERROR_CHECKING
    EmAssert(fStackPtr);
    EmAssert(param.fStackOffset >= 0);
#endif

    EmMemPut32(fStackPtr + param.fStackOffset, val);
}

// ---------------------------------------------------------------------------
//		� EmSubroutineCPU::SetParamVal
// ---------------------------------------------------------------------------

void EmSubroutineCPU::SetParamVal(EmParam& param, uint64 /*val*/) {
#if ERROR_CHECKING
    EmAssert(fStackPtr);
    EmAssert(param.fStackOffset >= 0);
#endif

    EmAssert(false);
}

// ---------------------------------------------------------------------------
//		� EmSubroutineCPU::SetParamVal
// ---------------------------------------------------------------------------

void EmSubroutineCPU::SetParamVal(EmParam& param, float /*val*/) {
#if ERROR_CHECKING
    EmAssert(fStackPtr);
    EmAssert(param.fStackOffset >= 0);
#endif

    EmAssert(false);
}

// ---------------------------------------------------------------------------
//		� EmSubroutineCPU::SetParamVal
// ---------------------------------------------------------------------------

void EmSubroutineCPU::SetParamVal(EmParam& param, double /*val*/) {
#if ERROR_CHECKING
    EmAssert(fStackPtr);
    EmAssert(param.fStackOffset >= 0);
#endif

    EmAssert(false);
}

// ---------------------------------------------------------------------------
//		� EmSubroutineCPU::SetParamVal
// ---------------------------------------------------------------------------

void EmSubroutineCPU::SetParamVal(EmParam& param, long double /*val*/) {
#if ERROR_CHECKING
    EmAssert(fStackPtr);
    EmAssert(param.fStackOffset >= 0);
#endif

    EmAssert(false);
}

#include "EmCPU68K.h"   // GetRegisters
#include "EmPalmOS.h"   // StackRange, RememberStackRange, ForgetStack
#include "EmSession.h"  // gSession
#include "UAE.h"        // m68k_dreg, etc.

const uint16 kOpcode_ROMCall = m68kTrapInstr + sysDispatchTrapNum;
const uint16 kOpcode_ATrapReturn = m68kTrapInstr + kATrapReturnTrapNum;

// ---------------------------------------------------------------------------
//		� EmSubroutineCPU68K::EmSubroutineCPU68K
// ---------------------------------------------------------------------------

EmSubroutineCPU68K::EmSubroutineCPU68K(void) : fReturnedA0(0), fReturnedD0(0) {}

// ---------------------------------------------------------------------------
//		� EmSubroutineCPU68K::~EmSubroutineCPU68K
// ---------------------------------------------------------------------------

EmSubroutineCPU68K::~EmSubroutineCPU68K(void) {}

// ---------------------------------------------------------------------------
//		� EmSubroutineCPU68K::FormatStack
// ---------------------------------------------------------------------------

long EmSubroutineCPU68K::FormatStack(EmParamList& params) {
    long offset = 0;

    EmParamList::iterator iter = params.begin();
    while (iter != params.end()) {
        iter->fStackOffset = offset;

        if (iter->fByRef) {
            offset += 4;
        } else {
            switch (iter->fType) {
                case kEm_SI1:
                case kEm_SI2:
                case kEm_UI1:
                case kEm_UI2:
                    offset += 2;
                    break;

                case kEm_SI4:
                case kEm_UI4:
                    offset += 4;
                    break;

                default:
                    EmAssert(false);
            }
        }

        ++iter;
    }

    return offset;
}

// ---------------------------------------------------------------------------
//		� EmSubroutineCPU68K::PrepareStack
// ---------------------------------------------------------------------------

Err EmSubroutineCPU68K::PrepareStack(Bool forCalling, Bool /*forStdArg*/, long stackSize) {
    if (forCalling) {
        // Give ourselves our own private stack.  We'll want this in case
        // we're in the debugger and the stack pointer is hosed.

        EmBankMapped::MapPhysicalMemory(this->GetStackBase(), kStackSize);

        char* stackBase = this->GetStackBase();
        char* stackTop = &stackBase[kStackSize - 4];
        emuptr eStackTop = EmBankMapped::GetEmulatedAddress(stackTop);

        fStackPtr = eStackTop - stackSize;
    } else {
        fStackPtr = m68k_areg(regs, 7);

        // Get these values, in case the EmSubroutine object is used
        // when tailpatching a function and we need to examine the
        // value being returned by the original version.

        fReturnedA0 = m68k_areg(regs, 0);
        fReturnedD0 = m68k_dreg(regs, 0);
    }

    return errNone;
}

// ---------------------------------------------------------------------------
//		� EmSubroutineCPU68K::PrepareStack
// ---------------------------------------------------------------------------

Err EmSubroutineCPU68K::PrepareStack(emuptr stackAddr) {
    fStackPtr = stackAddr;

    // Get these values, in case the EmSubroutine object is used
    // when tailpatching a function and we need to examine the
    // value being returned by the original version.

    fReturnedA0 = m68k_areg(regs, 0);
    fReturnedD0 = m68k_dreg(regs, 0);

    return errNone;
}

// ---------------------------------------------------------------------------
//		� EmSubroutineCPU68K::Call
// ---------------------------------------------------------------------------

Err EmSubroutineCPU68K::Call(uint16 trapWord) {
    Err err = errNone;

    // Save the current state.

    regstruct oldRegs;
    gCPU68K->GetRegisters(oldRegs);

    // Make sure the CPU is not stopped.  I suppose that we could force the CPU
    // to no longer be stopped, but I'd rather that the Palm OS itself woke up
    // first before we try making calls into it.  Therefore, anything making
    // an out-of-the-blue Palm OS call via this class (that is, a call outside
    // of the context of a Palm OS function head- or tailpatch) should first
    // bring the CPU to a halt by calling EmSession::SuspendThread (kStopOnSysCall)
    // first (usually by using EmSessionStopper (kStopOnSysCall)).

    EmAssert(oldRegs.stopped == 0);

    // Point us to the business end of our private stack.
    //
    // Make sure we do this using SetRegisters (instead of just slamming
    // regs[SP]) so that we can record the switch to the new stack (for
    // our stack access monitoring code).

    regstruct newRegs = oldRegs;

    m68k_areg(newRegs, 7) = fStackPtr;
    gCPU68K->SetRegisters(newRegs);

    // Call the Palm OS function.

    err = this->DoCall(trapWord);

    // Save the result.

    fReturnedA0 = m68k_areg(regs, 0);
    fReturnedD0 = m68k_dreg(regs, 0);

    EmAssert(gCPU68K);
    gCPU68K->SetRegisters(oldRegs);

    // Unmap the stack that got mapped in PrepareStack.

    EmBankMapped::UnmapPhysicalMemory(this->GetStackBase());

    // Check to see if anything interesting was registered while we
    // were making the Palm OS subroutine call.  The "check after end
    // of cycle" bit may have gotten cleared when restoring the old
    // registers, so set it on the off chance that it was.  Doing this
    // is harmless if there really aren't any scheduled tasks.

    EmAssert(gSession);
    gCPU68K->CheckAfterCycle();

    return err;
}

// ---------------------------------------------------------------------------
//		� EmSubroutineCPU68K::CallSelector
// ---------------------------------------------------------------------------

Err EmSubroutineCPU68K::CallSelector(uint16 trapWord, uint16 selector) {
    Err err = errNone;

    // Save the current state.

    regstruct oldRegs;
    gCPU68K->GetRegisters(oldRegs);

    // Set up the selector.

    regstruct newRegs = oldRegs;

    m68k_dreg(newRegs, 2) = selector;
    gCPU68K->SetRegisters(newRegs);

    // Call the function.

    err = this->Call(trapWord);

    // Restore the original state.

    EmAssert(gCPU68K);
    gCPU68K->SetRegisters(oldRegs);

    return err;
}

#pragma mark -

// ---------------------------------------------------------------------------
//		� EmSubroutineCPU68K::GetReturnRegInteger
// ---------------------------------------------------------------------------

uint32 EmSubroutineCPU68K::GetReturnRegInteger(void) { return fReturnedD0; }

// ---------------------------------------------------------------------------
//		� EmSubroutineCPU68K::GetReturnRegPointer
// ---------------------------------------------------------------------------

emuptr EmSubroutineCPU68K::GetReturnRegPointer(void) { return fReturnedA0; }

// ---------------------------------------------------------------------------
//		� EmSubroutineCPU68K::SetReturnRegInteger
// ---------------------------------------------------------------------------

void EmSubroutineCPU68K::SetReturnRegInteger(uint32 val) {
    fReturnedD0 = val;
    m68k_dreg(regs, 0) = val;
}

// ---------------------------------------------------------------------------
//		� EmSubroutineCPU68K::SetReturnRegPointer
// ---------------------------------------------------------------------------

void EmSubroutineCPU68K::SetReturnRegPointer(uint32 val) {
    fReturnedA0 = (uae_u32)val;
    m68k_areg(regs, 0) = (uae_u32)val;
}

#pragma mark -

// ---------------------------------------------------------------------------
//		� EmSubroutineCPU68K::DoCall
// ---------------------------------------------------------------------------

Err EmSubroutineCPU68K::DoCall(uint16 trapWord) {
    Err err = errNone;

    // Assert that the function we're trying to call is implemented.
    //
    // Oops...bad test...this doesn't work when we're calling a library.
    // Instead, since we now invoke ROM functions by creating a TRAP $F
    // sequence, we'll let our TRAP $F handler deal with validating the
    // function call (it does that anyway).

    //	EmAssert (LowMem::GetTrapAddress (trapWord));

    // We call the ROM function by dummying up a sequence of 68xxx instructions
    // for it.  The sequence of instructions is:
    //
    //			TRAP	$F
    //			DC.W	<dispatch number>
    //			TRAP	$C
    //
    // The first two words invoke the function (calling any head- or tailpatches
    // along the way).  The third word allows the emulator to regain control
    // after the function has returned.
    //
    // Note: this gets a little ugly on little-endian machines.  The following
    // instructions are stored on the emulator's stack.  This memory is mapped
    // into the emulated address space in such a fashion that no byteswapping of
    // word or long values occurs.  Thus, we have to put the data into Big Endian
    // format when putting it into the array.
    //
    // However, opcodes are a special case.  They are optimized in the emulator
    // for fast access.  Opcodes are *always* fetched a word at a time in host-
    // endian order.  Thus, the opcodes below have to be stored in host-endian
    // order.  That's why there's no call to Canonical to put them into Big
    // Endian order.

    uint16 code[] = {kOpcode_ROMCall, trapWord, kOpcode_ATrapReturn};

    // Oh, OK, we do have to byteswap the trapWord.  Opcodes are fetched with
    // EmMemDoGet16, which always gets the value in host byte order.  The
    // trapWord is fetched with EmMemGet16, which gets values according to the
    // rules of the memory bank.  For the dummy bank, the defined byte order
    // is Big Endian.

    Canonical(code[1]);

    // Map in the code stub so that the emulation code can access it.

    StMemoryMapper mapper(code, sizeof(code));

    // Prepare to handle the TRAP 12 exception.

    EmAssert(gCPU68K);
    gCPU68K->InstallHookException(kException_ATrapReturn, HandleTrap12);

    // Point the PC to our code.

    emuptr newPC = EmBankMapped::GetEmulatedAddress(code);
    m68k_setpc(newPC);

    // Execute until the next break.

    EmAssert(gSession);
    gSession->ExecuteSubroutine();

    // Remove the TRAP 12 exception handler.

    EmAssert(gCPU68K);
    gCPU68K->RemoveHookException(kException_ATrapReturn, HandleTrap12);

    return err;
}

// ---------------------------------------------------------------------------
//		� EmSubroutineCPU68K::HandleTrap12
// ---------------------------------------------------------------------------
// This function really takes an ExceptionNumber as a parameter.  However,
// we don't use/need it, and omitting it helps our forward declarations.

Bool EmSubroutineCPU68K::HandleTrap12(ExceptionNumber) {
    EmAssert(gSession);
    gSession->ScheduleSubroutineReturn();

    return true;
}

// ---------------------------------------------------------------------------
//		� EmSubroutineCPU68K::GetStackBase
// ---------------------------------------------------------------------------

char* EmSubroutineCPU68K::GetStackBase() {
    // Ensure that the stack is aligned to a longword address.

    long stackBase = (long)fStack;

    stackBase += 3;
    stackBase &= ~3;

    return (char*)stackBase;
}
