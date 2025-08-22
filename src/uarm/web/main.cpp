#include "SoC.h"

using namespace std;

#pragma clang diagnostic ignored "-Wunused-function"

extern "C" {

int socExtSerialReadChar(void) { return CHAR_NONE; }

void socExtSerialWriteChar(int chr) {
    if (!(chr & 0xFF00))
        printf("%c", chr);
    else
        printf("<<~~ EC_0x%x ~~>>", chr);

    fflush(stdout);
}
}

int main() {}
