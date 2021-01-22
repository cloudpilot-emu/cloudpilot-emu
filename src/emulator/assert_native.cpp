#include <iostream>

extern "C" void MyAssertFailed(const char* assertion, const char* file, unsigned int line) {
    std::cerr << "ASSERTION FAILED: " << assertion << " in " << file << ":" << line << std::endl;

    exit(1);
}
