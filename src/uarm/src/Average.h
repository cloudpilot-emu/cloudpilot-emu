#ifndef _AVERAGE_H_
#define _AVERAGE_H_

#include <algorithm>
#include <cstdint>
#include <memory>

template <typename T, typename U = T>
class Average {
   public:
    Average(size_t size) : size(size) { entries = std::make_unique<T[]>(size); }

    void Add(T value) {
        entries[(first + count) % size] = value;

        if (count == size)
            first = (first + 1) % size;
        else
            count++;
    }

    U Calculate() const {
        if (count == 0) return static_cast<U>(0);

        U avg = static_cast<U>(0);
        for (size_t i = 0; i < count; i++) avg += static_cast<U>(entries[(first + i) % size]);

        return avg / static_cast<U>(count);
    }

    void Reset(size_t keep) {
        keep = std::min(keep, count);

        first = (first + count - keep) % size;
        count = keep;
    }

    size_t GetSize() const { return size; }

    size_t GetCount() const { return count; }

   private:
    const size_t size{0};
    size_t first{0}, count{0};
    std::unique_ptr<T[]> entries;

   private:
    Average();
    Average(const Average<T, U>&);
    Average(Average<T, U>&&);
    Average<T, U>& operator=(const Average<T, U>&);
    Average<T, U>& operator=(Average<T, U>&&);
};

#endif  // _AVERAGE_H_
