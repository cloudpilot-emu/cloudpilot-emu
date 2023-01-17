#include "StackDump.h"

#include <algorithm>
#include <fstream>
#include <iomanip>

#include "EmBankROM.h"
#include "EmCPU68K.h"
#include "EmMemory.h"
#include "UAE.h"

namespace {
    constexpr uint32 RTS_SEARCH_LIMIT = 0x400;
    bool wroteRam{false};

    bool inRom(emuptr ptr) {
        emuptr romStart = EmBankROM::GetMemoryStart();
        uint32 romSize = EmBankROM::GetRomSize();

        return (ptr >= romStart) && (ptr - romStart < romSize);
    }

    string identifyFrame(emuptr pc) {
        emuptr rtsAddr;

        for (rtsAddr = pc; rtsAddr < pc + RTS_SEARCH_LIMIT; rtsAddr += 2) {
            if (!inRom(rtsAddr)) return "unknown: rts not in ROM";
            if (EmMemGet16(rtsAddr) == 0x4e75) break;
        }

        if (rtsAddr >= pc + 0x0400) return "unknown: rts not found";

        char name[64];
        memset(name, 0, sizeof(name));

        size_t i;
        for (i = 0; i < sizeof(name) - 1; i++) {
            unsigned char c = EmMemGet8(rtsAddr + 3 + i);

            if (c >= 128) {
                return "unknown: bad char in function name";
            }
            if (c == 0) break;

            name[i] = c;
        }

        return i == sizeof(name) ? "unknown: function name not terminated" : name;
    }

    void dumpMemory(emuptr start, emuptr stop, uint32 limit) {
        if (start >= stop) return;

        if (stop - start >= limit) {
            cerr << "[frame too large]" << endl;
            return;
        }

        cerr << hex << "0x" << start << " - 0x" << stop << endl;

        uint8 byteInLine = 0;

        for (emuptr addr = start; addr < stop; addr++) {
            cerr << hex << right << setfill('0') << setw(2) << (int)EmMemGet8(addr);

            if (byteInLine++ == 16) {
                byteInLine = 0;
                cerr << endl;
            } else {
                cerr << " ";
            }
        }

        cerr << dec << endl;
    }
}  // namespace

StackDump& StackDump::FrameCount(uint8 frameCount) {
    this->frameCount = frameCount;
    return *this;
}

StackDump& StackDump::DumpFrames(bool dumpFrames) {
    this->dumpFrames = dumpFrames;
    return *this;
}

StackDump& StackDump::DumpRegisters(bool dumpRegisters) {
    this->dumpRegisters = dumpRegisters;
    return *this;
}

StackDump& StackDump::MaxFrameSize(uint32 maxFrameSize) {
    this->maxFrameSize = maxFrameSize;
    return *this;
}

StackDump& StackDump::WriteRam(const string& file) {
    ramfile = file;

    return *this;
}

void StackDump::Dump() const {
    uint32 pc = regs.pc;
    uint32 fp = m68k_areg(regs, 6);

    if (dumpRegisters) {
        cerr << hex;
        cerr << "pc: 0x" << regs.pc << " ";
        for (int i = 0; i <= 7; i++) cerr << "d" << i << ": 0x" << m68k_dreg(regs, i) << " ";
        cerr << endl;
        for (int i = 0; i <= 7; i++) cerr << "a" << i << ": 0x" << m68k_dreg(regs, i) << " ";
        cerr << dec << endl;
    }

    for (uint8 i = 0; i < frameCount; i++) {
        if (!inRom(pc)) {
            cerr << "ERROR: pc 0x" << hex << pc << dec << " not in ROM" << endl << flush;
            break;
        }

        cerr << "frame " << (int)i << ": 0x" << hex << pc << dec << " [" << identifyFrame(pc) << "]"
             << endl;

        if (dumpFrames) {
            dumpMemory(fp, EmMemGet32(fp), maxFrameSize);
        }

        pc = EmMemGet32(fp + 4);
        fp = EmMemGet32(fp);
    }

    if (!ramfile.empty() && !wroteRam) {
        fstream stream(ramfile, ios_base::out);

        if (stream.fail()) {
            cerr << "failed to open " << ramfile << endl;
            return;
        }

        stream.write((const char*)EmMemory::GetForRegion(MemoryRegion::ram),
                     EmMemory::GetRegionSize(MemoryRegion::ram));

        if (stream.fail()) {
            cerr << "I/O error writing " << ramfile << endl;
        } else {
            cerr << "wrote RAM image to " << ramfile << endl;
            wroteRam = true;
        }
    }

    cerr << flush;
}
