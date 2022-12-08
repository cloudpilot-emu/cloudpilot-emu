#include "CmdCreate.h"

#include <cstdint>
#include <cstdlib>
#include <fstream>

#include "CardImage.h"
#include "CardVolume.h"
#include "cli.h"

using namespace std;

namespace {
    uint32_t determineImageSize(unsigned int size) {
        switch (size) {
            case 4:
                return 494 * 16 * 512;

            case 8:
                return (2 * 496 - 2) * 16 * 512;

            case 16:
                return (2 * 496 - 2) * 32 * 512;

            case 32:
                return (4 * 496 - 2) * 32 * 512;

            case 64:
                return (8 * 496 - 2) * 32 * 512;

            case 128:
                return (16 * 496 - 2) * 32 * 512;

            default:
                return 0;
        }
    }
}  // namespace

CmdCreate::CmdCreate(const argparse::ArgumentParser& cmd)
    : size(cmd.get<unsigned int>(ARGUMENT_SIZE)), imageFile(cmd.get(ARGUMENT_IMAGE)) {}

bool CmdCreate::Run() {
    const uint32_t sizeBytes = determineImageSize(size);
    if (sizeBytes == 0) {
        cout << "invalid image size" << endl;
        return false;
    }

    uint8_t* buffer = new uint8_t[sizeBytes];
    memset(buffer, 0, sizeBytes);

    CardImage image(buffer, sizeBytes >> 9);
    CardVolume volume(image);

    volume.Format();

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
