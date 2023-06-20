#include "Cli.h"

// clang-format off
#include <cstdio>
#include <readline/readline.h>
#include <readline/history.h>
// clang-format on

#include <algorithm>
#include <atomic>
#include <condition_variable>
#include <cstring>
#include <deque>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <mutex>
#include <sstream>
#include <thread>
#include <vector>

#include "DbBackup.h"
#include "DbInstaller.h"
#include "DebugSupport.h"
#include "Debugger.h"
#include "EmBankSRAM.h"
#include "EmCommon.h"
#include "EmErrCodes.h"
#include "EmHAL.h"
#include "EmMemory.h"
#include "EmSession.h"
#include "ExternalStorage.h"
#include "SessionImage.h"
#include "StackDump.h"
#include "ZipfileWalker.h"
#include "md5.h"
#include "util.h"

namespace {
    using Task = function<bool(const Cli::TaskContext& context)>;
    using Cmd = function<bool(vector<string>, const Cli::TaskContext& context)>;

    atomic<bool> stop(false);
    thread cliThread;

    Task task;
    mutex dispatchMutex;
    condition_variable cvExecuteTask;

    string translateInstallResult(DbInstaller::Result result) {
        switch (result) {
            case DbInstaller::Result::success:
                return "installation successful";

            case DbInstaller::Result::needsReboot:
                return "installation successful; device requires reset";

            case DbInstaller::Result::failureNotEnoughMemory:
                return "installation failed: not enough memory";

            case DbInstaller::Result::failureInternal:
                return "installation failed: internal error in PalmOS";

            case DbInstaller::Result::failureDbIsOpen:
                return "installation failed: DB is open";

            case DbInstaller::Result::failureDbIsCorrupt:
                return "installation failed: database file corrup";

            case DbInstaller::Result::failedCouldNotOverwrite:
                return "installation failed: could not overwrite existing DB";

            default:
                return "installation failed for unknown reason";
        }
    }

    void InstallOne(size_t len, uint8* buffer) {
        DbInstaller::Result installationResult = DbInstaller::Install(len, buffer);

        cout << translateInstallResult(installationResult) << endl << flush;
    }

    void InstallFile(string path) {
        unique_ptr<uint8[]> buffer;
        size_t len;

        if (!util::readFile(path, buffer, len)) {
            cout << "failed to read " << path << endl << flush;
            return;
        }

        if (path.length() >= 4 && (path.substr(path.length() - 4) == ".zip" ||
                                   path.substr(path.length() - 4) == ".ZIP")) {
            ZipfileWalker walker(len, buffer.get());

            while (walker.GetState() == ZipfileWalker::State::stateOpen) {
                uint8* content = walker.GetCurrentEntryContent();

                if (content) {
                    cout << "installing " << walker.GetCurrentEntryName() << "... ";
                    InstallOne(walker.GetCurrentEntrySize(), content);
                }

                walker.Next();
            }
        } else {
            InstallOne(len, buffer.get());
        }
    }

    void SaveImage(string file) {
        EmAssert(gSession);

        SessionImage image;
        if (!gSession->SaveImage(image)) {
            cout << "failed to serialize session image" << endl << flush;
            return;
        }

        fstream stream(file, ios_base::out);

        if (stream.fail()) {
            cout << "failed to open " << file << endl << flush;
            return;
        }

        stream.write((const char*)image.GetSerializedImage(), image.GetSerializedImageSize());

        if (stream.fail()) {
            cout << "I/O error writing " << file << endl << flush;
        }
    }

    void DumpMemory(string file) {
        const uint32 size = EmMemory::GetRegionSize(MemoryRegion::ram);
        unique_ptr<uint8[]> memory = make_unique<uint8[]>(size);

        for (uint32 i = 0; i < size; i++) memory[i] = EmMemGet8(gMemoryStart + i);

        fstream stream(file, ios_base::out);

        if (stream.fail()) {
            cout << "failed to open " << file << endl << flush;
            return;
        }

        stream.write(reinterpret_cast<const char*>(memory.get()), size);

        if (stream.fail()) {
            cout << "I/O error writing " << file << endl << flush;
        }
    }

    void SaveBackup(string file, bool includeRomDatabases) {
        unique_ptr<DbBackup> backup = DbBackup::create();

        if (!backup->Init(includeRomDatabases)) {
            cout << "backup failed" << endl << flush;

            return;
        }

        while (backup->IsInProgress()) {
            cout << "backing up " << backup->GetCurrentDatabase() << " ... ";

            if (backup->Save()) {
                cout << "success" << endl << flush;
            } else {
                cout << "failed" << endl << flush;
            }
        }

        auto [archiveSize, archive] = backup->GetArchive();

        if (archiveSize <= 0) return;

        cout << "saving backup to " << file << endl << flush;

        fstream stream(file, ios_base::out);

        if (stream.fail()) {
            cout << "failed to open " << file << endl << flush;
            return;
        }

        stream.write((const char*)archive, archiveSize);

        if (stream.fail()) {
            cout << "I/O error writing " << file << endl << flush;
        }
    }

    bool SaveCard(const string file) {
        EmHAL::Slot slot = util::mountedSlot();
        if (slot == EmHAL::Slot::none) return false;

        auto image = gExternalStorage.GetImageInSlot(slot);

        fstream stream(file, ios_base::out);

        if (stream.fail()) {
            cout << "failed to open " << file << endl << flush;
            return false;
        }

        stream.write((const char*)image->RawData(), image->BlocksTotal() * CardImage::BLOCK_SIZE);

        if (stream.fail()) {
            cout << "I/O error writing " << file << endl << flush;
            return false;
        }

        return true;
    }

    bool CmdQuit(vector<string> args, const Cli::TaskContext& context) { return true; }

    bool CmdInstallFile(vector<string> args, const Cli::TaskContext& context) {
        if (args.empty()) {
            cout << "usage: install <file> [file...]" << endl << flush;
            return false;
        }

        for (auto file : args) {
            cout << "installing '" << file << "'..." << endl << flush;
            InstallFile(file);
        }

        return false;
    }

    bool CmdSaveImage(vector<string> args, const Cli::TaskContext& context) {
        if (args.size() < 1 || args.size() > 2) {
            cout << "usage: save-image <image_file> [card_image_file]" << endl << flush;
            return false;
        }

        EmHAL::Slot slot = util::mountedSlot();
        if (args.size() == 2 && slot == EmHAL::Slot::none) {
            cout << "no mounted card" << endl << flush;
            return false;
        }

        cout << "saving session image to '" << args[0] << "'" << endl << flush;

        if (args.size() == 2) {
            if (!SaveCard(args[1])) return false;
            cout << "successfully saved card image to " << args[1] << endl << flush;

            const string oldKey = gExternalStorage.GetImageKeyInSlot(slot);
            CardImage* cardImage = gExternalStorage.GetImageInSlot(slot);
            const string newKey =
                md5(cardImage->RawData(), CardImage::BLOCK_SIZE * cardImage->BlocksTotal());

            gExternalStorage.RekeyImage(oldKey, newKey);
        }

        SaveImage(args[0]);

        return false;
    }

    bool CmdRandomSeed(vector<string> args, const Cli::TaskContext& context) {
        if (args.size() > 0) {
            cout << "usage: random-seed" << endl << flush;
            return false;
        }

        cout << "current random seed: " << gSession->GetRandomSeed() << endl << flush;

        return false;
    }

    bool CmdSetUserName(vector<string> args, const Cli::TaskContext& context) {
        if (args.size() == 0) {
            cout << "usage: set-user-name <hotsync user name>";
            return false;
        }

        string username;

        for (size_t i = 0; i < args.size(); i++) {
            username += args[i];
            if (i < args.size() - 1) username += " ";
        }

        gSession->SetHotsyncUserName(username);

        return false;
    }

    bool CmdResetSoft(vector<string> args, const Cli::TaskContext& context) {
        if (args.size() > 0) {
            cout << "usage: reset-soft" << endl << flush;
            return false;
        }

        gSession->Reset(EmSession::ResetType::soft);

        return false;
    }

    bool CmdResetHard(vector<string> args, const Cli::TaskContext& context) {
        if (args.size() > 0) {
            cout << "usage: reset-hard" << endl << flush;
            return false;
        }

        gSession->Reset(EmSession::ResetType::hard);

        return false;
    }

    bool CmdResetNoext(vector<string> args, const Cli::TaskContext& context) {
        if (args.size() > 0) {
            cout << "usage: reset-noext" << endl << flush;
            return false;
        }

        gSession->Reset(EmSession::ResetType::noext);

        return false;
    }

    bool CmdInvalidCommand(vector<string> args, const Cli::TaskContext& context) {
        cout << "invalid command" << endl << flush;
        return false;
    }

    bool CmdSwitchImage(vector<string> args, const Cli::TaskContext& context) {
        if (context.gdbStub.IsDebuggerConnected()) {
            cout << "this command is not available while a debugger is connected" << endl << flush;
            return false;
        }

        if (args.size() != 1) {
            cout << "usage: switch-image <file>" << endl << flush;
            return false;
        }

        gExternalStorage.Clear();
        util::initializeSession(args[0]);

        context.gdbStub.ClearRelocationOffset();

        return false;
    }

    bool CmdSaveBackup(vector<string> args, const Cli::TaskContext& context) {
        if (args.size() != 1) {
            cout << "usage: save-backup <file>" << endl << flush;
            return false;
        }

        SaveBackup(args[0], false);

        return false;
    }

    bool CmdSaveBackupWithRom(vector<string> args, const Cli::TaskContext& context) {
        if (args.size() != 1) {
            cout << "usage: save-backup-with-rom <file>" << endl << flush;
            return false;
        }

        SaveBackup(args[0], true);

        return false;
    }

    bool CmdLaunch(vector<string> args, const Cli::TaskContext& context) {
        if (args.size() != 1) {
            cout << "usage: launch <database>" << endl << flush;
            return false;
        }

        cout << (gSession->LaunchAppByName(args[0]) ? "app launched successfully" : "launch failed")
             << endl
             << flush;

        return false;
    }

    bool CmdUnmount(vector<string> args, const Cli::TaskContext& context) {
        if (args.size() > 0) {
            cout << "usage: unmount <image>" << endl << flush;
            return false;
        }

        EmHAL::Slot slot = util::mountedSlot();
        if (slot == EmHAL::Slot::none) {
            cout << "no mounted card" << endl << flush;
            return false;
        }

        if (gExternalStorage.RemoveImage(gExternalStorage.GetImageKeyInSlot(slot)))
            cout << "card ejected successfully" << endl << flush;
        else
            cout << "failed to eject card" << endl << flush;

        return false;
    }

    bool CmdMount(vector<string> args, const Cli::TaskContext& context) {
        if (args.size() != 1) {
            cout << "usage: mount <image>" << endl << flush;
            return false;
        }

        if (util::mountImage(args[0])) cout << args[0] << " mounted successfully" << endl << flush;

        return false;
    }

    bool CmdSaveCard(vector<string> args, const Cli::TaskContext& context) {
        if (args.size() != 1) {
            cout << "usage: save-card <image>" << endl << flush;
            return false;
        }

        EmHAL::Slot slot = util::mountedSlot();
        if (!gExternalStorage.IsMounted(slot)) {
            cout << "no mounted card" << endl << flush;
            return false;
        }

        if (SaveCard(args[0])) cout << "successfully saved " << args[0] << endl << flush;

        return false;
    }

    bool CmdTrace(vector<string> args, const Cli::TaskContext& context) {
        int frameCount{3};
        bool includeStack{false};

        try {
            if (args.size() > 2 ||
                (args.size() == 2 && args[1] != "stack" && args[1] != "nostack")) {
                throw invalid_argument("bad argument list");
            }

            if (args.size() > 0) frameCount = stoi(args[0]);
            if (frameCount <= 0) throw invalid_argument("invalid frame count");

            includeStack = args.size() == 2 && args[1] == "stack";
        } catch (exception&) {
            cout << "usage: trace [number of frames] [stack|nostack]" << endl << flush;
            return false;
        }

        StackDump().FrameCount(frameCount).DumpFrames(includeStack).Dump();

        return false;
    }

    bool CmdLocate(vector<string> args, const Cli::TaskContext& context) {
        if (args.size() != 1) {
            cout << "usage: locate <file>" << endl << flush;
            return false;
        }

        unique_ptr<uint8[]> buffer;
        size_t len;

        if (!util::readFile(args[0], buffer, len)) {
            cout << "failed to read " << args[0] << endl << flush;
            return false;
        }

        debug_support::Locate(buffer.get(), len);

        return false;
    }

    bool CmdDebugSetApp(vector<string> args, const Cli::TaskContext& context) {
        if (args.size() != 1 && args.size() != 2) {
            cout << "usage: debug-set-app <file> [db name]" << endl << flush;
            return false;
        }

        unique_ptr<uint8[]> buffer;
        size_t len;

        if (!util::readFile(args[0], buffer, len)) {
            cout << "failed to read " << args[0] << endl << flush;
            return false;
        }

        debug_support::SetApp(buffer.get(), len, args.size() == 2 ? args[1].c_str() : nullptr,
                              context.gdbStub, context.debugger);

        return false;
    }

    bool CmdDebugInfo(vector<string> args, const Cli::TaskContext& context) {
        switch (context.debugger.GetBreakMode()) {
            case Debugger::BreakMode::all:
                cout << "break mode: all" << endl;
                break;

            case Debugger::BreakMode::ramOnly:
                cout << "break mode: ram-only" << endl;
                break;

            case Debugger::BreakMode::appOnly:
                cout << "break mode: app-only: 0x" << hex << setw(8) << setfill('0')
                     << context.debugger.GetAppStart() << " - 0x" << setw(8) << setfill('0')
                     << (context.debugger.GetAppStart() + context.debugger.GetAppSize()) << endl;
                break;
        }

        if (context.gdbStub.IsDebuggerConnected())
            cout << "debugger connected" << endl << flush;
        else
            cout << "debugger disconnected" << endl << flush;

        return false;
    }

    bool CmdDebugSetBreakMode(vector<string> args, const Cli::TaskContext& context) {
        string modeStr = args.size() == 1 ? args[0] : "";

        if (modeStr == "all")
            context.debugger.SetBreakMode(Debugger::BreakMode::all);
        else if (modeStr == "app-only")
            context.debugger.SetBreakMode(Debugger::BreakMode::appOnly);
        else if (modeStr == "ram-only")
            context.debugger.SetBreakMode(Debugger::BreakMode::ramOnly);
        else
            cout << "usage: debug-set-break_mode <all|app-only|ram-only>" << endl << flush;

        return false;
    }

    bool CmdDumpMemory(vector<string> args, const Cli::TaskContext& context) {
        if (args.size() != 1) {
            cout << "usage: dump-memory <file>" << endl << flush;
            return false;
        }

        DumpMemory(args[0]);

        return false;
    }

    struct Command {
        string name;
        Cmd cmd;
    };

    Command commands[] = {{.name = "quit", .cmd = CmdQuit},
                          {.name = "exit", .cmd = CmdQuit},
                          {.name = "install", .cmd = CmdInstallFile},
                          {.name = "random-seed", .cmd = CmdRandomSeed},
                          {.name = "set-user-name", .cmd = CmdSetUserName},
                          {.name = "reset-soft", .cmd = CmdResetSoft},
                          {.name = "reset-hard", .cmd = CmdResetHard},
                          {.name = "reset-noext", .cmd = CmdResetNoext},
                          {.name = "save-image", .cmd = CmdSaveImage},
                          {.name = "switch-image", .cmd = CmdSwitchImage},
                          {.name = "save-backup", .cmd = CmdSaveBackup},
                          {.name = "save-backup-with-rom", .cmd = CmdSaveBackupWithRom},
                          {.name = "launch", .cmd = CmdLaunch},
                          {.name = "unmount", .cmd = CmdUnmount},
                          {.name = "mount", .cmd = CmdMount},
                          {.name = "save-card", .cmd = CmdSaveCard},
                          {.name = "trace", .cmd = CmdTrace},
                          {.name = "locate", .cmd = CmdLocate},
                          {.name = "debug-set-app", .cmd = CmdDebugSetApp},
                          {.name = "debug-info", .cmd = CmdDebugInfo},
                          {.name = "debug-set-break-mode", .cmd = CmdDebugSetBreakMode},
                          {.name = "dump-memory", .cmd = CmdDumpMemory}};

    vector<string> Split(const char* line) {
        istringstream iss(line);

        return vector<string>(istream_iterator<string>(iss), istream_iterator<string>());
    }

    char* RlCompletionFunction(const char* word, int state) {
        static vector<string> words;
        if (state == 0) words = Split(rl_line_buffer);

        if (words.size() > 1 || (words.size() == 1 && strlen(word) == 0))
            return rl_filename_completion_function(word, state);

        static vector<string> suggestions;

        if (state == 0) {
            const int cmdCount = sizeof(commands) / sizeof(commands[0]);

            suggestions = vector<string>(cmdCount);
            suggestions.clear();

            for (auto& command : commands) {
                if (strstr(command.name.c_str(), word) == command.name.c_str()) {
                    suggestions.push_back(command.name);
                }
            }
        }

        if (state >= (int)suggestions.size()) return nullptr;

        char* suggestion = (char*)malloc(suggestions[state].size() + 1);
        strcpy(suggestion, suggestions[state].c_str());

        return suggestion;
    }

    int ReadlineEventHook() {
        if (stop) {
            rl_done = 1;
        }

        return 0;
    }

    void Dispatch(Task t) {
        unique_lock<mutex> lock(dispatchMutex);
        if (stop) return;

        task = t;

        while (task) cvExecuteTask.wait(lock);
    }

    void ThreadMain() {
        rl_event_hook = ReadlineEventHook;
        rl_completion_entry_function = RlCompletionFunction;

        while (!stop) {
            char* lineBuffer = readline("> ");

            if (!lineBuffer) {
                cout << endl << flush;

                Dispatch(bind(CmdQuit, vector<string>(), std::placeholders::_1));

                continue;
            }

            add_history(lineBuffer);

            vector<string> words = Split(lineBuffer);

            if (!words.empty() && !stop) {
                vector<string> args(words.begin() + 1, words.end());
                Cmd cmd = CmdInvalidCommand;

                for (auto& command : commands)
                    if (command.name == words[0]) {
                        cmd = command.cmd;
                        break;
                    }

                Dispatch(bind(cmd, args, std::placeholders::_1));
            }

            free(lineBuffer);
        }
    }
}  // namespace

namespace Cli {

    void Start() {
        if (cliThread.joinable()) return;

        stop = false;
        task = Task();

        cliThread = thread(ThreadMain);
    }

    void Stop() {
        if (!cliThread.joinable()) return;

        {
            unique_lock<mutex> lock(dispatchMutex);

            stop = true;
        }

        cvExecuteTask.notify_one();

        cliThread.join();

        task = Task();
    }

    bool Execute(const Cli::TaskContext& context) {
        bool result = false;

        {
            unique_lock<mutex> lock(dispatchMutex);

            if (task) result = task(context);

            task = Task();
        }

        cvExecuteTask.notify_one();

        return result;
    }

}  // namespace Cli
