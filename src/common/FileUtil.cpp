#include "FileUtil.h"

#include <fstream>

using namespace std;

bool util::ReadFile(optional<string> file, unique_ptr<uint8_t[]>& buffer, size_t& len) {
    if (!file) return false;

    fstream stream(*file, ios_base::in);
    if (stream.fail()) return false;

    stream.seekg(0, ios_base::end);
    len = stream.tellg();

    stream.seekg(0, ios_base::beg);
    buffer = make_unique<uint8_t[]>(len);

    stream.read((char*)buffer.get(), len);
    if (static_cast<size_t>(stream.gcount()) != len) return false;

    return true;
}