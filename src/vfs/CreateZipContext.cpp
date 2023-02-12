#include "CreateZipContext.h"

#include <chrono>

#include "Defer.h"
#include "zip.h"

using namespace std;

namespace {
    constexpr int COMPRESSION_LEVEL = 1;
    constexpr size_t READ_BUFFER_SIZE = 32 * 1024;

    uint64_t epochMilliseconds() {
        return chrono::duration_cast<chrono::milliseconds>(
                   chrono::system_clock::now().time_since_epoch())
            .count();
    }
}  // namespace

CreateZipContext::CreateZipContext(const string& prefix, uint32_t timesliceMilliseconds)
    : prefix(prefix),
      timesliceMilliseconds(timesliceMilliseconds),
      readBuffer(make_unique<uint8_t[]>(READ_BUFFER_SIZE)) {
    if (prefix.size() > 0 && prefix[prefix.size() - 1] != '/') this->prefix.append("/");
}

CreateZipContext::~CreateZipContext() {
    if (zip) zip_stream_close(zip);
    if (archive) free(archive);
    CloseCurrentDir();
}

CreateZipContext& CreateZipContext::AddFile(const string& path) {
    if (state == State::initial) files.push_back(path);

    return *this;
}

CreateZipContext& CreateZipContext::AddDirectory(const string& path) {
    if (state == State::initial) directories.push_back(path);

    return *this;
}

int CreateZipContext::Continue() {
    switch (state) {
        case State::initial:
            zip = zip_stream_open(nullptr, 0, COMPRESSION_LEVEL, 'w');
            state = State::more;
            ExecuteSlice();
            break;

        case State::more:
        case State::errorFile:
        case State::errorDirectory:
            ExecuteSlice();
            break;

        case State::done:
            break;
    }

    return static_cast<int>(state);
}

int CreateZipContext::GetState() const { return static_cast<int>(state); }

uint8_t* CreateZipContext::GetZipContent() const { return archive; }

ssize_t CreateZipContext::GetZipSize() { return archiveSize; }

const char* CreateZipContext::GetErrorItem() const {
    switch (state) {
        case State::errorFile:
            return currentFile.c_str();

        case State::errorDirectory:
            return currentDirectory.c_str();

        default:
            return "";
    }
}

void CreateZipContext::ExecuteSlice() {
    timesliceStart = epochMilliseconds();

    if (state == State::errorFile || state == State::errorDirectory) state = State::more;

    while (state == State::more && epochMilliseconds() - timesliceStart < timesliceMilliseconds) {
        ExecuteStep();
    }
}

void CreateZipContext::ExecuteStep() {
    if (state != State::more) return;

    if (reading) {
        IncrementalReadCurrentFile();
    } else if (files.size() > 0) {
        AddFileToArchive(files.back());
        files.pop_back();
    } else if (scanning) {
        FILINFO filinfo;

        if (f_readdir(&dir, &filinfo) != FR_OK) {
            CloseCurrentDir();
            state = State::errorDirectory;
        } else {
            if (filinfo.fname[0] == '\0') {
                CloseCurrentDir();
            } else if (filinfo.fattrib & AM_DIR) {
                directories.push_back(currentDirectory + "/" + filinfo.fname);
            } else {
                AddFileToArchive(currentDirectory + "/" + filinfo.fname);
            }
        }
    } else if (directories.size() > 0) {
        currentDirectory = directories.back();
        directories.pop_back();

        if (OpenCurrentDir() != FR_OK) state = State::errorDirectory;
    } else {
        state = State::done;
        zip_stream_copy(zip, reinterpret_cast<void**>(&archive), &archiveSize);
    }
}

void CreateZipContext::AddFileToArchive(const std::string& name) {
    currentFile = name;

    string entryName = name;
    if (entryName.find(prefix) == 0) entryName.erase(0, prefix.length());

    if (zip_entry_open(zip, entryName.c_str()) != 0) {
        state = State::errorFile;
        return;
    }

    if (f_open(&file, name.c_str(), FA_READ) != FR_OK) {
        zip_entry_close(zip);
        state = State::errorFile;
        return;
    }

    reading = true;
    IncrementalReadCurrentFile();
}

void CreateZipContext::IncrementalReadCurrentFile() {
    if (!reading) return;

    UINT bytesRead = 0;
    do {
        if (f_read(&file, readBuffer.get(), READ_BUFFER_SIZE, &bytesRead) != FR_OK) {
            state = State::errorFile;
            break;
        }

        if (bytesRead > 0) {
            if (zip_entry_write(zip, readBuffer.get(), bytesRead) != 0) {
                state = State::errorFile;
                break;
            }
        }
    } while (bytesRead > 0 && epochMilliseconds() - timesliceStart < timesliceMilliseconds);

    if (bytesRead == 0 || state == State::errorFile) {
        f_close(&file);
        zip_entry_close(zip);
        reading = false;
    }
}

FRESULT CreateZipContext::OpenCurrentDir() {
    if (scanning) return FR_INT_ERR;

    FRESULT result = f_opendir(&dir, currentDirectory.c_str());
    scanning = result == FR_OK;

    return result;
}

void CreateZipContext::CloseCurrentDir() {
    if (!scanning) return;
    scanning = false;

    f_closedir(&dir);
}
