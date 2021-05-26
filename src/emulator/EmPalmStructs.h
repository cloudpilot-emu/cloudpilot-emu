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

#ifndef EmPalmStructs_h
#define EmPalmStructs_h

#include <stddef.h>  // ptrdiff_t

#include "EmCommon.h"
#include "EmMemory.h"  // EmMemGet8, EmMemPut8, EmMem_memcpy, etc.
#include "EmPalmStructs.i"
#include "HostControl.h"  // HostControlSelectorType, etc.

/*
        The Palm OS Emulator interacts closely with legacy Palm OS data
        structures.  These data structures are laid out by the compilers
        designed for the Palm OS.  However, the Palm OS Emulator -- which needs
        to access these structures at times -- is NOT necessarily compiled with
        a compiler that lays out the structures in a compatible fashion.  Thus,
        if the emulator needs to muck with, say, a variable of EventType, it
        can't merely create one and have at it.  Nor can it get a pointer to
        one, cast it to an EventType*, and have at it.  There's just no guarantee
        that the compiler used to build Poser will put the fields in the same
        places that the Palm OS expects it, or that the architecture will
        support accessing fields in those places.

        To solve this problem, for each Palm OS struct Poser wants to
        manipulate, it creates two parallel "Rep" classes: an "Alias" class and
        a "Proxy" class.  These Rep classes appear and act as much as the
        original struct as possible.  However, behind the scenes C++ magic
        allows Poser to access the data in a manner that is guaranteed to be
        binary-compatible with the Palm OS structs, regardless of what platform
        it's running on and what compiler it was built with.

        Aliases and Proxies are very similar to each other in the way the user
        interacts with them.  The have fields which, when used as an lvalue,
        will store the proferred data in the correct memory location and with
        the correct endianness and, when used as an rvalue, will produce the
        necessary value, fetching the data from the correct memory location and
        byteswapping it if necessary.

        Aliases and Proxies differ in their backing store.  An Alias class is
        initialized with a pointer to some buffer that already exists.  This
        buffer may exist as a Poser variable or as a buffer in the emulated Palm
        OS space.  All data accesses are made against that provided buffer.
        When an Alias is created from another Alias, they both end up pointing
        to the same space.  If one Alias is used to modify the buffer, the
        second Alias will also reflect that change.

        Proxies, on the other hand, are more stand-alone.  They provide their
        own buffers as a backing store.  Copying one Proxy to another or
        creating one Proxy from another results in a new independent object.
        After the copy has been made, modifying one object does not affect the
        second.

        Aliases are not only handy for wrapping a chunk of data that was
        generated elsewhere, but they are also essential for providing an "array
        operator" (that is, "operator[]") for both kinds of Rep classes.  That
        operator returns an Alias that points to the right location in memory.
        If we didn't have Aliases -- for instance, we only had Proxies -- then
        the Proxy returned by operator[] could not be used to modify the backing
        store, since it's independent of the backing store used by the object
        that created it.

        Proxies, on the other hand, are handy when you need to treat an object
        more like a real-live struct.  Proxies can be copied and put into
        STL-like collections without having to worry about the lifetime of the
        object they were created from.

        Let's look at the Alias class for EventType:

                template <class A>
                class EmAliasEventType
                {
                        public:
                                typedef typename A::ptr_type ptr_type;

                                EmAliasEventType (ptr_type);
                                EmAliasEventType (const EmAliasEventType&);

                                ptr_type GetPtr (void) const;
                                static size_t GetSize (void);

                                EmAliasEventType<A>& operator= (const EmAliasEventType<A>&);
                                EmAliasEventType<A> operator[] (int);

                        private:
                                EmAliasEventType (void);

                                EmAliasEventType<A>& operator= (const bool);
                                EmAliasEventType<A>& operator= (const char);
                                EmAliasEventType<A>& operator= (const unsigned char);
                                EmAliasEventType<A>& operator= (const unsigned short);
                                EmAliasEventType<A>& operator= (const unsigned int);
                                EmAliasEventType<A>& operator= (const unsigned long);
                                EmAliasEventType<A>& operator= (const signed char);
                                EmAliasEventType<A>& operator= (const signed short);
                                EmAliasEventType<A>& operator= (const signed int);
                                EmAliasEventType<A>& operator= (const signed long);
                                EmAliasEventType<A>& operator= (const void*);

                                ptr_type fPtr;

                        public:
                                EmAliasInt16<A> eType;
                                EmAliasBoolean<A> penDown;
                                EmAliasUInt8<A> tapCount;
                                EmAliasInt16<A> screenX;
                                EmAliasInt16<A> screenY;
                                EmAliasEventTypeData<A> data;
                };

        This is a template class parameterized with a data accessor.  This data
        accessor has static methods for reading and writing 1-, 2-, and 4-byte
        values.  In this module, we define two such accessors.  One, called LAS
        for Local Address Space, accesses Big Endian values in Poser's address
        space.  The second, called PAS for Palm Address Space, accesses Big
        Endian values in the emulated address space.

        There are two constructors.  The first constructor takes a pointer to an
        appropriately-sized buffer that already exists -- all reads and write
        take place against that buffer.  The second constructor is the copy
        constructor.  It copies the pointer used by the original object.  After
        the copy, if the original object modifies the data in the backing store,
        the copy with reflect that change, too.

        GetPtr returns the pointer to the buffer backing the struct.  The return
        type of this function is based on the class the proxy class is
        instantiated with.  It is void* if used with LAS, and emuptr if used
        with PAS.

        GetSize returns the size of the represented struct, in this case
        EventType, which is 24 bytes long.

        An assignment operator is provided to perform a memberwise copy
        operation.

        An array operator is provided to return Alias objects that point to the
        appropriately index location in memory.

        Following these standard methods are the fields.  These fields are also
        Alias classes, initialized with the base pointer passed into the
        EmAliasEventType constructor plus an appropriate offset.

        Now let's take a look at a Proxy class:

                class EmProxyEventType
                {
                        public:
                                typedef void ptr_type;

                                EmProxyEventType (void);
                                EmProxyEventType (const EmProxyEventType&);

                                ptr_type GetPtr (void) const;
                                static size_t GetSize (void);

                                EmProxyEventType& operator= (const EmProxyEventType&);
                                EmAliasEventType<LAS> operator[] (int);

                        private:
                                EmProxyEventType& operator= (const bool);
                                EmProxyEventType& operator= (const char);
                                EmProxyEventType& operator= (const unsigned char);
                                EmProxyEventType& operator= (const unsigned short);
                                EmProxyEventType& operator= (const unsigned int);
                                EmProxyEventType& operator= (const unsigned long);
                                EmProxyEventType& operator= (const signed char);
                                EmProxyEventType& operator= (const signed short);
                                EmProxyEventType& operator= (const signed int);
                                EmProxyEventType& operator= (const signed long);
                                EmProxyEventType<& operator= (const void*);

                                char fLocalData[24];

                        public:
                                EmAliasInt16<LAS> eType;
                                EmAliasBoolean<LAS> penDown;
                                EmAliasUInt8<LAS> tapCount;
                                EmAliasInt16<LAS> screenX;
                                EmAliasInt16<LAS> screenY;
                                EmAliasEventTypeData<LAS> data;
                };

        As you can see, this Proxy class is almost the same as an Alias class.
        However:

        *	Instead of having an fPtr field to hold the pointer to the data
                buffer, it has an fLocalData field that holds the data itself.

        *	There is no constructor that accepts a pointer.  Instead, there
                is a public default constructor.

        *	It is not a template class.  Since the data is local, it is
                always accessed in a manner consistant with the LAS accessor.

        *	The copy constructor copies the data instead of the pointer.

        There are two sorts of Rep classes: primitive and aggregate.  Primitive
        Rep classes are responsible for the management of simple scalar types.
        Aggregate Rep classes represent structs, and are composed of other
        primitive and aggregate Alias classes.  The biggest difference between
        primitive and aggregate Rep classes is the set of assignment operators
        clients are allowed to call.  For instance, you can assign integers to
        primitive Rep classes, but not to aggregate Rep classes

        EXAMPLE:

                Say you wanted to call void EvtAddEventToQueue (EventType*).
                You would use EmEventType as follows:

                EmProxyEventType	event;	// Use local buffer

                event.eType		= penDownEvent;
                event.penDown	= true;
                event.screenX	= 100;
                event.screenY	= 100;

                EvtAddEventToQueue((EventType*) event.GetPtr ());

        EFFICIENCY:

                There is some overhead using these classes instead of using structs
                directly.  At the very least, there's an additional level of
                redirection, as the fields accessed contain pointers to the actual
                data as opposed to the actual data itself.  Additionally, on Little
                Endian systems, there is the cost of byteswapping on every access.
                Finally, there is the cost of calling and executing the proxy class
                constructor.

                In EmPalmStructs.cpp, there are two functions: Test1 and Test2.  The
                first accesses the struct SysPktRPC2Type using the proxy class, the
                second using the struct directly.

                On the Mac, using CW Pro 5.2 with optimizations on, Test1 is 88
                bytes, calls an out-of-line constructor, and each field access takes
                one additional level of indirection.  Test2 is 64 bytes.  The 24-byte
                difference is taken up by an additional 4 bytes for each of the
                assignments, and an additional 12 bytes to call the proxy class's
                constructor.

        LIMITATIONS:

        *	Can't take the adress of an EmFoo.  Will get *its* address
                instead of what you want (the address of the storage).  Instead,
                call GetPtr.

        *	Need explicit casts to extract the value in cases where the
                type is ambigous.  Classic case is "printf("%d", my_EmInt)".  The
                object will be pushed onto the stack, not the wrapped integer.

        *	Can't take the size of the object.  Instead, call GetSize.

        *	Can't call offsetof to get field offsets.  Instead, call
                offsetof_<fieldName>.  Note that this returns the offset of the most
                immediate struct.  That is, struct1.struct2.offsetof_field()
                returns the offset from the beginning of struct2 to field.
                Calling offsetof(StructType1, struct2.field) returns the offset
                from the beginning of struct1 to the field.

        *	Need a way to copy between classes instantiated with different
                address space objects.
*/

int BadGetter(void);
void BadSetter(void);

#define PUT_BE_LONG(p, v)                                                \
    do {                                                                 \
        ((unsigned char*)(p))[0] = (unsigned char)(((UInt32)(v)) >> 24); \
        ((unsigned char*)(p))[1] = (unsigned char)(((UInt32)(v)) >> 16); \
        ((unsigned char*)(p))[2] = (unsigned char)(((UInt32)(v)) >> 8);  \
        ((unsigned char*)(p))[3] = (unsigned char)(((UInt32)(v)) >> 0);  \
    } while (0)

#define PUT_BE_SHORT(p, v)                                              \
    do {                                                                \
        ((unsigned char*)(p))[0] = (unsigned char)(((UInt16)(v)) >> 8); \
        ((unsigned char*)(p))[1] = (unsigned char)(((UInt16)(v)) >> 0); \
    } while (0)

#define PUT_BE_BYTE(p, v)                              \
    do {                                               \
        ((unsigned char*)(p))[0] = (unsigned char)(v); \
    } while (0)

#define GET_BE_LONG(p)                                           \
    (UInt32)((((unsigned long)((unsigned char*)(p))[0]) << 24) | \
             (((UInt32)((unsigned char*)(p))[1]) << 16) |        \
             (((UInt32)((unsigned char*)(p))[2]) << 8) |         \
             (((UInt32)((unsigned char*)(p))[3]) << 0))

#define GET_BE_SHORT(p) \
    (UInt16)((((UInt16)((unsigned char*)(p))[0]) << 8) | (((UInt16)((unsigned char*)(p))[1]) << 0))

#define GET_BE_BYTE(p) (unsigned char)((((unsigned char)((unsigned char*)(p))[0]) << 0))

// Provide methods for getting and setting Big Endian data in Local Address Space

class LAS {
   public:
    typedef void* ptr_type;

    inline static uint8 GetByte(ptr_type p) { return (uint8)GET_BE_BYTE(p); }
    inline static uint16 GetWord(ptr_type p) { return (uint16)GET_BE_SHORT(p); }
    inline static uint32 GetLong(ptr_type p) { return (uint32)GET_BE_LONG(p); }
    inline static void PutByte(ptr_type p, const uint8 v) { PUT_BE_BYTE(p, v); }
    inline static void PutWord(ptr_type p, const uint16 v) { PUT_BE_SHORT(p, v); }
    inline static void PutLong(ptr_type p, const uint32 v) { PUT_BE_LONG(p, v); }

    inline static void BlockCopy(ptr_type d, ptr_type s, size_t len) { memcpy(d, s, len); }

    inline static ptr_type add(ptr_type p, ptrdiff_t diff) {
        return (ptr_type)(((size_t)p) + diff);
    }
    inline static ptrdiff_t diff(ptr_type a, ptr_type b) {
        return (ptrdiff_t)(((ptrdiff_t)a) - (ptrdiff_t)b);
    }
};

// Provide methods for getting and setting Big Endian data in Palm Addres Space

class PAS {
   public:
    typedef emuptr ptr_type;

    inline static uint8 GetByte(ptr_type p) { return (uint8)EmMemGet8(p); }
    inline static uint16 GetWord(ptr_type p) { return (uint16)EmMemGet16(p); }
    inline static uint32 GetLong(ptr_type p) { return (uint32)EmMemGet32(p); }
    inline static void PutByte(ptr_type p, const uint8 v) { EmMemPut8(p, v); }
    inline static void PutWord(ptr_type p, const uint16 v) { EmMemPut16(p, v); }
    inline static void PutLong(ptr_type p, const uint32 v) { EmMemPut32(p, v); }

    inline static void BlockCopy(ptr_type d, ptr_type s, size_t len) { EmMem_memcpy(d, s, len); }

    inline static ptr_type add(ptr_type p, ptrdiff_t diff) {
        return (ptr_type)(((size_t)p) + diff);
    }
    inline static ptrdiff_t diff(ptr_type a, ptr_type b) {
        return (ptrdiff_t)(((ptrdiff_t)a) - (ptrdiff_t)b);
    }
};

// ======================================================================
// Macro for creating the class that wraps up simple types.
// ======================================================================

#define DECLARE_SCALAR_ALIAS(type, asType)                     \
                                                               \
    template <class A>                                         \
    class EmAlias##type {                                      \
       public:                                                 \
        typedef typename A::ptr_type ptr_type;                 \
                                                               \
        EmAlias##type(ptr_type);                               \
        EmAlias##type(const EmAlias##type<A>&);                \
                                                               \
        ptr_type GetPtr(void) const { return fPtr; }           \
        static size_t GetSize(void) { return sizeof(asType); } \
                                                               \
        EmAlias##type<A>& operator=(const bool);               \
        EmAlias##type<A>& operator=(const char);               \
                                                               \
        EmAlias##type<A>& operator=(const unsigned char);      \
        EmAlias##type<A>& operator=(const UInt16);             \
        EmAlias##type<A>& operator=(const UInt32);             \
                                                               \
        EmAlias##type<A>& operator=(const signed char);        \
        EmAlias##type<A>& operator=(const Int16);              \
        EmAlias##type<A>& operator=(const Int32);              \
                                                               \
        EmAlias##type<A>& operator=(const void*);              \
        EmAlias##type<A>& operator=(const EmAlias##type<A>&);  \
                                                               \
        operator type(void) const;                             \
                                                               \
        EmAlias##type<A> operator[](int);                      \
        const EmAlias##type<A> operator[](int) const;          \
                                                               \
       private:                                                \
        EmAlias##type(void);                                   \
                                                               \
        ptr_type fPtr;                                         \
    };

#define DECLARE_SCALAR_PROXY(type, asType)                            \
                                                                      \
    class EmProxy##type {                                             \
       public:                                                        \
        typedef void* ptr_type;                                       \
                                                                      \
        EmProxy##type(void);                                          \
        EmProxy##type(const EmProxy##type&);                          \
                                                                      \
        ptr_type GetPtr(void) const { return (ptr_type)&fLocalData; } \
        static size_t GetSize(void) { return sizeof(asType); }        \
                                                                      \
        EmProxy##type& operator=(const bool);                         \
        EmProxy##type& operator=(const char);                         \
                                                                      \
        EmProxy##type& operator=(const unsigned char);                \
        EmProxy##type& operator=(const UInt16);                       \
        EmProxy##type& operator=(const UInt32);                       \
                                                                      \
        EmProxy##type& operator=(const signed char);                  \
        EmProxy##type& operator=(const Int16);                        \
        EmProxy##type& operator=(const Int32);                        \
                                                                      \
        EmProxy##type& operator=(const void*);                        \
        EmProxy##type& operator=(const EmProxy##type&);               \
                                                                      \
        operator type(void) const;                                    \
                                                                      \
        EmAlias##type<LAS> operator[](int);                           \
        const EmAlias##type<LAS> operator[](int) const;               \
                                                                      \
       private:                                                       \
        struct {                                                      \
            char _bytes[sizeof(asType)];                              \
        } fLocalData;                                                 \
    };

#define DECLARE_SCALAR_CLASSES(type, asType) \
    DECLARE_SCALAR_ALIAS(type, asType)       \
    DECLARE_SCALAR_PROXY(type, asType)

#define DEFINE_SCALAR_ALIAS(type, asType)                                                          \
                                                                                                   \
    template <class A>                                                                             \
    INLINE_ EmAlias##type<A>::EmAlias##type(void) : fPtr((ptr_type)NULL) {}                        \
                                                                                                   \
    template <class A>                                                                             \
    INLINE_ EmAlias##type<A>::EmAlias##type(ptr_type p) : fPtr(p) {}                               \
                                                                                                   \
    template <class A>                                                                             \
    INLINE_ EmAlias##type<A>::EmAlias##type(const EmAlias##type<A>& that) : fPtr(that.GetPtr()) {} \
                                                                                                   \
    MAKE_ONE_SCALAR_ALIAS_ASSIGNMENT_OPERATOR(type, asType, bool)                                  \
    MAKE_ONE_SCALAR_ALIAS_ASSIGNMENT_OPERATOR(type, asType, char)                                  \
    MAKE_ONE_SCALAR_ALIAS_ASSIGNMENT_OPERATOR(type, asType, signed char)                           \
    MAKE_ONE_SCALAR_ALIAS_ASSIGNMENT_OPERATOR(type, asType, Int16)                                 \
    MAKE_ONE_SCALAR_ALIAS_ASSIGNMENT_OPERATOR(type, asType, Int32)                                 \
    MAKE_ONE_SCALAR_ALIAS_ASSIGNMENT_OPERATOR(type, asType, unsigned char)                         \
    MAKE_ONE_SCALAR_ALIAS_ASSIGNMENT_OPERATOR(type, asType, UInt16)                                \
    MAKE_ONE_SCALAR_ALIAS_ASSIGNMENT_OPERATOR(type, asType, UInt32)                                \
    MAKE_ONE_SCALAR_ALIAS_ASSIGNMENT_OPERATOR(type, asType, const void*)                           \
    MAKE_ONE_SCALAR_ALIAS_ASSIGNMENT_OPERATOR(type, asType, const EmAlias##type<A>&)               \
                                                                                                   \
    template <class A>                                                                             \
    INLINE_ EmAlias##type<A>::operator type(void) const {                                          \
        if (sizeof(asType) == 1) return (type)A::GetByte((ptr_type)fPtr);                          \
                                                                                                   \
        if (sizeof(asType) == 2) return (type)A::GetWord((ptr_type)fPtr);                          \
                                                                                                   \
        if (sizeof(asType) == 4) return (type)A::GetLong((ptr_type)fPtr);                          \
                                                                                                   \
        return (type)BadGetter();                                                                  \
    }                                                                                              \
                                                                                                   \
    template <class A>                                                                             \
    INLINE_ EmAlias##type<A> EmAlias##type<A>::operator[](int index) {                             \
        return EmAlias##type<A>((ptr_type)(((long)this->GetPtr()) + index * this->GetSize()));     \
    }                                                                                              \
                                                                                                   \
    template <class A>                                                                             \
    INLINE_ const EmAlias##type<A> EmAlias##type<A>::operator[](int index) const {                 \
        return EmAlias##type<A>((ptr_type)(((long)this->GetPtr()) + index * this->GetSize()));     \
    }

#define DEFINE_SCALAR_PROXY(type, asType)                                                        \
                                                                                                 \
    INLINE_ EmProxy##type::EmProxy##type(void) {}                                                \
                                                                                                 \
    INLINE_ EmProxy##type::EmProxy##type(const EmProxy##type& that) {                            \
        *this = (asType)(type)that;                                                              \
    }                                                                                            \
                                                                                                 \
    MAKE_ONE_SCALAR_PROXY_ASSIGNMENT_OPERATOR(type, asType, bool)                                \
    MAKE_ONE_SCALAR_PROXY_ASSIGNMENT_OPERATOR(type, asType, char)                                \
    MAKE_ONE_SCALAR_PROXY_ASSIGNMENT_OPERATOR(type, asType, signed char)                         \
    MAKE_ONE_SCALAR_PROXY_ASSIGNMENT_OPERATOR(type, asType, Int16)                               \
    MAKE_ONE_SCALAR_PROXY_ASSIGNMENT_OPERATOR(type, asType, Int32)                               \
    MAKE_ONE_SCALAR_PROXY_ASSIGNMENT_OPERATOR(type, asType, unsigned char)                       \
    MAKE_ONE_SCALAR_PROXY_ASSIGNMENT_OPERATOR(type, asType, UInt16)                              \
    MAKE_ONE_SCALAR_PROXY_ASSIGNMENT_OPERATOR(type, asType, UInt32)                              \
    MAKE_ONE_SCALAR_PROXY_ASSIGNMENT_OPERATOR(type, asType, const void*)                         \
    MAKE_ONE_SCALAR_PROXY_ASSIGNMENT_OPERATOR(type, asType, const EmProxy##type&)                \
                                                                                                 \
    INLINE_ EmProxy##type::operator type(void) const {                                           \
        if (sizeof(asType) == 1) return (type)LAS::GetByte((ptr_type)&fLocalData);               \
                                                                                                 \
        if (sizeof(asType) == 2) return (type)LAS::GetWord((ptr_type)&fLocalData);               \
                                                                                                 \
        if (sizeof(asType) == 4) return (type)LAS::GetLong((ptr_type)&fLocalData);               \
                                                                                                 \
        return (type)BadGetter();                                                                \
    }                                                                                            \
                                                                                                 \
    INLINE_ EmAlias##type<LAS> EmProxy##type::operator[](int index) {                            \
        return EmAlias##type<LAS>((ptr_type)(((long)this->GetPtr()) + index * this->GetSize())); \
    }                                                                                            \
                                                                                                 \
    INLINE_ const EmAlias##type<LAS> EmProxy##type::operator[](int index) const {                \
        return EmAlias##type<LAS>((ptr_type)(((long)this->GetPtr()) + index * this->GetSize())); \
    }

#define DEFINE_SCALAR_CLASSES(type, asType) \
    DEFINE_SCALAR_ALIAS(type, asType)       \
    DEFINE_SCALAR_PROXY(type, asType)

// Macro that creates the implementation for an assignment operator for simple types.

#define MAKE_ONE_SCALAR_ALIAS_ASSIGNMENT_OPERATOR(type, asType, rhs_type) \
                                                                          \
    template <class A>                                                    \
    INLINE_ EmAlias##type<A>& EmAlias##type<A>::operator=(rhs_type val) { \
        if (sizeof(asType) == 1)                                          \
            A::PutByte(this->GetPtr(), (unsigned char)(asType)(long)val); \
        else if (sizeof(asType) == 2)                                     \
            A::PutWord(this->GetPtr(), (UInt16)(asType)(long)val);        \
        else if (sizeof(asType) == 4)                                     \
            A::PutLong(this->GetPtr(), (UInt32)(asType)(long)val);        \
        else                                                              \
            BadSetter();                                                  \
                                                                          \
        return *this;                                                     \
    }

#define MAKE_ONE_SCALAR_PROXY_ASSIGNMENT_OPERATOR(type, asType, rhs_type)   \
                                                                            \
    INLINE_ EmProxy##type& EmProxy##type::operator=(rhs_type val) {         \
        if (sizeof(asType) == 1)                                            \
            LAS::PutByte(this->GetPtr(), (unsigned char)(asType)(long)val); \
        else if (sizeof(asType) == 2)                                       \
            LAS::PutWord(this->GetPtr(), (UInt16)(asType)(long)val);        \
        else if (sizeof(asType) == 4)                                       \
            LAS::PutLong(this->GetPtr(), (UInt32)(asType)(long)val);        \
        else                                                                \
            BadSetter();                                                    \
                                                                            \
        return *this;                                                       \
    }

// ======================================================================
// Macro for creating the class that wraps up aggregate types.
// ======================================================================

#define DECLARE_STRUCT_ALIAS(type, size, FOR_EACH_FIELD)      \
                                                              \
    template <class A>                                        \
    class EmAlias##type {                                     \
       public:                                                \
        typedef typename A::ptr_type ptr_type;                \
                                                              \
        EmAlias##type(ptr_type);                              \
        EmAlias##type(const EmAlias##type&);                  \
                                                              \
        ptr_type GetPtr(void) const { return fPtr; }          \
        static size_t GetSize(void) { return size; }          \
                                                              \
        EmAlias##type<A>& operator=(const EmAlias##type<A>&); \
                                                              \
        EmAlias##type<A> operator[](int);                     \
        const EmAlias##type<A> operator[](int) const;         \
                                                              \
       private:                                               \
        EmAlias##type(void);                                  \
                                                              \
        ptr_type fPtr;                                        \
                                                              \
       public:                                                \
        FOR_EACH_FIELD(DECLARE_FIELD_ALIAS)                   \
        FOR_EACH_FIELD(DECLARE_FIELD_OFFSET)                  \
    };

#define DECLARE_STRUCT_PROXY(type, size, FOR_EACH_FIELD)              \
                                                                      \
    class EmProxy##type {                                             \
       public:                                                        \
        typedef void* ptr_type;                                       \
                                                                      \
        EmProxy##type(void);                                          \
        EmProxy##type(const EmProxy##type&);                          \
                                                                      \
        ptr_type GetPtr(void) const { return (ptr_type)&fLocalData; } \
        static size_t GetSize(void) { return size; }                  \
                                                                      \
        EmProxy##type& operator=(const EmProxy##type&);               \
                                                                      \
        EmAlias##type<LAS> operator[](int);                           \
        const EmAlias##type<LAS> operator[](int) const;               \
                                                                      \
       private:                                                       \
        struct {                                                      \
            char _bytes[size];                                        \
        } fLocalData;                                                 \
                                                                      \
       public:                                                        \
        FOR_EACH_FIELD(DECLARE_FIELD_PROXY)                           \
        FOR_EACH_FIELD(DECLARE_FIELD_OFFSET)                          \
    };

#define DECLARE_STRUCT_CLASSES(type, size, FOR_EACH_FIELD) \
    DECLARE_STRUCT_ALIAS(type, size, FOR_EACH_FIELD)       \
    DECLARE_STRUCT_PROXY(type, size, FOR_EACH_FIELD)

#define DEFINE_STRUCT_ALIAS(type, size, FOR_EACH_FIELD)                                        \
                                                                                               \
    template <class A>                                                                         \
    INLINE_ EmAlias##type<A>::EmAlias##type(void)                                              \
        : fPtr((ptr_type)NULL) FOR_EACH_FIELD(MAKE_MEMBER_INITIALIZER_ALIAS) {}                \
                                                                                               \
    template <class A>                                                                         \
    INLINE_ EmAlias##type<A>::EmAlias##type(ptr_type p)                                        \
        : fPtr(p) FOR_EACH_FIELD(MAKE_MEMBER_INITIALIZER_ALIAS) {}                             \
                                                                                               \
    template <class A>                                                                         \
    INLINE_ EmAlias##type<A>::EmAlias##type(const EmAlias##type<A>& that)                      \
        : fPtr(that.fPtr) FOR_EACH_FIELD(MAKE_MEMBER_INITIALIZER_ALIAS) {}                     \
                                                                                               \
    template <class A>                                                                         \
    INLINE_ EmAlias##type<A>& EmAlias##type<A>::operator=(const EmAlias##type<A>& val) {       \
        if (this != &val) {                                                                    \
            A::BlockCopy(this->GetPtr(), val.GetPtr(), this->GetSize());                       \
        }                                                                                      \
                                                                                               \
        return *this;                                                                          \
    }                                                                                          \
                                                                                               \
    template <class A>                                                                         \
    INLINE_ EmAlias##type<A> EmAlias##type<A>::operator[](int index) {                         \
        return EmAlias##type<A>((ptr_type)(((long)this->GetPtr()) + index * this->GetSize())); \
    }                                                                                          \
                                                                                               \
    template <class A>                                                                         \
    INLINE_ const EmAlias##type<A> EmAlias##type<A>::operator[](int index) const {             \
        return EmAlias##type<A>((ptr_type)(((long)this->GetPtr()) + index * this->GetSize())); \
    }

#define DEFINE_STRUCT_PROXY(type, size, FOR_EACH_FIELD)                                          \
                                                                                                 \
    INLINE_ EmProxy##type::EmProxy##type(void)                                                   \
        : fLocalData() FOR_EACH_FIELD(MAKE_MEMBER_INITIALIZER_PROXY) {}                          \
                                                                                                 \
    INLINE_ EmProxy##type::EmProxy##type(const EmProxy##type& that)                              \
        : fLocalData() FOR_EACH_FIELD(MAKE_MEMBER_INITIALIZER_PROXY) {                           \
        *this = that;                                                                            \
    }                                                                                            \
                                                                                                 \
    INLINE_ EmProxy##type& EmProxy##type::operator=(const EmProxy##type& val) {                  \
        if (this != &val) {                                                                      \
            LAS::BlockCopy(this->GetPtr(), val.GetPtr(), this->GetSize());                       \
        }                                                                                        \
                                                                                                 \
        return *this;                                                                            \
    }                                                                                            \
                                                                                                 \
    INLINE_ EmAlias##type<LAS> EmProxy##type::operator[](int index) {                            \
        return EmAlias##type<LAS>((ptr_type)(((long)this->GetPtr()) + index * this->GetSize())); \
    }                                                                                            \
                                                                                                 \
    INLINE_ const EmAlias##type<LAS> EmProxy##type::operator[](int index) const {                \
        return EmAlias##type<LAS>((ptr_type)(((long)this->GetPtr()) + index * this->GetSize())); \
    }

#define DEFINE_STRUCT_CLASSES(type, size, FOR_EACH_FIELD) \
    DEFINE_STRUCT_ALIAS(type, size, FOR_EACH_FIELD)       \
    DEFINE_STRUCT_PROXY(type, size, FOR_EACH_FIELD)

// Macro for creating the data member that acts as a proxy for the data
// we're wrapping up.

#define DECLARE_FIELD_ALIAS(field_offset, field_type, field_name) EmAlias##field_type<A> field_name;

#define DECLARE_FIELD_PROXY(field_offset, field_type, field_name) \
                                                                  \
    EmAlias##field_type<LAS> field_name;

#define DECLARE_FIELD_OFFSET(field_offset, field_type, field_name) \
                                                                   \
    static size_t offsetof_##field_name(void) { return field_offset; }

// Macros for creating the data member initializers in the constructors

#define MAKE_MEMBER_INITIALIZER_ALIAS(field_offset, field_type, field_name) \
                                                                            \
    , field_name((ptr_type)(((long)fPtr) + field_offset))

#define MAKE_MEMBER_INITIALIZER_PROXY(field_offset, field_type, field_name) \
                                                                            \
    , field_name((ptr_type)(((long)this->GetPtr()) + field_offset))

typedef uint8 UIOptionsType;
typedef uint8 ScrOperation;

#define FOR_EACH_SCALAR_1(DO_TO_SCALAR) \
    DO_TO_SCALAR(char, char)            \
    DO_TO_SCALAR(Int8, Int8)            \
    DO_TO_SCALAR(UInt8, UInt8)          \
    DO_TO_SCALAR(Int16, Int16)          \
    DO_TO_SCALAR(UInt16, UInt16)        \
    DO_TO_SCALAR(Int32, Int32)          \
    DO_TO_SCALAR(UInt32, UInt32)

#define FOR_EACH_SCALAR_2(DO_TO_SCALAR)   \
    DO_TO_SCALAR(Boolean, uint8)          \
    DO_TO_SCALAR(Char, uint8)             \
    DO_TO_SCALAR(ControlStyleType, uint8) \
    DO_TO_SCALAR(Coord, uint16)           \
    DO_TO_SCALAR(DmResID, uint16)         \
    DO_TO_SCALAR(emuptr, emuptr)          \
    DO_TO_SCALAR(Err, uint16)             \
    DO_TO_SCALAR(FontID, uint8)           \
    DO_TO_SCALAR(FormObjectKind, uint8)   \
    DO_TO_SCALAR(LocalID, uint32)

#define FOR_EACH_SCALAR_3(DO_TO_SCALAR)       \
    DO_TO_SCALAR(NetIPAddr, uint32)           \
    DO_TO_SCALAR(PatternType, uint8)          \
    DO_TO_SCALAR(ScrOperation, uint8)         \
    DO_TO_SCALAR(SlkPktHeaderChecksum, uint8) \
    DO_TO_SCALAR(UnderlineModeType, uint8)    \
    DO_TO_SCALAR(UndoMode, uint8)             \
    DO_TO_SCALAR(UIOptionsType, uint8)        \
    DO_TO_SCALAR(WChar, uint16)

#define FOR_EACH_SCALAR_4(DO_TO_SCALAR)           \
    DO_TO_SCALAR(HostControlSelectorType, uint16) \
    DO_TO_SCALAR(HostBoolType, int32)             \
    DO_TO_SCALAR(HostClockType, int32)            \
    DO_TO_SCALAR(HostErrType, int32)              \
    DO_TO_SCALAR(HostIDType, int32)               \
    DO_TO_SCALAR(HostPlatformType, int32)         \
    DO_TO_SCALAR(HostSignalType, int32)           \
    DO_TO_SCALAR(HostSizeType, int32)             \
    DO_TO_SCALAR(HostTimeType, int32)

#define FOR_EACH_STRUCT_1(DO_TO_STRUCT)                                     \
    FOR_AbsRectType_STRUCT(DO_TO_STRUCT) FOR_PointType_STRUCT(DO_TO_STRUCT) \
        FOR_RectangleType_STRUCT(DO_TO_STRUCT) FOR_RGBColorType_STRUCT(DO_TO_STRUCT)

#define FOR_EACH_STRUCT_2(DO_TO_STRUCT)                                                     \
    FOR_BreakpointType_STRUCT(DO_TO_STRUCT) FOR_M68KRegsType_STRUCT(DO_TO_STRUCT)           \
                                                                                            \
        FOR_UsbHwrType_STRUCT(DO_TO_STRUCT) FOR_HwrBatCmdReadType_STRUCT(DO_TO_STRUCT)      \
            FOR_HwrJerryPLDType_STRUCT(DO_TO_STRUCT) FOR_HwrLAPType_STRUCT(DO_TO_STRUCT)    \
                FOR_HwrMediaQ11xxType_STRUCT(DO_TO_STRUCT)                                  \
                    FOR_HwrPalmI705PLDType_STRUCT(DO_TO_STRUCT)                             \
                        FOR_HwrSymbolASICType_STRUCT(DO_TO_STRUCT)                          \
                            FOR_SED1375RegsType_STRUCT(DO_TO_STRUCT)                        \
                                FOR_SED1376RegsType_STRUCT(DO_TO_STRUCT)                    \
                                                                                            \
                                    FOR_SysBatteryDataStructV1_STRUCT(DO_TO_STRUCT)         \
                                        FOR_SysBatteryDataStructV2_STRUCT(DO_TO_STRUCT)     \
                                            FOR_SysBatteryDataStructV3_STRUCT(DO_TO_STRUCT) \
                                                                                            \
                                                FOR_SysNVParamsType_STRUCT(DO_TO_STRUCT)

#define FOR_EACH_STRUCT_3(DO_TO_STRUCT)                                                         \
    FOR_ClipboardItem_STRUCT(DO_TO_STRUCT) FOR_FieldUndoType_STRUCT(DO_TO_STRUCT)               \
        FOR_GraphicStateTypeV1_STRUCT(DO_TO_STRUCT) FOR_GraphicStateTypeV2_STRUCT(DO_TO_STRUCT) \
            FOR_GraphicStateTypeV3_STRUCT(DO_TO_STRUCT) FOR_UIColorStateType_STRUCT(DO_TO_STRUCT)

#define FOR_EACH_STRUCT_4(DO_TO_STRUCT)                                                        \
    FOR_UIGlobalsType_STRUCT(DO_TO_STRUCT) FOR_UIGlobalsTypeCommon_STRUCT(DO_TO_STRUCT)        \
        FOR_UIGlobalsTypeV1_STRUCT(DO_TO_STRUCT) FOR_UIGlobalsTypeV2_STRUCT(DO_TO_STRUCT)      \
            FOR_UIGlobalsTypeV3_STRUCT(DO_TO_STRUCT) FOR_UIGlobalsTypeV31_STRUCT(DO_TO_STRUCT) \
                FOR_UIGlobalsTypeV32_STRUCT(DO_TO_STRUCT)                                      \
                    FOR_UIGlobalsTypeV35_STRUCT(DO_TO_STRUCT)

#define FOR_EACH_STRUCT_5(DO_TO_STRUCT)                                                         \
    FOR_M68KExcTableType_STRUCT(DO_TO_STRUCT) FOR_FixedGlobalsType_STRUCT(DO_TO_STRUCT)         \
        FOR_LowMemHdrType_STRUCT(DO_TO_STRUCT) FOR_SysAppInfoType_STRUCT(DO_TO_STRUCT)          \
            FOR_SysLibTblEntryType_STRUCT(DO_TO_STRUCT) FOR_PenBtnInfoType_STRUCT(DO_TO_STRUCT) \
                FOR_SndCommandType_STRUCT(DO_TO_STRUCT)                                         \
                    FOR_SysLibTblEntryTypeV10_STRUCT(DO_TO_STRUCT)                              \
                                                                                                \
                        FOR_kernel_info_task_STRUCT(DO_TO_STRUCT)                               \
                            FOR_kernel_info_semaphore_STRUCT(DO_TO_STRUCT)                      \
                                FOR_kernel_info_timer_STRUCT(DO_TO_STRUCT)                      \
                                    FOR_SysKernelInfoType_STRUCT(DO_TO_STRUCT)

#define FOR_EACH_STRUCT_6(DO_TO_STRUCT)                                                            \
    FOR_SlkPktFooterType_STRUCT(DO_TO_STRUCT) FOR_SlkPktHeaderType_STRUCT(DO_TO_STRUCT)            \
                                                                                                   \
        FOR_SysPktRPCParamType_STRUCT(DO_TO_STRUCT)                                                \
                                                                                                   \
            FOR_SysPktBodyType_STRUCT(DO_TO_STRUCT) FOR_SysPktContinueCmdType_STRUCT(DO_TO_STRUCT) \
                FOR_SysPktDbgBreakToggleCmdType_STRUCT(DO_TO_STRUCT)                               \
                    FOR_SysPktDbgBreakToggleRspType_STRUCT(DO_TO_STRUCT)                           \
                        FOR_SysPktFindCmdType_STRUCT(DO_TO_STRUCT)                                 \
                            FOR_SysPktFindRspType_STRUCT(DO_TO_STRUCT)                             \
                                FOR_SysPktGetBreakpointsCmdType_STRUCT(DO_TO_STRUCT)               \
                                    FOR_SysPktEmptyRspType_STRUCT(DO_TO_STRUCT)                    \
                                        FOR_SysPktGetBreakpointsRspType_STRUCT(DO_TO_STRUCT)

#define FOR_EACH_STRUCT_7(DO_TO_STRUCT)                                        \
    FOR_SysPktGetTrapBreaksCmdType_STRUCT(DO_TO_STRUCT)                        \
        FOR_SysPktGetTrapBreaksRspType_STRUCT(DO_TO_STRUCT)                    \
            FOR_SysPktGetTrapConditionsCmdType_STRUCT(DO_TO_STRUCT)            \
                FOR_SysPktGetTrapConditionsRspType_STRUCT(DO_TO_STRUCT)        \
                    FOR_SysPktReadMemCmdType_STRUCT(DO_TO_STRUCT)              \
                        FOR_SysPktReadMemRspType_STRUCT(DO_TO_STRUCT)          \
                            FOR_SysPktReadRegsCmdType_STRUCT(DO_TO_STRUCT)     \
                                FOR_SysPktReadRegsRspType_STRUCT(DO_TO_STRUCT) \
                                    FOR_SysPktRPC2Type_STRUCT(DO_TO_STRUCT)    \
                                        FOR_SysPktRPCType_STRUCT(DO_TO_STRUCT) \
                                            FOR_SysPktRtnNameCmdType_STRUCT(DO_TO_STRUCT)

#define FOR_EACH_STRUCT_8(DO_TO_STRUCT)                                                     \
    FOR_SysPktRtnNameRspType_STRUCT(DO_TO_STRUCT)                                           \
        FOR_SysPktSetBreakpointsCmdType_STRUCT(DO_TO_STRUCT)                                \
            FOR_SysPktSetBreakpointsRspType_STRUCT(DO_TO_STRUCT)                            \
                FOR_SysPktSetTrapBreaksCmdType_STRUCT(DO_TO_STRUCT)                         \
                    FOR_SysPktSetTrapConditionsCmdType_STRUCT(DO_TO_STRUCT)                 \
                        FOR_SysPktSetTrapConditionsRspType_STRUCT(DO_TO_STRUCT)             \
                            FOR_SysPktStateCmdType_STRUCT(DO_TO_STRUCT)                     \
                                FOR_SysPktStateRspType_STRUCT(DO_TO_STRUCT)                 \
                                    FOR_SysPktWriteMemCmdType_STRUCT(DO_TO_STRUCT)          \
                                        FOR_SysPktWriteMemRspType_STRUCT(DO_TO_STRUCT)      \
                                            FOR_SysPktWriteRegsCmdType_STRUCT(DO_TO_STRUCT) \
                                                FOR_SysPktWriteRegsRspType_STRUCT(DO_TO_STRUCT)

#define FOR_EACH_STRUCT_9(DO_TO_STRUCT)                                                            \
    FOR_RecordEntryType_STRUCT(DO_TO_STRUCT) FOR_RsrcEntryType_STRUCT(DO_TO_STRUCT)                \
        FOR_RecordListType_STRUCT(DO_TO_STRUCT) FOR_DatabaseHdrType_STRUCT(DO_TO_STRUCT)           \
            FOR_DatabaseDirEntryType_STRUCT(DO_TO_STRUCT) FOR_DatabaseDirType_STRUCT(DO_TO_STRUCT) \
                FOR_DmOpenInfoType_STRUCT(DO_TO_STRUCT) FOR_DmAccessType_STRUCT(DO_TO_STRUCT)      \
                    FOR_DmSearchStateType_STRUCT(DO_TO_STRUCT)                                     \
                                                                                                   \
                        FOR_CardHeaderType_STRUCT(DO_TO_STRUCT)                                    \
                            FOR_CardInfoType_STRUCT(DO_TO_STRUCT)                                  \
                                FOR_StorageHeaderType_STRUCT(DO_TO_STRUCT)                         \
                                                                                                   \
                                    FOR_ExgGoToType_STRUCT(DO_TO_STRUCT)                           \
                                        FOR_ExgSocketType_STRUCT(DO_TO_STRUCT)                     \
                                                                                                   \
                                            FOR_DlkDBCreatorList_STRUCT(DO_TO_STRUCT)              \
                                                FOR_DlkServerSessionType_STRUCT(DO_TO_STRUCT)

#define FOR_EACH_STRUCT_10(DO_TO_STRUCT)                                                          \
    FOR_generic_STRUCT(DO_TO_STRUCT) FOR_penUp_STRUCT(DO_TO_STRUCT) FOR_keyDown_STRUCT(           \
        DO_TO_STRUCT) FOR_winEnter_STRUCT(DO_TO_STRUCT) FOR_winExit_STRUCT(DO_TO_STRUCT)          \
        FOR_tsmConfirm_STRUCT(DO_TO_STRUCT) FOR_tsmFepButton_STRUCT(                              \
            DO_TO_STRUCT) FOR_tsmFepMode_STRUCT(DO_TO_STRUCT) FOR_ctlEnter_STRUCT(DO_TO_STRUCT)   \
            FOR_ctlSelect_STRUCT(DO_TO_STRUCT) FOR_ctlRepeat_STRUCT(                              \
                DO_TO_STRUCT) FOR_ctlExit_STRUCT(DO_TO_STRUCT) FOR_fldEnter_STRUCT(DO_TO_STRUCT)  \
                FOR_fldHeightChanged_STRUCT(DO_TO_STRUCT) FOR_fldChanged_STRUCT(                  \
                    DO_TO_STRUCT) FOR_fldExit_STRUCT(DO_TO_STRUCT)                                \
                    FOR_lstEnter_STRUCT(DO_TO_STRUCT) FOR_lstExit_STRUCT(                         \
                        DO_TO_STRUCT) FOR_lstSelect_STRUCT(DO_TO_STRUCT)                          \
                        FOR_tblEnter_STRUCT(DO_TO_STRUCT) FOR_tblExit_STRUCT(                     \
                            DO_TO_STRUCT) FOR_tblSelect_STRUCT(DO_TO_STRUCT)                      \
                            FOR_frmLoad_STRUCT(DO_TO_STRUCT) FOR_frmOpen_STRUCT(                  \
                                DO_TO_STRUCT) FOR_frmGoto_STRUCT(DO_TO_STRUCT)                    \
                                FOR_frmClose_STRUCT(DO_TO_STRUCT) FOR_frmUpdate_STRUCT(           \
                                    DO_TO_STRUCT) FOR_frmTitleEnter_STRUCT(DO_TO_STRUCT)          \
                                    FOR_frmTitleSelect_STRUCT(                                    \
                                        DO_TO_STRUCT) FOR_attnIndicatorEnter_STRUCT(DO_TO_STRUCT) \
                                        FOR_attnIndicatorSelect_STRUCT(                           \
                                            DO_TO_STRUCT) FOR_daySelect_STRUCT(DO_TO_STRUCT)      \
                                            FOR_menu_STRUCT(DO_TO_STRUCT) FOR_popSelect_STRUCT(   \
                                                DO_TO_STRUCT) FOR_sclEnter_STRUCT(DO_TO_STRUCT)   \
                                                FOR_sclExit_STRUCT(DO_TO_STRUCT)                  \
                                                    FOR_sclRepeat_STRUCT(DO_TO_STRUCT)            \
                                                        FOR_menuCmdBarOpen_STRUCT(DO_TO_STRUCT)   \
                                                            FOR_menuOpen_STRUCT(DO_TO_STRUCT)     \
                                                                FOR_gadgetEnter_STRUCT(           \
                                                                    DO_TO_STRUCT)                 \
                                                                    FOR_gadgetMisc_STRUCT(        \
                                                                        DO_TO_STRUCT)             \
                                                                                                  \
                                                                        FOR_EventTypeData_STRUCT( \
                                                                            DO_TO_STRUCT)         \
                                                                            FOR_EventType_STRUCT( \
                                                                                DO_TO_STRUCT)

#define FOR_EACH_STRUCT_11(DO_TO_STRUCT)                                                          \
    FOR_ROMDBRecordHeader1Type_STRUCT(DO_TO_STRUCT)                                               \
        FOR_ROMDBResourceHeader1Type_STRUCT(DO_TO_STRUCT)                                         \
            FOR_ROMDBHeader1Type_STRUCT(DO_TO_STRUCT) FOR_ROMCardHeader5Type_STRUCT(DO_TO_STRUCT) \
                FOR_ROMStoreHeader4Type_STRUCT(DO_TO_STRUCT)                                      \
                    FOR_ROMHeapHeader1Type_STRUCT(DO_TO_STRUCT)                                   \
                        FOR_ROMHeapHeader2Type_STRUCT(DO_TO_STRUCT)                               \
                            FOR_ROMHeapHeader3Type_STRUCT(DO_TO_STRUCT)                           \
                                FOR_ROMDBDir1Type_STRUCT(DO_TO_STRUCT)                            \
                                    FOR_ROMHeapChunkHdr2Type_STRUCT(DO_TO_STRUCT)                 \
                                        FOR_ROMHeapChunkHdr1Type_STRUCT(DO_TO_STRUCT)             \
                                            FOR_ROMFtrFeatureType_STRUCT(DO_TO_STRUCT)            \
                                                FOR_ROMFtrCreatorType_STRUCT(DO_TO_STRUCT)        \
                                                    FOR_ROMFtrTableType_STRUCT(DO_TO_STRUCT)

#define FOR_EACH_STRUCT_12(DO_TO_STRUCT)                                                         \
    FOR_ControlAttrType_STRUCT(DO_TO_STRUCT) FOR_ControlType_STRUCT(DO_TO_STRUCT)                \
        FOR_GraphicControlType_STRUCT(DO_TO_STRUCT) FOR_SliderControlType_STRUCT(DO_TO_STRUCT)   \
            FOR_BitmapTypeV2_STRUCT(DO_TO_STRUCT) FOR_BitmapTypeV3_STRUCT(DO_TO_STRUCT)          \
                FOR_FrameBitsType_STRUCT(DO_TO_STRUCT) FOR_WindowFlagsType_STRUCT(DO_TO_STRUCT)  \
                    FOR_WindowType_STRUCT(DO_TO_STRUCT) FOR_FormObjListType_STRUCT(DO_TO_STRUCT) \
                        FOR_FormAttrType_STRUCT(DO_TO_STRUCT) FOR_FormType_STRUCT(DO_TO_STRUCT)  \
                                                                                                 \
                            FOR_cjxln_STRUCT(DO_TO_STRUCT) FOR_cj_xgbh_STRUCT(DO_TO_STRUCT)      \
                                FOR_cj_xsmb_STRUCT(DO_TO_STRUCT)

#define FOR_EACH_STRUCT_13(DO_TO_STRUCT)                                                           \
    FOR_TimGlobalsType_STRUCT(DO_TO_STRUCT)                                                        \
                                                                                                   \
        FOR_FormObjAttrType_STRUCT(DO_TO_STRUCT)                                                   \
                                                                                                   \
            FOR_FormBitmapType_STRUCT(DO_TO_STRUCT) FOR_FormLineType_STRUCT(DO_TO_STRUCT)          \
                FOR_FormFrameType_STRUCT(DO_TO_STRUCT) FOR_FormRectangleType_STRUCT(DO_TO_STRUCT)  \
                    FOR_FormTitleType_STRUCT(DO_TO_STRUCT) FOR_FormPopupType_STRUCT(DO_TO_STRUCT)  \
                        FOR_FrmGraffitiStateType_STRUCT(DO_TO_STRUCT)                              \
                                                                                                   \
                            FOR_ScrollBarAttrType_STRUCT(DO_TO_STRUCT) FOR_ScrollBarType_STRUCT(   \
                                DO_TO_STRUCT)                                                      \
                                                                                                   \
                                FOR_FormGadgetAttrType_STRUCT(DO_TO_STRUCT)                        \
                                    FOR_FormGadgetType_STRUCT(DO_TO_STRUCT)                        \
                                                                                                   \
                                        FOR_FormLabelType_STRUCT(DO_TO_STRUCT)                     \
                                                                                                   \
                                            FOR_FieldAttrType_STRUCT(DO_TO_STRUCT)                 \
                                                FOR_FieldType_STRUCT(DO_TO_STRUCT)                 \
                                                                                                   \
                                                    FOR_ListAttrType_STRUCT(DO_TO_STRUCT)          \
                                                        FOR_ListType_STRUCT(DO_TO_STRUCT)          \
                                                                                                   \
                                                            FOR_TableAttrType_STRUCT(DO_TO_STRUCT) \
                                                                FOR_TableType_STRUCT(DO_TO_STRUCT)

#define FOR_EACH_STRUCT_14(DO_TO_STRUCT)                                                  \
    FOR_HostDirEntType_STRUCT(DO_TO_STRUCT) FOR_HostGremlinInfoType_STRUCT(DO_TO_STRUCT)  \
        FOR_HostStatType_STRUCT(DO_TO_STRUCT) FOR_HostTmType_STRUCT(DO_TO_STRUCT)         \
            FOR_HostUTimeType_STRUCT(DO_TO_STRUCT)                                        \
                                                                                          \
                FOR_NetHostInfoType_STRUCT(DO_TO_STRUCT)                                  \
                    FOR_NetHostInfoBufType_STRUCT(DO_TO_STRUCT)                           \
                        FOR_NetIOParamType_STRUCT(DO_TO_STRUCT)                           \
                            FOR_NetIOVecType_STRUCT(DO_TO_STRUCT)                         \
                                FOR_NetServInfoType_STRUCT(DO_TO_STRUCT)                  \
                                    FOR_NetServInfoBufType_STRUCT(DO_TO_STRUCT)           \
                                        FOR_NetSocketAddrINType_STRUCT(DO_TO_STRUCT)      \
                                            FOR_NetSocketAddrRawType_STRUCT(DO_TO_STRUCT) \
                                                FOR_NetSocketAddrType_STRUCT(DO_TO_STRUCT)

#if PLATFORM_MAC
    #define INLINE_SCALAR_IMPLEMENTATION 1
#else
    #define INLINE_SCALAR_IMPLEMENTATION 0
#endif

FOR_EACH_SCALAR_1(DECLARE_SCALAR_CLASSES)
FOR_EACH_SCALAR_2(DECLARE_SCALAR_CLASSES)
FOR_EACH_SCALAR_3(DECLARE_SCALAR_CLASSES)
FOR_EACH_SCALAR_4(DECLARE_SCALAR_CLASSES)

#if INLINE_SCALAR_IMPLEMENTATION

    #undef INLINE_
    #define INLINE_ inline

FOR_EACH_SCALAR_1(DEFINE_SCALAR_CLASSES)
FOR_EACH_SCALAR_2(DEFINE_SCALAR_CLASSES)
FOR_EACH_SCALAR_3(DEFINE_SCALAR_CLASSES)
FOR_EACH_SCALAR_4(DEFINE_SCALAR_CLASSES)

    #undef INLINE_
    #define INLINE_

#endif

FOR_EACH_STRUCT_1(DECLARE_STRUCT_CLASSES)
FOR_EACH_STRUCT_2(DECLARE_STRUCT_CLASSES)
FOR_EACH_STRUCT_3(DECLARE_STRUCT_CLASSES)
FOR_EACH_STRUCT_4(DECLARE_STRUCT_CLASSES)
FOR_EACH_STRUCT_5(DECLARE_STRUCT_CLASSES)
FOR_EACH_STRUCT_6(DECLARE_STRUCT_CLASSES)
FOR_EACH_STRUCT_7(DECLARE_STRUCT_CLASSES)
FOR_EACH_STRUCT_8(DECLARE_STRUCT_CLASSES)
FOR_EACH_STRUCT_9(DECLARE_STRUCT_CLASSES)
FOR_EACH_STRUCT_10(DECLARE_STRUCT_CLASSES)
FOR_EACH_STRUCT_11(DECLARE_STRUCT_CLASSES)
FOR_EACH_STRUCT_12(DECLARE_STRUCT_CLASSES)
FOR_EACH_STRUCT_13(DECLARE_STRUCT_CLASSES)
FOR_EACH_STRUCT_14(DECLARE_STRUCT_CLASSES)

#endif  // EmPalmStructs_h
