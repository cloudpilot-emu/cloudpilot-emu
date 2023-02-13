#include "VfsUtil.h"

#include <chrono>

using namespace std;

uint64_t util::epochMilliseconds() {
    return chrono::duration_cast<chrono::milliseconds>(
               chrono::system_clock::now().time_since_epoch())
        .count();
}
