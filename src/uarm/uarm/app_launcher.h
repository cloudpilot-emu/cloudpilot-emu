#ifndef _APP_LAUNCHER_H_
#define _APP_LAUNCHER_H_

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

struct SyscallDispatch;

bool launchAppByName(struct SyscallDispatch* sd, const char* name);

#ifdef __cplusplus
}
#endif

#endif  // _APP_LAUNCHER_H_