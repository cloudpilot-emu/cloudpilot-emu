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

#ifndef _EM_TRANSPORT_SERIAL_
#define _EM_TRANSPORT_SERIAL_

#include <string>

#include "EmCommon.h"
#include "EmEvent.h"

class EmTransportSerial {
   public:
    typedef string PortName;
    typedef long Baud;
    typedef long StopBits;
    typedef long DataBits;
    typedef long HwrHandshake;

    enum Parity { kNoParity, kOddParity, kEvenParity };

    enum RTSControl { kRTSOff, kRTSOn, kRTSAuto };

    // Note: this used to be named "Config", but that runs
    // afoul of a bug in VC++ (see KB Q143082).
    struct Config {
        Baud fBaud;
        Parity fParity;
        StopBits fStopBits;
        DataBits fDataBits;
        HwrHandshake fHwrHandshake;

        Config() = default;
        Config(const Config&);
        Config& operator=(const Config&);
    };

   public:
    virtual ~EmTransportSerial() = default;

    virtual bool Open() = 0;
    virtual bool Close() = 0;

    virtual bool Read(long&, void*) = 0;
    virtual bool Write(long&, const void*) = 0;

    virtual bool CanRead() = 0;
    virtual bool CanWrite() = 0;

    virtual size_t BytesPending() = 0;

    virtual const Config& GetConfig();
    virtual void SetConfig(const Config& config);

    virtual void SetRTS(RTSControl ctrl) = 0;
    virtual void SetDTR(bool dtr) = 0;
    virtual void SetBreak(bool breakActive) = 0;

    virtual bool RequiresSync() = 0;

   public:
    EmEvent<> onRequiresSyncChanged;

   protected:
    EmTransportSerial() = default;

   protected:
    Config config;
};

#endif  //  _EM_TRANSPORT_SERIAL_
