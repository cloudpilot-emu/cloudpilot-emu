#include "MemoryStick.h"

namespace {
    constexpr uint8 INT_TPC_OK = 0x80;
    constexpr uint8 INT_TPC_ERR = 0x40;
    // constexpr uint8 INT_TPC_READ_FOR_TRANSFER = 0x20;
    // constexpr uint8 INT_TPC_INVALID = 0x01;

    // constexpr uint8 ACCESS_BLOCK = 0x00;
    // constexpr uint8 ACCESS_PAGE = 0x20;
    // constexpr uint8 ACCESS_OOB_ONLY = 0x40;
    // constexpr uint8 ACCESS_OOB_ONLY_NO_EEC = 0x80;

    constexpr uint8 TPC_SET_REGS_WINDOW = 0x08;
    constexpr uint8 TPC_REGS_WRITE = 0x0b;
    constexpr uint8 TPC_REGS_READ = 0x04;
    constexpr uint8 TPC_SET_CMD = 0x0e;
    constexpr uint8 TPC_READ_LONG_DATA = 0x02;
    constexpr uint8 TPC_WRITE_LONG_DATA = 0x0c;
}  // namespace

void MemoryStick::Reset() {
    memset(&reg, 0, sizeof(reg));

    reg.msType = 0xff;
    reg.msCategory = 0xff;
    reg.msClass = 0xff;

    writeWindowStart = 0x10;
    writeWindowSize = 15;

    bufferOutSize = 0;
}

void MemoryStick::ExecuteTpc(uint8 tpcId, uint32 dataInCount, uint8* dataIn) {
    bufferOutSize = 0;

    switch (tpcId) {
        case TPC_REGS_WRITE: {
            if (dataInCount != writeWindowSize || writeWindowStart < offsetof(Registers, cfg) ||
                writeWindowSize + writeWindowStart > sizeof(Registers)) {
                reg.intFlags = INT_TPC_ERR;
                break;
            }

            uint8* registerFile = reinterpret_cast<uint8*>(&reg);
            memcpy(registerFile + writeWindowStart, dataIn, writeWindowSize);

            reg.intFlags = INT_TPC_OK;

            break;
        }

        default:
            cerr << "unsupported TCP 0x" << hex << (int)tpcId << dec << endl << flush;
            reg.intFlags = INT_TPC_ERR;
    }

    irq.Dispatch();
}

uint8* MemoryStick::GetDataOut() { return bufferOut; }

uint32 MemoryStick::GetDataOutSize() { return bufferOutSize; }

MemoryStick::TpcType MemoryStick::GetTpcType(uint8 tpcId) {
    switch (tpcId) {
        case TPC_SET_REGS_WINDOW:
        case TPC_REGS_WRITE:
        case TPC_SET_CMD:
        case TPC_WRITE_LONG_DATA:
            return TpcType::write;

        case TPC_REGS_READ:
        case TPC_READ_LONG_DATA:
        default:
            return TpcType::read;
    }
}
