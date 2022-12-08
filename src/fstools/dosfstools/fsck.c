#include "fsck.h"

#include <stdio.h>

#include "fsck.fat.h"
#include "io.h"

static int runFsckNoWrite() {
    static char* options[] = {"fsck", "-n", "-v", "-V", "memory card"};

    return !fsck_main(5, options);
}

static int runFsckWithWrite() {
    static char* options[] = {"fsck", "-a", "-v", "-V", "memory card"};

    return !fsck_main(5, options);
}

int runFsck(int fix) { return fix ? runFsckWithWrite() : runFsckNoWrite(); }
