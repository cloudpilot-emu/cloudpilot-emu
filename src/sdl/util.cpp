#include "util.h"

#include <fstream>

#include "EmSession.h"
#include "SessionImage.h"

bool util::readFile(string file, unique_ptr<uint8[]>& buffer, size_t& len) {
    fstream stream(file, ios_base::in);
    if (stream.fail()) return false;

    stream.seekg(0, ios_base::end);
    len = stream.tellg();

    stream.seekg(0, ios_base::beg);
    buffer = make_unique<uint8[]>(len);

    stream.read((char*)buffer.get(), len);
    if (static_cast<size_t>(stream.gcount()) != len) return false;

    return true;
}

void util::analyzeRom(EmROMReader& reader) {
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

bool util::initializeSession(string file) {
    unique_ptr<uint8[]> fileBuffer;
    size_t fileSize;

    if (!util::readFile(file, fileBuffer, fileSize)) {
        cerr << "unable to open " << file << endl;

        return false;
    }

    SessionImage sessionImage = SessionImage::Deserialize(fileSize, fileBuffer.get());
    if (sessionImage.IsValid()) {
        cout << "restoring session image" << endl << flush;

        if (!gSession->LoadImage(sessionImage)) {
            cerr << "failed to restore image" << endl << flush;
            return false;
        }

        return true;
    }

    EmROMReader reader(fileBuffer.get(), fileSize);

    if (!reader.Read()) {
        cerr << "unable to read ROM --- not a valid ROM image?" << endl;

        return false;
    }

    util::analyzeRom(reader);

    EmDevice* device = new EmDevice("PalmV");

    if (!device->SupportsROM(reader)) {
        delete device;
        device = new EmDevice("m515");
    }

    if (!device->SupportsROM(reader)) {
        cerr << "ROM not supported by Palm V or Palm m515" << endl;

        return false;
    }

    if (!gSession->Initialize(device, fileBuffer.get(), fileSize)) {
        cerr << "Session failed to initialize" << endl;

        return false;
    }

    return true;
}
