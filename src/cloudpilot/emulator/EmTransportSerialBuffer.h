#ifndef _EM_TRANSPORT_SERIAL_BUFFER_H_
#define _EM_TRANSPORT_SERIAL_BUFFER_H_

#include <memory>

#include "EmCommon.h"
#include "EmTransportSerial.h"
#include "Fifo.h"

class EmTransportSerialBuffer : public EmTransportSerial {
   public:
    typedef void (*request_transfer_callback_ptr)();

   public:
    // default: ~28kB = one second worth of data @ 230400 baud
    EmTransportSerialBuffer(size_t bufferSize = 230400 / 8 * 1024);

    bool Open() override;
    bool Close() override;

    bool Read(long&, void*) override;
    bool Write(long&, const void*) override;

    bool CanRead() override;
    bool CanWrite() override;

    size_t BytesPending() override;

    void SetRTS(RTSControl ctrl) override;
    void SetDTR(bool dtr) override;
    void SetBreak(bool breakActive) override;

    bool RequiresSync() override;

    void OnTransactionStateChange(EmUARTDragonball::TransactionState oldState,
                                  EmUARTDragonball::TransactionState newState) override;

    //-------------------------------------------------------------------------

    int RxBytesPending();
    void* Receive();

    int Send(int count, const void* data, bool frameComplete);

    bool IsOpen() const;
    int Rts() const;
    bool Dtr() const;
    bool IsBreak() const;

    int GetBaud() const;
    int GetParity() const;
    int GetStopBits() const;
    int GetDataBits() const;
    int GetHwrHandshake() const;

    int BufferSize() const;

    bool GetModeSync() const;
    void SetModeSync(bool modeSync);

    bool IsFrameComplete();
    void SetRequestTransferCallback(long cb);

   private:
    const size_t bufferSize;

    Fifo<uint8> rxBuffer;
    Fifo<uint8> txBuffer;

    unique_ptr<uint8[]> copyOutBuffer;

    bool isOpen{false};
    RTSControl rts{kRTSAuto};
    bool dtr{false};
    bool isBreak{false};

    bool modeSync{false};
    bool incomingFrameComplete{false};
    bool isXIDCommandFrame{false};

    EmUARTDragonball::TransactionState transactionState{EmUARTDragonball::TransactionState::idle};

    request_transfer_callback_ptr requestTransferCallback{nullptr};
};

#endif  //  _EM_TRANSPORT_SERIAL_BUFFER_H_
