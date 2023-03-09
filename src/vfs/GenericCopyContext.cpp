#include "GenericCopyContext.h"

#include "Defer.h"
#include "VfsUtil.h"

using namespace std;

namespace {
    FatfsDelegate defaultFatfsDelegate;

    struct ExtractContext {
        enum class Status { ok, cardFull, ioError };

        ExtractContext(FIL* file, FatfsDelegate& fatfsDelegate)
            : file(file), fatfsDelegate(fatfsDelegate) {}

        void OnExtract(const void* data, size_t size) {
            if (status != Status::ok) return;

            UINT bytesWritten;
            if (fatfsDelegate.f_write(file, data, size, &bytesWritten) != FR_OK)
                status = Status::ioError;

            if (bytesWritten != size) status = Status::cardFull;
        }

        FIL* file;
        Status status{Status::ok};
        FatfsDelegate& fatfsDelegate;
    };

    string dirname(const std::string& path) {
        size_t pos = path.find_last_of('/');

        return pos == string::npos ? "/" : path.substr(0, pos + 1);
    }
}  // namespace

GenericCopyContext::GenericCopyContext(uint32_t timesliceMilliseconds, const char* _destination,
                                       FatfsDelegate& fatfsDelegate)
    : fatfsDelegate(fatfsDelegate),
      timesliceMilliseconds(timesliceMilliseconds),
      destination("/" + string(_destination)) {}

GenericCopyContext::GenericCopyContext(uint32_t timesliceMilliseconds, const char* destination)
    : GenericCopyContext(timesliceMilliseconds, destination, defaultFatfsDelegate) {}

GenericCopyContext::State GenericCopyContext::GetState() const { return state; }

GenericCopyContext::State GenericCopyContext::Continue() {
    switch (state) {
        case State::done:
        case State::iteratorError:
        case State::ioError:
        case State::cardFull:
            return state;

        case State::collision:
        case State::collisionWithDirectory:
            iterator->Next();
            break;

        default:
            break;
    }

    uint64_t timestamp = util::epochMilliseconds();
    state = State::more;

    while (util::epochMilliseconds() - timestamp < timesliceMilliseconds && state == State::more)
        ExecuteSlice();

    return state;
}

GenericCopyContext::State GenericCopyContext::ContinueWithOverwrite() {
    if (state == State::collision) {
        RemoveConflictingFile();
        if (state != State::more) return state;
    }

    if (state == State::collisionWithDirectory) {
        deleteRecursiveContext = make_unique<DeleteRecursiveContext>(timesliceMilliseconds);
        deleteRecursiveContext->AddDirectory(collisionPath);
        state = State::more;
    }

    return Continue();
}

string GenericCopyContext::GetCurrentEntry() const { return currentEntry; }

string GenericCopyContext::GetCollisionPath() const { return collisionPath; }

uint32_t GenericCopyContext::GetEntriesSuccess() const { return entriesSuccess; }

void GenericCopyContext::Initialize(VfsIterator* iterator) {
    if (this->iterator) return;

    this->iterator = iterator;
    iterator->Next();

    state = iterator->GetState() == VfsIterator::State::error ? State::iteratorError : State::more;
}

void GenericCopyContext::ExecuteSlice() {
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

    switch (iterator->GetState()) {
        case VfsIterator::State::done:
            state = State::done;
            return;

        case VfsIterator::State::error:
            state = State::iteratorError;
            return;

        default:
            break;
    }

    Defer skipIfCollision([&]() {
        if (state != State::collision && state != State::collisionWithDirectory) iterator->Next();
    });

    currentEntry = iterator->GetCurrentEntry();
    currentPath = util::normalizePath(destination + "/" + currentEntry);

    if (iterator->IsDirectory()) {
        MkdirRecursive(currentPath);
        if (state == State::more) entriesSuccess++;
        return;
    }

    MkdirRecursive(dirname(currentPath));
    if (state != State::more) return;

    ExtractCurrentEntry();
    if (state == State::more) entriesSuccess++;
}

void GenericCopyContext::RemoveConflictingFile() {
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

void GenericCopyContext::ExtractCurrentEntry() {
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
        if (state == State::cardFull || state == State::iteratorError)
            fatfsDelegate.f_unlink(currentPath.c_str());
    });

    ExtractContext context(&file, fatfsDelegate);
    iterator->ReadCurrent(
        bind(&ExtractContext::OnExtract, &context, placeholders::_1, placeholders::_2));

    if (iterator->GetState() == VfsIterator::State::error) {
        state = State::iteratorError;
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

void GenericCopyContext::MkdirRecursive(std::string path) {
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
