#include "EmTransportSerial.h"

EmTransportSerial::Config::Config(const EmTransportSerial::Config& from) { *this = from; }

EmTransportSerial::Config& EmTransportSerial::Config::operator=(
    const EmTransportSerial::Config& from) {
    if (this == &from) return *this;

    fBaud = from.fBaud;
    fParity = from.fParity;
    fStopBits = from.fStopBits;
    fDataBits = from.fDataBits;
    fHwrHandshake = from.fHwrHandshake;

    return *this;
}

const EmTransportSerial::Config& EmTransportSerial::GetConfig() { return config; }

void EmTransportSerial::SetConfig(const EmTransportSerial::Config& config) {
    this->config = config;
}
