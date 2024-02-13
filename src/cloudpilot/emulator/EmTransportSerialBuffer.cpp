#include "EmTransportSerialBuffer.h"

#include "SuspendContextSerialSync.h"
#include "SuspendManager.h"

using TransactionState = EmUARTDragonball::TransactionState;

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

    if (modeSync && txBuffer.Size() == 0 && !incomingFrameComplete)
        SuspendManager::Suspend<SuspendContextSerialSync>();

    return true;
}

bool EmTransportSerialBuffer::Write(long& count, const void* buffer) {
    if (count == 0) return true;

    for (size_t i = 0; i < static_cast<size_t>(count); i++) {
        rxBuffer.Push(reinterpret_cast<const uint8*>(buffer)[i]);

        if (modeSync && rxBuffer.Free() == 0 && requestTransferCallback) requestTransferCallback();
    }

    return true;
}

bool EmTransportSerialBuffer::CanRead() { return txBuffer.Size() > 0; }

bool EmTransportSerialBuffer::CanWrite() { return true; }

size_t EmTransportSerialBuffer::BytesPending() { return txBuffer.Size(); }

void EmTransportSerialBuffer::SetRTS(RTSControl ctrl) { rts = ctrl; }

void EmTransportSerialBuffer::SetDTR(bool dtr) { this->dtr = dtr; }

void EmTransportSerialBuffer::SetBreak(bool breakActive) { isBreak = breakActive; }

bool EmTransportSerialBuffer::RequiresSync() { return modeSync; }

void EmTransportSerialBuffer::OnTransactionStateChange(TransactionState oldState,
                                                       TransactionState newState) {
    transactionState = newState;

    if (!modeSync || oldState == newState) return;

    if (oldState == TransactionState::sending && newState == TransactionState::idle) {
        SuspendManager::Suspend<SuspendContextSerialSync>();

        if (requestTransferCallback) requestTransferCallback();
    }

    if (oldState == TransactionState::waitingForData && newState == TransactionState::idle &&
        requestTransferCallback)
        requestTransferCallback();

    cout << "transaction state change " << (int)oldState << " -> " << (int)newState << endl;
}

int EmTransportSerialBuffer::RxBytesPending() { return rxBuffer.Size(); }

void* EmTransportSerialBuffer::Receive() {
    for (size_t i = 0; rxBuffer.Size() > 0; i++) copyOutBuffer[i] = rxBuffer.Pop();

    return copyOutBuffer.get();
}

int EmTransportSerialBuffer::Send(int count, const void* data, bool frameComplete) {
    for (size_t i = 0; i < static_cast<size_t>(count); i++)
        txBuffer.Push(reinterpret_cast<const uint8*>(data)[i]);

    incomingFrameComplete = frameComplete;

    if (SuspendManager::IsSuspended() &&
        SuspendManager::GetContext().GetKind() == SuspendContext::Kind::serialSync)
        SuspendManager::GetContext().AsContextSerialSync().Resume();

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
    if (modeSync == this->modeSync) return;

    this->modeSync = modeSync;
    transactionState = TransactionState::idle;

    onRequiresSyncChanged.Dispatch();
}

bool EmTransportSerialBuffer::IsFrameComplete() {
    return !modeSync || transactionState != TransactionState::sending;
}

void EmTransportSerialBuffer::SetRequestTransferCallback(long cb) {
    requestTransferCallback = (request_transfer_callback_ptr)cb;
}
