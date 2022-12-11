#include "MkfsContext.h"

#include <iostream>

#include "CardVolume.h"
#include "card_io.h"
#include "dosfstools/mkfs.h"
#include "fstools_util.h"

using namespace std;

bool MkfsContext::Mkfs(unsigned long size, const char* label) {
    if (image) {
        cout << "double invocation of Mkfs" << endl;
        return false;
    }

    uint32_t imageSize = FSToolsUtil::determineImageSize(size);
    if (imageSize == 0) return false;

    image = new CardImage(new uint8_t[imageSize], imageSize >> 9);
    CardVolume volume(*image);
    card_initialize(&volume);

    volume.Format();
    if (!mkfs(volume.AdvicedClusterSize(), label)) {
        cout << "failed to create FAT fs";
        return false;
    }

    volume.FixupPartitionType();

    return true;
}

void* MkfsContext::GetImage() const { return image ? image->RawData() : nullptr; }

unsigned long MkfsContext::GetImageSize() const {
    return image ? image->BlocksTotal() * CardImage::BLOCK_SIZE : 0;
}

MkfsContext::~MkfsContext() {
    if (image) delete image;
}
