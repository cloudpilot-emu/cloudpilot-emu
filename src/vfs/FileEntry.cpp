#include "FileEntry.h"

#include <cstring>
#include <ctime>

using namespace std;

FILINFO* FileEntry::GetFilinfo() { return &filinfo; }

const char* FileEntry::GetName() const { return IsFilenameValid() ? filinfo.fname : ""; }

bool FileEntry::IsDirectory() const { return filinfo.fattrib & AM_DIR; }

unsigned int FileEntry::GetSize() const { return filinfo.fsize; }

unsigned int FileEntry::GetModifiedTS() const {
    time_t rawtime;
    tm calendar;

    time(&rawtime);
    tm* calendarCurrent = localtime(&rawtime);

    if (!calendarCurrent) return 0;
    memcpy(&calendar, calendarCurrent, sizeof(calendar));

    calendar.tm_year = 80 + ((filinfo.fdate >> 9) & 0x7f);
    calendar.tm_mon = ((filinfo.fdate >> 5) & 0x0f) - 1;
    calendar.tm_mday = filinfo.fdate & 0x1f;
    calendar.tm_hour = (filinfo.ftime >> 11) & 0x1f;
    calendar.tm_min = (filinfo.ftime >> 5) & 0x3f;
    calendar.tm_sec = (filinfo.ftime & 0x1f) << 1;

    return mktime(&calendar);
}

unsigned int FileEntry::GetAttributes() const { return filinfo.fattrib; }

bool FileEntry::IsFilenameValid() const {
    for (size_t i = 0; i < FF_LFN_BUF; i++)
        if (filinfo.fname[i] == '\0') return true;

    return false;
}
