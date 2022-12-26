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

    void* Malloc(long size);
    void Free(void* buffer);
    void* Nullptr();

    void AllocateImage(unsigned long blockCount);
    bool MountImage(unsigned long slot);
    void UnmountImage(unsigned long slot);

    long GetPendingImageSize() const;
    long GetSize(long slot) const;
    void* GetPendingImage() const;
    void* GetImage(long slot) const;

   private:
    std::unique_ptr<CardImage> cardImages[FF_VOLUMES];
    std::unique_ptr<CardVolume> cardVolumes[FF_VOLUMES];
    std ::unique_ptr<uint8_t[]> pendingImage;
    size_t pendingImageSize{0};

    FATFS fs[FF_VOLUMES];
};

#endif  // _VFS_H_
