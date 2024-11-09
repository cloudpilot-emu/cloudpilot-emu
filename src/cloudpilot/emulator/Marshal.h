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

#ifndef _MARSHAL_H_
#define _MARSHAL_H_

#include "EmBankMapped.h"  // UnmapPhysicalMemory
#include "EmCommon.h"
#include "EmMemory.h"       // EmMemGet32, EmMemGet16, EmMemGet8, EmMem_memcpy
#include "EmPalmStructs.h"  // EmProxy
#include "EmSubroutine.h"   // EmSubroutine
#include "Platform.h"       // Platform::AllocateMemory

/* ===========================================================================

        Functions and macros for helping with marshaling and unmarshaling
        parameters from and to the emulated process's stack.  First, the stack
        is described with a class called EmSubroutine which lists the function's
        parameters in order.  For example, the definition for NetLibDmReceive is
        as follows:

                EmSubroutine	sub ("Int16", "UInt16 libRefNum, NetSocketRef socket,"
                        "void *recordP, UInt32 recordOffset, UInt16 rcvLen, UInt16 flags, "
                        "void *fromAddrP, UInt16 *fromLenP, Int32 timeout, Err *errP");

        This class describes the format of the parameters on the stack.  You
        then use one of several macros to fetch and possibly return values from
        and to those parameters.

        Use PARAM_VAL to manage pass-by-value parameter, such as a Long or a
        Word. Example:

                PARAM_VAL(Word, libRefNum);

        This defines a local variable called libRefNum that contains the value
        passed on the stack.  The variable is an instance of a class that can
        produce a Word, and so can be passed to functions that take a Word.

        Use PARAM_REF to manage a pass-by-reference parameter, such as a Long*
        or a Word*.  Example:

                PARAM_REF(Word, fromLenP, kInput);

        This defines a local variable called fromLenP that contains the pointer
        passed on the stack as well as a copy of the value that it points to.
        The variable is an instance of a class that can produce a Word*, and so
        can be passed to functions that take a Word*.  If the parameter
        reference was NULL, then the class produces NULL as well.  The third
        parameter to the macro describes whether or not the value passed in is
        an input parameter (kInput), output parameter (kOutput) or both
        (kInOut). If kInput or kInOut, the referenced value is fetched and
        converted to host format.  Otherwise, it is assumed that the parameter
        references uninitialized contents.  If kOutput or kInOut, the value is
        copied back into the memory specified by the parameter reference.
        Values are explicitly copied back to emulated memory by calling
        fromLenP's Put() method.

        Use PARAM_PTR to manage an arbitrary block of data pointed to by a
        function parameter, such as a void* or BytePtr.  Example:

                PARAM_PTR(void, recordP, len, kInOut);

        This defines a local variable called recordP that points to a block of
        data "len" bytes long.  If the fourth parameter is kInput or kInOut, the
        memory pointed to by the function parameter is copied into this block of
        data.  If the fourth parameter is kOutput or kInOut, the contents of the
        buffer are copied back to the emulated memory when the Put() method is
        called.  NULL pointers are handled.

        Use PARAM_STR as a special kind of PARAM_PTR where the length is not
        explicitly known, but the referenced data is NULL terminated.  Example:

                PARAM_STR(Char, nameP);

        This defines a local variable called nameP that contains a pointer to a
        string, the contents of which are copied from emulated memory.  These
        variables are actually like PARAM_STRs with kInput as the third
        parameter, but there's no real need for that restriction.

        The above macros expand to variable definitions.  The name of the
        variable is given by the second macro parameter, and the type of the
        variable is determined by the first parameter.  The type of variable is
        actually created from a class template.  There are four class templates:
        ParamVal, ParamRef, ParamPtr, and ParamStr.

        The constructors for these classes fetch the the appropriate values from
        memory, allocating any necessary buffers.  Their destructors do nothing
        (which really saves in terms of exception handling overhead).

        When the fetched values have been used and it's time to return any
        changed values back to the emulated stack/heap, all the classes (except
        ParamVal) have Put methods.  You must call this method when you are done
        with the values.  The Put method writes back any altered values and
        releases any memory allocated in the constructor.

=========================================================================== */

struct HostGremlinInfoType;
struct HostStatType;
struct HostTmType;
struct HostUTimeType;

typedef struct ExgSocketType* ExgSocketPtr;
typedef UInt8* UInt8Ptr;

#define CALLED_SETUP(return_decl, parameter_decl)      \
    static EmSubroutine sub;                           \
                                                       \
    static Bool initialized;                           \
    if (!initialized) {                                \
        initialized = true;                            \
        sub.DescribeDecl(return_decl, parameter_decl); \
    }                                                  \
                                                       \
    sub.PrepareStack(kForBeingCalled, false)

#define CALLED_SETUP_HC(return_decl, parameter_decl)                                         \
    static EmSubroutine sub;                                                                 \
                                                                                             \
    static Bool initialized;                                                                 \
    if (!initialized) {                                                                      \
        initialized = true;                                                                  \
        sub.DescribeDecl(return_decl, "HostControlSelectorType _selector, " parameter_decl); \
    }                                                                                        \
                                                                                             \
    sub.PrepareStack(kForBeingCalled, false)

#define CALLED_SETUP_STDARG(return_decl, parameter_decl) \
    EmSubroutine sub;                                    \
    sub.DescribeDecl(return_decl, parameter_decl);       \
    sub.PrepareStack(kForBeingCalled, true)

#define CALLED_SETUP_STDARG_HC(return_decl, parameter_decl)                             \
    EmSubroutine sub;                                                                   \
    sub.DescribeDecl(return_decl, "HostControlSelectorType selector, " parameter_decl); \
    sub.PrepareStack(kForBeingCalled, true)

#define CALLER_SETUP(return_decl, parameter_decl)      \
    static EmSubroutine sub;                           \
                                                       \
    static Bool initialized;                           \
    if (!initialized) {                                \
        initialized = true;                            \
        sub.DescribeDecl(return_decl, parameter_decl); \
    }                                                  \
                                                       \
    sub.PrepareStack(kForCalling, false)

#define GET_RESULT_VAL(type) \
    type result;             \
    Marshal::GetReturnVal(sub, result);

#define GET_RESULT_PTR() \
    emuptr result;       \
    Marshal::GetReturnVal(sub, result);

#define PUT_RESULT_VAL(type, val) Marshal::PutReturnVal(sub, (type)(val));

#define PUT_RESULT_PTR(type, val) Marshal::PutReturnVal(sub, (type)(val));

#define RETURN_RESULT_VAL(type) \
    GET_RESULT_VAL(type);       \
    return result

#define RETURN_RESULT_PTR(type) \
    GET_RESULT_PTR();           \
    return (type)result

// Macros used when being called by emulated code
// (e.g., from trap patches).

#define CALLED_GET_PARAM_VAL(type, name) ParamVal<type> name(sub, #name)

#define CALLED_GET_PARAM_REF(type, name, io) ParamRef<type, io> name(sub, #name)

#define CALLED_GET_PARAM_PTR(type, name, len, io) ParamPtr<type, io> name(sub, #name, len)

#define CALLED_GET_PARAM_STR(type, name) ParamStr<type> name(sub, #name)

#define CALLED_PUT_PARAM_REF(name) name.Put()

// Macros used when calling emulated code
// (e.g., the stuff in ROMStubs.cpp)

#define CALLER_PUT_PARAM_VAL(type, name)                           \
    PushParamVal<type> _##name(sub, #name, (type)name);            \
    /* *Use* the variable this way.  GCC will complain that the */ \
    /* PushParamVal variable is unused otherwise. */               \
    ((void)_##name)

#define CALLER_PUT_PARAM_REF(type, name, io) PushParamRef<type, io> _##name(sub, #name, (type*)name)

#define CALLER_PUT_PARAM_PTR(type, name, len, io) \
    PushParamPtr<type, io> _##name(sub, #name, name, len)

#define CALLER_PUT_PARAM_STR(type, name) PushParamStr<type> _##name(sub, #name, name)

#define CALLER_GET_PARAM_REF(name) _##name.Get()

class Marshal {
   public:
    // VC++ is a bit medieval here...
    //		static const int	kInput	= 0x01;
    //		static const int	kOutput	= 0x02;
    //		static const int	kInOut	= kInput | kOutput;
    enum { kInput = 0x01, kOutput = 0x02, kInOut = kInput | kOutput };

#define INPUT(io) (((io) & Marshal::kInput) != 0)
#define OUTPUT(io) (((io) & Marshal::kOutput) != 0)

    static void* GetBuffer(emuptr p, long len);
#if (__GNUC__ == 2)
    static void PutBuffer(emuptr p, unsigned char* const buf, long len) {
        if (p) {
            EmMem_memcpy(p, (void*)buf, len);
            void* b = buf;
            Platform::DisposeMemory(b);
        }
    }
#else
    template <class T>
    static void PutBuffer(emuptr p, T* const buf, long len) {
        if (p) {
            EmMem_memcpy(p, (void*)buf, len);
            void* b = buf;
            Platform::DisposeMemory(b);
        }
    }
#endif

    /* ===========================================================================
            The ParamFoo classes are templatized on the type of values they are to
            fetch.  In order to perform the actual fetching, they call GetParamVal
            (and put the value back with PutParamVal).  In order for those calls to
            succeed, we have to have overloaded versions of that function for each
            type we ever fetch.  Those overloaded functions appear below.
    =========================================================================== */

#define DECLARE_POINTER_MARSHALLER(type)                                                    \
    inline static void GetParamVal(EmSubroutine& sub, EmParamNameArg name, type*& v) {      \
        sub.GetParamVal(name, (emuptr&)v);                                                  \
    }                                                                                       \
                                                                                            \
    inline static void PutParamVal(EmSubroutine& sub, EmParamNameArg name, const type* v) { \
        sub.SetParamVal(name, (emuptr)v);                                                   \
    }                                                                                       \
                                                                                            \
    inline static void PutReturnVal(EmSubroutine& sub, const type* v) {                     \
        sub.SetReturnVal((emuptr)v);                                                        \
    }

#if 0  // CSBAD
    DECLARE_POINTER_MARSHALLER(void)
    DECLARE_POINTER_MARSHALLER(FormType)
    DECLARE_POINTER_MARSHALLER(ExgSocketType)
    DECLARE_POINTER_MARSHALLER(_opaque)
#endif

#define DECLARE_SCALAR_MARSHALLER(type, asType, num_bits)                                         \
    inline static void GetParamVal(EmSubroutine& sub, EmParamNameArg name, type& v) {             \
        asType temp;                                                                              \
        sub.GetParamVal(name, temp);                                                              \
        v = (type)temp;                                                                           \
    }                                                                                             \
                                                                                                  \
    inline static void PutParamVal(EmSubroutine& sub, EmParamNameArg name, type v) {              \
        sub.SetParamVal(name, (asType)v);                                                         \
    }                                                                                             \
                                                                                                  \
    inline static void GetParamRef(emuptr p, type& v) { v = (type)EmMemGet##num_bits(p); }        \
                                                                                                  \
    inline static void PutParamRef(emuptr p, const type& v) { EmMemPut##num_bits(p, (asType)v); } \
                                                                                                  \
    inline static void GetReturnVal(EmSubroutine& sub, type& v) {                                 \
        asType temp;                                                                              \
        sub.GetReturnVal(temp);                                                                   \
        v = (type)temp;                                                                           \
    }                                                                                             \
                                                                                                  \
    inline static void PutReturnVal(EmSubroutine& sub, type v) { sub.SetReturnVal((asType&)v); }  \
                                                                                                  \
    inline static long GetBufSize(const type&) { return sizeof(asType); }

    DECLARE_SCALAR_MARSHALLER(int8, int8, 8)
    DECLARE_SCALAR_MARSHALLER(uint8, uint8, 8)
    DECLARE_SCALAR_MARSHALLER(int16, int16, 16)
    DECLARE_SCALAR_MARSHALLER(uint16, uint16, 16)
    DECLARE_SCALAR_MARSHALLER(int32, int32, 32)
    DECLARE_SCALAR_MARSHALLER(uint32, uint32, 32)

    DECLARE_SCALAR_MARSHALLER(ClipboardFormatType, uint8, 8)
    DECLARE_SCALAR_MARSHALLER(DlkSyncStateType, uint8, 8)
    DECLARE_SCALAR_MARSHALLER(FormObjectKind, uint8, 8)
    DECLARE_SCALAR_MARSHALLER(LocalIDKind, uint8, 8)
    DECLARE_SCALAR_MARSHALLER(NetSocketAddrEnum, uint8, 8)
    DECLARE_SCALAR_MARSHALLER(NetSocketTypeEnum, uint8, 8)
    DECLARE_SCALAR_MARSHALLER(SystemPreferencesChoice, uint8, 8)

#if 0  // CSBAD
    DECLARE_SCALAR_MARSHALLER(SysAppInfoPtr, uint32, 32)
    DECLARE_SCALAR_MARSHALLER(UInt8Ptr, uint32, 32)
#endif

#define DECLARE_STRUCT_MARSHALLER(type)                                      \
    static void Get##type(emuptr p, type&);                                  \
    static void Put##type(emuptr p, const type&);                            \
                                                                             \
    static long GetBufSize(const type&) { return EmProxy##type::GetSize(); } \
                                                                             \
    inline static void GetParamRef(emuptr p, type& v) { Get##type(p, v); }   \
                                                                             \
    inline static void PutParamRef(emuptr p, const type& v) { Put##type(p, v); }

    DECLARE_STRUCT_MARSHALLER(DlkServerSessionType)
    DECLARE_STRUCT_MARSHALLER(DmSearchStateType)
    DECLARE_STRUCT_MARSHALLER(EventType)
    DECLARE_STRUCT_MARSHALLER(FieldAttrType)
    DECLARE_STRUCT_MARSHALLER(HostGremlinInfoType)
    DECLARE_STRUCT_MARSHALLER(HostStatType)
    DECLARE_STRUCT_MARSHALLER(HostTmType)
    DECLARE_STRUCT_MARSHALLER(HostUTimeType)
    DECLARE_STRUCT_MARSHALLER(HwrBatCmdReadType)
    DECLARE_STRUCT_MARSHALLER(NetSocketAddrType)
    DECLARE_STRUCT_MARSHALLER(NetIOParamType)
    DECLARE_STRUCT_MARSHALLER(NetHostInfoBufType)
    DECLARE_STRUCT_MARSHALLER(NetServInfoBufType)
    DECLARE_STRUCT_MARSHALLER(PointType)
    DECLARE_STRUCT_MARSHALLER(RectangleType)
    DECLARE_STRUCT_MARSHALLER(SndCommandType)
    DECLARE_STRUCT_MARSHALLER(SysAppInfoType)
    DECLARE_STRUCT_MARSHALLER(SysKernelInfoType)
    DECLARE_STRUCT_MARSHALLER(SysNVParamsType)
};

/* ===========================================================================
        Class that manages an immediate value from the emulated stack. This
        class can fetch the value from the stack and produce that value via a
        type operator.  The value cannot be changed, you can't take the address
        of it, nor can the value be written back to the stack.
=========================================================================== */

template <typename T>
class ParamVal {
   public:
    ParamVal(EmSubroutine& sub, EmParamNameArg name) { Marshal::GetParamVal(sub, name, fVal); }

    operator T(void) { return fVal; }

   private:
    T fVal;
};

/* ===========================================================================
        Class that manages a value passed by reference on the emulated stack.
        This class can fetch the pointer to the value and make a copy of the
        value pointed to if the pointer was NULL.  The class can produce a
        pointer to the local copy of the value, or NULL if the parameter was
        NULL.  The class has a Put method which copies the local copy back to
        the emulated memory the original came from.  Whether or not the
        referenced value is fetched or returned is determined by the inOut
        template parameter.  If the kInput bit is set, the value is copied from
        emulated memory to a local copy.  If the kOutput bit is set, the local
        copy is copied back to emulated memory when the Put method is called.
=========================================================================== */

template <typename T, long inOut>
class ParamRef {
   public:
    ParamRef(EmSubroutine& sub, EmParamNameArg name) : fSub(&sub), fName(name), fPtr(EmMemNULL) {
        // Get and cache the pointer to the data.

        fSub->GetParamVal(fName.c_str(), fPtr);

        // If there's a pointer and this is an input
        // variable, get the data.

        if (fPtr && INPUT(inOut)) {
            Marshal::GetParamRef(fPtr, fVal);
        }
    }

    void Put(void) {
        // If there's a pointer and this is an output
        // variable, store the data.

        if (fPtr && OUTPUT(inOut)) {
            Marshal::PutParamRef(fPtr, fVal);
        }
    }

    operator T*(void) { return fPtr ? &fVal : NULL; }
    const T& operator*(void) const { return fVal; }
    T& operator*(void) { return fVal; }

    operator emuptr(void) { return fPtr; }

   private:
    EmSubroutine* fSub;
    EmParamName fName;
    emuptr fPtr;
    T fVal;
};

/* ===========================================================================
        Class that manages a block of memory pointed to by a pointer on the
        emulated stack.  If the pointer is non-NULL, a local block of memory is
        allocated.  If the kInput bit of the inOut template parameter is set,
        the range of emulated memory is copied into the local buffer. This class
        can produce a pointer to the local block of memory on demand (if the
        stack parameter was NULL, this class produces NULL). If the kOutput bit
        of the inOut template paraemter is set, the local block's contents are
        copied back into emulated memory when the Put method is called.
        Regardless of inOut values, the Put method must be called in order to
        release the lock block of memory.
=========================================================================== */

template <typename T, long inOut>
class ParamPtr {
   public:
    ParamPtr(EmSubroutine& sub, EmParamNameArg name, long len)
        : fSub(&sub), fName(name), fPtr(EmMemNULL), fLen(len), fVal(NULL) {
        // Get and cache the pointer to the data.

        fSub->GetParamVal(fName.c_str(), fPtr);

        if (fPtr) {
            fVal = (T*)Platform::AllocateMemory(fLen);
            if (fVal && INPUT(inOut)) {
                EmMem_memcpy((void*)fVal, fPtr, fLen);
            }
        }
    }

    ~ParamPtr()  // !!! Update comments about d'tors and disposing memory
    {
        Platform::DisposeMemory(fVal);
    }

    void Put(void) {
        if (fPtr && fVal && OUTPUT(inOut)) {
            EmMem_memcpy(fPtr, (const void*)fVal, fLen);
        }
    }

    operator T*(void) { return fVal; }

    operator emuptr(void) { return fPtr; }

   private:
    EmSubroutine* fSub;
    EmParamName fName;
    emuptr fPtr;
    long fLen;
    T* fVal;
};

/* ===========================================================================
        Class that manages a NULL-terminated range of memory pointed to by a
        pointer on the emulated stack.  The input pointer can be NULL. The
        elements of the string can be anything, but will normally be Char
        (actually, they may *have* to be Char, since I get the length of the
        string by calling EmMem_strlen).  The string is modifiable in-place, but
        any changes are not copied back to emulated memory (this behavior can be
        changed if needed).
=========================================================================== */

template <typename T>
class ParamStr {
   public:
    ParamStr(EmSubroutine& sub, EmParamNameArg name)
        : fSub(&sub), fName(name), fPtr(EmMemNULL), fVal(NULL) {
        fSub->GetParamVal(fName.c_str(), fPtr);

        if (fPtr) {
            fVal = (T*)Platform::AllocateMemory(EmMem_strlen(fPtr) + 1);
            if (fVal) {
                EmMem_strcpy(fVal, fPtr);
            }
        }
    }

    ~ParamStr()  // !!! Update comments about d'tors and disposing memory
    {
        Platform::DisposeMemory(fVal);
    }

    void Put(void) {}

    operator T*(void) { return fVal; }

    operator emuptr(void) { return fPtr; }

   private:
    EmSubroutine* fSub;
    EmParamName fName;
    emuptr fPtr;
    T* fVal;
};

template <typename T>
class PushParamVal {
   public:
    PushParamVal(EmSubroutine& sub, EmParamNameArg name, T val) {
        Marshal::PutParamVal(sub, name, val);
    }
};

template <typename T, long inOut>
class PushParamRef {
   public:
    PushParamRef(EmSubroutine& sub, EmParamNameArg name, T* ptr)
        : fSub(&sub), fName(name), fHostPtr(ptr), fMappedData(NULL), fMappedPtr(EmMemNULL) {
        if (fHostPtr) {
            // Allocate a buffer big enough for the mapped/translated data.

            long size = Marshal::GetBufSize(*fHostPtr);
            fMappedData = Platform::AllocateMemory(size);

            // Map the buffer

            EmBankMapped::MapPhysicalMemory(fMappedData, size);
            fMappedPtr = EmBankMapped::GetEmulatedAddress(fMappedData);

            // Copy/translate the data into it.

            if (INPUT(inOut)) {
                Marshal::PutParamRef(fMappedPtr, *fHostPtr);
            }

            // Pass the pointer to the data.

            fSub->SetParamVal(fName.c_str(), fMappedPtr);
        } else {
            fMappedPtr = EmMemNULL;
        }

        // Pass the pointer to the data.

        fSub->SetParamVal(fName.c_str(), fMappedPtr);
    }

    ~PushParamRef(void) {
        if (fMappedData) {
            EmBankMapped::UnmapPhysicalMemory(fMappedData);
            Platform::DisposeMemory(fMappedData);
        }
    }

    void Get(void) {
        if (fHostPtr && OUTPUT(inOut)) {
            Marshal::GetParamRef(fMappedPtr, *fHostPtr);
        }
    }

   private:
    EmSubroutine* fSub;
    EmParamName fName;
    T* fHostPtr;
    void* fMappedData;
    emuptr fMappedPtr;
};

template <typename T, long inOut>
class PushParamPtr {
   public:
    PushParamPtr(EmSubroutine& sub, EmParamNameArg name, const T* ptr, long size)
        : fSub(&sub), fName(name), fHostPtr(ptr), fMappedPtr(EmMemNULL) {
        if (fHostPtr) {
            // Map the buffer

            EmBankMapped::MapPhysicalMemory(fHostPtr, size);
            fMappedPtr = EmBankMapped::GetEmulatedAddress(fHostPtr);
        } else {
            fMappedPtr = EmMemNULL;
        }

        // Pass the pointer to the data.

        fSub->SetParamVal(fName.c_str(), fMappedPtr);
    }

    ~PushParamPtr(void) {
        if (fHostPtr) {
            EmBankMapped::UnmapPhysicalMemory(fHostPtr);
        }
    }

   private:
    EmSubroutine* fSub;
    EmParamName fName;
    const T* fHostPtr;
    emuptr fMappedPtr;
};

template <typename T>
class PushParamStr {
   public:
    PushParamStr(EmSubroutine& sub, EmParamNameArg name, const T* const ptr)
        : fSub(&sub), fName(name), fHostPtr(ptr), fMappedPtr(EmMemNULL) {
        if (fHostPtr) {
            // Map the buffer

            long size = strlen(ptr) + 1;
            EmBankMapped::MapPhysicalMemory(fHostPtr, size);
            fMappedPtr = EmBankMapped::GetEmulatedAddress(fHostPtr);
        } else {
            fMappedPtr = EmMemNULL;
        }

        // Pass the pointer to the data.

        fSub->SetParamVal(fName.c_str(), fMappedPtr);
    }

    ~PushParamStr(void) {
        if (fHostPtr) {
            EmBankMapped::UnmapPhysicalMemory(fHostPtr);
        }
    }

   private:
    EmSubroutine* fSub;
    EmParamName fName;
    const T* const fHostPtr;
    emuptr fMappedPtr;
};

#endif  // _MARSHAL_H_
