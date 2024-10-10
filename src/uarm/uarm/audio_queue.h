#ifndef _AUDIO_QUEUE_H_
#define _AUDIO_QUEUE_H_

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

struct AudioQueue;

struct AudioQueue* audioQueueCreate(size_t capacity);

void audioQueuePush(struct AudioQueue* audioQueue, uint32_t sample);

size_t audioQueuePopChunk(struct AudioQueue* audioQueue, uint32_t* destination, size_t count);

size_t audioQueuePendingSamples(struct AudioQueue* audioQueue);

void audioQueueClear(struct AudioQueue* audioQueue);

#ifdef __cplusplus
}
#endif

#endif  //  _AUDIO_QUEUE_H_