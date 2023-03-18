#include "VfsUtil.h"

#include <chrono>

using namespace std;

uint64_t util::epochMilliseconds() {
    return chrono::duration_cast<chrono::milliseconds>(
               chrono::system_clock::now().time_since_epoch())
        .count();
}

string util::normalizePath(const string& path) {
    string normalizedPath;
    normalizedPath.reserve(path.size() + 1);

    bool slashes = false;
    if (path.size() < 2 || path[1] != ':') {
        normalizedPath.push_back('/');
        slashes = true;
    }

    for (size_t i = 0; i < path.size(); i++) {
        if (path[i] == '/' || path[i] == '\\') {
            if (!slashes) normalizedPath.push_back('/');
            slashes = true;
        } else {
            slashes = false;
            normalizedPath.push_back(path[i]);
        }
    }

    return normalizedPath;
}

const char* util::drivePrefix(unsigned int slot) {
    static char prefix[3] = {'0', ':', '\0'};
    prefix[0] = '0' + (slot % 10);

    return prefix;
}
