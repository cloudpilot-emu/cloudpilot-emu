#include <SDL.h>
#include <SDL_image.h>

#include <csignal>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <memory>
#include <optional>
#include <string>

#include "Cli.h"
#include "Commands.h"
#include "DebugSupport.h"
#include "Debugger.h"
#include "EmCommon.h"
#include "EmDevice.h"
#include "EmROMReader.h"
#include "EmSession.h"
#include "ExternalStorage.h"
#include "Feature.h"
#include "GdbStub.h"
#include "MainLoop.h"
#include "ProxyHandler.h"
#include "ScreenDimensions.h"
#include "SessionImage.h"
#include "SuspendContextClipboardCopy.h"
#include "SuspendContextClipboardPaste.h"
#include "SuspendManager.h"
#include "argparse.h"
#include "proxyClientNative.h"
#include "proxyClientWs.h"
#include "uri/uri.h"
#include "util.h"

using namespace std;

struct ProxyConfiguration {
    string host;
    unsigned int port;
    string path;
};

struct DebuggerConfiguration {
    bool enabled{false};
    unsigned int port{0};
    bool waitForAttach{false};
    optional<string> appFile;
};

struct Options {
    string image;
    optional<string> deviceId;
    bool netNative;
    optional<ProxyConfiguration> proxyConfiguration;
    optional<string> scriptFile;
    bool traceNetlib;
    bool traceDebugger;
    optional<string> mountImage;
    DebuggerConfiguration debuggerConfiguration;
};

void handleSuspend() {
    if (!SuspendManager::IsSuspended()) return;

    SuspendContext& context = SuspendManager::GetContext();

    switch (context.GetKind()) {
        case SuspendContext::Kind::clipboardCopy:
            SDL_SetClipboardText(context.AsContextClipboardCopy().GetClipboardContent());

            context.AsContextClipboardCopy().Resume();

            break;

        case SuspendContext::Kind::clipboardPaste: {
            const char* clipboardContent = SDL_GetClipboardText();

            context.AsContextClipboardPaste().Resume(clipboardContent ? clipboardContent : "");

            break;
        }

        default:
            break;
    }
}

void setupCard(const Options& options) {
    string imageKey;
    if (options.mountImage) imageKey = util::registerImage(*options.mountImage);

    if (!(options.deviceId ? util::initializeSession(options.image, *options.deviceId)
                           : util::initializeSession(options.image)))
        exit(1);

    if (!imageKey.empty() && gExternalStorage.RemountFailed()) {
        cout << "remount failed" << endl << flush;

        gExternalStorage.RemoveImage(imageKey);
        imageKey.clear();
    }

    if (!imageKey.empty() && util::mountKey(imageKey))
        cout << *options.mountImage << " mounted successfully" << endl << flush;
}

void setupProxy(ProxyClient*& proxyClient, ProxyHandler*& proxyHandler, const Options& options) {
    if (options.netNative) {
        proxyClient = proxyClientNative::Create();
    } else if (options.proxyConfiguration) {
        proxyClient = proxyClientWs::Create(options.proxyConfiguration->host,
                                            options.proxyConfiguration->port,
                                            options.proxyConfiguration->path);
    }

    if (proxyClient) {
        proxyHandler = new ProxyHandler(*proxyClient);
        proxyHandler->Initialize();

        Feature::SetNetworkRedirection(true);
    }
}

void setupDebugger(GdbStub& gdbStub, const Options& options) {
    if (options.debuggerConfiguration.enabled) {
        gdbStub.Listen();

        if (options.debuggerConfiguration.waitForAttach) {
            cout << "waiting for debugger to attach..." << endl << flush;
            while (gdbStub.GetConnectionState() == GdbStub::ConnectionState::listening)
                gdbStub.Cycle(1000);
        }

        if (options.debuggerConfiguration.appFile) {
            unique_ptr<uint8[]> buffer;
            size_t len;

            if (!util::ReadFile(*options.debuggerConfiguration.appFile, buffer, len))
                cout << "failed to read " << *options.debuggerConfiguration.appFile << endl
                     << flush;

            else
                debug_support::SetApp(buffer.get(), len, nullptr, gdbStub, gDebugger);
        }
    }
}

void initSDL(SDL_Window*& window, SDL_Renderer*& renderer, int& scale) {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
    IMG_Init(IMG_INIT_PNG);
    SDL_StartTextInput();

    ScreenDimensions::Kind screenDimensionsKind = gSession->GetDevice().GetScreenDimensions();
    ScreenDimensions screenDimensions(screenDimensionsKind);
    scale = screenDimensionsKind == ScreenDimensions::screen160x160 ? 3 : 2;

    if (SDL_CreateWindowAndRenderer(
            screenDimensions.Width() * scale,
            (screenDimensions.Height() + screenDimensions.SilkscreenHeight()) * scale, 0, &window,
            &renderer) != 0) {
        cerr << "unable to create SDL window: " << SDL_GetError() << endl;
        exit(1);
    }
}

void run(const Options& options) {
    srand(time(nullptr));
    signal(SIGPIPE, SIG_IGN);

    setupCard(options);

    ProxyClient* proxyClient = nullptr;
    ProxyHandler* proxyHandler = nullptr;
    setupProxy(proxyClient, proxyHandler, options);

    GdbStub gdbStub(gDebugger, options.debuggerConfiguration.port);
    setupDebugger(gdbStub, options);

    if (options.traceNetlib) logging::enableDomain(logging::domainNetlib);
    if (options.traceDebugger) logging::enableDomain(logging::domainDebugger);

    Feature::SetClipboardIntegration(true);

    SDL_Window* window;
    SDL_Renderer* renderer;
    int scale;
    initSDL(window, renderer, scale);

    MainLoop mainLoop(window, renderer, scale);

    commands::Register();
    cli::Start(options.scriptFile);
    commands::Context commandContext = {.debugger = gDebugger, .gdbStub = gdbStub};

    while (mainLoop.IsRunning()) {
        mainLoop.Cycle();

        if (cli::Execute(&commandContext)) break;

        handleSuspend();
        if (proxyHandler) proxyHandler->HandleSuspend();

#ifdef ENABLE_DEBUGGER
        gdbStub.Cycle(gDebugger.IsStopped() ? 10 : 0);
#endif
    };

    cli::Stop();
    if (proxyHandler) proxyHandler->Teardown();

    SDL_Quit();
    IMG_Quit();
}

int main(int argc, const char** argv) {
    class bad_device_id : public exception {};

    argparse::ArgumentParser program("cloudpilot-emu");

    program.add_description("CloudpilotEmu is an emulator for dragonball-based PalmOS devices.");

    program.add_argument("image").help("image or ROM file").required();

    program.add_argument("--device-id", "-d")
        .help("specify device ID")
        .metavar("<device>")
        .action([](const string& value) -> string {
            for (auto& deviceId : util::SUPPORTED_DEVICES)
                if (value == deviceId) return deviceId;

            throw bad_device_id();
        });

    program.add_argument("--net-native")
        .help("enable native network")
        .default_value(false)
        .implicit_value(true);

    program.add_argument("--net-proxy")
        .help("enable network redirection via specified proxy URI")
        .metavar("<proxy URI>")
        .action([](const string& value) {
            try {
                uri parsed(value);

                string scheme = parsed.get_scheme();
                string host = parsed.get_host();
                unsigned int port = static_cast<unsigned int>(parsed.get_port());
                string path = parsed.get_path();

                if (scheme != "http") throw runtime_error("bad URI scheme - must be http");

                while (path.size() > 0 && path[path.size() - 1] == '/') path.pop_back();

                return ProxyConfiguration{host, port != 0 ? port : 80, path.empty() ? "" : path};
            } catch (const invalid_argument& e) {
                throw runtime_error("invalid proxy URI");
            }
        });

    program.add_argument("--trace-netlib")
        .help("trace network API")
        .default_value(false)
        .implicit_value(true);

    program.add_argument("--mount").metavar("<image file>").help("mount card image");

    program.add_argument("--script", "-s")
        .metavar("<script file>")
        .help("execute script on startup");

#ifdef ENABLE_DEBUGGER
    program.add_argument("--listen", "-l")
        .metavar("<port>")
        .help("listen for GDB on port")
        .scan<'u', unsigned int>();

    program.add_argument("--wait-for-attach")
        .help("wait for debugger on launch")
        .default_value(false)
        .implicit_value(true);

    program.add_argument("--trace-debugger")
        .help("trace gdb stub")
        .default_value(false)
        .implicit_value(true);

    program.add_argument("--debug-app")
        .metavar("<elf file>")
        .help("configure debugger to debug app (elf file)");
#endif

    try {
        program.parse_args(argc, argv);
    } catch (const bad_device_id& e) {
        cerr << "bad device ID; valid IDs are:" << endl;

        for (auto& deviceId : util::SUPPORTED_DEVICES) cerr << "  " << deviceId << endl;

        exit(1);
    } catch (const invalid_argument& e) {
        cerr << "invalid argument" << endl << endl;
        cerr << program;

        exit(1);
    } catch (const runtime_error& e) {
        cerr << e.what() << endl << endl;
        cerr << program;

        exit(1);
    }

    Options options{.traceDebugger = false};

    options.image = program.get("image");
    options.traceNetlib = program.get<bool>("--trace-netlib");
    options.mountImage = program.present("--mount");
    options.deviceId = program.present("--device-id");
    options.proxyConfiguration = program.present<ProxyConfiguration>("--net-proxy");
    options.netNative = program.get<bool>("--net-native");
    options.mountImage = program.present("--mount");
    options.scriptFile = program.present("--script");

#ifdef ENABLE_DEBUGGER
    if (auto port = program.present<unsigned int>("--listen"))
        options.debuggerConfiguration = {.enabled = true,
                                         .port = *port,
                                         .waitForAttach = program.get<bool>("--wait-for-attach")};

    options.traceDebugger = program.get<bool>("--trace-debugger");
    options.debuggerConfiguration.appFile = program.present("--debug-app");
#endif

    run(options);
}
