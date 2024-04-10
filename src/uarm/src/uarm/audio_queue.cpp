#include "audio_queue.h"

#include "queue.h"

#ifndef __EMSCRIPTEN__
    #define AUDIO_QUEUE_THREADSAFE
#endif

#ifdef AUDIO_QUEUE_THREADSAFE
    #include <mutex>

    #define AUDIO_QUEUE_LOCK std::lock_guard lock(audioQueue->mutex)
#else
    #define AUDIO_QUEUE_LOCK std::lock_guard lock(audioQueue->mutex)
#endif

struct AudioQueue {
    Queue<uint32_t> queue;

    AudioQueue(size_t capacity) : queue(capacity) {}

#ifdef AUDIO_QUEUE_THREADSAFE
    std::mutex mutex;
#endif
};

struct AudioQueue* audioQueueCreate(size_t capacity) {
    AudioQueue* audioQueue = new AudioQueue(capacity);

    return audioQueue;
}

void audioQueuePush(struct AudioQueue* audioQueue, uint32_t sample) {
    AUDIO_QUEUE_LOCK;

    audioQueue->queue.Push(sample);
}

size_t audioQueuePopChunk(struct AudioQueue* audioQueue, uint32_t* destination, size_t count) {
    AUDIO_QUEUE_LOCK;

    size_t actualCount = 0;
    while (audioQueue->queue.GetSize() > 0 && actualCount < count)
        destination[actualCount++] = audioQueue->queue.Pop();

    return actualCount;
}

size_t audioQueuePendingSamples(struct AudioQueue* audioQueue) {
    AUDIO_QUEUE_LOCK;

    return audioQueue->queue.GetSize();
}
