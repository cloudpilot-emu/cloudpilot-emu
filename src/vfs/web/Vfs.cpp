#include "Vfs.h"

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string_view>

#include "AutocloseFile.h"
#include "fatfs/diskio.h"

using namespace std;

namespace {
    const char* drivePrefix(unsigned int slot) {
        static char prefix[3] = {'0', ':', '\0'};
        prefix[0] = '0' + (slot % 10);

        return prefix;
    }
}  // namespace

Vfs::~Vfs() {
    for (int i = 0; i < FF_VOLUMES; i++) UnmountImage(i);
}

void* Vfs::Malloc(int size) { return malloc(size); }

void Vfs::Free(void* buffer) { free(buffer); }

void* Vfs::Nullptr() { return nullptr; }

void Vfs::AllocateImage(unsigned int blockCount) {
    pendingImageSize = blockCount * CardImage::BLOCK_SIZE;
    pendingImage = make_unique<uint8_t[]>(pendingImageSize);
}

bool Vfs::MountImage(unsigned int slot) {
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

void Vfs::UnmountImage(unsigned int slot) {
    if (slot >= FF_VOLUMES) return;

    f_unmount(drivePrefix(slot));
    unregister_card_volume(slot);

    cardVolumes[slot].reset();
    cardImages[slot].reset();
}

int Vfs::GetPendingImageSize() const { return pendingImage ? pendingImageSize : -1; }

int Vfs::GetSize(unsigned int slot) const {
    if (slot >= FF_VOLUMES || !cardImages[slot]) return -1;

    return cardImages[slot]->BlocksTotal() * CardImage::BLOCK_SIZE;
}

void* Vfs::GetPendingImage() const { return pendingImage.get(); }

void* Vfs::GetImage(unsigned int slot) const {
    if (slot >= FF_VOLUMES || !cardImages[slot]) return nullptr;

    return cardImages[slot]->RawData();
}

void* Vfs::GetDirtyPages(unsigned int slot) const {
    if (slot >= FF_VOLUMES || !cardImages[slot]) return nullptr;

    return cardImages[slot]->DirtyPages();
}

int Vfs::RenameFile(const char* from, const char* to) { return f_rename(from, to); }

int Vfs::ChmodFile(const char* path, int attr, int mask) { return f_chmod(path, attr, mask); }

int Vfs::StatFile(const char* path) { return f_stat(path, fileEntry.GetFilinfo()); }

int Vfs::UnlinkFile(const char* path) { return f_unlink(path); }

const FileEntry& Vfs::GetEntry() const { return fileEntry; }

unsigned int Vfs::BytesFree(unsigned int slot) const {
    if (slot >= FF_VOLUMES || !cardImages[slot]) return 0;

    DWORD clustersFree;
    FATFS* fs;
    if (f_getfree(drivePrefix(slot), &clustersFree, &fs) != FR_OK) return 0;

    return clustersFree * fs->csize * 512;
}

unsigned int Vfs::BytesTotal(unsigned int slot) const {
    if (slot >= FF_VOLUMES || !cardImages[slot]) return 0;

    return (fs[slot].n_fatent - 2) * fs[slot].csize * 512;
}

bool Vfs::ReadFile(const char* path) {
    currentFileSize = 0;
    currentFileContent.release();

    FILINFO filinfo;
    if (f_stat(path, &filinfo) != FR_OK || (filinfo.fattrib & AM_DIR)) return false;

    currentFileContent = make_unique<uint8_t[]>(filinfo.fsize);
    currentFileSize = filinfo.fsize;

    FIL file;
    if (f_open(&file, path, FA_READ) != FR_OK) return false;
    AutocloseFile autoclose(&file);

    UINT bytesRead;
    return f_read(&file, currentFileContent.get(), filinfo.fsize, &bytesRead) == FR_OK &&
           (bytesRead == filinfo.fsize);
}

size_t Vfs::GetCurrentFileSize() const { return currentFileSize; }

void* Vfs::GetCurrentFileContent() const { return currentFileContent.get(); }

void Vfs::ReleaseCurrentFile() {
    currentFileContent.reset();
    currentFileSize = 0;
}
