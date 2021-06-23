#include <iostream>

#include "stacktrace.h"

extern "C" void MyAssertFailed(const char *assertion, const char *file, unsigned int line) {
    std::cerr << "ASSERTION FAILED: " << assertion << " in " << file << ":" << line << std::endl;

    std::cerr << Backtrace(2) << std::endl << std::endl;

    exit(1);
}
