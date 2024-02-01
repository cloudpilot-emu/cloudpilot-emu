#include "EmTransportSerialNull.h"

EmTransportSerialNull::EmTransportSerialNull() {}

void EmTransportSerialNull::Reset() {}

bool EmTransportSerialNull::Open(void) { return true; }

bool EmTransportSerialNull::Close(void) { return true; }

bool EmTransportSerialNull::Read(long& count, void*) { return count == 0; }

bool EmTransportSerialNull::Write(long&, const void*) { return true; }

bool EmTransportSerialNull::CanRead(void) { return false; }

bool EmTransportSerialNull::CanWrite(void) { return true; }

size_t EmTransportSerialNull::BytesPending() { return 0; }

void EmTransportSerialNull::SetRTS(RTSControl ctrl) {}

void EmTransportSerialNull::SetDTR(bool dtr) {}

void EmTransportSerialNull::SetBreak(bool breakActive) {}
