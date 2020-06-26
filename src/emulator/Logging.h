#ifndef _LOGGING_H_
#define _LOGGING_H_

namespace log {
    int printf(const char* format, ...);

    void enable();

    void disable();
}  // namespace log

#endif  // _LOGGING_H
