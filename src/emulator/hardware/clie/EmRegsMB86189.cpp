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
}  // namespace

EmRegsMB86189::EmRegsMB86189(emuptr baseAddress) : baseAddress(baseAddress) {}

void EmRegsMB86189::Reset(Bool hardwareReset) {
    reg.mscmd = 0;
    reg.mscs = 0x0a05;
    reg.msics = 0x8000;
    reg.msppcd = 0;
}

uint8* EmRegsMB86189::GetRealAddress(emuptr address) {
    EmAssert(false);

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

    cerr << "MSDATA -> 0x00" << endl << flush;

    return 0;
}

uint32 EmRegsMB86189::msicsRead(emuptr address, int size) {
    uint32 value = compositeRegisterRead(baseAddress + OFFSET_MSICS, address, size, reg.msics);

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
    if (size != 2) {
        cerr << "illegal write of size " << size << " (byte " << (address - baseAddress)
             << ") to MSCMD (0x" << hex << value << dec << ")" << endl
             << flush;

        return;
    }

    cerr << "MSCMD <- 0x" << hex << value << dec << endl << flush;

    reg.mscmd = value;
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
