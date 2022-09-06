#include "EmSPISlaveSD.h"

namespace {
    constexpr uint8 ERR_ILLEGAL_COMMAND = 0x04;
}  // namespace

void EmSPISlaveSD::Reset() {
    state = SpiState::notSelected;
    lastCmd = 0;
    idle = true;
    acmd = false;
}

uint16 EmSPISlaveSD::DoExchange(uint16 control, uint16 data) {
    uint8 bits = (control & 0x0f) + 1;
    switch (bits) {
        case 16: {
            uint16 dataOut = DoExchange8(data >> 8) << 8;
            dataOut |= DoExchange8(data);

            return dataOut;
        }

        case 8:
            return DoExchange8(data);

        default:
            cout << "unsupported SPI shift width " << (int)bits << endl << flush;

            return 0xff;
    }
}

void EmSPISlaveSD::Enable(void) {
    state = SpiState::rxCmdByte;
    lastCmd = 0;
}

void EmSPISlaveSD::Disable(void) { state = SpiState::notSelected; }

uint8 EmSPISlaveSD::DoExchange8(uint8 data) {
    switch (state) {
        case SpiState::notSelected:
            return 0xff;

        case SpiState::rxCmdByte:
            if ((data & 0xc0) == 0x40) {
                lastCmd = data & 0x3f;
                state = SpiState::rxArg;
                BufferStart(5);
            }

            return 0xff;

        case SpiState::rxArg:
            buffer[bufferIndex++] = data;

            if (bufferIndex == bufferCount) {
                DoCmd();
                state = SpiState::txResult;
            }

            return 0xff;

        case SpiState::txResult: {
            uint8 dataOut = buffer[bufferIndex++];
            if (bufferIndex == bufferCount) {
                state = SpiState::rxCmdByte;
            }

            return dataOut;
        }
    }
}

void EmSPISlaveSD::BufferStart(uint32 count) {
    bufferCount = count;
    bufferIndex = 0;
}

void EmSPISlaveSD::DoCmd() {
    if (acmd) return DoAcmd();

    cout << "SD card CMD" << (int)lastCmd << endl << flush;

    switch (lastCmd) {
        case 0:
            PrepareR1(0x00);
            return;

        case 1:
            PrepareR1(0x00);
            idle = false;
            return;

        case 55:
            PrepareR1(0x00);
            acmd = true;
            return;

        default:
            cout << "usupported SD CMD" << endl << flush;
            PrepareR1(ERR_ILLEGAL_COMMAND);
            return;
    }
}

void EmSPISlaveSD::DoAcmd() {
    cout << "SD card ACMD" << (int)lastCmd << endl << flush;
    cout << "usupported SD ACMD" << endl << flush;

    PrepareR1(ERR_ILLEGAL_COMMAND);
    acmd = false;
}

void EmSPISlaveSD::PrepareR1(uint8 flags) {
    buffer[0] = 0xff;
    buffer[1] = flags & 0x7f;

    BufferStart(2);
}
