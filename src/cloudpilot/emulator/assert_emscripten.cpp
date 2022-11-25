#include <emscripten.h>

#include <sstream>

EM_JS(void, __jsAssertThrow, (const char* msg), {
    const err = new Error(UTF8ToString(msg));
    err.isAssert = true;

    throw err;
})

extern "C" void MyAssertFailed(const char* assertion, const char* file, unsigned int line) {
    std::stringstream ss;

    ss << "ASSERTION FAILED: " << assertion << " in " << file << ":" << line;
    std::string err = ss.str();

    __jsAssertThrow(err.c_str());
}
