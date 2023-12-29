#ifndef _CREATE_ZIP_CONTEXT_
#define _CREATE_ZIP_CONTEXT_

#include <cstddef>
#include <cstdint>
#include <string>

struct zip_t;

class CreateZipContext {
   public:
    CreateZipContext();
    ~CreateZipContext();

    bool Initialize(int compressionLevel);

    bool AddEntry(const std::string& name);

    bool WriteData(void* data, size_t size);

    const void* GetZipData();
    size_t GetZipDataSize() const;

    const char* GetLastError() const;

   private:
    enum class State { created, streamReady, entryPending, streamFinalized };

   private:
    bool ClosePendingEntry();

   private:
    zip_t* zip{nullptr};
    State state{State::created};

    uint8_t* zipData{nullptr};
    size_t zipDataSize{0};

    const char* lastError{"none"};

   private:
    CreateZipContext(const CreateZipContext&) = delete;
    CreateZipContext(CreateZipContext&&) = delete;
    CreateZipContext& operator=(const CreateZipContext&) = delete;
    CreateZipContext& operator=(CreateZipContext&&) = delete;
};

#endif  // _CREATE_ZIP_CONTEXT_
