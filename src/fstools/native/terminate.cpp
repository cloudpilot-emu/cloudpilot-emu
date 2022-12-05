#include "../terminate.h"

#include <cstdlib>
#include <iostream>

using namespace std;

void terminate() {
    cerr << "terminated" << endl;
    exit(1);
}
