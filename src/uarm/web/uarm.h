#ifndef __UARM_H__
#define __UARM_H__

#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>

#include "MainLoop.h"

struct SoC;
struct AudioQueue;

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

   private:
    size_t ramSize{16 << 20};

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