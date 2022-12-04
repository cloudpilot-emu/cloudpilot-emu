#include "fsck.h"

#include "fsck.fat.h"
#include "io.h"

static int runFsckNoWrite() {
    static char* options[] = {"-n", "null"};

    fsck_main(2, options);

    return fs_changed();
}

static int runFsckWithWrite() {
    static char* options[] = {"-a", "null"};

    fsck_main(2, options);

    return fs_changed();
}

int runFsck(int fix) { return fix ? runFsckWithWrite() : runFsckNoWrite(); }
