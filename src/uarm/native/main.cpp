#include <SDL.h>
#include <SDL_image.h>
#include <unistd.h>

#include <cstring>
#include <iomanip>
#include <iostream>
#include <memory>
#include <optional>
#include <sstream>

#include "Cli.h"
#include "Commands.h"
#include "FileUtil.h"
#include "MainLoop.h"
#include "SdlAudioDriver.h"
#include "SdlEventHandler.h"
#include "SdlRenderer.h"
#include "SoC.h"
#include "argparse.h"
#include "audio_queue.h"
#include "buffer.h"
#include "cputil.h"
#include "device.h"
#include "savestate/session_file.h"
#include "sdcard.h"

using namespace std;

struct Options {
    string norOrSession;
    optional<string> nand;
    optional<string> sd;
    optional<unsigned int> gdbPort;
    unsigned int mips;
    bool disableAudio;
    optional<string> script;
};

extern "C" int socExtSerialReadChar(void) { return CHAR_NONE; }

extern "C" void socExtSerialWriteChar(int chr) {
    if (!(chr & 0xFF00))
        printf("%c", chr);
    else
        printf("<<~~ EC_0x%x ~~>>", chr);

    fflush(stdout);
}

namespace {
    constexpr size_t AUDIO_QUEUE_SIZE = 44100 / MAIN_LOOP_FPS * 10;
    constexpr int SCALE = 2;
    constexpr size_t NAND_SIZE = 34603008;

    bool initSdl(struct DeviceDisplayConfiguration displayConfiguration, int scale,
                 SDL_Window*& window, SDL_Renderer*& renderer) {
        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_AUDIO) < 0) {
            cerr << "could not initialize SDL: " << SDL_GetError() << endl;
            return false;
        }

        IMG_Init(IMG_INIT_PNG);

        atexit(SDL_Quit);

        if (SDL_CreateWindowAndRenderer(
                displayConfiguration.width * scale,
                (displayConfiguration.height + displayConfiguration.graffitiHeight) * scale, 0,
                &window, &renderer) != 0) {
            cout << "unable to setup window" << endl;
            return false;
        }

        SDL_SetWindowTitle(window, "cp-uarm");

        return true;
    }

    bool readSession(const Options& options, Buffer& nor, Buffer& nand, Buffer& ram,
                     Buffer& savestate) {
        size_t norOrSessionLen{0};
        unique_ptr<uint8_t[]> norOrSessionData;
        if (!util::ReadFile(options.norOrSession, norOrSessionData, norOrSessionLen)) return false;

        if (isSessionFile({.size = norOrSessionLen, .data = norOrSessionData.get()})) {
            if (!sessionFile_read({.size = norOrSessionLen, .data = norOrSessionData.get()}, &nor,
                                  &nand, &ram, &savestate))
                return false;

            if (options.nand) {
                cerr << "separate NAND image cannot be used with session file" << endl;
                return false;
            }
        } else {
            size_t nandLen{0};
            unique_ptr<uint8_t[]> nandData;

            if (options.nand) {
                if (!util::ReadFile(options.nand, nandData, nandLen)) return false;
            } else {
                nandLen = NAND_SIZE;
                nandData = make_unique<uint8_t[]>(NAND_SIZE);
                memset(nandData.get(), 0xff, NAND_SIZE);
            }

            nor.size = norOrSessionLen;
            nor.data = norOrSessionData.release();

            nand.size = nandLen;
            nand.data = nandData.release();

            ram.size = 0;
            ram.data = nullptr;

            savestate.size = 0;
            savestate.data = nullptr;
        }

        return true;
    }

    bool run(const Options& options) {
        if (options.mips == 0) {
            cerr << "MIPS must be finite" << endl;
            return false;
        }

        Buffer nor, nand, ram, savestate;

        if (!readSession(options, nor, nand, ram, savestate)) return false;

        if (nand.size != NAND_SIZE) {
            cerr << "invalid NAND size; expected " << NAND_SIZE << " bytes" << endl;
            return false;
        }

        size_t sdLen{0};
        unique_ptr<uint8_t[]> sdData;
        if (options.sd && !util::ReadFile(options.sd, sdData, sdLen)) return false;

        if (sdData) {
            if (sdLen % SD_SECTOR_SIZE) {
                cout << "sd card image has bad size" << endl;
                return false;
            }

            sdCardInitializeWithData(sdLen / SD_SECTOR_SIZE, sdData.release());
        }

        SoC* soc = socInit(nor.data, nor.size, reinterpret_cast<uint8_t*>(nand.data), nand.size,
                           options.gdbPort.value_or(0), deviceGetSocRev());

        if (ram.data && ram.size != socGetRamData(soc).size) {
            cerr << "RAM size mismatch" << endl;
            return false;
        }

        if (ram.data) memcpy(socGetRamData(soc).data, ram.data, ram.size);

        if (sdCardInitialized()) socSdInsert(soc);

        AudioQueue* audioQueue = audioQueueCreate(AUDIO_QUEUE_SIZE);
        socSetAudioQueue(soc, audioQueue);

        MainLoop mainLoop(soc);
        mainLoop.SetCyclesPerSecondLimit(options.mips * 1000000);

        DeviceDisplayConfiguration displayConfiguration;
        deviceGetDisplayConfiguration(&displayConfiguration);

        SDL_Window* window;
        SDL_Renderer* renderer;
        if (!initSdl(displayConfiguration, SCALE, window, renderer)) return false;

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xff);
        SDL_RenderClear(renderer);
        SdlRenderer sdlRenderer(window, renderer, soc, SCALE);
        SdlEventHandler sdlEventHandler(soc, SCALE);

        SdlAudioDriver audioDriver(soc, audioQueue);
        if (!options.disableAudio) audioDriver.Start();

        commands::Register();
        cli::Start(options.script);
        commands::Context commandContext{
            .soc = soc, .mainLoop = mainLoop, .audioDriver = audioDriver};

        uint64_t lastSpeedDump = timestampUsec();

        while (true) {
            uint64_t now = timestampUsec();

            if (!options.disableAudio) socSetPcmSuspended(soc, audioDriver.GetAudioBackpressure());

            mainLoop.Cycle(now);

            sdlRenderer.Draw(sdlEventHandler.RedrawRequested());
            sdlEventHandler.ClearRedrawRequested();

            sdlEventHandler.HandleEvents();
            if (sdlEventHandler.QuitRequested()) break;

            if (now - lastSpeedDump > 1000000) {
                const uint64_t currentIps = mainLoop.GetCurrentIps();
                const uint64_t currentIpsMax = mainLoop.GetCurrentIpsMax();
                lastSpeedDump = now;

                ostringstream s;
                s << "cp-uarm @ " << fixed << setprecision(2)
                  << static_cast<float>(currentIps) / 1000000 << " MIPS, limit "
                  << static_cast<float>(currentIpsMax) / 1000000 << " IPS -> "
                  << (100 * currentIps) / currentIpsMax << "%" << endl
                  << flush;

                SDL_SetWindowTitle(window, s.str().c_str());
            }

            const int64_t timesliceRemaining =
                mainLoop.GetTimesliceSizeUsec() - static_cast<int64_t>(timestampUsec() - now);

            if (cli::Execute(&commandContext)) break;

            if (timesliceRemaining > 10) usleep(timesliceRemaining);
        }

        audioDriver.Pause();
        cli::Stop();

        return true;
    }

}  // namespace

int main(int argc, const char** argv) {
    argparse::ArgumentParser program("cp-uarm");

    program.add_description("cp-uarm emulates a Palm Tungsten E2");

    program.add_argument("nor_or_session").help("NOR rom or saved session").required();

    program.add_argument("--nand", "-n").help("NAND rom file").metavar("<nand file>");

    program.add_argument("--sd", "-s").help("SD card file").metavar("<SD card file>");

    program.add_argument("--no-sound", "-q")
        .help("start with audio off")
        .default_value(false)
        .implicit_value(true);

    program.add_argument("--gdb")
        .help("listen for GDB")
        .metavar("<port>")
        .scan<'u', unsigned int>();

    program.add_argument("--mips")
        .help("target speed in MIPS")
        .metavar("<mips>")
        .scan<'u', unsigned int>()
        .default_value(100u);

    program.add_argument("--script").help("execute script on startup").metavar("<script file>");

    try {
        program.parse_args(argc, argv);
    } catch (const invalid_argument& e) {
        cerr << "invalid argument" << endl << endl;
        cerr << program;

        exit(1);
    } catch (const runtime_error& e) {
        cerr << e.what() << endl << endl;
        cerr << program;

        exit(1);
    }

    Options options = {.norOrSession = program.get("nor_or_session"),
                       .nand = program.present("--nand"),
                       .sd = program.present("--sd"),
                       .gdbPort = program.present<unsigned int>("--gdb"),
                       .mips = program.get<unsigned int>("--mips"),
                       .disableAudio = program.get<bool>("--no-sound"),
                       .script = program.present("--script")};

    if (!run(options)) exit(1);
}