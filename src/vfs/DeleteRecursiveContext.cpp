#include "DeleteRecursiveContext.h"

#include "VfsUtil.h"

using namespace std;

namespace {
    constexpr uint32_t BATCH_SIZE = 10;
    FatfsDelegate defaultFatfsDelegate;
}  // namespace

DeleteRecursiveContext::DeleteRecursiveContext(uint32_t timesliceMilliseconds,
                                               FatfsDelegate& fatfsDelegate)
    : timesliceMilliseconds(timesliceMilliseconds),
      iterator(fatfsDelegate),
      fatfsDelegate(fatfsDelegate) {}

DeleteRecursiveContext::DeleteRecursiveContext(uint32_t timesliceMilliseconds)
    : DeleteRecursiveContext(timesliceMilliseconds, defaultFatfsDelegate) {}

DeleteRecursiveContext& DeleteRecursiveContext::AddFile(const std::string& path) {
    iterator.AddFile(path);

    return *this;
}

int DeleteRecursiveContext::Continue() {
    if (More()) ExecuteSlice();

    return GetState();
}

int DeleteRecursiveContext::GetState() {
    if (failed) return static_cast<int>(State::error);

    switch (iterator.GetState()) {
        case RecursiveFsIterator::State::initial:
            return static_cast<int>(State::initial);

        case RecursiveFsIterator::State::valid:
            return static_cast<int>(State::more);

        case RecursiveFsIterator::State::done:
            return static_cast<int>(State::done);

        default:
            return static_cast<int>(State::error);
    }
}

const char* DeleteRecursiveContext::GetFailingPath() {
    return failed ? iterator.GetCurrentEntry().c_str() : "";
}

void DeleteRecursiveContext::ExecuteSlice() {
    uint64_t timesliceStart = util::epochMilliseconds();

    while (More() && util::epochMilliseconds() - timesliceStart < timesliceMilliseconds) {
        for (uint32_t batchIndex = 0; batchIndex < BATCH_SIZE && More(); batchIndex++) {
            ExecuteStep();
        }
    }
}

void DeleteRecursiveContext::ExecuteStep() {
    iterator.Next();
    if (!More()) return;

    failed = fatfsDelegate.f_unlink(iterator.GetCurrentEntry().c_str()) != FR_OK;
}

bool DeleteRecursiveContext::More() {
    if (failed) return false;

    switch (iterator.GetState()) {
        case RecursiveFsIterator::State::initial:
        case RecursiveFsIterator::State::valid:
            return true;

        default:
            return false;
    }
}
