#pragma GCC diagnostic ignored "-Wmultichar"

#include <SDL.h>
#include <SDL_image.h>
#include <unistd.h>

#include <cstring>
#include <iomanip>
#include <iostream>
#include <memory>
#include <optional>
#include <sstream>
#include <string>

#include "Cli.h"
#include "Commands.h"
#include "FileUtil.h"
#include "Logging.h"
#include "MainLoop.h"
#include "Rotation.h"
#include "SdlAudioDriver.h"
#include "SdlEventHandler.h"
#include "SdlRenderer.h"
#include "SoC.h"
#include "argparse.h"
#include "audio_queue.h"
#include "buffer.h"
#include "cputil.h"
#include "device.h"
#include "md5.h"
#include "rom_info5.h"
#include "sdcard.h"
#include "session/session_file5.h"

using namespace std;

struct Options {
    string norOrSession;
    optional<string> nand;
    optional<string> sd;
    optional<unsigned int> gdbPort;
    unsigned int mips;
    bool disableAudio;
    optional<string> script;
    optional<unsigned int> ramSize;
    bool smallWindow;
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
    constexpr size_t NAND_SIZE = 34603008;

    int windowWidth(DeviceDisplayConfiguration& displayConfiguration, Rotation rotation) {
        switch (rotation) {
            case Rotation::landscape_90:
            case Rotation::landscape_270:
                return displayConfiguration.height + displayConfiguration.graffitiHeight;

            default:
                return displayConfiguration.width;
        }
    }

    int windowHeight(DeviceDisplayConfiguration& displayConfiguration, Rotation rotation) {
        switch (rotation) {
            case Rotation::landscape_90:
            case Rotation::landscape_270:
                return displayConfiguration.width;

            default:
                return displayConfiguration.height + displayConfiguration.graffitiHeight;
        }
    }

    SDL_Window* initSdl(DeviceDisplayConfiguration& displayConfiguration, int scale,
                        Rotation rotation) {
        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_AUDIO) < 0) {
            cerr << "could not initialize SDL: " << SDL_GetError() << endl;
            return nullptr;
        }

        IMG_Init(IMG_INIT_PNG);

        atexit(SDL_Quit);

        auto window = SDL_CreateWindow("cp-uarm", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                       scale * windowWidth(displayConfiguration, rotation),
                                       scale * windowHeight(displayConfiguration, rotation),
                                       SDL_WINDOW_ALLOW_HIGHDPI);

        return window;
    }

    void sdlResizeWindow(SDL_Window* window, DeviceDisplayConfiguration& displayConfiguration,
                         int scale, Rotation rotation) {
        SDL_SetWindowSize(window, scale * windowWidth(displayConfiguration, rotation),
                          scale * windowHeight(displayConfiguration, rotation));
    }

    void copy(Buffer& buffer, size_t size, const void* data) {
        if (size == 0 || data == nullptr) {
            buffer.data = nullptr;
            buffer.size = 0;

            return;
        }

        buffer.data = malloc(size);
        buffer.size = size;

        memcpy(buffer.data, data, size);
    }

    bool readSession(const Options& options, Buffer& nor, Buffer& nand, Buffer& ram,
                     Buffer& savestate, uint32_t& ramSize) {
        SessionFile5 sessionFile;

        size_t norOrSessionLen{0};
        unique_ptr<uint8_t[]> norOrSessionData;
        if (!util::ReadFile(options.norOrSession, norOrSessionData, norOrSessionLen)) return false;

        if (SessionFile5::IsSessionFile(norOrSessionLen, norOrSessionData.get()) &&
            sessionFile.Deserialize(norOrSessionLen, norOrSessionData.get())) {
            if (options.nand) {
                cerr << "separate NAND image cannot be used with session file" << endl;
                return false;
            }

            ramSize = sessionFile.GetRamSize();
            copy(nor, sessionFile.GetNorSize(), sessionFile.GetNor());
            copy(nand, sessionFile.GetNandSize(), sessionFile.GetNand());
            copy(ram, sessionFile.GetMemorySize(), sessionFile.GetMemory());
            copy(savestate, sessionFile.GetSavestateSize(), sessionFile.GetSavestate());
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

            ramSize = 0;

            nor.size = norOrSessionLen;
            nor.data = norOrSessionData.release();

            nand.size = nandLen;
            nand.data = nandData.release();

            ram.size = savestate.size = 0;
            ram.data = savestate.data = nullptr;
        }

        return true;
    }

    bool run(const Options& options) {
        if (options.mips == 0) {
            cerr << "MIPS must be finite" << endl;
            return false;
        }

        Buffer nor, nand, memory, savestate;
        uint32_t ramSize{0};

        if (!readSession(options, nor, nand, memory, savestate, ramSize)) return false;

        RomInfo5 romInfo(reinterpret_cast<uint8_t*>(nor.data), nor.size);
        cerr << romInfo;

        if (!romInfo.IsValid() || romInfo.GetDeviceType() == DeviceType5::deviceTypeInvalid)
            return false;

        if (options.ramSize) {
            if (ramSize == 0) {
                ramSize = *options.ramSize << 20;
            } else {
                cerr << "cannot specify RAM size for an existing session" << endl;
                return false;
            }
        }

        if (ramSize == 0) ramSize = romInfo.GetRecommendedRamSize();

        if (!deviceSupportsRamSize(ramSize)) {
            cerr << "unsupported RAM size: " << ramSize << " bytes" << endl;
            return false;
        }

        cerr << "using RAM size: " << ramSize << " bytes" << endl << endl;

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

            string key = md5(sdData.get(), sdLen);
            sdCardInitializeWithData(sdLen / SD_SECTOR_SIZE, sdData.release(), key.c_str());
        }

        SoC* soc = socInit(romInfo.GetDeviceType(), ramSize, nor.data, nor.size,
                           reinterpret_cast<uint8_t*>(nand.data), nand.size,
                           options.gdbPort.value_or(0), deviceGetSocRev());

        if (memory.data && memory.size > socGetMemoryData(soc).size) {
            cerr << "RAM size mismatch" << endl;
            return false;
        }

        if (memory.data) {
            memcpy(socGetMemoryData(soc).data, memory.data, memory.size);
            free(memory.data);
        }

        if (!socLoad(soc, savestate.size, savestate.data)) {
            cerr << "failed to restore savestate" << endl;
        }

        if (savestate.data) free(savestate.data);

        if (socSdInserted(soc)) {
            if (!socSdRemount(soc)) {
                cerr << "failed to remount SD card" << endl;
                sdCardReset();
            }
        } else if (sdCardInitialized()) {
            socSdInsert(soc);
        }

        AudioQueue* audioQueue = audioQueueCreate(AUDIO_QUEUE_SIZE);
        socSetAudioQueue(soc, audioQueue);

        MainLoop mainLoop(soc);
        mainLoop.SetCyclesPerSecondLimit(options.mips * 1000000);

        DeviceDisplayConfiguration displayConfiguration;
        deviceGetDisplayConfiguration(romInfo.GetDeviceType(), &displayConfiguration);

        Rotation rotation = Rotation::portrait_0;
        const int scale = options.smallWindow ? 1 : 2;

        SDL_Window* window = initSdl(displayConfiguration, scale, rotation);
        if (!window) {
            cerr << "failed to init SDL" << endl;
            return false;
        }

        auto sdlRenderer = make_unique<SdlRenderer>(window, soc, scale, rotation);
        SdlEventHandler sdlEventHandler(soc, scale, displayConfiguration, rotation);

        SdlAudioDriver audioDriver(soc, audioQueue);
        if (!options.disableAudio) audioDriver.Start();

        commands::Register();
        cli::Start(options.script);
        commands::Context commandContext{
            .soc = soc, .mainLoop = mainLoop, .audioDriver = audioDriver, .rotation = rotation};

        uint64_t lastSpeedDump = timestampUsec();

        while (true) {
            uint64_t now = timestampUsec();

            if (!options.disableAudio) socSetPcmSuspended(soc, audioDriver.GetAudioBackpressure());

            mainLoop.Cycle(now);

            sdlRenderer->Draw(sdlEventHandler.RedrawRequested());
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
                  << static_cast<float>(currentIpsMax) / 1000000 << " MIPS -> "
                  << (100 * currentIps) / currentIpsMax << "%" << endl
                  << flush;

                SDL_SetWindowTitle(window, s.str().c_str());
            }

            const int64_t timesliceRemaining =
                mainLoop.GetTimesliceSizeUsec() - static_cast<int64_t>(timestampUsec() - now);

            if (cli::Execute(&commandContext)) break;

            if (commandContext.rotation != rotation) {
                rotation = commandContext.rotation;

                sdlRenderer.reset();
                sdlResizeWindow(window, displayConfiguration, scale, rotation);

                sdlRenderer = make_unique<SdlRenderer>(window, soc, scale, rotation);
                sdlEventHandler.SetRotation(rotation);

                socSetFramebufferDirty(soc);
            }

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

    program.add_argument("--ram-size")
        .help("RAM size in MB (16 or 32)")
        .metavar("<size>")
        .scan<'u', unsigned int>();
    ;

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

    program.add_argument("--small-window")
        .help("use small window")
        .default_value(false)
        .implicit_value(true);

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
                       .script = program.present("--script"),
                       .ramSize = program.present<unsigned int>("--ram-size"),
                       .smallWindow = program.get<bool>("--small-window")};

    logEnable();

    if (!run(options)) exit(1);
}