#include "DeleteRecursiveContext.h"

#include "VfsUtil.h"

using namespace std;

namespace {
    constexpr uint32_t BATCH_SIZE = 10;
}

DeleteRecursiveContext::DeleteRecursiveContext(uint32_t timesliceMilliseconds)
    : timesliceMilliseconds(timesliceMilliseconds) {}

DeleteRecursiveContext::~DeleteRecursiveContext() {
    if (scanning) f_closedir(&dir);
}

DeleteRecursiveContext& DeleteRecursiveContext::AddFile(const std::string& path) {
    if (state == State::initial) files.push_back(path);

    return *this;
}

DeleteRecursiveContext& DeleteRecursiveContext::AddDirectory(const std::string& path) {
    if (state == State::initial) directories.push_back(path);

    return *this;
}

int DeleteRecursiveContext::Continue() {
    switch (state) {
        case State::initial:
            state = State::more;
            ExecuteSlice();
            break;

        case State::more:
            ExecuteSlice();
            break;

        default:
            break;
    }

    return static_cast<int>(state);
}

int DeleteRecursiveContext::GetState() const { return static_cast<int>(state); }

const char* DeleteRecursiveContext::GetFailingPath() const { return failingPath.c_str(); }

void DeleteRecursiveContext::ExecuteSlice() {
    uint64_t timesliceStart = util::epochMilliseconds();

    while (state == State::more &&
           util::epochMilliseconds() - timesliceStart < timesliceMilliseconds) {
        for (uint32_t batchIndex = 0; batchIndex < BATCH_SIZE && state == State::more;
             batchIndex++) {
            ExecuteStep();
        }
    }
}

void DeleteRecursiveContext::ExecuteStep() {
    if (files.size() > 0) {
        Unlink(files.back());
        files.pop_back();
    } else if (scanning) {
        FILINFO filinfo;
        if (f_readdir(&dir, &filinfo) != FR_OK) {
            state = State::error;
            failingPath = directories[directoryIndex];
            return;
        }

        if (filinfo.fname[0] == '\0') {
            CloseDir();

            if (directoryIndex == directories.size() - 1) {
                cleanup = true;
            } else {
                directoryIndex++;
            }
        } else if (filinfo.fattrib & AM_DIR) {
            directories.push_back(directories[directoryIndex] + "/" + filinfo.fname);
        } else {
            Unlink(directories[directoryIndex] + "/" + filinfo.fname);
        }
    } else if (cleanup) {
        Unlink(directories[directoryIndex]);
        if (state == State::error) return;

        if (directoryIndex == 0) {
            state = State::done;
        } else {
            directoryIndex--;
        }
    } else {
        if (directoryIndex >= directories.size()) {
            state = State::done;
        } else {
            if (OpenDir(directories[directoryIndex]) != FR_OK) {
                state = State::error;
                failingPath = directories[directoryIndex];
            }
        }
    }
}

void DeleteRecursiveContext::Unlink(const string& path) {
    if (f_unlink(path.c_str()) != FR_OK) {
        state = State::error;
        failingPath = path;
    }
}

FRESULT DeleteRecursiveContext::OpenDir(const string& path) {
    FRESULT result = f_opendir(&dir, path.c_str());
    scanning = result == FR_OK;

    return result;
}

void DeleteRecursiveContext::CloseDir() {
    if (!scanning) return;
    scanning = false;

    f_closedir(&dir);
}
