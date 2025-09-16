#ifndef _BUFFER_H_
#define _BUFFER_H_

#include <stddef.h>

struct Buffer {
    size_t size;
    void* data;
};

#endif  // _BUFFER_H_