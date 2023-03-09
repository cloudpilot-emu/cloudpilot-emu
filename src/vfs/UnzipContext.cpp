#include "UnzipContext.h"

using namespace std;

UnzipContext::UnzipContext(uint32_t timesliceMilliseconds, const char* destination, void* data,
                           size_t size, FatfsDelegate& fatfsDelegate)
    : GenericCopyContext(timesliceMilliseconds, destination, fatfsDelegate), iterator(data, size) {
    Initialize(&iterator);
}

UnzipContext::UnzipContext(uint32_t timesliceMilliseconds, const char* destination, void* data,
                           size_t size)
    : GenericCopyContext(timesliceMilliseconds, destination), iterator(data, size) {
    Initialize(&iterator);
}

int UnzipContext::GetState() const { return static_cast<int>(GenericCopyContext::GetState()); }

int UnzipContext::Continue() { return static_cast<int>(GenericCopyContext::Continue()); }

int UnzipContext::ContinueWithOverwrite() {
    return static_cast<int>(GenericCopyContext::ContinueWithOverwrite());
}

uint32_t UnzipContext::GetEntriesTotal() const { return iterator.GetEntriesTotal(); }
