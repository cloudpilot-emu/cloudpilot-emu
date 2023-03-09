#include "ZipfileIterator.h"

#include "VfsUtil.h"

using namespace std;

namespace {
    static size_t OnExtract(void* opaque, unsigned long long offset, const void* data,
                            size_t size) {
        auto cb = reinterpret_cast<ZipfileIterator::read_callback*>(opaque);
        (*cb)(data, size);

        return size;
    }
}  // namespace

ZipfileIterator::ZipfileIterator(void* data, size_t size) {
    zip_t* zip = zip_stream_open(static_cast<const char*>(data), size, 0, 'r');
    if (!zip) {
        state = State::error;
        return;
    }

    this->zip = zip;
    entriesTotal = zip_entries_total(zip);

    state = entriesTotal > 0 ? State::initial : State::done;
}

ZipfileIterator::~ZipfileIterator() {
    if (openEntryPending && zip) zip_entry_close(zip);
    if (zip) zip_close(zip);
}

ZipfileIterator::State ZipfileIterator::GetState() { return state; }

ZipfileIterator::State ZipfileIterator::Next() {
    if (state != State::valid && state != State::initial) return state;

    if (openEntryPending && zip_entry_close(zip) < 0) return state = State::error;
    if (currentEntryIndex >= entriesTotal) return state = State::done;

    if (zip_entry_openbyindex(zip, currentEntryIndex++) < 0) return state = State::error;
    openEntryPending = true;

    const char* name = zip_entry_name(zip);
    if (!name) return state = State::error;

    currentEntry = util::normalizePath(string(name));
    state = State::valid;

    return state;
}

const string& ZipfileIterator::GetCurrentEntry() { return currentEntry; }

bool ZipfileIterator::IsDirectory() { return zip ? zip_entry_isdir(zip) : false; }

void ZipfileIterator::ReadCurrent(read_callback cb) {
    if (!zip || state != State::valid) return;
    if (zip_entry_extract(zip, OnExtract, &cb) < 0) state = State::error;
}

uint32_t ZipfileIterator::GetEntriesTotal() const { return entriesTotal; }
