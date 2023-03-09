#ifndef _VFS_ITERATOR_H_
#define _VFS_ITERATOR_H_

#include <cstddef>
#include <functional>
#include <string>

class VfsIterator {
   public:
    using read_callback = std::function<void(const void*, size_t)>;

    enum class State { initial, valid, done, error };

   public:
    virtual State GetState() = 0;
    virtual State Next() = 0;
    virtual const std::string& GetCurrentEntry() = 0;
    virtual bool IsDirectory() = 0;
    virtual void ReadCurrent(read_callback cb) = 0;
};

#endif  //  _VFS_ITERATOR_H_
