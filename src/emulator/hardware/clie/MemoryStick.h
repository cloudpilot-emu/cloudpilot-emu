#ifndef _MEMORY_STICK_
#define _MEMORY_STICK_

#include "CardImage.h"
#include "EmCommon.h"
#include "EmEvent.h"

class MemoryStick {
   public:
    enum class TpcType { read, write };

   public:
    static TpcType GetTpcType(uint8 tpcId);
    static bool IsSizeRepresentable(size_t pagesTotal);

    MemoryStick() = default;

    void Reset();

    void ExecuteTpc(uint8 tpcId, uint32 dataInCount = 0, uint8* dataIn = nullptr);
    void FinishReadTpc();

    uint8* GetDataOut();
    uint32 GetDataOutSize();

    void Mount(CardImage* cardImage);
    void Unmount();

   private:
    void TpcSetCommand(uint8 commandByte);
    bool PreparePage();
    void SetFlags(uint8 flags);
    void ClearFlags();

   public:
    EmEvent<> irq;

   private:
#pragma pack(push, 1)
    struct Registers {
        uint8 _filler_0;

        uint8 intFlags;
        uint8 status0;
        uint8 status1;
        uint8 msType;

        uint8 _filler_1;

        uint8 msCategory;
        uint8 msClass;

        uint8 _filler_2[8];

        uint8 cfg;
        uint8 blockHi;
        uint8 blockMid;
        uint8 blockLo;
        uint8 accessType;
        uint8 page;
        uint8 oob[9];
    };
#pragma pack(pop)

    enum class Operation : uint8 {
        none = 0,
        readOne = 1,
        readMulti = 2,
        programOne = 3,
        programMulti = 4
    };

   private:
    Registers reg;

    uint8 readWindowStart{0};
    uint8 readWindowSize{0};
    uint8 writeWindowStart{0};
    uint8 writeWindowSize{0};

    uint8* bufferOut{nullptr};
    uint32 bufferOutSize{0};

    uint8 preparedPage[512];

    CardImage* cardImage = nullptr;
    uint8 pagesPerBlock{0};
    uint8 segments{0};

    Operation currentOperation{Operation::none};

   private:
    MemoryStick(const MemoryStick&) = delete;
    MemoryStick(MemoryStick&&) = delete;
    MemoryStick& operator=(const MemoryStick&) = delete;
    MemoryStick& operator=(MemoryStick&&) = delete;
};

#endif  // _MEMORY_STICK_
