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

#ifndef EmTransport_h
#define EmTransport_h

#include <string>  // string
#include <vector>  // vector

#include "EmCommon.h"
#include "EmTypes.h"  // ErrCode

enum EmTransportType {
    kTransportNull,    // "null:"
    kTransportSerial,  // "serial:"
    kTransportSocket,  // "tcp:"
    kTransportUSB,     // "usb:"

    kTransportUnknown
};

class EmTransport;

#if 0  // CSTODO
// Wraps up a "transport descriptor", something that identifies
// a tranport that a user can point to and use in the UI.

class EmTransportDescriptor {
   public:
    EmTransportDescriptor(void);
    EmTransportDescriptor(EmTransportType);
    EmTransportDescriptor(EmTransportType, const string&);
    EmTransportDescriptor(const string&);
    EmTransportDescriptor(const EmTransportDescriptor&);
    ~EmTransportDescriptor(void);
    EmTransportDescriptor& operator=(const EmTransportDescriptor&);

    EmTransport* CreateTransport(void) const;

    string GetMenuName(void) const;        // For menus (e.g., "COM1", "TCP/IP")
    string GetDescriptor(void) const;      // Full text (e.g., "serial:COM1")
    string GetScheme(void) const;          // First part of text (e.g., "serial", "socket")
    string GetSchemeSpecific(void) const;  // Second part of text (e.g., "127.0.0.1:5001")
    EmTransportType GetType(void) const;   // Scheme in numeric form

    static string GetSchemePrefix(EmTransportType);  // Convert numeric format to scheme prefix

    bool operator==(const EmTransportDescriptor&) const;

   private:
    Bool PrvTestType(EmTransportType type) const;

   private:
    string fDescriptor;
};

typedef vector<EmTransportDescriptor> EmTransportDescriptorList;

#endif

class EmTransport {
   public:
    struct Config {
        Config(void){};
        virtual ~Config(void){};

        virtual EmTransport* NewTransport(void) = 0;
        virtual EmTransport* GetTransport(void) = 0;
    };

   public:
    EmTransport(void);
    virtual ~EmTransport(void);

    virtual ErrCode Open(void);
    virtual ErrCode Close(void);

    virtual ErrCode Read(long&, void*);
    virtual ErrCode Write(long&, const void*);

    virtual Bool CanRead(void);
    virtual Bool CanWrite(void);

    virtual long BytesInBuffer(long minBytes);

    virtual string GetSpecificName(void) = 0;

    static void CloseAllTransports(void);
};

class EmTransportNull : public EmTransport {
   public:
    // Note: this used to be named "Config", but that runs
    // afoul of a bug in VC++ (see KB Q143082).
    struct ConfigNull : public EmTransport::Config {
        ConfigNull(void){};
        virtual ~ConfigNull(void){};

        virtual EmTransport* NewTransport(void) { return new EmTransportNull; };
        virtual EmTransport* GetTransport(void);
    };

   public:
    EmTransportNull(void);
#if 0  // CSTODO
    EmTransportNull(const EmTransportDescriptor&);
#endif
    EmTransportNull(const ConfigNull&);
    virtual ~EmTransportNull(void);

    virtual ErrCode Open(void);
    virtual ErrCode Close(void);

    virtual ErrCode Read(long&, void*);
    virtual ErrCode Write(long&, const void*);

    virtual Bool CanRead(void);
    virtual Bool CanWrite(void);

    virtual long BytesInBuffer(long minBytes);

    virtual string GetSpecificName(void);

#if 0  // CSTODO
    static void GetDescriptorList(EmTransportDescriptorList&);
#endif
};

#endif /* EmTransport_h */
