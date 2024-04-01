#ifndef _QUEUE_H_
#define _QUEUE_H_

#include <cstddef>
#include <vector>

template <typename T>
class Queue {
   public:
    explicit Queue(size_t capacity);

    void Push(const T& item);
    T Pop();
    const T& Peek() const;

    size_t GetSize() const;

   private:
    std::vector<T> items;

    size_t size{0};
    size_t next{0};
};

///////////////////////////////////////////////////////////////////////////////
// IMPLEMENTATION
///////////////////////////////////////////////////////////////////////////////

template <typename T>
Queue<T>::Queue(size_t capacity) : items(capacity) {}

template <typename T>
void Queue<T>::Push(const T& item) {
    items[next] = item;

    next = (next + 1) % items.size();
    if (size < items.size()) size++;
}

template <typename T>
T Queue<T>::Pop() {
    const size_t index = (next + items.size() - size) % items.size();
    if (size > 0) size--;

    return items[index];
}

template <typename T>
const T& Queue<T>::Peek() const {
    const size_t index = (next + items.size() - size) % items.size();

    return items[index];
}
template <typename T>
size_t Queue<T>::GetSize() const {
    return size;
}

#endif  // _QUEUE_H_