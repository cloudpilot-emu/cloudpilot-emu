#include "RecursiveFsIterator.h"

#include <memory>

#include "Defer.h"
#include "VfsUtil.h"

using namespace std;

namespace {
    constexpr size_t READ_BUFFER_SIZE = 1024;
}

RecursiveFsIterator::RecursiveFsIterator(FatfsDelegate& fatfsDelegate)
    : fatfsDelegate(fatfsDelegate) {}

RecursiveFsIterator::~RecursiveFsIterator() {
    if (scanning) fatfsDelegate.f_closedir(&dir);
}

RecursiveFsIterator& RecursiveFsIterator::AddFile(const std::string& path) {
    if (state == State::initial) files.push_back(path);

    return *this;
}

RecursiveFsIterator& RecursiveFsIterator::AddDirectory(const std::string& path) {
    if (state == State::initial) directories.push_back(path);

    return *this;
}

RecursiveFsIterator::State RecursiveFsIterator::GetState() { return state; }

RecursiveFsIterator::State RecursiveFsIterator::Next() {
    if (state == State::initial) state = State::valid;
    if (state != State::valid) return state;

    while (true) {
        if (files.size() > 0) {
            currentEntry = util::normalizePath(files.back());
            files.pop_back();

            return state;
        } else if (scanning) {
            if (fatfsDelegate.f_readdir(&dir, &filinfo) != FR_OK) {
                state = State::error;
                failingPath = directories[directoryIndex];

                return state;
            }

            if (filinfo.fname[0] == '\0') {
                CloseDir();
                directoryIndex++;

                continue;
            } else if (filinfo.fattrib & AM_DIR) {
                directories.push_back(directories[directoryIndex] + "/" + filinfo.fname);

                continue;
            } else {
                currentEntry =
                    util::normalizePath(directories[directoryIndex] + "/" + filinfo.fname);

                return state;
            }
        } else if (cleanup) {
            if (directories.size() >= 0) {
                currentEntry = util::normalizePath(directories.back());
                directories.pop_back();
            } else {
                state = State::done;
            }

            return state;
        } else if (directoryIndex >= directories.size()) {
            cleanup = true;
            continue;
        } else if (OpenDir(directories[directoryIndex]) != FR_OK) {
            failingPath = directories[directoryIndex];
            return state = State::error;
        }
    }
}

const std::string& RecursiveFsIterator::GetCurrentEntry() { return currentEntry; }

bool RecursiveFsIterator::IsDirectory() { return cleanup; }

void RecursiveFsIterator::ReadCurrent(read_callback cb) {
    if (state != State::valid || cleanup) return;

    unique_ptr<uint8_t[]> buffer = make_unique<uint8_t[]>(READ_BUFFER_SIZE);

    FIL file;
    if (fatfsDelegate.f_open(&file, currentEntry.c_str(), FA_READ) != FR_OK) {
        state = State::error;
        return;
    }

    Defer deferClose([&]() { fatfsDelegate.f_close(&file); });

    size_t bytesReadTotal = 0;
    while (bytesReadTotal < filinfo.fsize) {
        UINT bytesRead;
        if (fatfsDelegate.f_read(&file, buffer.get(), READ_BUFFER_SIZE, &bytesRead) != FR_OK ||
            bytesRead == 0) {
            state = State::error;
            return;
        }

        cb(buffer.get(), bytesRead);
        bytesReadTotal += bytesRead;
    }
}

void RecursiveFsIterator::CloseDir() {
    if (!scanning) return;
    scanning = false;

    fatfsDelegate.f_closedir(&dir);
}

FRESULT RecursiveFsIterator::OpenDir(const string& path) {
    FRESULT result = fatfsDelegate.f_opendir(&dir, path.c_str());
    scanning = result == FR_OK;

    return result;
}
