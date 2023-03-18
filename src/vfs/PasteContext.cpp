#include "PasteContext.h"

namespace {
    FatfsDelegate defaultFatfsDelegate;
}

PasteContext::PasteContext(uint32_t timesliceMilliseconds, const char* destination,
                           const char* prefix, FatfsDelegate& fatfsDelegate)
    : GenericCopyContext(timesliceMilliseconds, destination, fatfsDelegate),
      iterator(fatfsDelegate, prefix),
      fatfsDelegate(fatfsDelegate) {
    Initialize(&iterator);
}

PasteContext::PasteContext(uint32_t timesliceMilliseconds, const char* destination,
                           const char* prefix)
    : PasteContext(timesliceMilliseconds, destination, prefix, defaultFatfsDelegate) {}

PasteContext& PasteContext::AddFile(const std::string& path) {
    iterator.AddFile(path);
    return *this;
}

PasteContext& PasteContext::AddDirectory(const std::string& path) {
    iterator.AddDirectory(path);
    return *this;
}

PasteContext& PasteContext::SetDeleteAfterCopy(bool deleteAfterCopy) {
    this->deleteAfterCopy = deleteAfterCopy;
    return *this;
}

int PasteContext::GetState() const {
    if (deleteFailed) return static_cast<int>(State::ioError);

    switch (GenericCopyContext::GetState()) {
        case GenericCopyContext::State::cardFull:
            return static_cast<int>(State::cardFull);

        case GenericCopyContext::State::collision:
            return static_cast<int>(State::collision);

        case GenericCopyContext::State::collisionWithDirectory:
            return static_cast<int>(State::collisionWithDirectory);

        case GenericCopyContext::State::done:
            return static_cast<int>(State::done);

        case GenericCopyContext::State::invalidEntry:
            return static_cast<int>(State::invalidEntry);

        case GenericCopyContext::State::more:
            return static_cast<int>(State::more);

        default:
            return static_cast<int>(State::ioError);
    }
}

int PasteContext::Continue() {
    if (!deleteFailed) GenericCopyContext::Continue();

    return GetState();
}

int PasteContext::ContinueWithOverwrite() {
    if (!deleteFailed) GenericCopyContext::ContinueWithOverwrite();

    return GetState();
}

void PasteContext::OnAfterCopy() {
    if (deleteAfterCopy)
        deleteFailed = fatfsDelegate.f_unlink(iterator.GetFullPath().c_str()) != FR_OK;
}
