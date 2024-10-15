#include <SDL.h>
#include <SDL_image.h>
#include <unistd.h>

#include <cstring>
#include <iomanip>
#include <iostream>
#include <memory>
#include <optional>
#include <sstream>

#include "FileUtil.h"
#include "MainLoop.h"
#include "SdlAudioDriver.h"
#include "SdlEventHandler.h"
#include "SdlRenderer.h"
#include "SoC.h"
#include "argparse.h"
#include "audio_queue.h"
#include "cputil.h"
#include "device.h"
#include "sdcard.h"

using namespace std;

struct Options {
    string nor;
    optional<string> nand;
    optional<string> sd;
    optional<unsigned int> gdbPort;
    unsigned int mips;
    bool disableAudio;
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

    bool readFile(const optional<string>& name, unique_ptr<uint8_t[]>& buffer, size_t& size) {
        if (!name) return true;

        if (!util::ReadFile(*name, buffer, size)) {
            cerr << "unabled to read " << *name << endl;
            return false;
        }

        return true;
    }

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

    bool run(const Options& options) {
        if (options.mips == 0) {
            cerr << "MIPS must be finite" << endl;
            return false;
        }

        size_t norLen{0};
        unique_ptr<uint8_t[]> norData;
        if (!readFile(options.nor, norData, norLen)) return false;

        size_t nandLen{0};
        unique_ptr<uint8_t[]> nandData;
        if (!readFile(options.nand, nandData, nandLen)) return false;

        if (!nandData) {
            nandData = make_unique<uint8_t[]>(NAND_SIZE);
            memset(nandData.get(), 0xff, NAND_SIZE);
            nandLen = NAND_SIZE;
        }

        if (nandLen != NAND_SIZE) {
            cerr << "invalid NAND size; expected " << NAND_SIZE << " bytes" << endl;
            return false;
        }

        size_t sdLen{0};
        unique_ptr<uint8_t[]> sdData;
        if (!readFile(options.sd, sdData, sdLen)) return false;

        if (sdData) {
            if (sdLen % SD_SECTOR_SIZE) {
                cout << "sd card image has bad size" << endl;
                return false;
            }

            sdCardInitializeWithData(sdLen / SD_SECTOR_SIZE, sdData.get());
        }

        SoC* soc = socInit(norData.get(), norLen, sdCardSectorCount(), sdCardRead, sdCardWrite,
                           nandData.get(), nandLen, options.gdbPort.value_or(0), deviceGetSocRev());

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

        uint64_t lastSpeedDump = timestampUsec();

        while (true) {
            uint64_t now = timestampUsec();

            if (!options.disableAudio) socSetPcmSuspended(soc, audioDriver.GetAudioBackpressure());

            mainLoop.Cycle(now);

            sdlRenderer.Draw(sdlEventHandler.RedrawRequested());
            sdlEventHandler.ClearRedrawRequested();

            sdlEventHandler.HandleEvents();

            if (now - lastSpeedDump > 1000000) {
                const uint64_t currentIps = mainLoop.GetCurrentIps();
                const uint64_t currentIpsMax = mainLoop.GetCurrentIpsMax();
                lastSpeedDump = now;

                ostringstream s;
                s << "cp-uarm running @ " << fixed << setprecision(2)
                  << static_cast<float>(currentIps) / 1000000 << " MIPS, limit "
                  << static_cast<float>(currentIpsMax) / 1000000 << " IPS -> "
                  << (100 * currentIps) / currentIpsMax << "%" << endl
                  << flush;

                SDL_SetWindowTitle(window, s.str().c_str());
            }

            const int64_t timesliceRemaining =
                mainLoop.GetTimesliceSizeUsec() - static_cast<int64_t>(timestampUsec() - now);

            if (timesliceRemaining > 10) usleep(timesliceRemaining);
        }

        return true;
    }

}  // namespace

int main(int argc, const char** argv) {
    argparse::ArgumentParser program("cp-uarm");

    program.add_description("cp-uarm emulates a Palm Tungsten E2");

    program.add_argument("nor").help("NOR rom file").required();

    program.add_argument("--nand", "-n").help("NAND rom file").metavar("<nand file>");

    program.add_argument("--sd", "-s").help("SD card file");

    program.add_argument("--no-sound", "-q")
        .help("disable audio")
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

    Options options = {.nor = program.get("nor"),
                       .nand = program.present("--nand"),
                       .sd = program.present("--sd"),
                       .gdbPort = program.present<unsigned int>("--gdb"),
                       .disableAudio = program.get<bool>("--no-sound"),
                       .mips = program.get<unsigned int>("--mips")};

    if (!run(options)) exit(1);
}