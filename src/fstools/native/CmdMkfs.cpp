#include "CmdMkfs.h"

#include <cstdint>
#include <cstdlib>
#include <fstream>

#include "CardImage.h"
#include "CardVolume.h"
#include "card_io.h"
#include "cli.h"
#include "dosfstools/mkfs.h"
#include "fstools_util.h"

using namespace std;

CmdMkfs::CmdMkfs(const argparse::ArgumentParser& cmd)
    : size(cmd.get<unsigned int>(ARGUMENT_SIZE)), imageFile(cmd.get(ARGUMENT_IMAGE)) {}

bool CmdMkfs::Run() {
    const uint32_t sizeBytes = FSToolsUtil::determineImageSize(size);
    if (sizeBytes == 0) {
        cout << "invalid image size" << endl;
        return false;
    }

    uint8_t* buffer = new uint8_t[sizeBytes];

    CardImage image(buffer, sizeBytes >> 9);
    CardVolume volume(image);
    card_initialize(&volume);

    cout << "creating file system..." << endl << endl;

    volume.Format();

    if (!mkfs(volume.AdvicedClusterSize(), "card")) {
        cout << "failed to create FAT fs";
        return false;
    }

    volume.FixupPartitionType();

    cout << endl;

    {
        fstream stream(imageFile, ios_base::out);
        if (stream.fail()) {
            cout << "failed to open " << imageFile << endl;
            return false;
        }

        stream.write(reinterpret_cast<const char*>(image.RawData()),
                     image.BlocksTotal() * CardImage::BLOCK_SIZE);

        if (stream.fail()) {
            cout << "failed to write " << imageFile << endl;
            return false;
        }

        cout << "image written to " << imageFile << endl;
    }

    return true;
}
