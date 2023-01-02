#include "ReaddirContext.h"

#include <cstddef>
#include <cstring>
#include <ctime>
#include <iostream>

using namespace std;

ReaddirContext::ReaddirContext(const char* path) : path(path) {
    err = f_opendir(&dir, path);
    if (err != FR_OK) return;

    status = Status::more;
    Next();
}

ReaddirContext::~ReaddirContext() {
    if (status == Status::more) f_closedir(&dir);
}

int ReaddirContext::Next() {
    if (status != Status::more) return static_cast<int>(status);

    err = f_readdir(&dir, fileEntry.GetFilinfo());
    if (err != FR_OK) {
        f_closedir(&dir);
        return static_cast<int>(status = Status::error);
    }

    if (*fileEntry.GetName() == '\0') {
        f_closedir(&dir);
        return static_cast<int>(status = Status::done);
    }

    return static_cast<int>(status);
}

const char* ReaddirContext::GetPath() const { return path.c_str(); }

const FileEntry& ReaddirContext::GetEntry() const { return fileEntry; }

int ReaddirContext::GetStatus() const { return static_cast<int>(status); }

int ReaddirContext::GetError() const {
    switch (err) {
        case FR_OK:
            return static_cast<int>(Error::none);

        case FR_NO_PATH:
            return static_cast<int>(Error::no_such_directory);

        default:
            return static_cast<int>(Error::unknown);
    }
}

const char* ReaddirContext::GetErrorDescription() const {
    switch (static_cast<Error>(GetError())) {
        case Error::none:
            return "no error";

        case Error::no_such_directory:
            return "no such directory";

        default:
            return "unknown error";
    }
}
