#ifndef _APP_LAUNCHER_H_
#define _APP_LAUNCHER_H_

struct SyscallDispatch;

bool launchAppByName(struct SyscallDispatch* sd, const char* name);

#endif  // _APP_LAUNCHER_H_
