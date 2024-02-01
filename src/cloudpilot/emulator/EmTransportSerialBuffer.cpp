#include "EmTransportSerialBuffer.h"

EmTransportSerialBuffer::EmTransportSerialBuffer(size_t bufferSize)
    : bufferSize(bufferSize),
      rxBuffer(bufferSize),
      txBuffer(bufferSize),
      copyOutBuffer(make_unique<uint8[]>(bufferSize)) {
    Reset();
}

void EmTransportSerialBuffer::Reset() {
    rxBuffer.Clear();
    txBuffer.Clear();

    isOpen = false;
    isBreak = false;
    rts = kRTSAuto;
    dtr = false;
}

bool EmTransportSerialBuffer::Open() {
    isOpen = true;
    return true;
}

bool EmTransportSerialBuffer::Close() {
    isOpen = false;
    return true;
}

bool EmTransportSerialBuffer::Read(long& count, void* buffer) {
    size_t i;
    for (i = 0; i < static_cast<size_t>(count) && txBuffer.Size() > 0; i++)
        reinterpret_cast<uint8*>(buffer)[i] = txBuffer.Pop();

    count = static_cast<long>(i);

    return true;
}

bool EmTransportSerialBuffer::Write(long& count, const void* buffer) {
    for (size_t i = 0; i < static_cast<size_t>(count); i++)
        rxBuffer.Push(reinterpret_cast<const uint8*>(buffer)[i]);

    return true;
}

bool EmTransportSerialBuffer::CanRead() { return rxBuffer.Size() > 0; }

bool EmTransportSerialBuffer::CanWrite() { return txBuffer.Free() > 0; }

size_t EmTransportSerialBuffer::BytesPending() { return rxBuffer.Size(); }

void EmTransportSerialBuffer::SetRTS(RTSControl ctrl) { rts = ctrl; }

void EmTransportSerialBuffer::SetDTR(bool dtr) { this->dtr = dtr; }

void EmTransportSerialBuffer::SetBreak(bool breakActive) { isBreak = breakActive; }

int EmTransportSerialBuffer::RxBytesPending() { return txBuffer.Size(); }

void* EmTransportSerialBuffer::Receive() {
    for (size_t i = 0; txBuffer.Size() > 0; i++) copyOutBuffer[i] = txBuffer.Pop();

    return copyOutBuffer.get();
}

int EmTransportSerialBuffer::Send(int count, const void* data) {
    for (size_t i = 0; i < static_cast<size_t>(count); i++)
        rxBuffer.Push(reinterpret_cast<const uint8*>(data)[i]);

    return count;
}

bool EmTransportSerialBuffer::IsOpen() const { return isOpen; }

int EmTransportSerialBuffer::Rts() const { return rts; }

bool EmTransportSerialBuffer::Dtr() const { return dtr; }

bool EmTransportSerialBuffer::IsBreak() const { return isBreak; }

int EmTransportSerialBuffer::BufferSize() const { return bufferSize; }
