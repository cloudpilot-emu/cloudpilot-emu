#ifndef _STACK_DUMP_H_
#define _STACK_DUMP_H_

#include "EmCommon.h"

class StackDump {
   public:
    StackDump() = default;

    StackDump& FrameCount(uint8 frameCount);
    StackDump& DumpFrames(bool dumpFrames);
    StackDump& DumpRegisters(bool dumpRegisters);
    StackDump& MaxFrameSize(uint32 maxFrameSize);

    void Dump() const;

   private:
    uint8 frameCount{2};

    bool dumpFrames{false};
    bool dumpRegisters{false};
    uint32 maxFrameSize{1024};
};

#endif  // _STACK_DUMP_H_
