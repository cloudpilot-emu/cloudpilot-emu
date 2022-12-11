#ifndef _MKFS_CONTEXT_H_
#define _MKFS_CONTEXT_H_

#include <cstdint>

#include "CardImage.h"

class MkfsContext {
   public:
    ~MkfsContext();

    bool Mkfs(unsigned long size, const char* label);

    void* GetImage() const;
    unsigned long GetImageSize() const;

   private:
    CardImage* image{nullptr};
};

#endif  // _MKFS_CONTEXT_H_
