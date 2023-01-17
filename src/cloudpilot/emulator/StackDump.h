#ifndef _STACK_DUMP_H_
#define _STACK_DUMP_H_

#include <string>

#include "EmCommon.h"

class StackDump {
   public:
    StackDump() = default;

    StackDump& FrameCount(uint8 frameCount);
    StackDump& DumpFrames(bool dumpFrames);
    StackDump& DumpRegisters(bool dumpRegisters);
    StackDump& MaxFrameSize(uint32 maxFrameSize);
    StackDump& WriteRam(const string& file);

    void Dump() const;

   private:
    uint8 frameCount{2};

    bool dumpFrames{false};
    bool dumpRegisters{false};
    uint32 maxFrameSize{1024};
    string ramfile;
};

#endif  // _STACK_DUMP_H_
