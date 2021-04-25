#ifndef _FIFO_H_
#define _FIFO_H_

#include "EmCommon.h"

class Fifo {
   public:
    Fifo(uint32 capacity);

    ~Fifo();

    void Push(uint32 value);

    uint32 Pop();

    uint32 Size() const;

    void Clear();

    template <class T>
    void DoSaveLoad(T& helper);

   private:
    const uint32 capacity;

    uint32* buffer;
    uint32 start{0};
    uint32 size{0};
};

///////////////////////////////////////////////////////////////////////////////
// IMPLEMENTATION
///////////////////////////////////////////////////////////////////////////////

template <class T>
void Fifo::DoSaveLoad(T& helper) {
    helper.Do32(start).Do32(size).DoBuffer(buffer, capacity * sizeof(uint32));
}

#endif  // _FIFO_H_:
