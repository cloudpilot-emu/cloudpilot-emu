#ifndef _DEBUGGER_H_
#define _DEBUGGER_H_

#include <array>
#include <unordered_set>

#include "EmCommon.h"

class Debugger {
   public:
    static constexpr size_t REGISTER_COUNT = 18;

    enum class BreakState {
        none,
        breakpoint,
        step,
        trapRead,
        trapWrite,
        trapInternal,
        externalInterrupt
    };

    enum class BreakMode { all, ramOnly, appOnly };

    enum class WatchpointType { read, write, readwrite };

   public:
    Debugger() = default;

    BreakState GetBreakState() const;
    bool IsStopped() const;

    void Reset();
    void Enable();

    void ResetBreakMode();
    void ResetAppRegion();

    void SetBreakMode(BreakMode mode);
    void SetAppRegion(emuptr start, uint32 size);

    BreakMode GetBreakMode() const;
    emuptr GetAppStart() const;
    uint32 GetAppSize() const;

    void NotificyPc(emuptr pc);

    void NotifyMemoryRead8(emuptr address);
    void NotifyMemoryRead16(emuptr address);
    void NotifyMemoryRead32(emuptr address);

    void NotifyMemoryWrite8(emuptr address);
    void NotifyMemoryWrite16(emuptr address);
    void NotifyMemoryWrite32(emuptr address);

    void SetBreakpoint(emuptr pc);
    void ClearBreakpoint(emuptr pc);

    void SetWatchpoint(emuptr address, WatchpointType type, size_t len);
    void ClearWatchpoint(emuptr address, WatchpointType type, size_t len);

    WatchpointType GetWatchpointType() const;
    emuptr GetWatchpointAddress() const;

    void Interrupt();
    void Continue();
    void Step();

    uint8 MemoryRead8(emuptr addr);
    uint16 MemoRead16(emuptr addr);
    uint32 MemoryRead32(emuptr addr);

    void MemoryWrite(emuptr addr, uint8* data, size_t len);

    bool IsMemoryAccess() const;

    void UpdateBreakState();

    const array<uint32, REGISTER_COUNT>& ReadRegisters();
    void SetRegister(size_t index, uint32 value);

   private:
    void Break(BreakState state);

   private:
    bool enabled{false};
    BreakState breakState{BreakState::none};

    array<uint32, REGISTER_COUNT> registers;

    bool stepping{false};
    emuptr lastBreakAtPc{0xffffffff};

    bool memoryAccess{false};

    unordered_set<emuptr> breakpoints;
    unordered_set<emuptr> watchpointsRead;
    unordered_set<emuptr> watchpointsWrite;

    emuptr watchpointAddress;

    BreakMode breakMode{BreakMode::all};
    emuptr appStart{0};
    uint32 appSize{0};

    emuptr romStart{0};
    uint32 romSize{0};

   private:
    Debugger(const Debugger&) = delete;
    Debugger(Debugger&&) = delete;
    Debugger& operator=(const Debugger&) = delete;
    Debugger& operator=(Debugger&&) = delete;
};

extern Debugger gDebugger;

#endif  // _DEBUGGER_H_
