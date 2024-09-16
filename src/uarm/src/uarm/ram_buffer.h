#ifndef _RAM_BACKBUFFER_H_
#define _RAM_BACKBUFFER_H_

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

#define RAM_BUFFER_MARK_DIRTY(buf, addr) \
    (buf->dirtyPages[(addr) >> 14] |= (1u << (((addr) >> 9) & 0x1f)))

struct RamBuffer {
    size_t size;

    uint32_t* buffer;
    uint32_t* dirtyPages;
};

void ramBufferAllocate(struct RamBuffer* ramBuffer, size_t size);

void ramBufferRelease(struct RamBuffer* ramBuffer);

#ifdef __cplusplus
}
#endif

#endif  // _RAM_BACKBUFFER_H_