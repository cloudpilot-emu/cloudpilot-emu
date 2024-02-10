#include "EmTransportSerialBuffer.h"

EmTransportSerialBuffer::EmTransportSerialBuffer(size_t bufferSize)
    : bufferSize(bufferSize),
      rxBuffer(bufferSize),
      txBuffer(bufferSize),
      copyOutBuffer(make_unique<uint8[]>(bufferSize)) {}

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

bool EmTransportSerialBuffer::CanRead() { return txBuffer.Size() > 0; }

bool EmTransportSerialBuffer::CanWrite() { return true; }

size_t EmTransportSerialBuffer::BytesPending() { return txBuffer.Size(); }

void EmTransportSerialBuffer::SetRTS(RTSControl ctrl) { rts = ctrl; }

void EmTransportSerialBuffer::SetDTR(bool dtr) { this->dtr = dtr; }

void EmTransportSerialBuffer::SetBreak(bool breakActive) { isBreak = breakActive; }

bool EmTransportSerialBuffer::RequiresSync() { return modeSync; }

int EmTransportSerialBuffer::RxBytesPending() { return rxBuffer.Size(); }

void* EmTransportSerialBuffer::Receive() {
    for (size_t i = 0; rxBuffer.Size() > 0; i++) copyOutBuffer[i] = rxBuffer.Pop();

    return copyOutBuffer.get();
}

int EmTransportSerialBuffer::Send(int count, const void* data) {
    for (size_t i = 0; i < static_cast<size_t>(count); i++)
        txBuffer.Push(reinterpret_cast<const uint8*>(data)[i]);

    return count;
}

bool EmTransportSerialBuffer::IsOpen() const { return isOpen; }

int EmTransportSerialBuffer::Rts() const { return rts; }

bool EmTransportSerialBuffer::Dtr() const { return dtr; }

bool EmTransportSerialBuffer::IsBreak() const { return isBreak; }

int EmTransportSerialBuffer::GetBaud() const { return config.fBaud; }

int EmTransportSerialBuffer::GetParity() const { return config.fParity; }

int EmTransportSerialBuffer::GetStopBits() const { return config.fStopBits; }

int EmTransportSerialBuffer::GetDataBits() const { return config.fDataBits; }

int EmTransportSerialBuffer::GetHwrHandshake() const { return config.fHwrHandshake; }

int EmTransportSerialBuffer::BufferSize() const { return bufferSize; }

bool EmTransportSerialBuffer::GetModeSync() const { return modeSync; }

void EmTransportSerialBuffer::SetModeSync(bool modeSync) {
    this->modeSync = modeSync;
    onRequiresSyncChanged.Dispatch();
}
