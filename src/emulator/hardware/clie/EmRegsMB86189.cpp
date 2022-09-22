#include "EmRegsMB86189.h"

#define INSTALL_HANDLER(read, write, offset, size)                                             \
    this->SetHandler((ReadFunction)&EmRegsMB86189::read, (WriteFunction)&EmRegsMB86189::write, \
                     baseAddress + offset, size)

namespace {
    constexpr uint32 REGISTER_FILE_SIZE = 0x14;

    constexpr uint32 OFFSET_MSCMD = 0x00;
    constexpr uint32 OFFSET_MSCS = 0x02;
    constexpr uint32 OFSSET_MSDATA = 0x04;
    constexpr uint32 OFFSET_MSICS = 0x06;
    constexpr uint32 OFFSET_MSPPCD = 0x08;

    constexpr uint8 IRQ_RDY = 0x80;
    constexpr uint8 IRQ_SIF = 0x40;
}  // namespace

EmRegsMB86189::EmRegsMB86189(emuptr baseAddress) : baseAddress(baseAddress) {
    memoryStick.irq.AddHandler([=]() { RaiseIrq(IRQ_SIF); });
}

void EmRegsMB86189::Reset(Bool hardwareReset) {
    reg.mscmd = 0;
    reg.mscs = 0x0a05;
    reg.msics = 0x8000;
    reg.msppcd = 0;

    state = State::idle;

    irqStat = 0;
    memoryStick.Reset();

    fifo.Clear();

    outBufferSize = 0;
    inBufferIndex = 0;
}

uint8* EmRegsMB86189::GetRealAddress(emuptr address) {
    EmAssert(false);

    // We don't support raw access
    return nullptr;
}

emuptr EmRegsMB86189::GetAddressStart(void) { return baseAddress; }

uint32 EmRegsMB86189::GetAddressRange(void) { return REGISTER_FILE_SIZE; }

void EmRegsMB86189::SetSubBankHandlers(void) {
    INSTALL_HANDLER(mscmdRead, mscmdWrite, OFFSET_MSCMD, 2);
    INSTALL_HANDLER(mscsRead, mscsWrite, OFFSET_MSCS, 2);
    INSTALL_HANDLER(msdataRead, msdataWrite, OFSSET_MSDATA, 2);
    INSTALL_HANDLER(msicsRead, msicsWrite, OFFSET_MSICS, 2);
    INSTALL_HANDLER(msppcdRead, msppcdWrite, OFFSET_MSPPCD, 2);
    INSTALL_HANDLER(stubRead, stubRead, OFFSET_MSPPCD + 2, REGISTER_FILE_SIZE - OFFSET_MSPPCD - 2);
}

void EmRegsMB86189::RaiseIrq(uint8 bits) {
    if ((reg.msics & 0x08) == 0) return;

    irqStat |= bits;

    UpdateIrqLine();
    TransferIrqStat();
}

void EmRegsMB86189::NegateIrq(uint8 bits) {
    if ((reg.msics & 0x08) == 0) return;

    irqStat &= ~bits;

    UpdateIrqLine();
}

void EmRegsMB86189::ClearIrq(uint8 bits) {
    if ((reg.msics & 0x08) == 0) return;

    NegateIrq(bits);
    TransferIrqStat();
}

void EmRegsMB86189::UpdateIrqLine() {
    if (irqStat != 0) {
        if ((reg.mscs & 0x8000) == 0) irq.Dispatch();
        reg.mscs |= 0x8000;
    }
}

void EmRegsMB86189::TransferIrqStat() {
    reg.msics &= 0x00ff;
    reg.msics |= (irqStat << 8);
}

void EmRegsMB86189::BeginTpc() {
    uint8 tpcId = reg.mscmd >> 12;
    uint32 transferSize = reg.mscmd & 0x1ff;

    state = MemoryStick::GetTpcType(tpcId) == MemoryStick::TpcType::read ? State::tpcRead
                                                                         : State::tpcWrite;

    outBufferSize = 0;
    inBufferIndex = 0;
    if (state == State::tpcRead) memoryStick.ExecuteTpc(tpcId);

    if (transferSize == 0) {
        FinishTpc();
    } else if (state == State::tpcWrite) {
        while (fifo.Size() > 0 && state != State::idle) FifoWrite(fifo.Pop());
    }
}

void EmRegsMB86189::FinishTpc() {
    memoryStick.ExecuteTpc(reg.mscmd >> 12, reg.mscmd & 0x1ff, outBuffer);

    fifo.Clear();

    state = State::idle;
    RaiseIrq(IRQ_RDY);
}

void EmRegsMB86189::FifoWrite(uint16 data) {
    if (state != State::idle) {
        outBuffer[outBufferSize++] = data >> 8;
        if (outBufferSize >= (reg.mscmd & 0x1ff)) return FinishTpc();

        outBuffer[outBufferSize++] = data;
        if (outBufferSize >= (reg.mscmd & 0x1ff)) FinishTpc();

        return;
    }

    if (fifo.Size() == 8) {
        cerr << "MSHCS: fifo overflow during write" << endl << flush;
        return;
    }

    fifo.Push(data);
}

uint32 EmRegsMB86189::mscmdRead(emuptr address, int size) {
    if (size != 2) {
        cerr << "illegal read of size " << size << " from MSCMD (byte " << (address - baseAddress)
             << ")" << endl
             << flush;
        return 0;
    }

    cerr << "MSCMD -> 0x" << hex << reg.mscmd << dec << endl << flush;

    return reg.mscmd;
}

uint32 EmRegsMB86189::mscsRead(emuptr address, int size) {
    uint32 value = compositeRegisterRead(baseAddress + OFFSET_MSCS, address, size, reg.mscs);

    cerr << "MSCS_" << (address - baseAddress - OFFSET_MSCS) << " -> 0x" << hex << value << dec
         << endl
         << flush;

    return value;
}

uint32 EmRegsMB86189::msdataRead(emuptr address, int size) {
    if (size != 2) {
        cerr << "illegal read of size " << size << " from MSDATA" << endl << flush;
        return 0;
    }

    uint32 value = 0;

    if (state == State::tpcRead) {
        uint8* buffer = memoryStick.GetDataOut();
        uint32 bufferSize = memoryStick.GetDataOutSize();

        value = buffer[inBufferIndex++] << 8;
        if (inBufferIndex < bufferSize) value |= buffer[inBufferIndex++];

        if (inBufferIndex >= bufferSize) FinishTpc();
    }

    cerr << "MSDATA -> 0x" << hex << value << dec << endl << flush;

    return value;
}

uint32 EmRegsMB86189::msicsRead(emuptr address, int size) {
    uint32 value = compositeRegisterRead(baseAddress + OFFSET_MSICS, address, size, reg.msics);

    NegateIrq(IRQ_SIF);

    if (state == State::idle)
        value &= ~0x8000;
    else
        value |= 0x8000;

    cerr << "MSICS_" << (address - baseAddress - OFFSET_MSICS) << " -> 0x" << hex << value << dec
         << endl
         << flush;

    return value;
}

uint32 EmRegsMB86189::msppcdRead(emuptr address, int size) {
    uint32 value = compositeRegisterRead(baseAddress + OFFSET_MSPPCD, address, size, reg.msppcd);

    cerr << "MSPPCD_" << (address - baseAddress - OFFSET_MSPPCD) << " -> 0x" << hex << value << dec
         << endl
         << flush;

    return value;
}

void EmRegsMB86189::mscmdWrite(emuptr address, int size, uint32 value) {
    if (state != State::idle) {
        cerr << "ignoring MSCMD write during running transaction" << endl << flush;
        return;
    }

    if (size != 2) {
        cerr << "illegal write of size " << size << " (byte " << (address - baseAddress)
             << ") to MSCMD (0x" << hex << value << dec << ")" << endl
             << flush;

        return;
    }

    cerr << "MSCMD <- 0x" << hex << value << dec << endl << flush;

    ClearIrq(IRQ_RDY | IRQ_SIF);

    reg.mscmd = value;
    BeginTpc();
}

void EmRegsMB86189::mscsWrite(emuptr address, int size, uint32 value) {
    cerr << "MSCS_" << (address - baseAddress - OFFSET_MSCS) << " <- 0x" << hex << value << dec
         << endl
         << flush;

    compositeRegisterWrite(baseAddress + OFFSET_MSCS, address, size, value, reg.mscs);
}

void EmRegsMB86189::msdataWrite(emuptr address, int size, uint32 value) {
    if (size != 2) {
        cerr << "illegal write of size " << size << " (0x" << hex << value << dec << ") to MSDATA"
             << endl
             << flush;

        return;
    }

    cerr << "MSDATA <- 0x" << hex << value << dec << endl << flush;

    FifoWrite(value);
}

void EmRegsMB86189::msicsWrite(emuptr address, int size, uint32 value) {
    cerr << "MSICS_" << (address - baseAddress - OFFSET_MSICS) << " <- 0x" << hex << value << dec
         << endl
         << flush;

    compositeRegisterWrite(baseAddress + OFFSET_MSICS, address, size, value, reg.msics);
}

void EmRegsMB86189::msppcdWrite(emuptr address, int size, uint32 value) {
    cerr << "MSCPPCD_" << (address - baseAddress - OFFSET_MSPPCD) << " <- 0x" << hex << value << dec
         << endl
         << flush;

    compositeRegisterWrite(baseAddress + OFFSET_MSPPCD, address, size, value, reg.msppcd);
}

uint32 EmRegsMB86189::stubRead(emuptr address, int size) {
    cerr << "unimplemented MSHC read of size " << size << " from 0x" << hex << address << dec
         << endl
         << flush;

    return 0;
}

void EmRegsMB86189::stubdWrite(emuptr address, int size, uint32 value) {
    cerr << "unimplemented MSHC write of size " << size << " to 0x" << hex << address << " (0x"
         << value << dec << ")" << endl
         << flush;
}

uint32 EmRegsMB86189::compositeRegisterRead(emuptr base, emuptr address, int size, uint16 target) {
    switch (size) {
        case 1:
            return base == address ? (target >> 8) : (target & 0xff);

        case 2:
            return target;

        case 4:
            cerr << "invalid 32 bit read from MB86189 0x" << hex << address << dec << endl << flush;
            return 0;

        default:
            EmAssert(false);

            return 0;
    }
}

inline void EmRegsMB86189::compositeRegisterWrite(emuptr base, emuptr address, int size,
                                                  uint32 value, uint16& target) {
    switch (size) {
        case 1:
            if (base == address)
                target = (target & 0x00ff) | (value << 8);
            else
                target = (target & 0xff00) | (value & 0xff);

            return;

        case 2:
            target = value;
            return;

        case 4:
            cerr << "invalid 32 bit read from MB86189 0x" << hex << address << " <- 0x" << value
                 << dec << endl
                 << flush;

            return;

        default:
            EmAssert(false);
    }
}
