#include "fsck.h"

#include <stdio.h>

#include "fsck.fat.h"
#include "io.h"

int runFsck(int fix) {
    char* args[] = {"fsck", fix ? "-a" : "-n", "-v", "-V", "memory card"};

    return !fsck_main(sizeof(args) / sizeof(char*), args);
}
