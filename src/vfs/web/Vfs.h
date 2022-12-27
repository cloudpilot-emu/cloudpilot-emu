#ifndef _VFS_H_
#define _VFS_H_

#include <cstdint>
#include <memory>

#include "CardImage.h"
#include "CardVolume.h"
#include "fatfs/ff.h"

class Vfs {
   public:
    ~Vfs();

    void* Malloc(int size);
    void Free(void* buffer);
    void* Nullptr();

    void AllocateImage(unsigned int blockCount);
    bool MountImage(unsigned int slot);
    void UnmountImage(unsigned int slot);

    int GetPendingImageSize() const;
    int GetSize(int slot) const;
    void* GetPendingImage() const;
    void* GetImage(int slot) const;

   private:
    std::unique_ptr<CardImage> cardImages[FF_VOLUMES];
    std::unique_ptr<CardVolume> cardVolumes[FF_VOLUMES];
    std ::unique_ptr<uint8_t[]> pendingImage;
    size_t pendingImageSize{0};

    FATFS fs[FF_VOLUMES];
};

#endif  // _VFS_H_
