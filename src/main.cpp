#include <iostream>
#include <cstdlib>

using namespace std;

extern "C"
void MyAssertFailed (const char* assertion, const char* file, unsigned int line)
{
    cerr << "ASSERTION FAILED: " << assertion << " in " << file << ":" << line << endl;

    exit(1);
}

int main() {
    cout << "Hello world!" << endl;
}
