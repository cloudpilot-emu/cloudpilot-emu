
#ifndef _EM_TRANSPORT_SERIAL_NULL_H_
#define _EM_TRANSPORT_SERIAL_NULL_H_

#include "EmTransportSerial.h"

class EmTransportSerialNull : public EmTransportSerial {
   public:
    EmTransportSerialNull();

    bool Open(void) override;
    bool Close(void) override;

    bool Read(long&, void*) override;
    bool Write(long&, const void*) override;

    bool CanRead(void) override;
    bool CanWrite(void) override;

    size_t BytesPending() override;

    void SetRTS(RTSControl ctrl) override;
    void SetDTR(bool dtr) override;
    void SetBreak(bool breakActive) override;
};

#endif  // _EM_TRANSPORT_SERIAL_NULL_H_
