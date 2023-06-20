#include "DebugSupport.h"

#include <iomanip>

#include "Debugger.h"
#include "Defer.h"
#include "ElfParser.h"
#include "EmBankSRAM.h"
#include "EmHAL.h"
#include "EmMemory.h"
#include "ROMStubs.h"

namespace {
    emuptr locateCodeResource(const char* name, uint32& size) {
        LocalID lidDB = DmFindDatabase(0, name);
        if (lidDB == 0) {
            cout << "unable to find DB " << name << endl << flush;
            return 0xffffffff;
        }

        uint16 attributes;
        if (DmDatabaseInfo(0, lidDB, nullptr, &attributes, nullptr, nullptr, nullptr, nullptr,
                           nullptr, nullptr, nullptr, nullptr, nullptr) != errNone) {
            cout << "failed to get DB info" << endl << flush;
            return 0xffffffff;
        }

        if ((attributes & dmHdrAttrResDB) == 0) {
            cout << "not a resource DB" << endl << flush;
            return 0xffffffff;
        }

        emuptr dbRef = DmOpenDatabase(0, lidDB, dmModeReadOnly);
        if (dbRef == 0) {
            cout << "unable to open DB" << endl << flush;
            return 0xffffffff;
        }

        Defer deferCloseDB([=]() { DmCloseDatabase(dbRef); });

        emuptr rscHandle = DmGet1Resource('code', 1);
        if (rscHandle == 0) {
            cout << "unable to locate code resource" << endl << flush;
            return 0xffffffff;
        }

        Defer deferReleaseResource([=]() { DmReleaseResource(rscHandle); });

        emuptr rscPtr = MemHandleLock(rscHandle);
        if (rscHandle == 0) {
            cout << "unable to lock code resource" << endl << flush;
            return 0xffffffff;
        }

        size = MemPtrSize(rscPtr);

        MemHandleUnlock(rscHandle);

        return rscPtr;
    }
}  // namespace

void debug_support::SetApp(const uint8* elfData, size_t elfSize, const char* dbName,
                           GdbStub& gdbStub, Debugger& debugger) {
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

    emuptr searchBase = gMemoryStart;
    uint32 searchSize = EmMemory::GetRegionSize(MemoryRegion::ram);

    if (dbName) {
        searchBase = locateCodeResource(dbName, searchSize);
        if (searchBase == 0xffffffff) return;
    }

    const emuptr textBase =
        FindRegion(elfData + sectionText->offset, sectionText->size, searchBase, searchSize);

    if (textBase == 0xffffffff) {
        cout << "unable to locate application in " << (dbName ? "database" : "memory") << endl
             << flush;
        return;
    }

    const int64 relocation = static_cast<int64>(textBase) - sectionText->virtualAddress;
    cout << "found .text relocated by " << relocation << " bytes" << endl;
    cout << "set break mode to app-only" << endl << flush;

    gdbStub.SetRelocationOffset(relocation);
    debugger.SetBreakMode(Debugger::BreakMode::appOnly);
    debugger.SetAppRegion(textBase, sectionText->size);
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
