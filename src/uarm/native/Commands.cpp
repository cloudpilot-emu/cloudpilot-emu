#pragma GCC diagnostic ignored "-Wmissing-field-initializers"

#include "Commands.h"

#include <cstdint>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "Cli.h"
#include "FileUtil.h"
#include "SoC.h"
#include "md5.h"
#include "savestate/SessionFile.h"
#include "sdcard.h"

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

        SessionFile sessionFile;

        const Buffer rom = socGetRomData(ctx->soc);
        const Buffer nand = socGetNandData(ctx->soc);
        const Buffer memory = socGetMemoryData(ctx->soc);
        const Buffer savestate = socGetSavestate(ctx->soc);
        const Buffer sd = sdCardData();

        sessionFile.SetDeviceId(socGetDeviceType(ctx->soc))
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
         {.name = "rotate", .description = "Rotate 90° CCW", .cmd = CmdRotate},
         {.name = "save-session",
          .usage = "save-session <session file> [card image]",
          .description = "Save session.",
          .cmd = CmdSaveSession},
         {.name = "save-sd",
          .usage = "save-sd <card image>",
          .description = "Save SD card.",
          .cmd = CmdSaveSd}});
}  // namespace

void commands::Register() { cli::AddCommands(commandList); }