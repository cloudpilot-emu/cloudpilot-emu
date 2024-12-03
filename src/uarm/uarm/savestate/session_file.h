#ifndef _SESSION_FILE_H_
#define _SESSION_FILE_H_

#include <stdbool.h>

#include "buffer.h"

#ifdef __cplusplus
extern "C" {
#endif

bool sessionFile_read(const struct Buffer sessionData, struct Buffer* nor, struct Buffer* nand,
                      struct Buffer* ram, struct Buffer* savestate);

bool isSessionFile(const struct Buffer sessionData);

#ifdef __cplusplus
}
#endif

#endif  // _SESSION_FILE_H_