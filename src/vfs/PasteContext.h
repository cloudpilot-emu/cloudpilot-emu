#ifndef _PASTE_CONTEXT_H_
#define _PASTE_CONTEXT_H_

#include "FatfsDelegate.h"
#include "GenericCopyContext.h"
#include "RecursiveFsIterator.h"

class PasteContext : public GenericCopyContext {
   public:
    enum class State : int {
        more = 0,
        done = 1,
        collision = 2,
        invalidEntry = 3,
        collisionWithDirectory = 4,
        ioError = -1,
        cardFull = -2
    };

   public:
    PasteContext(uint32_t timesliceMilliseconds, const char* destination, const char* prefix,
                 FatfsDelegate& fatfsDelegate);

    PasteContext(uint32_t timesliceMilliseconds, const char* destination, const char* prefix);

    PasteContext& AddFile(const std::string& path);
    PasteContext& AddDirectory(const std::string& path);
    PasteContext& SetDeleteAfterCopy(bool deleteAfterCopy);

    int GetState() const;
    int Continue();
    int ContinueWithOverwrite();

   protected:
    void OnAfterCopy() override;

   private:
    RecursiveFsIterator iterator;
    FatfsDelegate& fatfsDelegate;
    bool deleteAfterCopy{false};
    bool deleteFailed{false};

   private:
    PasteContext(const PasteContext&) = delete;
    PasteContext(PasteContext&&) = delete;
    PasteContext& operator=(const PasteContext&) = delete;
    PasteContext& operator=(PasteContext&&) = delete;
};

#endif  // _PASTE_CONTEXT_H_
