#include "DebugSupport.h"

#include <iomanip>

#include "ElfParser.h"
#include "EmBankSRAM.h"
#include "EmHAL.h"
#include "EmMemory.h"

void debug_support::SetApp(const uint8* elfData, size_t elfSize, GdbStub& gdbStub) {
    gdbStub.ClearRelocationOffset();

    ElfParser parser;

    try {
        parser.Parse(elfData, elfSize);
    } catch (const ElfParser::EInvalidElf& e) {
        cout << e.GetReason() << endl << flush;
        return;
    }

    if (parser.GetMachine() != 0x04) {
        cout << "bad architecture" << endl << flush;
        return;
    }

    const auto sectionText = parser.GetSection(".text");
    if (!sectionText.has_value()) {
        cout << ".text not found in binary" << endl << flush;
        return;
    }

    const emuptr textBase = FindRegion(elfData + sectionText->offset, sectionText->size,
                                       gMemoryStart, EmMemory::GetRegionSize(MemoryRegion::ram));
    if (textBase == 0xffffffff) {
        cout << "unable to locate application in memory" << endl << flush;
    }

    const int64 relocation = static_cast<int64>(textBase) - sectionText->virtualAddress;
    cout << "found .text relocated by " << relocation << " bytes" << endl << flush;

    gdbStub.SetRelocationOffset(relocation);
}

emuptr debug_support::FindRegion(const uint8* region, size_t regionSize, emuptr start,
                                 size_t size) {
    if (regionSize > size) return 0xffffffff;

    for (emuptr address = start; address < start + size - regionSize; address++) {
        for (emuptr i = 0; i < regionSize; i++)
            if (region[i] != EmMemGet8(i + address)) goto nextAddress;

        return address;

    nextAddress:;
    }

    return 0xffffffff;
}

void debug_support::Locate(const uint8* data, size_t size) {
    emuptr address = gMemoryStart;
    const size_t ramSize = Memory::GetRegionSize(MemoryRegion::ram);
    bool found = false;

    do {
        address = FindRegion(data, size, address, ramSize + gMemoryStart - address);

        if (address < 0xffffffff) {
            cout << "located file content in RAM at 0x" << hex << setfill('0') << setw(8) << address
                 << dec << endl
                 << flush;

            found = true;
        } else {
            break;
        }

        address += size;
    } while (address >= gMemoryStart && address < gMemoryStart + ramSize);

    const emuptr romStart = EmHAL::GetROMBaseAddress();
    const size_t romSize = EmHAL::GetROMSize();
    address = romStart;

    do {
        address = FindRegion(data, size, address, romSize + romStart - address);

        if (address < 0xffffffff) {
            cout << "located file content in ROM at 0x" << hex << setfill('0') << setw(8) << address
                 << dec << endl
                 << flush;

            found = true;
        } else {
            break;
        }

        address += size;
    } while (address >= romStart && address < romStart + romSize);

    if (!found) cout << "unable to locate file" << endl << flush;
}
