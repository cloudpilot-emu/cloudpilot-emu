#include "VfsCli.h"

#include <fstream>
#include <iostream>
#include <memory>

#include "fatfs/diskio.h"
#include "fatfs/ff.h"

using namespace std;

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

    DIR dir;
    res = f_opendir(&dir, "/Palm");
    if (res != FR_OK) {
        cout << "failed to open /Palm: " << res << endl;
        return false;
    }

    cout << endl;

    FILINFO filinfo;
    while (true) {
        res = f_readdir(&dir, &filinfo);
        if (res != FR_OK) {
            cout << "failed to read dir: " << res << endl;
            return false;
        }

        if (filinfo.fname[0] == '\0') break;

        cout << filinfo.fname << endl;
    }

    return true;
}
