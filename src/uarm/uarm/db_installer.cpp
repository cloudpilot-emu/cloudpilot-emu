#include "db_installer.h"

#include <iostream>

#include "CPU.h"
#include "Defer.h"
#include "cputil.h"
#include "pace.h"
#include "palmos_errors.h"
#include "syscall_dispatch.h"

using namespace std;

int32_t db_installer_install(struct SyscallDispatch* sd, size_t len, void* data) {
    if (!syscallDispatchM68kSupport(sd)) return DB_INSTALL_RESULT_ERR_NOT_SUPPORTED;
    if (!syscallDispatchPrepare(sd)) return DB_INSTALL_RESULT_ERR_NOT_CURRENTLY_POSSIBLE;

    const uint32_t scratch = syscall68k_MemPtrNew(sd, 5);
    if (!scratch) return DB_INSTALL_RESULT_ERR_UNKNOWN;

    const uint32_t deleteProcP = scratch;
    const uint32_t readProcP = scratch + 2;
    const uint32_t needsResetP = scratch + 4;
    size_t bytesRead = 0;
    bool couldNotOverwrite = false;

    syscallDispatchRegisterM68Stub(
        sd, deleteProcP,
        [&](struct ArmCpu*, uint32_t parameterBase, std::function<void()> deadMansSwitch) {
            paceSetDreg(0, 0);
            paceResetFsr();

            const uint16_t cardNo = paceGet16(parameterBase + 6);
            const uint32_t dbID = paceGet32(parameterBase + 8);

            if (paceGetFsr() > 0) ERR("failed reading parameters in deleteProcP");

            const uint16_t err = syscall68k_DmDeleteDatabase(sd, cardNo, dbID);
            couldNotOverwrite = couldNotOverwrite || err;

            paceSetDreg(0, !err);
        });

    syscallDispatchRegisterM68Stub(
        sd, readProcP,
        [&](struct ArmCpu* cpu, uint32_t parameterBase, std::function<void()> deadMansSwitch) {
            deadMansSwitch();

            paceSetDreg(0, -1);
            paceResetFsr();

            const uint32_t dataP = paceGet32(parameterBase);
            const uint32_t sizeP = paceGet32(parameterBase + 4);
            uint32_t size = paceGet32(sizeP);

            if (paceGetFsr() > 0) ERR("failed reading parameters in readProcP");

            if (size > len - bytesRead) size = len - bytesRead;

            if (!syscallDispatch_memcpy_fromHost(
                    sd, dataP, reinterpret_cast<uint8_t*>(data) + bytesRead, size)) {
                ERR("memory error copying DB data");
            }

            paceResetFsr();
            paceSet32(sizeP, size);
            if (paceGetFsr() > 0) ERR("failed to write back size in readProcP");

            bytesRead += size;
            paceSetDreg(0, 0);
        });

    Defer unregisterStubs([=]() {
        syscallDispatchUnregisterM68Stub(sd, readProcP);
        syscallDispatchUnregisterM68Stub(sd, deleteProcP);
    });

    uint16_t err = syscall68k_ExgDBRead(sd, readProcP, deleteProcP, 0, 0, 0, needsResetP, true);

    paceResetFsr();
    bool needsReset = paceGet8(needsResetP);
    if (paceGetFsr() > 0) ERR("failed to read needsReset");

    if (syscall68k_MemPtrFree(sd, scratch) != 0) ERR("failed to release scratch memory");

    if (couldNotOverwrite) return DB_INSTALL_RESULT_ERR_COULD_NOT_OVERWRITE;

    switch (err) {
        case 0:
            return needsReset ? DB_INSTALL_RESULT_NEEDS_RESET : DB_INSTALL_RESULT_OK;

        case dmErrMemError:
            return DB_INSTALL_RESULT_ERR_OOM;

        case dmErrCorruptDatabase:
            return DB_INSTALL_RESULT_ERR_CORRUPT;

        case dmErrDatabaseOpen:
            return DB_INSTALL_RESULT_ERR_IS_OPEN;

        default:
            return DB_INSTALL_RESULT_ERR_UNKNOWN;
    }
}