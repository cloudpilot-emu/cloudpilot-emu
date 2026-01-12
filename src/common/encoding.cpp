#include <encoding.h>

using namespace std;

string Utf8ToIsolatin1(const string& input, size_t maxLen) {
    size_t len = 0;

    maxLen = min(input.size(), maxLen);
    char* output = new char[maxLen + 1];

    memset(output, 0, maxLen + 1);

    const char* inputChar = input.c_str();

    for (size_t i = 0; i < input.size(); i++) {
        if ((inputChar[i] & 0x80) == 0)
            output[len++] = inputChar[i];
        else if ((inputChar[i] & 0xfc) == 0xc0 && i + 1 < input.size()) {
            output[len++] = ((inputChar[i] & 0x03) << 6) | (inputChar[i + 1] & 0x3f);
            i++;
        } else if ((inputChar[i] & 0xc0) != 0x80)
            output[len++] = '_';

        if (len >= maxLen) break;
    }

    string result = output;
    delete[] output;

    return result;
}

string Isolatin1ToUtf8(const string& input) {
    size_t len = 0;

    char* output = new char[2 * input.size() + 1];
    memset(output, 0, 2 * input.size() + 1);

    const char* inputChar = input.c_str();

    for (size_t i = 0; i < input.size(); i++) {
        if ((inputChar[i] & 0x80) == 0)
            output[len++] = inputChar[i];
        else {
            output[len++] = 0xc0 | ((inputChar[i] >> 6) & 0x03);
            output[len++] = 0x80 | (inputChar[i] & 0x3f);
        }
    }

    string result = output;
    delete[] output;

    return result;
}
