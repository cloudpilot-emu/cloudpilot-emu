#ifndef _ZIPFILE_ITERATOR_H_
#define _ZIPFILE_ITERATOR_H_

#include "VfsIterator.h"
#include "zip.h"

class ZipfileIterator : public VfsIterator {
   public:
    using VfsIterator::read_callback;
    using VfsIterator::State;

   public:
    ZipfileIterator(void* data, size_t size);
    ~ZipfileIterator();

    State GetState() override;
    State Next() override;
    const std::string& GetCurrentEntry() override;
    bool IsDirectory() override;
    void ReadCurrent(read_callback cb) override;

    uint32_t GetEntriesTotal() const;

   private:
    zip_t* zip{nullptr};

    uint32_t entriesTotal{0};
    uint32_t currentEntryIndex{0};
    bool openEntryPending{false};

    State state{State::initial};
    std::string currentEntry;
};

#endif  // _ZIPFILE_ITERATOR_H_
