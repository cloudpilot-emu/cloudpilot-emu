#ifndef _MKFS_H_
#define _MKFS_H_

#ifdef __cplusplus
extern "C" {
#endif

int mkfs(int clusterSize, const char* label);

#ifdef __cplusplus
}
#endif

#endif  // _MKFS_H_
