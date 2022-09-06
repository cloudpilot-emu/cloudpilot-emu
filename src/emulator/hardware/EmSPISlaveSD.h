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

   private:
    uint8 DoExchange8(uint8 data);

    void BufferStart(uint32 count);

    void DoCmd();
    void DoAcmd();

    void PrepareR1(uint8 flags);

   private:
    bool idle = true;
    bool acmd = false;

    SpiState state{SpiState::notSelected};
    uint8 lastCmd{0};
    uint8 buffer[515];
    uint32 bufferCount{0};
    uint32 bufferIndex{0};

   private:
    EmSPISlaveSD(const EmSPISlaveSD&);
    EmSPISlaveSD(EmSPISlaveSD&&);
    EmSPISlaveSD& operator=(const EmSPISlaveSD&);
    EmSPISlaveSD& operator=(EmSPISlaveSD&&);
};

#endif  // _EM_SPI_SLAVE_SD_H_
