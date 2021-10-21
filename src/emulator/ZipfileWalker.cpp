#include "ZipfileWalker.h"

#include "zip.h"

ZipfileWalker::ZipfileWalker(size_t bufferSize, uint8* buffer) {
    this->buffer = make_unique<char[]>(bufferSize);
    memcpy(this->buffer.get(), buffer, bufferSize);

    zip = zip_stream_open(this->buffer.get(), bufferSize, 0, 'r');

    if (zip) {
        entriesTotal = zip_entries_total(zip);
        Next();
    }
}

ZipfileWalker::~ZipfileWalker() {
    if (zip) {
        zip_close(zip);
    }

    if (currentEntryContent) free(currentEntryContent);
}

ZipfileWalker::State ZipfileWalker::GetState() const {
    if (!zip || entriesTotal < 0) return State::error;
    if (done) return State::done;

    return State::open;
}

ZipfileWalker::State ZipfileWalker::Next() {
    while (GetState() == State::open) {
        if (currentEntryContent) {
            free(currentEntryContent);
            currentEntryContent = nullptr;
        }

        if (static_cast<ssize_t>(iNextEntry) >= entriesTotal) {
            done = true;
            break;
        }

        if (zip_entry_openbyindex(zip, iNextEntry++) < 0) continue;
        if (zip_entry_isdir(zip)) continue;

        break;
    }

    return GetState();
}

size_t ZipfileWalker::GetCurrentEntrySize() {
    EmAssert(GetState() == State::open);

    return zip_entry_size(zip);
}

const char* ZipfileWalker::GetCurrentEntryName() {
    EmAssert(GetState() == State::open);

    const char* name = zip_entry_name(zip);
    return name ? name : "";
}

uint8* ZipfileWalker::GetEntryContent() {
    if (currentEntryContent) return currentEntryContent;

    size_t bufferSize;
    if (zip_entry_read(zip, reinterpret_cast<void**>(&currentEntryContent), &bufferSize) < 0 ||
        bufferSize != GetCurrentEntrySize()) {
        if (currentEntryContent) free(currentEntryContent);

        currentEntryContent = nullptr;
        return nullptr;
    }

    return currentEntryContent;
}
