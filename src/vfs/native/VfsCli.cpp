#include "VfsCli.h"

#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <memory>

#include "ReaddirContext.h"
#include "fatfs/diskio.h"
#include "fatfs/ff.h"

using namespace std;

namespace {
    string chomp(const string& s) {
        return s.substr(0, !s.empty() && (s[s.length() - 1] == '\n') ? s.length() - 1 : s.length());
    }
}  // namespace

VfsCli::VfsCli(const std::string& imageFile) : imageFile(imageFile) {}

bool VfsCli::Run() {
    size_t len;
    unique_ptr<uint8_t[]> data;

    {
        fstream stream(imageFile, ios_base::in);
        if (stream.fail()) {
            cout << "unable to open '" << imageFile << "'" << endl;
            return false;
        }

        stream.seekg(0, ios_base::end);
        len = stream.tellg();
        stream.seekg(0, ios_base::beg);

        if (len % 512) {
            cout << "invalid image: not a multiple of 512 byte sectors" << endl;
            return false;
        }

        data = make_unique<uint8_t[]>(len);
        stream.read(reinterpret_cast<char*>(data.get()), len);
        if (static_cast<size_t>(stream.gcount()) != len) {
            cout << "failed to read '" << imageFile << "'" << endl;
            return false;
        }
    }

    CardImage image(data.release(), len >> 9);
    CardVolume volume(image);

    switch (volume.GetType()) {
        case CardVolume::Type::partition:
            cout << "found image: MBR image with FAT partition, " << volume.GetGeometryHeads()
                 << " heads, " << volume.GetGeometrySectors() << " sectors, "
                 << volume.GetSectorsPerCluster() << " sectors/cluster" << endl;
            break;

        case CardVolume::Type::bigFloppy:
            cout << "found image: big floppy, " << volume.GetGeometryHeads() << " heads, "
                 << volume.GetGeometrySectors() << " sectors, " << volume.GetSectorsPerCluster()
                 << " sectors/cluster" << endl;
            break;

        default:
            cout << "invalid image: " << volume.InvalidReason() << endl;
            return false;
    }

    register_card_volume(0, &volume);

    FATFS fs;
    int res;

    res = f_mount(&fs, "", 1);
    if (res != FR_OK) {
        cout << "failed to mount fs: " << res << endl;
        return false;
    }

    ReaddirContext readdirContext("/Palm/Launcher");

    while (static_cast<ReaddirContext::Status>(readdirContext.GetStatus()) ==
           ReaddirContext::Status::more) {
        const FileEntry& entry = readdirContext.GetEntry();

        time_t timestamp = entry.GetModifiedTS();
        tm* calendar = localtime(&timestamp);

        cout << left << (entry.IsDirectory() ? "[D] " : "[F] ") << setw(33)
             << string(entry.GetName()).substr(0, 32) << setw(10) << entry.GetSize() << setw(0)
             << string(calendar ? chomp(asctime(calendar)) : "") << endl;

        readdirContext.Next();
    }

    if (static_cast<ReaddirContext::Status>(readdirContext.GetStatus()) ==
        ReaddirContext::Status::error) {
        cout << "readdir failed for " << readdirContext.GetPath() << ": "
             << readdirContext.GetErrorDescription() << endl;
    }

    return true;
}
