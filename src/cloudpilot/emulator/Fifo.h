#ifndef _FIFO_H_
#define _FIFO_H_

#include "EmCommon.h"

template <typename T>
class Fifo {
   public:
    Fifo(uint32 capacity);

    ~Fifo();

    void Push(T value);

    T Pop();

    uint32 Size() const;

    void Clear();

    template <class U>
    void DoSaveLoad(U& helper);

   private:
    const uint32 capacity;

    T* buffer;
    uint32 start{0};
    uint32 size{0};
};

///////////////////////////////////////////////////////////////////////////////
// IMPLEMENTATION
///////////////////////////////////////////////////////////////////////////////

template <class T>
template <class U>
void Fifo<T>::DoSaveLoad(U& helper) {
    helper.Do32(start).Do32(size).DoBuffer(buffer, capacity * sizeof(T));
}

template <class T>
Fifo<T>::Fifo(uint32 capacity) : capacity(capacity) {
    buffer = new T[capacity];
    memset(buffer, 0, sizeof(T) * capacity);
}

template <class T>
Fifo<T>::~Fifo() {
    delete[] buffer;
}

template <class T>
void Fifo<T>::Push(T value) {
    buffer[(start + size) % capacity] = value;

    if (size < capacity)
        size++;
    else
        start = (start + 1) % capacity;
}

template <class T>
T Fifo<T>::Pop() {
    if (size == 0) return 0;

    const T value = buffer[start];
    start = (start + 1) % capacity;

    size--;

    return value;
}

template <class T>
uint32 Fifo<T>::Size() const {
    return size;
}

template <class T>
void Fifo<T>::Clear() {
    size = 0;
    start = 0;
}

#endif  // _FIFO_H_:
