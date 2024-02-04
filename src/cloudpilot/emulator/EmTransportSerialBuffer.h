#ifndef _EM_TRANSPORT_SERIAL_BUFFER_H_
#define _EM_TRANSPORT_SERIAL_BUFFER_H_

#include <memory>

#include "EmCommon.h"
#include "EmTransportSerial.h"
#include "Fifo.h"

class EmTransportSerialBuffer : public EmTransportSerial {
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

    //-------------------------------------------------------------------------

    int RxBytesPending();
    void* Receive();

    int Send(int count, const void* data);

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

   private:
    const size_t bufferSize;

    Fifo<uint8> rxBuffer;
    Fifo<uint8> txBuffer;

    unique_ptr<uint8[]> copyOutBuffer;

    bool isOpen{false};
    RTSControl rts{kRTSAuto};
    bool dtr{false};
    bool isBreak{false};
};

#endif  //  _EM_TRANSPORT_SERIAL_BUFFER_H_
