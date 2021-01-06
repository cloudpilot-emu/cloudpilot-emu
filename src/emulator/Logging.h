#ifndef _LOGGING_H_
#define _LOGGING_H_

namespace logging {
    int printf(const char* format, ...);

    void enable();

    void disable();
}  // namespace logging

#endif  // _LOGGING_H
