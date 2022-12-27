#ifndef _FSCK_CONTEXT_H_
#define _FSCK_CONTEXT_H_

#include "CardImage.h"

class FsckContext {
   public:
    enum class FsckResult { pending = 0, ok = 1, fixed = 2, invalid = 3, unfixable = 4 };

   public:
    FsckContext(unsigned int blockCount);

    void *GetImage();
    void *GetDirtyPages();
    unsigned int GetImageSize() const;

    unsigned int Fsck();
    unsigned int GetResult() const;

   private:
    CardImage image;
    FsckResult result{FsckResult::pending};
};

#endif  // _FSCK_CONTEXT_H_
