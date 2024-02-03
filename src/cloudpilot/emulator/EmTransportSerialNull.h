
#ifndef _EM_TRANSPORT_SERIAL_NULL_H_
#define _EM_TRANSPORT_SERIAL_NULL_H_

#include "EmTransportSerial.h"

class EmTransportSerialNull : public EmTransportSerial {
   public:
    EmTransportSerialNull();

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
};

#endif  // _EM_TRANSPORT_SERIAL_NULL_H_
