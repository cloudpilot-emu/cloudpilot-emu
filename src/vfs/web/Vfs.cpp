#include "Vfs.h"

#include <cstdio>
#include <cstdlib>
#include <iostream>

#include "fatfs/diskio.h"

using namespace std;

namespace {
    const char* drivePrefix(unsigned long slot) {
        static char prefix[3];
        snprintf(prefix, 3, "%ld:", slot);

        return prefix;
    }
}  // namespace

Vfs::~Vfs() {
    for (int i = 0; i < FF_VOLUMES; i++) UnmountImage(i);
}

void* Vfs::Malloc(long size) { return malloc(size); }

void Vfs::Free(void* buffer) { free(buffer); }

void* Vfs::Nullptr() { return nullptr; }

void Vfs::AllocateImage(unsigned long blockCount) {
    pendingImageSize = blockCount * CardImage::BLOCK_SIZE;
    pendingImage = make_unique<uint8_t[]>(pendingImageSize);
}

bool Vfs::MountImage(unsigned long slot) {
    if (slot >= FF_VOLUMES) {
        cerr << "failed to mount: invalid vfs slot " << slot << endl;
        return false;
    }

    if (!pendingImage) {
        cerr << "failed to mount: no pending image" << endl;
        return false;
    }

    UnmountImage(slot);

    auto image = make_unique<CardImage>(pendingImage.release(), pendingImageSize >> 9);
    auto volume = make_unique<CardVolume>(*image);

    if (volume->GetType() == CardVolume::Type::invalid) {
        cerr << "failed to mount: no filesystem" << endl;
        return false;
    }

    cardImages[slot] = std::move(image);
    cardVolumes[slot] = std::move(volume);
    register_card_volume(slot, cardVolumes[slot].get());

    FRESULT mountResult = f_mount(&fs[slot], drivePrefix(slot), 1);
    if (mountResult != FR_OK) {
        cerr << "failed to mount: fatfs error " << (int)mountResult << endl;
        UnmountImage(slot);
        return false;
    }

    return true;
}

void Vfs::UnmountImage(unsigned long slot) {
    if (slot >= FF_VOLUMES) return;

    f_unmount(drivePrefix(slot));
    unregister_card_volume(slot);

    cardVolumes[slot].release();
    cardImages[slot].release();
}

long Vfs::GetPendingImageSize() const { return pendingImage ? pendingImageSize : -1; }

long Vfs::GetSize(long slot) const {
    if (slot >= FF_VOLUMES || !cardImages[slot]) return -1;

    return cardImages[slot]->BlocksTotal() * CardImage::BLOCK_SIZE;
}

void* Vfs::GetPendingImage() const { return pendingImage.get(); }

void* Vfs::GetImage(long slot) const {
    if (slot >= FF_VOLUMES || !cardImages[slot]) return nullptr;

    return cardImages[slot].get();
}
