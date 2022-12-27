#include "FsckContext.h"

#include <cstdint>

#include "CardVolume.h"
#include "card_io.h"
#include "dosfstools/fsck.h"

using namespace std;

FsckContext::FsckContext(unsigned int blockCount)
    : image(new uint8_t[blockCount * CardImage::BLOCK_SIZE], blockCount) {}

void* FsckContext::GetImage() { return image.RawData(); }

void* FsckContext::GetDirtyPages() { return image.DirtyPages(); }

unsigned int FsckContext::GetImageSize() const {
    return image.BlocksTotal() * CardImage::BLOCK_SIZE;
}

unsigned int FsckContext::Fsck() {
    if (result != FsckResult::pending) return static_cast<unsigned int>(result);
    result = FsckResult::invalid;

    CardVolume volume(image);
    switch (volume.GetType()) {
        case CardVolume::Type::bigFloppy:
        case CardVolume::Type::partition:
            break;

        default:
            return static_cast<unsigned int>(result);
    }

    card_initialize(&volume);

    result = runFsck(true) ? FsckResult::ok : FsckResult::fixed;
    return static_cast<unsigned int>(result);
}

unsigned int FsckContext::GetResult() const {
    return static_cast<unsigned int>(result);
    ;
}
