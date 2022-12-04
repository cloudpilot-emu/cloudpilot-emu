
#include "CmdFsck.h"

#include <fstream>
#include <iostream>

#include "CardImage.h"
#include "CardVolume.h"
#include "cli.h"
#include "dosfstools/fsck.h"

using namespace std;

CmdFsk::CmdFsk(const argparse::ArgumentParser& cmd) : imageFile(cmd.get(ARGUMENT_IMAGE)) {}

bool CmdFsk::Run() {
    fstream stream(imageFile, ios_base::in);
    if (stream.fail()) {
        cout << "unable to open '" << imageFile << "'" << endl;
        return false;
    }

    stream.seekg(0, ios_base::end);
    size_t len = stream.tellg();
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

    CardImage image(data.release(), len >> 9);
    CardVolume volume(image);

    switch (volume.GetType()) {
        case CardVolume::Type::partition:
            cout << "found image: MBR image with FAT partition" << endl;
            break;

        case CardVolume::Type::bigFloppy:
            cout << "found image: big floppy" << endl;
            break;

        default:
            cout << "invalid image: " << volume.InvalidReason() << endl;
            return false;
    }

    runFsck(true);

    return true;
}
