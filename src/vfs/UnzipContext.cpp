#include "UnzipContext.h"

#include <regex>

#include "Defer.h"
#include "VfsUtil.h"

using namespace std;

namespace {
    regex REGEX_MULTI_SLASH("/{2,}|\\\\+");

    FatfsDelegate defaultFatfsDelegate;

    struct ExtractContext {
        enum class Status { ok, cardFull, ioError };

        ExtractContext(FIL* file, FatfsDelegate& fatfsDelegate)
            : file(file), fatfsDelegate(fatfsDelegate) {}

        static size_t OnExtract(void* opaque, unsigned long long offset, const void* data,
                                size_t size) {
            ExtractContext* self = reinterpret_cast<ExtractContext*>(opaque);
            if (self->status != Status::ok) return size;

            UINT bytesWritten;
            if (self->fatfsDelegate.f_write(self->file, data, size, &bytesWritten) != FR_OK)
                self->status = Status::ioError;

            if (bytesWritten != size) self->status = Status::cardFull;

            return size;
        }

        FIL* file;
        Status status{Status::ok};
        FatfsDelegate& fatfsDelegate;
    };

    string dirname(const std::string& path) {
        size_t pos = path.find_last_of('/');

        return pos == string::npos ? "/" : path.substr(0, pos + 1);
    }

    string normalize(const std::string& path) {
        return regex_replace(path, REGEX_MULTI_SLASH, "/");
    }
}  // namespace

UnzipContext::UnzipContext(uint32_t timesliceMilliseconds, const char* _destination, void* data,
                           size_t size, FatfsDelegate& fatfsDelegate)
    : fatfsDelegate(fatfsDelegate),
      timesliceMilliseconds(timesliceMilliseconds),
      destination("/" + string(_destination)) {
    zip_t* zip = zip_stream_open(static_cast<const char*>(data), size, 0, 'r');
    if (!zip) {
        state = State::zipfileError;
        return;
    }

    this->zip = zip;

    entriesTotal = zip ? zip_entries_total(zip) : 0;
    state = zip ? (entriesTotal > 0 ? State::more : State::done) : State::zipfileError;
}

UnzipContext::UnzipContext(uint32_t timesliceMilliseconds, const char* destination, void* data,
                           size_t size)
    : UnzipContext(timesliceMilliseconds, destination, data, size, defaultFatfsDelegate) {}

UnzipContext::~UnzipContext() {
    if (zip) zip_close(zip);
}

int UnzipContext::GetState() const { return static_cast<int>(state); }

int UnzipContext::Continue() {
    switch (state) {
        case State::done:
        case State::zipfileError:
        case State::ioError:
        case State::cardFull:
            return static_cast<int>(state);

        case State::collision:
        case State::collisionWithDirectory:
            currentEntryIndex++;
            break;

        default:
            break;
    }

    uint64_t timestamp = util::epochMilliseconds();
    state = State::more;

    while (util::epochMilliseconds() - timestamp < timesliceMilliseconds && state == State::more)
        ExecuteSlice();

    return static_cast<int>(state);
}

int UnzipContext::ContinueWithOverwrite() {
    if (state == State::collision) {
        RemoveConflictingFile();
        if (state != State::more) return static_cast<int>(state);
    }

    if (state == State::collisionWithDirectory) {
        deleteRecursiveContext = make_unique<DeleteRecursiveContext>(timesliceMilliseconds);
        deleteRecursiveContext->AddDirectory(collisionPath);
        state = State::more;
    }

    return Continue();
}

const char* UnzipContext::GetCurrentEntry() const { return currentEntry.c_str(); }

const char* UnzipContext::GetCollisionPath() const { return collisionPath.c_str(); }

uint32_t UnzipContext::GetEntriesTotal() const { return entriesTotal; }

uint32_t UnzipContext::GetEntriesSuccess() const { return entriesSuccess; }

void UnzipContext::ExecuteSlice() {
    if (state != State::more) return;

    if (deleteRecursiveContext) {
        switch (deleteRecursiveContext->Continue()) {
            case static_cast<int>(DeleteRecursiveContext::State::more):
                return;

            case static_cast<int>(DeleteRecursiveContext::State::done):
                deleteRecursiveContext.reset();
                return;

            default:
                state = State::ioError;
                deleteRecursiveContext.reset();
                return;
        }
    }

    if (currentEntryIndex >= entriesTotal) {
        state = State::done;
        return;
    }

    if (zip_entry_openbyindex(zip, currentEntryIndex) < 0) {
        state = State::zipfileError;
        return;
    }

    Defer deferClose([&]() {
        zip_entry_close(zip);
        if (state != State::collision && state != State::collisionWithDirectory)
            currentEntryIndex++;
    });

    const char* name = zip_entry_name(zip);
    if (!name) return;

    currentEntry = name;
    currentPath = normalize(destination + "/" + currentEntry);

    if (zip_entry_isdir(zip)) {
        MkdirRecursive(currentPath);
        if (state == State::more) entriesSuccess++;
        return;
    }

    MkdirRecursive(dirname(currentPath));
    if (state != State::more) return;

    ExtractCurrentEntry();
    if (state == State::more) entriesSuccess++;
}

void UnzipContext::RemoveConflictingFile() {
    switch (fatfsDelegate.f_unlink(collisionPath.c_str())) {
        case FR_INVALID_NAME:
            state = State::invalidEntry;
            return;

        case FR_OK:
            state = State::more;
            return;

        default:
            state = State::ioError;
            return;
    }
}

void UnzipContext::ExtractCurrentEntry() {
    FILINFO fileInfo;
    switch (fatfsDelegate.f_stat(currentPath.c_str(), &fileInfo)) {
        case FR_INVALID_NAME:
            state = State::invalidEntry;
            return;

        case FR_OK:
            state = (fileInfo.fattrib & AM_DIR) ? State::collisionWithDirectory : State::collision;
            collisionPath = currentPath;
            return;

        case FR_NO_FILE:
            break;

        default:
            state = State::ioError;
            return;
    }

    FIL file;
    switch (fatfsDelegate.f_open(&file, currentPath.c_str(), FA_WRITE | FA_CREATE_ALWAYS)) {
        case FR_INVALID_NAME:
            state = State::invalidEntry;
            return;

        case FR_OK:
            break;

        default:
            state = State::ioError;
            return;
    }

    Defer deferClose([&]() {
        f_close(&file);
        if (state == State::cardFull || state == State::zipfileError)
            fatfsDelegate.f_unlink(currentPath.c_str());
    });

    ExtractContext context(&file, fatfsDelegate);
    if (zip_entry_extract(zip, ExtractContext::OnExtract, static_cast<void*>(&context)) < 0) {
        state = State::zipfileError;
        return;
    }

    switch (context.status) {
        case ExtractContext::Status::ioError:
            state = State::ioError;
            return;

        case ExtractContext::Status::cardFull:
            state = State::cardFull;
            return;

        default:
            state = State::more;
            return;
    }
}

void UnzipContext::MkdirRecursive(std::string path) {
    state = State::more;

    string next = "";
    size_t pos;

    do {
        pos = path.find_first_of('/');
        next = next + "/" + (pos == string::npos ? path : path.substr(0, pos));

        if (pos != string::npos) path.erase(0, pos + 1);
        if (next == "/") {
            next.clear();
            continue;
        }

        if (visitedDirectories.find(next) != visitedDirectories.end()) {
            if (visitedDirectories[next]) {
                continue;
            } else {
                state = State::invalidEntry;
                return;
            }
        }

        FILINFO fileInfo;
        switch (fatfsDelegate.f_stat(next.c_str(), &fileInfo)) {
            case FR_INVALID_NAME:
                state = State::invalidEntry;
                visitedDirectories[next] = false;
                return;

            case FR_OK:
                if (fileInfo.fattrib & AM_DIR) {
                    visitedDirectories[next] = true;
                    continue;
                } else {
                    state = State::collision;
                    collisionPath = next;
                    return;
                }

                break;

            case FR_NO_FILE:
                break;

            default:
                state = State::ioError;
                return;
        }

        switch (fatfsDelegate.f_mkdir(next.c_str())) {
            case FR_INVALID_NAME:
                state = State::invalidEntry;
                visitedDirectories[next] = false;
                return;

            case FR_OK:
                visitedDirectories[next] = true;
                break;

            default:
                state = State::ioError;
                return;
        }
    } while (pos != string::npos && path.length() != 0);
}
