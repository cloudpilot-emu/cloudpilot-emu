#ifndef _EM_SPI_SLAVE_SD_H_
#define _EM_SPI_SLAVE_SD_H_

#include "EmCommon.h"
#include "EmSPISlave.h"

class EmSPISlaveSD : public EmSPISlave {
   public:
    EmSPISlaveSD() = default;

    void Reset();

    uint16 DoExchange(uint16 control, uint16 data) override;
    void Enable(void) override;
    void Disable(void) override;

   private:
    enum class SpiState : uint8 { notSelected = 0, rxCmdByte = 1, rxArg = 2, txResult = 3 };
    enum class CardState : uint8 { idle = 0, initialized = 1 };

   private:
    uint8 DoExchange8(uint8 data);

    void BufferStart(uint32 size);

    template <class T, class... Us>
    void BufferAdd(T value, Us...);

    template <class T>
    void BufferAdd(T value);

    void BufferAddBlock(uint8 token, uint8* data, size_t size);

    void DoCmd();
    void DoAcmd();

    uint32 Param() const;

    void PrepareR1(uint8 flags);
    void PrepareCSD();
    void PrepareCID();

   private:
    bool acmd = false;

    SpiState spiState{SpiState::notSelected};
    CardState cardState{CardState::idle};
    uint8 lastCmd{0};

    uint8 buffer[550];
    uint32 bufferSize{0};
    uint32 bufferIndex{0};

   private:
    EmSPISlaveSD(const EmSPISlaveSD&);
    EmSPISlaveSD(EmSPISlaveSD&&);
    EmSPISlaveSD& operator=(const EmSPISlaveSD&);
    EmSPISlaveSD& operator=(EmSPISlaveSD&&);
};

///////////////////////////////////////////////////////////////////////////////
// IMPLEMENTATION
///////////////////////////////////////////////////////////////////////////////

template <class T, class... Us>
void EmSPISlaveSD::BufferAdd(T value, Us... rest) {
    BufferAdd(value);
    BufferAdd(rest...);
}

template <class T>
void EmSPISlaveSD::BufferAdd(T value) {
    buffer[bufferSize++] = value;
}

#endif  // _EM_SPI_SLAVE_SD_H_
