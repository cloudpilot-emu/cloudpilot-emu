#include "mkfs.h"

#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "mkfs.fat.h"

int mkfs(int clusterSize, const char* label) {
    char clusterSizeString[5];
    snprintf(clusterSizeString, 5, "%d", clusterSize);

    char labelCopy[12];
    strncpy(labelCopy, label, 12);
    labelCopy[11] = '\0';

    for (int i = 0; i < 12 && labelCopy[i] != 0; i++) labelCopy[i] = toupper(labelCopy[i]);

    char* args[] = {"fsck", "-a", "-v", "-n", labelCopy, "-s", clusterSizeString, "memory card"};

    return !mkfs_main(sizeof(args) / sizeof(char*), args);
}
