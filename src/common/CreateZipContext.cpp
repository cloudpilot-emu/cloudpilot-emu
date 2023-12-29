#include "CreateZipContext.h"

#include "zip/zip.h"

CreateZipContext::CreateZipContext() {}

CreateZipContext::~CreateZipContext() {
    if (zip) zip_stream_close(zip);
    if (zipData) free(zipData);
}

bool CreateZipContext::Initialize(int compressionLevel) {
    if (state != State::created) {
        lastError = "already initialized";
        return false;
    }

    zip = zip_stream_open(nullptr, 0, compressionLevel, 'w');
    if (!zip) {
        lastError = "failed to initialize zip stream";
        return false;
    }

    state = State::streamReady;
    return true;
}

bool CreateZipContext::AddEntry(const std::string& name) {
    if (!ClosePendingEntry()) return false;

    if (state != State::streamReady) {
        lastError = "cannot add entry: stream not ready";
        return false;
    }

    if (zip_entry_open(zip, name.c_str()) != 0) {
        lastError = "failed to create entry";
        return false;
    }

    state = State::entryPending;
    return true;
}

bool CreateZipContext::WriteData(void* data, size_t size) {
    if (state != State::entryPending) {
        lastError = "no pending entry";
        return false;
    }

    if (zip_entry_write(zip, data, size) != 0) {
        lastError = "failed to add data to entry";
        return false;
    }

    return true;
}

const void* CreateZipContext::GetZipData() {
    if (state == State::streamFinalized) return zipData;

    if (!ClosePendingEntry()) return nullptr;

    if (state != State::streamReady) {
        lastError = "stream not initialized";
        return nullptr;
    }

    if (zip_stream_copy(zip, reinterpret_cast<void**>(&zipData),
                        reinterpret_cast<ssize_t*>(&zipDataSize)) <= 0) {
        lastError = "failed to copy out zip data";
        return nullptr;
    }

    zip_stream_close(zip);
    zip = nullptr;

    return zipData;
}

size_t CreateZipContext::GetZipDataSize() const { return zipDataSize; }

const char* CreateZipContext::GetLastError() const { return lastError; }

bool CreateZipContext::ClosePendingEntry() {
    if (state != State::entryPending) return true;

    if (zip_entry_close(zip) != 0) {
        lastError = "failed to close pending entry";
        return false;
    }

    state = State::streamReady;
    return true;
}
