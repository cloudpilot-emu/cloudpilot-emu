
#ifndef _DB_INSTALLER_H_
#define _DB_INSTALLER_H_

#include <stdint.h>

#include "SoC.h"

#ifdef __cplusplus

    #include <functional>

extern "C" {
#endif

#define DB_INSTALL_RESULT_NEEDS_RESET 1
#define DB_INSTALL_RESULT_OK 0
#define DB_INSTALL_RESULT_ERR_CORRUPT -2
#define DB_INSTALL_RESULT_ERR_OOM -3
#define DB_INSTALL_RESULT_ERR_IS_OPEN -4
#define DB_INSTALL_RESULT_ERR_COULD_NOT_OVERWRITE -5
#define DB_INSTALL_RESULT_ERR_NOT_SUPPORTED -6
#define DB_INSTALL_RESULT_ERR_NOT_CURRENTLY_POSSIBLE -7
#define DB_INSTALL_RESULT_ERR_UNKNOWN -8

struct SyscallDispatch;

int32_t dbInstallerInstall(struct SyscallDispatch* sd, size_t len, void* data);

#ifdef __cplusplus
}
#endif

#endif  // _DB_INSTALLER_H_