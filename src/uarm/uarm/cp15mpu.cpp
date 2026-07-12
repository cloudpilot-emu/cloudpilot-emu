#include "cp15mpu.h"

#include "CPU.h"
#include "MPU.h"
#include "cputil.h"
#include "icache.h"
#include "savestate/savestateAll.h"

#define SAVESTATE_VERSION 0

struct ArmCP15MPU {
    struct ArmCpu* cpu;
    struct ArmMpu* mpu;
    struct icache* ic;

    uint32_t control;

    uint32_t cpuid;
    uint32_t cacheId;

    template <typename T>
    void DoSaveLoad(T& chunkHelper) {
        chunkHelper.Do32(control).Do32(cpuid).Do32(cacheId);
    }
};

static bool cp15prvCoprocRegXferFunc(struct ArmCpu* cpu, void* userData, bool two, bool read,
                                     uint8_t op1, uint8_t Rx, uint8_t CRn, uint8_t CRm,
                                     uint8_t op2) {
    struct ArmCP15MPU* cp15 = reinterpret_cast<ArmCP15MPU*>(userData);
    uint32_t val = 0;

    if (!read) val = cpuGetRegExternal(cpu, Rx);

    if (op1 != 0 || two) goto fail;  // CP15 only accessed with MCR/MRC with op1 == 0

    switch (CRn) {
        case 0:  // ID codes

            if (!read) goto fail;     // cannot write to ID codes register
            if (CRm != 0) goto fail;  // CRm must be zero for this read
            if (op2 == 0) {           // main ID register

                val = cp15->cpuid;
                goto success;
            } else if (op2 == 1) {  // cache info

                val = cp15->cacheId;
                goto success;
            }

            break;

        case 1:  // control register

            if (op2 == 0 && CRm == 0) {
                if (read) {
                    val = cp15->control | 2;
                } else {
                    // some bits ignore writes. pretend they were wirtten as proper
                    val |= 0x70;
                    val &= ~0xfff00f02;
                    cp15->control = val;

                    cpuSetVectorAddr(cp15->cpu, (val & 0x00002000UL) ? 0xFFFF0000UL : 0x00000000UL);
                    mpuSetEnabled(cp15->mpu, !!(val & 0x00000001ul));
                }
            } else
                break;
            goto success;

        case 2:  // cacheability bits
            if (read)
                val = mpuGetCacheable(cp15->mpu);
            else
                mpuSetCacheable(cp15->mpu, val);
            goto success;

        case 3:  // bufferability bits
            if (read)
                val = mpuGetBufferable(cp15->mpu);
            else
                mpuSetBufferable(cp15->mpu, val);
            goto success;

        case 5:  // acess permission bits
            if (read)
                val = mpuGetAP(cp15->mpu);
            else
                mpuSetAP(cp15->mpu, val);
            goto success;

        case 6:  // region config
            if (read)
                val = mpuGetRegionConfig(cp15->mpu, CRm);
            else
                mpuSetRegionConfig(cp15->mpu, CRm, val & 0xfffff07f);
            goto success;

        case 7:  // cache ops
            if ((CRm == 5 || CRm == 7) && op2 == 0) {
                if (cp15->ic)
                    icacheInval(cp15->ic);  // invalidate entire {icache(5) or both i and dcache(7)}
                if (CRm == 7) {
                    // dcacheInval(cp15->dc);
                }
            } else if ((CRm == 5 || CRm == 7) && op2 == 1) {
                if (cp15->ic)
                    icacheInvalAddr(
                        cp15->ic,
                        val);  // invalidate {icache(5) or both i and dcache(7)} line, given VA
                if (CRm == 7) {
                    // dcacheInvalAddr(cp15->dc, val);
                }
            } else if ((CRm == 5 || CRm == 7) && op2 == 2) {
                if (cp15->ic)
                    icacheInval(cp15->ic);  // invalidate {icache(5) or both i and dcache(7)} line,
                                            // given set/index. i dont know how to do this, so flush
                                            // the whole thing
                if (CRm == 7) {
                    // dcacheInvalSetWayRaw(cp15->dc, val);
                }
            } else if (CRm == 10 && op2 == 4) { /* drain write buffer = nothing */
            } else if (CRm == 10 && op2 == 1) {
                // dcacheCleanSetWayRaw(cp15->dc, val);
            } else if (CRm == 6 && op2 == 0) {
                // dcacheInval(cp15->dc);
            } else if (CRm == 6 && op2 == 1) {
                // dcacheInvalAddr(cp15->dc, val);
            } else if (CRm == 6 && op2 == 2) {
                // dcacheInvalSetWayRaw(cp15->dc, val);
            } else if (CRm == 2 && op2 == 5) {
                // dcacheAllocAddr(cp15->dc, val);
            } else if (CRm == 5 && op2 == 6) {  /* flush btb = nothing */
            } else if (CRm == 0 && op2 == 4) {  /* idle = nothing */
            } else if (CRm == 14 && op2 == 2) { /* clean and inval d-cache line */
            } else if (CRm == 10 && op2 == 0) { /* clean entire d-cache - omap can do this */
            } else if (CRm == 10 && op2 == 2) { /* clean d-cache line */
            } else
                break;
            goto success;

        case 9:  // cache lockdown
            if (CRm == 0 && op2 == 0)
                fprintf(stderr, "Attempt to lock 0x%08lx+32 in icache\r\n", (unsigned long)val);
            else if (CRm == 0 && op2 == 1)
                fprintf(stderr, "Dcache now %s lock mode\r\n", val ? "in" : "out of");

            else
                break;
            goto success;
    }

fail:
    return false;

success:

    if (read) cpuSetReg(cpu, Rx, val);

    return true;
}

struct ArmCP15MPU* cp15MPUInit(struct ArmCpu* cpu, struct ArmMpu* mpu, struct icache* ic,
                               uint32_t cpuid, uint32_t cacheId) {
    ArmCP15MPU* cp15 = new ArmCP15MPU();

    cp15->cpu = cpu;
    cp15->mpu = mpu;
    cp15->ic = ic;

    cp15->control = 0x78;
    cp15->cpuid = cpuid;
    cp15->cacheId = cacheId;

    struct ArmCoprocessor cp = {
        .regXfer = cp15prvCoprocRegXferFunc,
        .userData = cp15,
    };

    cpuCoprocessorRegister(cpu, 15, &cp);

    return cp15;
}

template <typename T>
void cp15MPUSave(struct ArmCP15MPU* cp15, T& savestate) {
    auto chunk = savestate.GetChunk(ChunkType::cp15mpu, SAVESTATE_VERSION);
    if (!chunk) ERR("unable to allocate chunk");

    SaveChunkHelper helper(*chunk);
    cp15->DoSaveLoad(helper);
}

template <typename T>
void cp15MPULoad(struct ArmCP15MPU* cp15, T& loader) {
    auto chunk = loader.GetChunkOrFail(ChunkType::cp15mpu, SAVESTATE_VERSION, "cp15mpu");
    if (!chunk) return;

    LoadChunkHelper helper(*chunk);
    cp15->DoSaveLoad(helper);
}

template void cp15MPUSave<Savestate<ChunkType>>(ArmCP15MPU* cp15, Savestate<ChunkType>& savestate);
template void cp15MPUSave<SavestateProbe<ChunkType>>(ArmCP15MPU* cp15,
                                                     SavestateProbe<ChunkType>& savestate);
template void cp15MPULoad<SavestateLoader<ChunkType>>(ArmCP15MPU* cp15,
                                                      SavestateLoader<ChunkType>& loader);
