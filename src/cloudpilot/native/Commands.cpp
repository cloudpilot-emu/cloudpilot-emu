#include "Commands.h"

#include <cstring>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <unordered_map>

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
using namespace std;

namespace {
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

    void CmdQuit(vector<string> args, cli::CommandContext& context) {
        if (args.size() > 0) return context.PrintUsage();

        context.RequestQuit();
    }

    void CmdInstallFile(vector<string> args, cli::CommandContext& context) {
        if (args.empty()) return context.PrintUsage();

        for (auto file : args) {
            cout << "installing '" << file << "'..." << endl << flush;
            InstallFile(file);
        }
    }

    void CmdSaveImage(vector<string> args, cli::CommandContext& context) {
        if (args.size() < 1 || args.size() > 2) return context.PrintUsage();

        EmHAL::Slot slot = util::mountedSlot();
        if (args.size() == 2 && slot == EmHAL::Slot::none) {
            cout << "no mounted card" << endl << flush;
            return;
        }

        cout << "saving session image to '" << args[0] << "'" << endl << flush;

        if (slot != EmHAL::Slot::none) {
            CardImage* cardImage = gExternalStorage.GetImageInSlot(slot);

            const string oldKey = gExternalStorage.GetImageKeyInSlot(slot);
            const string newKey =
                md5(cardImage->RawData(), CardImage::BLOCK_SIZE * cardImage->BlocksTotal());

            gExternalStorage.RekeyImage(oldKey, newKey);
        }

        if (args.size() == 2) {
            if (!SaveCard(args[1])) return;
        }

        SaveImage(args[0]);
    }

    void CmdSetUserName(vector<string> args, cli::CommandContext& context) {
        if (args.size() == 0) return context.PrintUsage();

        string username;

        for (size_t i = 0; i < args.size(); i++) {
            username += args[i];
            if (i < args.size() - 1) username += " ";
        }

        gSession->SetHotsyncUserName(username);
    }

    void CmdResetSoft(vector<string> args, cli::CommandContext& context) {
        if (args.size() > 0) return context.PrintUsage();

        gSession->Reset(EmSession::ResetType::soft);
    }

    void CmdResetHard(vector<string> args, cli::CommandContext& context) {
        if (args.size() > 0) return context.PrintUsage();

        gSession->Reset(EmSession::ResetType::hard);
    }

    void CmdResetNoext(vector<string> args, cli::CommandContext& context) {
        if (args.size() > 0) return context.PrintUsage();

        gSession->Reset(EmSession::ResetType::noext);
    }

    void CmdSwitchImage(vector<string> args, cli::CommandContext& context) {
        if (args.size() != 1) return context.PrintUsage();

        if (context.GetGdbStub().IsDebuggerConnected()) {
            cout << "this command is not available while a debugger is connected" << endl << flush;
            return;
        }

        gExternalStorage.Clear();
        util::initializeSession(args[0]);

        context.GetGdbStub().ClearRelocationOffset();
    }

    void CmdSaveBackup(vector<string> args, cli::CommandContext& context) {
        if (args.size() != 1) return context.PrintUsage();

        SaveBackup(args[0], false);
    }

    void CmdSaveBackupWithRom(vector<string> args, cli::CommandContext& context) {
        if (args.size() != 1) return context.PrintUsage();

        SaveBackup(args[0], true);
    }

    void CmdLaunch(vector<string> args, cli::CommandContext& context) {
        if (args.size() != 1) return context.PrintUsage();

        cout << (gSession->LaunchAppByName(args[0]) ? "app launched successfully" : "launch failed")
             << endl
             << flush;
    }

    void CmdUnmount(vector<string> args, cli::CommandContext& context) {
        if (args.size() > 0) return context.PrintUsage();

        EmHAL::Slot slot = util::mountedSlot();
        if (slot == EmHAL::Slot::none) {
            cout << "no mounted card" << endl << flush;
            return;
        }

        if (gExternalStorage.RemoveImage(gExternalStorage.GetImageKeyInSlot(slot)))
            cout << "card ejected successfully" << endl << flush;
        else
            cout << "failed to eject card" << endl << flush;
    }

    void CmdMount(vector<string> args, cli::CommandContext& context) {
        if (args.size() != 1) return context.PrintUsage();

        if (util::mountImage(args[0])) cout << args[0] << " mounted successfully" << endl << flush;
    }

    void CmdSaveCard(vector<string> args, cli::CommandContext& context) {
        if (args.size() != 1) return context.PrintUsage();

        EmHAL::Slot slot = util::mountedSlot();
        if (!gExternalStorage.IsMounted(slot)) {
            cout << "no mounted card" << endl << flush;
            return;
        }

        if (SaveCard(args[0])) cout << "successfully saved " << args[0] << endl << flush;
    }

    void CmdTrace(vector<string> args, cli::CommandContext& context) {
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
            return context.PrintUsage();
        }

        StackDump().FrameCount(frameCount).DumpFrames(includeStack).Dump();
    }

    void CmdLocate(vector<string> args, cli::CommandContext& context) {
        if (args.size() != 1) return context.PrintUsage();

        unique_ptr<uint8[]> buffer;
        size_t len;

        if (!util::readFile(args[0], buffer, len)) {
            cout << "failed to read " << args[0] << endl << flush;
            return;
        }

        debug_support::Locate(buffer.get(), len);
    }

    void CmdDebugSetApp(vector<string> args, cli::CommandContext& context) {
        if (args.size() != 1 && args.size() != 2) return context.PrintUsage();

        unique_ptr<uint8[]> buffer;
        size_t len;

        if (!util::readFile(args[0], buffer, len)) {
            cout << "failed to read " << args[0] << endl << flush;
            return;
        }

        debug_support::SetApp(buffer.get(), len, args.size() == 2 ? args[1].c_str() : nullptr,
                              context.GetGdbStub(), context.GetDebugger());
    }

    void CmdDebugInfo(vector<string> args, cli::CommandContext& context) {
        if (args.size() > 0) return context.PrintUsage();

        switch (context.GetDebugger().GetBreakMode()) {
            case Debugger::BreakMode::all:
                cout << "break mode: all" << endl;
                break;

            case Debugger::BreakMode::ramOnly:
                cout << "break mode: ram-only" << endl;
                break;

            case Debugger::BreakMode::appOnly:
                cout << "break mode: app-only: 0x" << hex << setw(8) << setfill('0')
                     << context.GetDebugger().GetAppStart() << " - 0x" << setw(8) << setfill('0')
                     << (context.GetDebugger().GetAppStart() + context.GetDebugger().GetAppSize())
                     << endl;
                break;
        }

        if (context.GetGdbStub().IsDebuggerConnected())
            cout << "debugger connected" << endl << flush;
        else
            cout << "debugger disconnected" << endl << flush;
    }

    void CmdDebugSetBreakMode(vector<string> args, cli::CommandContext& context) {
        string modeStr = args.size() == 1 ? args[0] : "";

        if (modeStr == "all")
            context.GetDebugger().SetBreakMode(Debugger::BreakMode::all);
        else if (modeStr == "app-only")
            context.GetDebugger().SetBreakMode(Debugger::BreakMode::appOnly);
        else if (modeStr == "ram-only")
            context.GetDebugger().SetBreakMode(Debugger::BreakMode::ramOnly);
        else
            context.PrintUsage();
    }

    void CmdDumpMemory(vector<string> args, cli::CommandContext& context) {
        if (args.size() != 1) return context.PrintUsage();

        DumpMemory(args[0]);
    }

    void CmdShowSyscallTraps(vector<string> args, cli::CommandContext& context) {
        if (args.size() > 0) return context.PrintUsage();

        cout << "syscall traps:" << endl << flush;

        for (const uint16 trapWord : gDebugger.GetSyscallTraps())
            cout << "  0x" << hex << setw(4) << setfill('0') << trapWord << dec << endl;

        cout << flush;
    }

    void CmdSetSyscallTrap(vector<string> args, cli::CommandContext& context) {
        if (args.size() != 1) return context.PrintUsage();

        uint32 trapWord;
        istringstream sstream;
        if (args[0].size() > 2 && args[0].substr(0, 2) == "0x") {
            sstream = istringstream(args[0].substr(2));
            sstream >> hex >> trapWord;
        } else {
            sstream = istringstream(args[0]);
            sstream >> trapWord;
        }

        if (sstream.fail() || !sstream.eof() || trapWord > 0xffff) {
            cout << "invalid trap word" << endl << flush;
            return;
        }

        gDebugger.SetSyscallTrap(trapWord);
    }

    void CmdClearSyscallTrap(vector<string> args, cli::CommandContext& context) {
        if (args.size() != 1) context.PrintUsage();

        uint32 trapWord;
        istringstream sstream;
        if (args[0].size() > 2 && args[0].substr(0, 2) == "0x") {
            sstream = istringstream(args[0].substr(2));
            sstream >> hex >> trapWord;
        } else {
            sstream = istringstream(args[0]);
            sstream >> trapWord;
        }

        if (sstream.fail() || !sstream.eof() || trapWord > 0xffff) {
            cout << "invalid trap word" << endl << flush;
            return;
        }

        gDebugger.ClearSyscallTrap(trapWord);
    }

    void CmdClearAllSyscallTraps(vector<string> args, cli::CommandContext& context) {
        if (args.size() > 0) return context.PrintUsage();

        gDebugger.ClearAllSyscallTraps();
    }

    void CmdHelp(vector<string> args, cli::CommandContext& context) {
        if (args.size() > 1) return context.PrintUsage();

        cout << endl;

        const cli::Command* command = nullptr;
        if (args.size() == 1) {
            command = cli::GetCommand(args[0]);
            if (!command) cout << "invalid command: " << args[0] << endl;
        }

        if (command) {
            cout << "usage: " << (command->usage ? command->usage : command->name) << endl;

            if (command->help) {
                const char* help = command->help;
                if (help[0] == '\r') help++;
                if (help[0] == '\n') help++;

                cout << endl << help << endl;
            } else if (command->description) {
                cout << endl << command->description << endl;
            }
        } else {
            cout << "available commands:" << endl << endl;

            for (auto& command : cli::commands) {
                const char* usage = command.usage ? command.usage : command.name;

                cout << left << setw(50) << usage;

                if (!command.description) {
                    cout << endl;
                    continue;
                }

                cout << command.description << endl;
            }
        }

        cout << endl << flush;
    }
}  // namespace

const vector<cli::Command> cli::commands({
    {.name = "help", .description = "Show help.", .cmd = CmdHelp},
    {.name = "quit", .description = "Quit CloudpilotEmu.", .cmd = CmdQuit},
    {.name = "exit", .description = "Quit CloudpilotEmu.", .cmd = CmdQuit},
    {
        .name = "install",
        .usage = "install <file> [file...]",
        .description = "Install databases.",
        .cmd = CmdInstallFile,
    },
    {.name = "set-user-name",
     .usage = "set-user-name <hotsync user name>",
     .description = "Set PalmOS user name.",
     .cmd = CmdSetUserName},
    {.name = "reset-soft", .description = "Perform a soft reset.", .cmd = CmdResetSoft},
    {.name = "reset-hard", .description = "Perform a hard reset.", .cmd = CmdResetHard},
    {.name = "reset-noext",
     .description = "Perform a reset without loading extensions.",
     .cmd = CmdResetNoext},
    {.name = "save-image",
     .usage = "save-image <image_file> [card_image_file]",
     .description = "Save image file.",
     .help = R"HELP(
Write a image (save state) file to disk. A file for writing an image of a
mounted card can be specified as second argument. If save state and card
image are written simultaneously, CloudpilotEmu will be able to restore the
mounted image on load (if the image is specified with --mount on launch).)HELP",
     .cmd = CmdSaveImage},
    {.name = "switch-image",
     .usage = "switch-image <file>",
     .description = "Switch to another ROM or image.",
     .help = R"HELP(
Switch emulator to another ROM or image file. The resolution of the new device
must match the current device - otherwise, the image will not display.)HELP",
     .cmd = CmdSwitchImage},
    {.name = "save-backup",
     .usage = "save-backup <file>",
     .description = "Save backup (RAM databases only).",
     .cmd = CmdSaveBackup},
    {.name = "save-backup-with-rom",
     .usage = "save-backup-with-rom <file>",
     .description = "Save backup (including ROM databases).",
     .cmd = CmdSaveBackupWithRom},
    {.name = "launch",
     .usage = "launch <database>",
     .description = "Launch application.",
     .cmd = CmdLaunch},
    {.name = "unmount", .description = "Unmount card.", .cmd = CmdUnmount},
    {.name = "mount",
     .usage = "mount <image>",
     .description = "Mount card image.",
     .cmd = CmdMount},
    {.name = "save-card",
     .usage = "save-card <image>",
     .description = "Save card image.",
     .cmd = CmdSaveCard},
    {.name = "trace",
     .usage = "trace [number of frames] [stack|nostack]",
     .description = "Print m68k stack trace.",
     .help = "Print m68k stack trace, using macbug function names if available.",
     .cmd = CmdTrace},
    {.name = "locate",
     .usage = "locate <file>",
     .description = "Locate file contents in RAM.",
     .cmd = CmdLocate},
#ifdef ENABLE_DEBUGGER
    {.name = "debug-set-app",
     .usage = "debug-set-app <file> [db name]",
     .description = "Configure debugger for app.",
     .help = R"HELP(
Load an ELF file and try to locate .text in PalmOS memory. If successful, this
information is used to configure GDB for debugging the application built from
this ELF when GDB connects. Specifying the application database will limit
the search to the corresponding code resource. Break mode will be configured
to app only.)HELP",
     .cmd = CmdDebugSetApp},
    {.name = "debug-info", .description = "Show break mode.", .cmd = CmdDebugInfo},
    {.name = "debug-set-break-mode",
     .usage = "debug-set-break_mode <all|app-only|ram-only>",
     .description = "Set break mode.",
     .cmd = CmdDebugSetBreakMode},
    {.name = "dump-memory",
     .usage = "dump-memory <file>",
     .description = "Dump PalmOS memory to disk",
     .cmd = CmdDumpMemory},
    {.name = "show-syscall-traps",
     .description = "Show all registered syscall traps.",
     .cmd = CmdShowSyscallTraps},
    {.name = "set-syscall-trap",
     .usage = "set-syscall-trap <trap>",
     .description = "Set syscall trap.",
     .cmd = CmdSetSyscallTrap},
    {.name = "clear-syscall-trap",
     .usage = "clear-syscall-trap <trap>",
     .description = "Remove syscall trap.",
     .cmd = CmdClearSyscallTrap},
    {.name = "clear-all-syscall-traps",
     .description = "Remove all registered syscall traps.",
     .cmd = CmdClearAllSyscallTraps},
#endif
});

const cli::Command* cli::GetCommand(const string& name) {
    static unordered_map<string, const Command&> commandMap;

    if (commandMap.size() == 0) {
        for (auto& command : cli::commands) commandMap.insert({string(command.name), command});
    }

    auto it = commandMap.find(name);
    return (it == commandMap.end()) ? nullptr : &(it->second);
}
