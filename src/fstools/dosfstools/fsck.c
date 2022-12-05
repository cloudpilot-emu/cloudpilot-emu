#include "fsck.h"

#include "fsck.fat.h"
#include "io.h"

static int runFsckNoWrite() {
    static char* options[] = {"fsck", "-n", "-v", "card"};

    fsck_main(4, options);

    return fs_changed();
}

static int runFsckWithWrite() {
    static char* options[] = {"fsck", "-a", "card"};

    fsck_main(3, options);

    return fs_changed();
}

int runFsck(int fix) { return fix ? runFsckWithWrite() : runFsckNoWrite(); }
