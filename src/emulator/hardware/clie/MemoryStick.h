#ifndef _MEMORY_STICK_
#define _MEMORY_STICK_

#include "CardImage.h"
#include "EmCommon.h"
#include "EmEvent.h"

class MemoryStick {
   public:
    enum class TpcType { read, write };

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

        Registers& SetBlock(uint32 block);
        Registers& SetPage(uint8 page);
        Registers& SetLogicalBlock(uint16 block);
    };
#pragma pack(pop)

    static constexpr uint32 BOOT_BLOCK = 0;
    static constexpr uint32 BOOT_BLOCK_BACKUP = 1;

   public:
    static TpcType GetTpcType(uint8 tpcId);
    static bool IsSizeRepresentable(size_t pagesTotal);

    MemoryStick();

    ~MemoryStick();

    void Reset();

    void ExecuteTpc(uint8 tpcId, uint32 dataInCount = 0, uint8* dataIn = nullptr);
    void FinishReadTpc();

    uint8* GetDataOut();
    uint32 GetDataOutSize();

    void Mount(CardImage* cardImage);
    void Unmount();

    bool PreparePage(uint8* destination, bool oobOnly);
    bool ProgramPage(uint8* data);
    bool EraseBlock();

    Registers& GetRegisters();
    uint32 BlocksTotal() const;
    uint8 PagesPerBlock() const;

   private:
    void DoCmd(uint8 commandByte);
    void DoCmdRead();

    void PreparePageBootBlock(uint8 page, uint8* destination, bool oobOnly);

    void SetFlags(uint8 flags);
    void ClearFlags();

   public:
    EmEvent<> irq;

   private:
    enum class Operation : uint8 {
        none = 0,
        readOne = 1,
        readMulti = 2,
        programOne = 3,
        programMulti = 4
    };

    enum class Access : uint8 { full = 0, oobOnly = 1 };

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

    uint16* blockMap;

    Operation currentOperation{Operation::none};

   private:
    MemoryStick(const MemoryStick&) = delete;
    MemoryStick(MemoryStick&&) = delete;
    MemoryStick& operator=(const MemoryStick&) = delete;
    MemoryStick& operator=(MemoryStick&&) = delete;
};

#endif  // _MEMORY_STICK_
