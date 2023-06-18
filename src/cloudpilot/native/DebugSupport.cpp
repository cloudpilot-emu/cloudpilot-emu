#include "DebugSupport.h"

#include <iomanip>

#include "ElfParser.h"

void debug_support::SetApp(const uint8* elfData, size_t elfSize) {
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

    cout << "parsing complete" << endl << flush;
    cout << "start address 0x" << hex << setw(8) << setfill('0') << parser.GetEntrypoint() << dec
         << endl
         << flush;

    for (const ElfParser::Section& section : parser.GetSections()) {
        cout << "section " << section.name << ", " << section.size << " bytes at 0x" << hex
             << setw(8) << setfill('0') << section.virtualAddress << dec << endl
             << flush;
    }
}
