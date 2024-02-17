#include "EmTransportSerialBuffer.h"

#include "EmUARTDragonball.h"
#include "SuspendContextSerialSync.h"
#include "SuspendManager.h"

using TransactionState = EmUARTDragonball::TransactionState;

namespace {
    bool isXIDSniffingCommand(const uint8* buffer, size_t size) {
        size_t i = 0;

        // scan for frame start
        for (i = 0; i < size && buffer[i] != 0xc0; i++) {
            if (buffer[i] == 0xff)
                continue;
            else
                return false;
        }
        i++;

        if (size - i < 3) return false;
        if (buffer[i++] != 0xff) return false;  // addr = 0xff
        if (buffer[i++] != 0x3f) return false;  // command XID= 0x3f
        if (buffer[i++] != 0x01) return false;  // FI = 0x01

        // skip source address
        uint8 j = 0;
        while (j < 4) {
            if (i >= size) return false;
            if (buffer[i++] == 0x7d) continue;  // skip escape byte

            j++;
        }

        // check destination address (0xffffffff for sniffing)
        if (size - i < 6) return false;
        for (j = 0; j < 4; j++) {
            if (buffer[i++] != 0xff) return false;
        }

        return buffer[i + 1] != 0xff;
    }
}  // namespace

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

    if (modeSync && txBuffer.Size() == 0 && !incomingFrameComplete) {
        SuspendManager::Suspend<SuspendContextSerialSync>();
#ifdef TRACE_UART_SYNC
        cout << "received partial frame, suspending emulation" << endl;
#endif
    }

    return true;
}

bool EmTransportSerialBuffer::Write(long& count, const void* buffer) {
    if (count == 0) return true;

    for (size_t i = 0; i < static_cast<size_t>(count); i++) {
        rxBuffer.Push(reinterpret_cast<const uint8*>(buffer)[i]);

        if (modeSync && rxBuffer.Free() == 0 && requestTransferCallback) {
            requestTransferCallback();
#ifdef TRACE_UART_SYNC
            cout << "receive buffer overflow, sending partial frame now" << endl;
#endif
        }
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

#ifdef TRACE_UART_SYNC
        cout << "frame complete; suspending emulation" << endl;
#endif
    }

    if (oldState == TransactionState::waitingForData && newState == TransactionState::idle) {
        if (requestTransferCallback) requestTransferCallback();
#ifdef TRACE_UART_SYNC
        cout << "timeout waiting for response frame, sending empty frame" << endl;
#endif

        if (isXIDCommandFrame) {
#ifdef TRACE_UART_SYNC
            cout << "suspending while waiting for the next XID timeslot" << endl << flush;
#endif
            SuspendManager::Suspend<SuspendContextSerialSync>();
        }
    }

#ifdef TRACE_UART_SYNC
    cout << "transaction state change " << (int)oldState << " -> " << (int)newState << endl;
#endif
}

int EmTransportSerialBuffer::RxBytesPending() { return rxBuffer.Size(); }

void* EmTransportSerialBuffer::Receive() {
#ifdef TRACE_UART_SYNC
    cout << "copying out " << rxBuffer.Size() << " bytes of data" << endl;
#endif
    for (size_t i = 0; rxBuffer.Size() > 0; i++) copyOutBuffer[i] = rxBuffer.Pop();

    return copyOutBuffer.get();
}

int EmTransportSerialBuffer::Send(int count, const void* data, bool frameComplete) {
    for (size_t i = 0; i < static_cast<size_t>(count); i++)
        txBuffer.Push(reinterpret_cast<const uint8*>(data)[i]);

    incomingFrameComplete = frameComplete;

    isXIDCommandFrame = modeSync && frameComplete &&
                        isXIDSniffingCommand(reinterpret_cast<const uint8*>(data), count);
#ifdef TRACE_UART_SYNC
    if (isXIDCommandFrame) cout << "received XID sniffing command" << endl;
#endif

    if (SuspendManager::IsSuspended() &&
        SuspendManager::GetContext().GetKind() == SuspendContext::Kind::serialSync) {
        SuspendManager::GetContext().AsContextSerialSync().Resume();

#ifdef TRACE_UART_SYNC
        cout << "received " << count << " bytes of frame data, resuming emulation" << endl;
#endif
    } else {
#ifdef TRACE_UART_SYNC
        cout << "received " << count << " bytes of frame data" << endl;
#endif
    }

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
    isXIDCommandFrame = false;

    onRequiresSyncChanged.Dispatch();
}

bool EmTransportSerialBuffer::IsFrameComplete() {
    return !modeSync || transactionState != TransactionState::sending;
}

void EmTransportSerialBuffer::SetRequestTransferCallback(long cb) {
    requestTransferCallback = (request_transfer_callback_ptr)cb;
}
