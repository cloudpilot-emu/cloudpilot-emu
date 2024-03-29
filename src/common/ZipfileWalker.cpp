#include "ZipfileWalker.h"

#include <cstdlib>

#include "zip/zip.h"

ZipfileWalker::ZipfileWalker(size_t bufferSize, void* buffer) {
    this->buffer = std::make_unique<char[]>(bufferSize);
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
    if (!zip || entriesTotal < 0) return State::stateError;
    if (done) return State::stateDone;

    return State::stateOpen;
}

ZipfileWalker::State ZipfileWalker::Next() {
    while (GetState() == State::stateOpen) {
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

uint32_t ZipfileWalker::GetTotalEntries() { return entriesTotal; }

size_t ZipfileWalker::GetCurrentEntrySize() {
    return GetState() == State::stateOpen ? zip_entry_size(zip) : 0;
}

const char* ZipfileWalker::GetCurrentEntryName() {
    if (GetState() != State::stateOpen) return "";

    const char* name = zip_entry_name(zip);
    return name ? name : "";
}

uint8_t* ZipfileWalker::GetCurrentEntryContent() {
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
