#pragma GCC diagnostic ignored "-Wmissing-field-initializers"

#include "Commands.h"

#include <cstdint>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "CPEndian.h"
#include "Cli.h"
#include "FileUtil.h"
#include "SoC.h"
#include "app_launcher.h"
#include "db_backup.h"
#include "db_installer.h"
#include "db_list.h"
#include "md5.h"
#include "sdcard.h"
#include "session/session_file5.h"
#include "syscall_dispatch.h"

using namespace std;

namespace {
    Rotation rotate(Rotation rotation) {
        switch (rotation) {
            case Rotation::portrait_0:
                return Rotation::landscape_90;

            case Rotation::landscape_90:
                return Rotation::portrait_180;

            case Rotation::portrait_180:
                return Rotation::landscape_270;

            default:
                return Rotation::portrait_0;
        }
    }

    void CmdSetMips(vector<string> args, cli::CommandEnvironment& env, void* context) {
        if (args.size() != 1) return env.PrintUsage();

        uint32_t mips;
        istringstream s(args[0]);

        s >> mips;

        if (s.fail() || !s.eof()) {
            cout << "invalid argument" << endl;
            return;
        }

        static_cast<commands::Context*>(context)->mainLoop.SetCyclesPerSecondLimit(mips * 1000000);
    }

    void CmdEnableAudio(vector<string> args, cli::CommandEnvironment& env, void* context) {
        static_cast<commands::Context*>(context)->audioDriver.Start();
    }

    void CmdDisableAudio(vector<string> args, cli::CommandEnvironment& env, void* context) {
        static_cast<commands::Context*>(context)->audioDriver.Pause();
    }

    void CmdUnmount(vector<string> args, cli::CommandEnvironment& env, void* context) {
        if (!sdCardInitialized()) {
            cout << "no sd card mounted" << endl;
            return;
        }

        auto ctx = reinterpret_cast<commands::Context*>(context);

        socSdEject(ctx->soc);
        sdCardReset();
    }

    void CmdMount(vector<string> args, cli::CommandEnvironment& env, void* context) {
        if (sdCardInitialized()) {
            cout << "sd card already mounted" << endl;
            return;
        }

        if (args.size() != 1) return env.PrintUsage();

        size_t len{0};
        unique_ptr<uint8_t[]> data;

        if (!util::ReadFile(args[0], data, len)) {
            cout << "failed to read " << args[0];
            return;
        }

        if (len % SD_SECTOR_SIZE) {
            cout << "sd card image has bad size" << endl;
            return;
        }

        auto ctx = reinterpret_cast<commands::Context*>(context);
        string key = md5(data.get(), len);

        sdCardInitializeWithData(len / SD_SECTOR_SIZE, data.release(), key.c_str());
        socSdInsert(ctx->soc);
    }

    void CmdReset(vector<string> args, cli::CommandEnvironment& env, void* context) {
        auto ctx = reinterpret_cast<commands::Context*>(context);

        socReset(ctx->soc);
    }

    void CmdResetNoExtensions(vector<string> args, cli::CommandEnvironment& env, void* context) {
        auto ctx = reinterpret_cast<commands::Context*>(context);

        socJamKey(ctx->soc, keyIdUp, 2000);
        socReset(ctx->soc);
    }

    void CmdResetHard(vector<string> args, cli::CommandEnvironment& env, void* context) {
        auto ctx = reinterpret_cast<commands::Context*>(context);

        socJamKey(ctx->soc, keyIdPower, 2000);
        socReset(ctx->soc);
    }

    void CmdRotate(vector<string> args, cli::CommandEnvironment& env, void* context) {
        auto ctx = reinterpret_cast<commands::Context*>(context);

        ctx->rotation = rotate(ctx->rotation);
    }

    void CmdSaveSession(vector<string> args, cli::CommandEnvironment& env, void* context) {
        if (args.size() != 1 && args.size() != 2) return env.PrintUsage();

        auto ctx = reinterpret_cast<commands::Context*>(context);

        if (sdCardInitialized()) {
            string key = md5(reinterpret_cast<uint8_t*>(sdCardData().data), sdCardData().size);
            sdCardRekey(key.c_str());
        }

        if (!socSave(ctx->soc)) {
            cout << "failed to save state" << endl;
            return;
        }

        SessionFile5 sessionFile;

        const Buffer rom = socGetRomData(ctx->soc);
        const Buffer nand = socGetNandData(ctx->soc);
        const Buffer memory = socGetMemoryData(ctx->soc);
        const Buffer savestate = socGetSavestate(ctx->soc);
        const Buffer sd = sdCardData();

        sessionFile.SetDeviceId(socGetDeviceType(ctx->soc))
            .SetRamSize(socGetRamSize(ctx->soc))
            .SetNor(rom.size, reinterpret_cast<uint8_t*>(rom.data))
            .SetNand(nand.size, reinterpret_cast<uint8_t*>(nand.data))
            .SetMemory(memory.size, reinterpret_cast<uint8_t*>(memory.data))
            .SetSavestate(savestate.size, reinterpret_cast<uint8_t*>(savestate.data));

        if (!sessionFile.Serialize()) {
            cout << "failed to serialize session" << endl;
            return;
        }

        if (!util::WriteFile(args[0],
                             reinterpret_cast<const uint8_t*>(sessionFile.GetSerializedSession()),
                             sessionFile.GetSerializedSessionSize())) {
            cout << "failed to write session to " << args[0] << endl;
        } else {
            cout << "wrote session to " << args[0] << endl;
        }

        if (args.size() == 2 && sdCardInitialized()) {
            if (!util::WriteFile(args[1], reinterpret_cast<uint8_t*>(sd.data), sd.size)) {
                cout << "failed to write SD image to " << args[1] << endl;
            } else {
                cout << "wrote SD image to " << args[1] << endl;
            }
        }
    }

    void CmdSaveSd(vector<string> args, cli::CommandEnvironment& env, void* context) {
        if (args.size() != 1) return env.PrintUsage();

        if (!sdCardInitialized()) {
            cout << "no SD card mounted" << endl;
            return;
        }

        const Buffer sd = sdCardData();

        if (!util::WriteFile(args[0], reinterpret_cast<uint8_t*>(sd.data), sd.size)) {
            cout << "failed to write SD image to " << args[0] << endl;
        } else {
            cout << "wrote SD image to " << args[0] << endl;
        }
    }

    void CmdDumpMMU(vector<string> args, cli::CommandEnvironment& env, void* context) {
        if (args.size() != 0) return env.PrintUsage();
        auto ctx = reinterpret_cast<commands::Context*>(context);

        socDumpMMU(ctx->soc);
    }

    void CmdOsVersion(vector<string> args, cli::CommandEnvironment& env, void* context) {
        if (args.size() != 0) return env.PrintUsage();
        auto ctx = reinterpret_cast<commands::Context*>(context);
        SyscallDispatch* sd = socGetSyscallDispatch(ctx->soc);

        if (!syscallDispatchM68kSupport(sd)) {
            cout << "m68k syscalls not supported" << endl;
            return;
        }

        if (!syscallDispatchPrepare(sd)) {
            cout << "unable to prepare save environment for syscalls" << endl;
            return;
        }

        char version[32];
        const uint32_t ptr = syscall68k_SysGetOsVersionString(sd, SC_EXECUTE_FULL);

        if (!ptr) {
            cout << "failed to retrieve OS version string" << endl;
            return;
        }

        syscallDispatch_strncpy_toHost(sd, version, ptr, sizeof(version));
        version[sizeof(version) - 1] = '\0';

        if (syscall68k_MemPtrFree(sd, SC_EXECUTE_PURE, ptr) != 0) {
            cout << "very bad: failed to release m68k pointer" << endl;
            return;
        }

        cout << version << endl;
    }

    void CmdSetAutoOff(vector<string> args, cli::CommandEnvironment& env, void* context) {
        if (args.size() != 1) return env.PrintUsage();

        uint32_t timeout;
        if (!(istringstream(args[0]) >> timeout)) {
            cout << "invalid timeout" << endl;
            return env.PrintUsage();
        }

        auto ctx = reinterpret_cast<commands::Context*>(context);
        SyscallDispatch* sd = socGetSyscallDispatch(ctx->soc);

        if (!syscallDispatchM68kSupport(sd)) {
            cout << "m68k syscalls not supported" << endl;
            return;
        }

        if (!syscallDispatchPrepare(sd)) {
            cout << "unable to prepare save environment for syscalls" << endl;
            return;
        }

        syscall_SysSetAutoOffTime(sd, SC_EXECUTE_PURE, timeout);
    }

    void CmdInstall(vector<string> args, cli::CommandEnvironment& env, void* context) {
        if (args.size() != 1) return env.PrintUsage();

        size_t len;
        unique_ptr<uint8_t[]> data;
        if (!util::ReadFile(args[0], data, len)) {
            cout << "unable to read " << args[0] << endl;
            return;
        }

        auto ctx = reinterpret_cast<commands::Context*>(context);

        const auto result = dbInstallerInstall(socGetSyscallDispatch(ctx->soc), len, data.get());

        switch (result) {
            case DB_INSTALL_RESULT_NEEDS_RESET:
                cout << "installation successful, needs reset" << endl;
                break;

            case DB_INSTALL_RESULT_OK:
                cout << "installation successful" << endl;
                break;

            case DB_INSTALL_RESULT_ERR_CORRUPT:
                cout << "database corrupt" << endl;
                break;

            case DB_INSTALL_RESULT_ERR_OOM:
                cout << "out of memory" << endl;
                break;

            case DB_INSTALL_RESULT_ERR_IS_OPEN:
                cout << "db cannot is open and cannot be deleted" << endl;
                break;

            case DB_INSTALL_RESULT_ERR_COULD_NOT_OVERWRITE:
                cout << "unable to overwrite db" << endl;
                break;

            case DB_INSTALL_RESULT_ERR_NOT_SUPPORTED:
                cout << "db installation not supported on this ROM" << endl;
                break;

            case DB_INSTALL_RESULT_ERR_NOT_CURRENTLY_POSSIBLE:
                cout << "db installation not currently possible" << endl;
                break;

            case DB_INSTALL_RESULT_ERR_UNKNOWN:
                cout << "unknown error during installation" << endl;
                break;
        }
    }

    void CmdDbList(vector<string> args, cli::CommandEnvironment& env, void* context) {
        if (args.size() != 0) return env.PrintUsage();
        auto ctx = reinterpret_cast<commands::Context*>(context);
        SyscallDispatch* sd = socGetSyscallDispatch(ctx->soc);

        DbMetadataList metadataList;
        if (!dbListGet(sd, metadataList)) return;

        for (const auto& meta : metadataList) {
            union {
                uint32_t numeric;
                char text[5];
            } type, creator;

            type.text[4] = creator.text[4] = '\0';
            type.numeric = be32toh(meta.type);
            creator.numeric = be32toh(meta.creator);

            printf("ID 0x%08x cardNo %u index %u %s %s %s %s\n", meta.localID, meta.cardNo,
                   meta.index, meta.isRom ? "ROM" : "ram", type.text, creator.text, meta.name);
        }
    }

    void CmdDbExport(vector<string> args, cli::CommandEnvironment& env, void* context, int type) {
        if (args.size() != 1) return env.PrintUsage();

        auto ctx = reinterpret_cast<commands::Context*>(context);
        SyscallDispatch* sd = socGetSyscallDispatch(ctx->soc);

        DbBackup backup(sd, type);

        if (!backup.Init()) {
            cout << "failed to initialize export" << endl;
            return;
        }

        while (backup.GetState() == DbBackup::STATE_IN_PROGRESS) {
            if (backup.Continue()) {
                if (backup.HasLastProcessedDb()) {
                    cout << "OK " << backup.GetLastProcessedDb() << endl;
                }
            } else {
                cout << "FAILED " << backup.GetLastProcessedDb() << endl;
            }
        }

        cout << endl;

        if (backup.GetState() != DbBackup::STATE_DONE) {
            cout << "export FAILED" << endl;
            return;
        }

        if (util::WriteFile(args[0], reinterpret_cast<const uint8_t*>(backup.GetArchiveData()),
                            backup.GetArchiveSize())) {
            cout << "export written to " << args[0] << endl;
        } else {
            cout << "failed to write export to " << args[0] << endl;
        }
    }

    void CmdDbExportAll(vector<string> args, cli::CommandEnvironment& env, void* context) {
        CmdDbExport(args, env, context, DbBackup::BACKUP_TYPE_EVERYTHING);
    }

    void CmdDbExportRamRom(vector<string> args, cli::CommandEnvironment& env, void* context) {
        CmdDbExport(args, env, context, DbBackup::BACKUP_TYPE_RAM_ROM);
    }

    void CmdDbExportRam(vector<string> args, cli::CommandEnvironment& env, void* context) {
        CmdDbExport(args, env, context, DbBackup::BACKUP_TYPE_RAM);
    }

    void CmdLaunch(vector<string> args, cli::CommandEnvironment& env, void* context) {
        if (args.size() != 1) return env.PrintUsage();

        auto ctx = reinterpret_cast<commands::Context*>(context);
        SyscallDispatch* sd = socGetSyscallDispatch(ctx->soc);

        launchAppByName(sd, args[0].c_str());
    }

    const vector<cli::Command> commandList(
        {{.name = "set-mips",
          .usage = "set-mips <mips>",
          .description = "Set target MIPS.",
          .cmd = CmdSetMips},
         {.name = "audio-on", .description = "Enable audio.", .cmd = CmdEnableAudio},
         {.name = "audio-off", .description = "Disable audio.", .cmd = CmdDisableAudio},
         {.name = "unmount", .description = "Unmount SD card.", .cmd = CmdUnmount},
         {.name = "mount",
          .usage = "mount <image>",
          .description = "Unmount SD card.",
          .cmd = CmdMount},
         {.name = "reset", .description = "Reset Pilot.", .cmd = CmdReset},
         {.name = "reset-no-ext",
          .description = "Reset Pilot w/o loading extensions.",
          .cmd = CmdResetNoExtensions},
         {.name = "reset-hard", .description = "Hard reset Pilot.", .cmd = CmdResetHard},
         {.name = "rotate", .description = "Rotate 90° CCW", .cmd = CmdRotate},
         {.name = "save-session",
          .usage = "save-session <session file> [card image]",
          .description = "Save session.",
          .cmd = CmdSaveSession},
         {.name = "save-sd",
          .usage = "save-sd <card image>",
          .description = "Save SD card.",
          .cmd = CmdSaveSd},
         {.name = "dump-mmu", .description = "Dump MMU mappings.", .cmd = CmdDumpMMU},
         {.name = "os-version", .description = "Retrieve OS version. ", .cmd = CmdOsVersion},
         {.name = "set-auto-off",
          .usage = "set-auto-off",
          .description = "Set auto off timeout (warning: wakeup will currently fail). ",
          .cmd = CmdSetAutoOff},
         {.name = "install",
          .usage = "install <database>",
          .description = "Install database.",
          .cmd = CmdInstall},
         {.name = "db-list", .description = "List databases.", .cmd = CmdDbList},
         {.name = "db-export-all",
          .usage = "db-export-all <file>",
          .description = "Export all databases.",
          .cmd = CmdDbExportAll},
         {.name = "db-export-rom",
          .usage = "db-export-rom <file>",
          .description = "Export RAM and ROM databases (excluding PACE cache).",
          .cmd = CmdDbExportRamRom},
         {.name = "db-export",
          .usage = "db-export <file>",
          .description = "Export all RAM databases (excluding PACE cache).",
          .cmd = CmdDbExportRam},
         {.name = "launch",
          .usage = "launch <name>",
          .description = "Launch app",
          .cmd = CmdLaunch}});
}  // namespace

void commands::Register() { cli::AddCommands(commandList); }