#include <emscripten.h>

#include <iostream>

using namespace std;

int EMSCRIPTEN_KEEPALIVE main() {
    cout << "Hello world from C++" << endl << flush;
    return 0;
}
