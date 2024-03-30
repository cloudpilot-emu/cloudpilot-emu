#ifndef _ZIPFILE_ITERATOR_H_
#define _ZIPFILE_ITERATOR_H_

#include <cstdint>

#include "VfsIterator.h"
#include "zip.h"

class ZipfileIterator : public VfsIterator {
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

   private:
    ZipfileIterator(const ZipfileIterator&) = delete;
    ZipfileIterator(ZipfileIterator&&) = delete;
    ZipfileIterator& operator=(const ZipfileIterator&) = delete;
    ZipfileIterator operator=(ZipfileIterator&&) = delete;
};

#endif  // _ZIPFILE_ITERATOR_H_
