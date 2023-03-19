#include "PasteContext.h"

#include "VfsUtil.h"

using namespace std;

namespace {
    FatfsDelegate defaultFatfsDelegate;

    string qualifyPath(const string& path) {
        string qualifiedPath = util::normalizePath(path);
        if (qualifiedPath.size() >= 2 && path[1] != ':') qualifiedPath = "0:" + qualifiedPath;

        return qualifiedPath;
    }
}  // namespace

PasteContext::PasteContext(uint32_t timesliceMilliseconds, const char* destination,
                           const char* prefix, FatfsDelegate& fatfsDelegate)
    : GenericCopyContext(timesliceMilliseconds, destination, fatfsDelegate),
      iterator(fatfsDelegate, qualifyPath(prefix)),
      fatfsDelegate(fatfsDelegate),
      destination(qualifyPath(destination)) {
    iterator.SetSkipDirectory(
        [&](const string& path) { return this->destination.find(path) == 0; });
    Initialize(&iterator);
}

PasteContext::PasteContext(uint32_t timesliceMilliseconds, const char* destination,
                           const char* prefix)
    : PasteContext(timesliceMilliseconds, destination, prefix, defaultFatfsDelegate) {}

PasteContext& PasteContext::AddFile(const std::string& path) {
    iterator.AddFile(path);
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
    if (!deleteAfterCopy) return;

    FRESULT result = fatfsDelegate.f_unlink(iterator.GetFullPath().c_str());

    deleteFailed = result != FR_OK && result != FR_DENIED;
}
