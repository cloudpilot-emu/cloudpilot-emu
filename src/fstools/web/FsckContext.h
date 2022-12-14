#ifndef _FSCK_CONTEXT_H_
#define _FSCK_CONTEXT_H_

#include "CardImage.h"

class FsckContext {
   public:
    enum class FsckResult { pending = 0, ok = 1, fixed = 2, invalid = 3 };

   public:
    FsckContext(unsigned long blockCount);

    void *GetImage();
    void *GetDirtyPages();
    unsigned long GetImageSize() const;

    unsigned long Fsck();
    unsigned long GetResult() const;

   private:
    CardImage image;
    FsckResult result{FsckResult::pending};
};

#endif  // _FSCK_CONTEXT_H_
