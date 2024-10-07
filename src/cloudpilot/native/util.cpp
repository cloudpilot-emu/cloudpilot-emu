#include "util.h"

#include <fstream>

#include "EmSession.h"
#include "ExternalStorage.h"
#include "SessionImage.h"
#include "md5.h"

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

bool util::initializeSession(string file, optional<string> deviceId) {
    unique_ptr<uint8[]> fileBuffer;
    size_t fileSize;

    if (!util::ReadFile(file, fileBuffer, fileSize)) {
        cerr << "unable to open " << file << endl;

        return false;
    }

    SessionImage sessionImage;

    if (sessionImage.Deserialize(fileBuffer.get(), fileSize)) {
        cout << "restoring session image" << endl << flush;

        if (deviceId && *deviceId != sessionImage.GetDeviceId()) {
            cerr << "device ID " << *deviceId << " does not match session ID "
                 << sessionImage.GetDeviceId() << endl;

            return false;
        }

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

    EmDevice* device = nullptr;

    for (auto id : SUPPORTED_DEVICES) {
        if (deviceId && id != *deviceId) continue;

        EmDevice* d = new EmDevice(id);

        if (d->SupportsROM(reader)) {
            device = d;
            break;
        }

        delete d;
    }

    if (!device) {
        cerr << (deviceId ? "deviceId not supported by ROM" : "unsupported ROM") << endl;

        return false;
    }

    cout << "using device: " << device->GetIDString() << endl;

    if (!gSession->Initialize(device, fileBuffer.get(), fileSize)) {
        cerr << "Session failed to initialize" << endl;

        return false;
    }

    return true;
}

bool util::mountImage(const string& image) { return mountKey(registerImage(image)); }

string util::registerImage(const string& image) {
    unique_ptr<uint8[]> fileBuffer;
    size_t fileSize;

    if (!util::ReadFile(image, fileBuffer, fileSize)) {
        cerr << "unable to open card " << image << endl;

        return "";
    }

    string key = md5(fileBuffer.get(), fileSize);

    if (!gExternalStorage.AddImage(key, fileBuffer.get(), fileSize)) {
        cerr << "failed to register card " << image << endl;

        return "";
    }

    fileBuffer.release();
    return key;
}

bool util::mountKey(const string& key) {
    if (key.empty()) return false;

    if (!gExternalStorage.IsMounted(key) && !gExternalStorage.Mount(key)) {
        cerr << "failed to mount card" << endl << flush;
        gExternalStorage.RemoveImage(key);

        return false;
    }

    return true;
}

EmHAL::Slot util::mountedSlot() {
    EmHAL::Slot slot = EmHAL::Slot::none;
    for (auto s : {EmHAL::Slot::sdcard, EmHAL::Slot::memorystick})
        if (gExternalStorage.IsMounted(s)) slot = s;

    return slot;
}
