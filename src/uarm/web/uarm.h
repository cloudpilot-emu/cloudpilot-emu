#ifndef __UARM_H__
#define __UARM_H__

#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>

#include "MainLoop.h"
#include "device.h"

struct SoC;
struct AudioQueue;
class DbBackup;

class Uarm {
   public:
    explicit Uarm() = default;

    Uarm& SetRamSize(unsigned int size);
    Uarm& SetNand(unsigned int size, void* data);
    Uarm& SetMemory(unsigned int size, void* data);
    Uarm& SetSavestate(unsigned int size, void* data);
    Uarm& SetSd(unsigned int size, void* data, const char* id);
    Uarm& SetDefaultMips(unsigned int defaultMips);

    bool Launch(unsigned int romSize, void* romData);

    double Cycle(uint64_t now);

    void* GetFrame();
    void ResetFrame();

    uint32_t GetTimesliceSizeUsec();

    void PenDown(int x, int y);
    void PenUp();

    uint32_t CurrentIps();
    uint64_t CurrentIpsMax();
    void SetMaxLoad(unsigned int maxLoad);
    void SetCyclesPerSecondLimit(unsigned int cyclesPerSecondLimit);

    uint64_t GetTimestampUsec();

    void KeyDown(int key);
    void KeyUp(int key);

    uint32_t PendingSamples();
    uint32_t* PopQueuedSamples();
    void SetPcmOutputEnabled(bool enabled);
    void SetPcmSuspended(bool suspended);

    uint32_t GetRomDataSize();
    void* GetRomData();

    uint32_t GetNandDataSize();
    void* GetNandData();
    void* GetNandDirtyPages();
    bool IsNandDirty();
    void SetNandDirty(bool isDirty);

    uint32_t GetSdCardDataSize();
    void* GetSdCardData();
    void* GetSdCardDirtyPages();
    bool IsSdCardDirty();
    void SetSdCardDirty(bool isDirty);

    uint32_t GetRamDataSize();
    void* GetRamData();
    void* GetRamDirtyPages();

    uint32_t GetDeviceType();

    bool SdCardInsert(void* data, int length, const char* id);
    void SdCardEject();
    void Reset();

    void Save();
    uint32_t GetSavestateSize();
    void* GetSavestateData();
    bool IsSdInserted();

    uint32_t GetRamSize();

    uint32_t InstallDatabase(uint32_t len, void* data);

    DbBackup* NewDbBackup(int type);

   private:
    size_t ramSize{0};

    size_t nandSize{0};
    uint8_t* nandData{nullptr};

    size_t memorySize{0};
    uint8_t* memoryData{nullptr};

    size_t savestateSize{0};
    uint8_t* savestateData{nullptr};

    size_t sdSize{0};
    uint8_t* sdData{nullptr};
    std::string sdId;

    uint32_t defaultMips{0};
    DeviceType5 deviceType{deviceTypeInvalid};

    SoC* soc{nullptr};
    AudioQueue* audioQueue{nullptr};

    std::unique_ptr<MainLoop> mainLoop;

   private:
    Uarm(const Uarm&) = delete;
    Uarm(Uarm&&) = delete;
    Uarm& operator=(const Uarm&) = delete;
    Uarm& operator=(Uarm&&) = delete;
};

#endif  // __UARM_H__