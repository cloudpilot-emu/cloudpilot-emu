#include "SoC.h"

#include "RAM.h"
#include "ROM.h"
#include "audio_queue.h"
#include "mem.h"
#include "nand.h"
#include "pace_patch.h"
#include "patch_dispatch.h"
#include "sdcard.h"
#include "system_state.h"
#include "vSD.h"

#define EVENT_QUEUE_CAPACITY 64
#define MIN_EVENT_QUEUE_TICKS_BEFORE_PEN_UP 2

using namespace std;

SoC::PenEvent SoC::PenEvent::PenDown(int x, int y) { return {.penDown = true, .x = x, .y = y}; }
SoC::PenEvent SoC::PenEvent::PenUp() { return {.penDown = false, .x = -1, .y = -1}; }

SoC::KeyEvent SoC::KeyEvent::KeyDown(enum KeyId key) { return {.keyDown = true, .key = key}; }
SoC::KeyEvent SoC::KeyEvent::KeyUp(enum KeyId key) { return {.keyDown = false, .key = key}; }

void SoC::Reset() {
    penEventQueue->Clear();
    keyEventQueue->Clear();

    SetFramebufferDirty();
}

SoC::SoC()
    : savestate(make_unique<Savestate<ChunkType>>()),
      powerOnState(make_unique<Savestate<ChunkType>>()),
      penEventQueue(make_unique<Queue<PenEvent>>(EVENT_QUEUE_CAPACITY)),
      keyEventQueue(make_unique<Queue<KeyEvent>>(EVENT_QUEUE_CAPACITY)) {
    pacePatch = createPacePatch();
    mem = memInit();

    patchDispatch = initPatchDispatch();
    systemState = createSystemState();

    vSD = vsdInit(sdCardRead, sdCardWrite, 0);
}

void SoC::PumpEventQueues() {
    eventQueueTicks++;

    PumpPenEventQueue();
    PumpKeyEventQueue();
}

void SoC::PumpPenEventQueue() {
    if (penEventQueue->GetSize() == 0) return;

    if (penDown && !penEventQueue->Peek().penDown &&
        eventQueueTicks - eventQueueTicksAtPenDown < MIN_EVENT_QUEUE_TICKS_BEFORE_PEN_UP) {
        return;
    }

    PenEvent evt(penEventQueue->Pop());

    if (evt.penDown) {
        while (penEventQueue->GetSize() > 0 && penEventQueue->Peek().penDown) {
            evt = penEventQueue->Pop();
        }
    }

    if (evt.penDown || penDown) {
        if (evt.penDown && !penDown) eventQueueTicksAtPenDown = eventQueueTicks;
        penDown = evt.penDown;

        OnTouch(evt.x, evt.y);
    }
}

void SoC::PumpKeyEventQueue() {
    if (jammedKey != keyInvalid && GetTime() >= releaseJammedKeyAt) {
        OnEngageKey(jammedKey, false);
        jammedKey = keyInvalid;
    }

    if (keyEventQueue->GetSize() > 0) {
        KeyEvent evt(keyEventQueue->Pop());
        if (evt.key == jammedKey) return;

        OnEngageKey(evt.key, evt.keyDown);
    }
}

void SoC::KeyDown(enum KeyId key) { keyEventQueue->Push(KeyEvent::KeyDown(key)); }

void SoC::KeyUp(enum KeyId key) { keyEventQueue->Push(KeyEvent::KeyUp(key)); }

void SoC::PenDown(int x, int y) {
    penEventQueue->Push(PenEvent::PenDown(std::min<int>(std::max<int>(x, 0), 320),
                                          std::min<int>(std::max<int>(y, 0), 480)));
}

void SoC::PenUp() { penEventQueue->Push(PenEvent::PenUp()); }

void SoC::SetFramebufferDirty() {
    if (framebufferDirty) return;

    framebufferDirty = true;
    OnSetFramebufferDirty();
}

bool SoC::SetFramebuffer(uint32_t start, uint32_t size) {
    if (start < ramBase || start - ramBase + size > ramSize) {
        fprintf(stderr, "framebuffer not in RAM\n");
        size = 0;
    }

    ramSetFramebuffer(ram, start, size);

    return size != 0;
}

void SoC::ClearFramebufferDirty() { framebufferDirty = false; }

void SoC::Sleep() {
    if (sleeping) return;
    sleeping = true;

    cpuSetSleeping(cpu);
    OnSleep();

    // soc->sleepAtTime = soc->scheduler->GetTime();
    // printf("sleep\n");
}

void SoC::Wakeup(uint8_t wakeupSource) {
    if (!sleeping) return;
    sleeping = false;

    cpuWakeup(cpu);
    OnWakeup();

    // printf("wakeupt after %llu nsec from %u\n", soc->scheduler->GetTime() - soc->sleepAtTime,
    //        (int)wakeupSource);
}

void SoC::SetAudioQueue(struct AudioQueue *audioQueue) {
    this->audioQueue = audioQueue;

    OnSetAudioQueue(audioQueue);
}

void SoC::SetPcmOutputEnabled(bool pcmOutputEnabled) {
    if (audioQueue && pcmOutputEnabled) audioQueueClear(audioQueue);
    if (pcmOutputEnabled == pcmEnabled) return;

    pcmEnabled = pcmOutputEnabled;

    OnSetPcmOutputEnabled();
}

void SoC::SetPcmSuspended(bool pcmSuspended) {
    if (this->pcmSuspended == pcmSuspended) return;

    this->pcmSuspended = pcmSuspended;

    OnSetPcmSuspended();
}

Buffer SoC::GetRomData() { return {.size = romGetSize(rom), .data = romGetData(rom)}; }

struct Buffer SoC::GetNandData() { return nand ? nandGetData(nand) : Buffer{0, nullptr}; }

struct Buffer SoC::GetNandDirtyPages() {
    return nand ? nandGetDirtyPages(nand) : Buffer{0, nullptr};
}

bool SoC::IsNandDirty() { return nand ? nandIsDirty(nand) : false; }

void SoC::SetNandDirty(bool isDirty) {
    if (nand) nandSetDirty(nand, isDirty);
}

Buffer SoC::GetMemoryData() { return {.size = bufferMemory.size, .data = bufferMemory.buffer}; }

Buffer SoC::GetMemoryDirtyPages() {
    return {.size = bufferMemory.dirtyPagesSize, .data = bufferMemory.dirtyPages};
}
