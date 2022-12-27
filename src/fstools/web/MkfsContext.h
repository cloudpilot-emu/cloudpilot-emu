#ifndef _MKFS_CONTEXT_H_
#define _MKFS_CONTEXT_H_

#include <cstdint>

#include "CardImage.h"

class MkfsContext {
   public:
    ~MkfsContext();

    bool Mkfs(unsigned int size, const char* label);

    void* GetImage() const;
    unsigned int GetImageSize() const;

   private:
    CardImage* image{nullptr};
};

#endif  // _MKFS_CONTEXT_H_
