#ifndef _MEMORY_BUFFER_H_
#define _MEMORY_BUFFER_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MEMORY_BUFFER_GRANULARITY (32 * 512)

#define MEMORY_BUFFER_MARK_DIRTY(buf, addr) \
    ((buf).dirtyPages[(addr) >> 14] |= (1u << (((addr) >> 9) & 0x1f)))

struct MemoryBuffer {
    size_t size;
    size_t dirtyPagesSize;

    uint8_t* buffer;
    uint32_t* dirtyPages;

    bool isSubBuffer;
};

bool memoryBufferAllocate(struct MemoryBuffer* memoryBuffer, size_t size);

void memoryBufferRelease(struct MemoryBuffer* memoryBuffer);

bool memoryBufferGetSubBuffer(struct MemoryBuffer* memoryBuffer, struct MemoryBuffer* subBuffer,
                              size_t offset, size_t size);

void memoryBufferMarkRangeDirty(struct MemoryBuffer* memoryBuffer, size_t address, size_t size);

#ifdef __cplusplus
}
#endif

#endif  // _MEMORY_BACKBUFFER_H_