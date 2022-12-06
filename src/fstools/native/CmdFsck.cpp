
#include "CmdFsck.h"

#include <fstream>
#include <iostream>

#include "CardImage.h"
#include "CardVolume.h"
#include "card_io.h"
#include "cli.h"
#include "dosfstools/fsck.h"

using namespace std;

CmdFsk::CmdFsk(const argparse::ArgumentParser& cmd) : imageFile(cmd.get(ARGUMENT_IMAGE)) {
    if (cmd.present("--write")) writeFile = cmd.get("--write");
}

bool CmdFsk::Run() {
    size_t len;

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

    card_initialize(&volume);

    cout << "running fsck..." << endl << endl;

    bool result = runFsck(!writeFile.empty());

    cout << endl
         << (result ? "fsck successful" : "check failed, damaged file system") << endl
         << flush;

    if (!writeFile.empty()) {
        fstream stream(writeFile, ios_base::out);
        if (stream.fail()) {
            cout << "failed to open " << writeFile << endl;
            return false;
        }

        stream.write(reinterpret_cast<const char*>(image.RawData()),
                     image.BlocksTotal() * CardImage::BLOCK_SIZE);

        if (stream.fail()) {
            cout << "failed to write " << writeFile << endl;
            return false;
        }

        cout << "modified image written to " << writeFile << endl;
    }

    return true;
}
