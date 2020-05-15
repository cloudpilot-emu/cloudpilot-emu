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

#include "EmTransport.h"

#include "EmCommon.h"

#if 0                               // CSTODO
    #include "EmTransportSerial.h"  // EmTransportSerial
    #include "EmTransportSocket.h"  // EmTransportSocket
    #include "EmTransportUSB.h"     // EmTransportUSB
#endif

#include <algorithm>  // find()
#include <vector>     // vector

typedef vector<EmTransport*> EmTransportList;
EmTransportList gTransports;

/***********************************************************************
 *
 * FUNCTION:	EmTransport c'tor
 *
 * DESCRIPTION:	Constructor.  Initialize our data members.
 *
 * PARAMETERS:	None
 *
 * RETURNED:	Nothing
 *
 ***********************************************************************/

EmTransport::EmTransport(void) { gTransports.push_back(this); }

/***********************************************************************
 *
 * FUNCTION:	EmTransport d'tor
 *
 * DESCRIPTION:	Destructor.  Delete our data members.
 *
 * PARAMETERS:	None
 *
 * RETURNED:	Nothing
 *
 ***********************************************************************/

EmTransport::~EmTransport(void) {
    EmTransportList::iterator iter = find(gTransports.begin(), gTransports.end(), this);

    if (iter != gTransports.end()) {
        gTransports.erase(iter);
    }
}

/***********************************************************************
 *
 * FUNCTION:	EmTransport::Open
 *
 * DESCRIPTION:	Open the transport using the information provided
 *				either in the constructor or with SetConfig.
 *
 * PARAMETERS:	None
 *
 * RETURNED:	0 if no error.
 *
 ***********************************************************************/

ErrCode EmTransport::Open(void) { return errNone; }

/***********************************************************************
 *
 * FUNCTION:	EmTransport::Close
 *
 * DESCRIPTION:	Close the transport.
 *
 * PARAMETERS:	None
 *
 * RETURNED:	0 if no error.
 *
 ***********************************************************************/

ErrCode EmTransport::Close(void) { return errNone; }

/***********************************************************************
 *
 * FUNCTION:	EmTransport::Read
 *
 * DESCRIPTION:	Read up to the given number of bytes, storing them in
 *				the given buffer.
 *
 * PARAMETERS:	len - maximum number of bytes to read.
 *				data - buffer to receive the bytes.
 *
 * RETURNED:	0 if no error.  The number of bytes actually read is
 *				returned in len if there was no error.
 *
 ***********************************************************************/

ErrCode EmTransport::Read(long&, void*) { return errNone; }

/***********************************************************************
 *
 * FUNCTION:	EmTransport::Write
 *
 * DESCRIPTION:	Write up the the given number of bytes, using the data
 *				in the given buffer.
 *
 * PARAMETERS:	len - number of bytes in the buffer.
 *				data - buffer containing the bytes.
 *
 * RETURNED:	0 if no error.  The number of bytes actually written is
 *				returned in len if there was no error.
 *
 ***********************************************************************/

ErrCode EmTransport::Write(long&, const void*) { return errNone; }

/***********************************************************************
 *
 * FUNCTION:	EmTransport::CanRead
 *
 * DESCRIPTION:	Return whether or not the transport is available for
 *				a read operation (that is, it's connected to another
 *				entity).  Does NOT indicate whether or not there are
 *				actually any bytes available to be read.
 *
 * PARAMETERS:	None
 *
 * RETURNED:	True if so.
 *
 ***********************************************************************/

Bool EmTransport::CanRead(void) { return false; }

/***********************************************************************
 *
 * FUNCTION:	EmTransport::CanWrite
 *
 * DESCRIPTION:	Return whether or not the transport is available for
 *				a write operation (that is, it's connected to another
 *				entity).  Does NOT indicate whether or not there is
 *				actually any room in the transport's internal buffer
 *				for the data being written.
 *
 * PARAMETERS:	None
 *
 * RETURNED:	True if so.
 *
 ***********************************************************************/

Bool EmTransport::CanWrite(void) { return false; }

/***********************************************************************
 *
 * FUNCTION:	EmTransport::BytesInBuffer
 *
 * DESCRIPTION:	Returns the number of bytes that can be read with the
 *				Read method.  Note that bytes may be received in
 *				between the time BytesInBuffer is called and the time
 *				Read is called, so calling the latter with the result
 *				of the former is not guaranteed to fetch all received
 *				and buffered bytes.
 *
 * PARAMETERS:	minBytes - try to buffer at least this many bytes.
 *					Return when we have this many bytes buffered, or
 *					until some small timeout has occurred.
 *
 * RETURNED:	Number of bytes that can be read.
 *
 ***********************************************************************/

long EmTransport::BytesInBuffer(long /*minBytes*/) { return 0; }

/***********************************************************************
 *
 * FUNCTION:	EmTransport::CloseAllTransports
 *
 * DESCRIPTION:	Shutdown routine.  Close all existing transports.
 *
 * PARAMETERS:	None
 *
 * RETURNED:	Nothing
 *
 ***********************************************************************/

void EmTransport::CloseAllTransports(void) {
    EmTransportList::iterator iter = gTransports.begin();
    while (iter != gTransports.end()) {
        EmTransport* transport = *iter;
        transport->Close();

        ++iter;
    }
}

#pragma mark -

// ---------------------------------------------------------------------------
//		� EmTransportNull::EmTransportNull
// ---------------------------------------------------------------------------

EmTransportNull::EmTransportNull(void) {}

#if 0  // CSTODO
EmTransportNull::EmTransportNull(const EmTransportDescriptor&) {}
#endif

EmTransportNull::EmTransportNull(const ConfigNull&) {}

EmTransportNull::~EmTransportNull(void) { this->Close(); }

ErrCode EmTransportNull::Open(void) { return errNone; }

ErrCode EmTransportNull::Close(void) { return errNone; }

ErrCode EmTransportNull::Read(long& size, void*) {
    size = 0;
    return errNone;
}

ErrCode EmTransportNull::Write(long&, const void*) { return errNone; }

Bool EmTransportNull::CanRead(void) { return true; }

Bool EmTransportNull::CanWrite(void) { return true; }

long EmTransportNull::BytesInBuffer(long /*minBytes*/) { return 0; }

string EmTransportNull::GetSpecificName(void) { return "Bit Bucket"; }

#if 0  // CSTODO
/***********************************************************************
 *
 * FUNCTION:	EmTransportNull:: GetDescriptorList
 *
 * DESCRIPTION:	Return the list of TCP ports on this computer.  Used
 *				to prepare a menu of TCP port choices.
 *
 * PARAMETERS:	nameList - port names are added to this list.
 *
 * RETURNED:	Nothing
 *
 ***********************************************************************/

void EmTransportNull::GetDescriptorList(EmTransportDescriptorList& descList) {
    descList.clear();

    descList.push_back(EmTransportDescriptor(kTransportNull));
}

    #pragma mark -

// EmTransportDescriptor
//
// This is a simple class that manages the creation of an EmTransport.
// It can be initialized with information describing what kind of transport
// to create (Serial, Socket, USB, etc.) and the parameters used to create
// it (serial configuration, IP address, etc.).  Once that information has
// be established, calling CreateTransport will create the appropriate
// transport object.
//
// EmTransportDescriptor is also called upon to provide UI information
// appropriate for display in dialogs (in menus or edit text items).
//
// Internally, this information is stored as a string.  The string is
// divided into two parts: a "scheme", and scheme-specific data.  These
// two parts are divided by a colon.  The scheme identifies what kind of
// transport is to be created, and any option scheme-specific data
// describes the creation parameters.

// ---------------------------------------------------------------------------
//		� EmTransportDescriptor::EmTransportDescriptor
// ---------------------------------------------------------------------------

EmTransportDescriptor::EmTransportDescriptor(void) : fDescriptor("unknown") {}

// ---------------------------------------------------------------------------
//		� EmTransportDescriptor::EmTransportDescriptor
// ---------------------------------------------------------------------------

EmTransportDescriptor::EmTransportDescriptor(EmTransportType type)
    : fDescriptor(GetSchemePrefix(type) + ":") {}

// ---------------------------------------------------------------------------
//		� EmTransportDescriptor::EmTransportDescriptor
// ---------------------------------------------------------------------------

EmTransportDescriptor::EmTransportDescriptor(EmTransportType type, const string& s)
    : fDescriptor(GetSchemePrefix(type) + ":" + s) {}

// ---------------------------------------------------------------------------
//		� EmTransportDescriptor::EmTransportDescriptor
// ---------------------------------------------------------------------------

EmTransportDescriptor::EmTransportDescriptor(const string& s) : fDescriptor(s) {
    #if PLATFORM_UNIX
    if (this->GetType() == kTransportUnknown) {
        // On Unix, we'll be handed something that looks either like a device
        // or an IP address.  Try to guess which and set up the scheme type
        // based on what we decide.

        if (s.size() == 0) {
            // Empty string -- turn into a NULL descriptor.

            fDescriptor = this->GetSchemePrefix(kTransportNull) + ":";
        } else if (s[0] == '/') {
            // It looks like a device name -- turn into a Serial descriptor.

            fDescriptor = this->GetSchemePrefix(kTransportSerial) + ":" + fDescriptor;
        } else {
            // Assume IP address.  I'm not sure if there's a good way to
            // ensure this.  What's a good heuristic check?

            fDescriptor = this->GetSchemePrefix(kTransportSocket) + ":" + fDescriptor;
        }
    }
    #endif
}

// ---------------------------------------------------------------------------
//		� EmTransportDescriptor::EmTransportDescriptor
// ---------------------------------------------------------------------------

EmTransportDescriptor::EmTransportDescriptor(const EmTransportDescriptor& other)
    : fDescriptor(other.fDescriptor) {}

// ---------------------------------------------------------------------------
//		� EmTransportDescriptor::~EmTransportDescriptor
// ---------------------------------------------------------------------------

EmTransportDescriptor::~EmTransportDescriptor(void) {}

// ---------------------------------------------------------------------------
//		� EmTransportDescriptor::operator=
// ---------------------------------------------------------------------------

EmTransportDescriptor& EmTransportDescriptor::operator=(const EmTransportDescriptor& other) {
    fDescriptor = other.fDescriptor;

    return *this;
}

// ---------------------------------------------------------------------------
//		� EmTransportDescriptor::operator==
// ---------------------------------------------------------------------------

bool EmTransportDescriptor::operator==(const EmTransportDescriptor& other) const {
    return fDescriptor == other.fDescriptor;
}

// ---------------------------------------------------------------------------
//		� EmTransportDescriptor::CreateTransport
// ---------------------------------------------------------------------------

EmTransport* EmTransportDescriptor::CreateTransport(void) const {
    EmTransport* result;
    EmTransportType type = this->GetType();

    switch (type) {
        case kTransportNull:
            result = new EmTransportNull(*this);
            break;

        case kTransportSerial:
            result = new EmTransportSerial(*this);
            break;

        case kTransportSocket:
            result = new EmTransportSocket(*this);
            break;

        case kTransportUSB:
            result = new EmTransportUSB(*this);
            break;

        default:
            EmAssert(false);
            result = new EmTransportNull(*this);
            break;
    }

    return result;
}

// ---------------------------------------------------------------------------
//		� EmTransportDescriptor::GetMenuName
// ---------------------------------------------------------------------------

string EmTransportDescriptor::GetMenuName(void) const {
    string result;
    EmTransportType type = this->GetType();

    switch (type) {
    #if !PLATFORM_UNIX
        case kTransportNull:
            result = "No Port";
            break;

        case kTransportSerial:
            result = this->GetSchemeSpecific();
            break;

        case kTransportSocket:
            result = "TCP/IP";
            break;

        case kTransportUSB:
            result = "USB";
            break;

        default:
            EmAssert(false);
            result = "No Port";
            break;
    #else
        case kTransportNull:
        case kTransportSerial:
        case kTransportSocket:
        case kTransportUSB:
            result = this->GetSchemeSpecific();
            break;

        default:
            EmAssert(false);
            result = "";
            break;
    #endif
    }

    return result;
}

// ---------------------------------------------------------------------------
//		� EmTransportDescriptor::GetDescriptor
// ---------------------------------------------------------------------------

string EmTransportDescriptor::GetDescriptor(void) const {
    EmAssert(this->GetType() != kTransportUnknown);

    return this->fDescriptor;
}

// ---------------------------------------------------------------------------
//		� EmTransportDescriptor::GetScheme
// ---------------------------------------------------------------------------

string EmTransportDescriptor::GetScheme(void) const {
    string::size_type pos = this->fDescriptor.find(':');
    EmAssert(pos != string::npos);
    return this->fDescriptor.substr(0, pos);
}

// ---------------------------------------------------------------------------
//		� EmTransportDescriptor::GetSchemeSpecific
// ---------------------------------------------------------------------------

string EmTransportDescriptor::GetSchemeSpecific(void) const {
    string::size_type pos = this->fDescriptor.find(':');
    EmAssert(pos != string::npos);
    return this->fDescriptor.substr(pos + 1);
}

// ---------------------------------------------------------------------------
//		� EmTransportDescriptor::GetType
// ---------------------------------------------------------------------------

EmTransportType EmTransportDescriptor::GetType(void) const {
    if (this->PrvTestType(kTransportNull)) return kTransportNull;

    if (this->PrvTestType(kTransportSerial)) return kTransportSerial;

    if (this->PrvTestType(kTransportSocket)) return kTransportSocket;

    if (this->PrvTestType(kTransportUSB)) return kTransportUSB;

    return kTransportUnknown;
}

// ---------------------------------------------------------------------------
//		� EmTransportDescriptor::GetSchemePrefix
// ---------------------------------------------------------------------------
// Return the descriptor prefix for the given scheme.  The separating ":" is
// NOT included.

string EmTransportDescriptor::GetSchemePrefix(EmTransportType type) {
    string result;

    switch (type) {
        case kTransportNull:
            result = "null";
            break;

        case kTransportSerial:
            result = "serial";
            break;

        case kTransportSocket:
            result = "socket";
            break;

        case kTransportUSB:
            result = "usb";
            break;

        default:
            EmAssert(false);
            result = "null";
            break;
    }

    return result;
}

// ---------------------------------------------------------------------------
//		� EmTransportDescriptor::PrvTestType
// ---------------------------------------------------------------------------
// Return whether or not the controlled descriptor is part of the given scheme.

Bool EmTransportDescriptor::PrvTestType(EmTransportType type) const {
    string scheme = this->GetSchemePrefix(type) + ":";

    if (this->fDescriptor.size() < scheme.size()) return false;

    string prefix = this->fDescriptor.substr(0, scheme.size());

    return prefix == scheme;
}

#endif
