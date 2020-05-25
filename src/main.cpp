#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>

#include "EmDevice.h"
#include "EmROMReader.h"
#include "EmSession.h"

using namespace std;

bool readFile(string file, unique_ptr<uint8[]>& buffer, long& len) {
    fstream stream(file, ios_base::in);
    if (stream.fail()) return false;

    stream.seekg(0, ios_base::end);
    len = stream.tellg();

    stream.seekg(0, ios_base::beg);
    buffer = make_unique<uint8[]>(len);

    stream.read((char*)buffer.get(), len);
    if (stream.gcount() != len) return false;

    return true;
}

void analyzeRom(EmROMReader& reader) {
    cout << "ROM info" << endl;
    cout << "================================================================================"
         << endl;
    cout << "Card version:          " << reader.GetCardVersion() << endl;
    cout << "Card name:             " << reader.GetCardName() << endl;
    cout << "Card manufacturer:     " << reader.GetCardManufacturer() << endl;
    cout << "Store version:         " << reader.GetStoreVersion() << endl;
    cout << "Company ID:            " << reader.GetCompanyID() << endl;
    cout << "HAL ID:                " << reader.GetHalID() << endl;
    cout << "ROM version:           " << reader.GetRomVersion() << endl;
    cout << "ROM version string:    " << reader.GetRomVersionString() << endl;

    cout << "CPU:                   ";
    if (reader.GetFlag328()) cout << "328 ";
    if (reader.GetFlagEZ()) cout << "EZ ";
    if (reader.GetFlagSZ()) cout << "SZ ";
    if (reader.GetFlagVZ()) cout << "VZ ";
    cout << endl;

    cout << "Databases:             ";
    for (auto&& database : reader.Databases()) {
        cout << database.Name() << " ";
    }
    cout << endl;

    cout << "================================================================================"
         << endl;
}

int main(int argc, const char** argv) {
    if (argc != 2) {
        cerr << "usage: cloudpalm <romimage.rom>" << endl;

        exit(1);
    }

    string file = argv[1];
    unique_ptr<uint8[]> buffer;
    long len;

    if (!readFile(file, buffer, len)) {
        cerr << "unable to open " << file << endl;

        exit(1);
    }

    EmROMReader reader(buffer.get(), len);

    if (!reader.AcquireCardHeader() || !reader.AcquireROMHeap() || !reader.AcquireDatabases() ||
        !reader.AcquireFeatures()) {
        cerr << "unable to read ROM --- not a valid ROM image?" << endl;

        exit(1);
    }

    analyzeRom(reader);

    EmDevice* device = new EmDevice("PalmV");

    if (!device->SupportsROM(reader)) {
        cerr << "ROM not supported by Palm V" << endl;

        exit(1);
    }

    if (!gSession->Initialize(device, buffer.get(), len)) {
        cerr << "Session failed to initialize" << endl;

        exit(1);
    }

    gSession->RunEmulation();
}
