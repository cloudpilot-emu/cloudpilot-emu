#include <iostream>

#include "SoC.h"

using namespace std;

extern "C" int socExtSerialReadChar(void) {
#if 1
    return CHAR_NONE;
#else
    timeval tv;
    fd_set set;
    char c;
    int i, ret = CHAR_NONE;

    tv.tv_sec = 0;
    tv.tv_usec = 0;

    FD_ZERO(&set);
    FD_SET(0, &set);

    i = select(1, &set, NULL, NULL, &tv);
    if (i == 1 && 1 == read(0, &c, 1)) {
        ret = c;
    }

    return ret;
#endif
}

extern "C" void socExtSerialWriteChar(int chr) {
    if (!(chr & 0xFF00))
        printf("%c", chr);
    else
        printf("<<~~ EC_0x%x ~~>>", chr);

    fflush(stdout);
}

int main(int argc, const char** argv) { cout << "hello world" << endl; }