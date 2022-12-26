#include "ReaddirContext.h"

#include <cstddef>
#include <cstring>
#include <ctime>

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

long ReaddirContext::Next() {
    if (status != Status::more) return static_cast<long>(status);

    err = f_readdir(&dir, &filinfo);
    if (err != FR_OK) {
        f_closedir(&dir);
        return static_cast<long>(status = Status::error);
    }

    if (filinfo.fname[0] == '\0') {
        f_closedir(&dir);
        return static_cast<long>(status = Status::done);
    }

    return static_cast<long>(status);
}

const char* ReaddirContext::GetPath() const { return path.c_str(); }

const char* ReaddirContext::GetEntryName() const {
    if (status == Status::error || !IsFilenameValid()) return "";

    return filinfo.fname;
}

bool ReaddirContext::IsEntryDirectory() const {
    if (status == Status::error) return false;

    return filinfo.fattrib & AM_DIR;
}

unsigned long ReaddirContext::GetSize() const {
    if (status == Status::error) return 0;

    return filinfo.fsize;
}

unsigned long ReaddirContext::GetTSModified() const {
    if (status == Status::error) return 0;

    time_t rawtime;
    tm calendar;

    time(&rawtime);
    tm* calendarCurrent = localtime(&rawtime);

    if (!calendarCurrent) return 0;
    memcpy(&calendar, calendarCurrent, sizeof(calendar));

    calendar.tm_year = 80 + (filinfo.fdate >> 9) & 0x7f;
    calendar.tm_mon = ((filinfo.fdate >> 5) & 0x0f) - 1;
    calendar.tm_mday = filinfo.fdate & 0x1f;
    calendar.tm_hour = (filinfo.ftime >> 11) & 0x1f;
    calendar.tm_min = (filinfo.ftime >> 5) & 0x3f;
    calendar.tm_sec = (filinfo.ftime & 0x1f) << 1;

    return mktime(&calendar);
}

long ReaddirContext::GetStatus() const { return static_cast<long>(status); }

long ReaddirContext::GetError() const {
    switch (err) {
        case FR_OK:
            return static_cast<long>(Error::none);

        case FR_NO_PATH:
            return static_cast<long>(Error::no_such_directory);

        default:
            return static_cast<long>(Error::unknown);
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

bool ReaddirContext::IsFilenameValid() const {
    for (size_t i = 0; i < FF_LFN_BUF; i++)
        if (filinfo.fname[i] == '\0') return true;

    return false;
}
