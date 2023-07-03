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
#include <functional>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <mutex>
#include <sstream>
#include <thread>
#include <vector>

#include "Commands.h"

namespace {
    atomic<bool> stop(false);
    thread cliThread;

    const cli::Command* command = nullptr;
    vector<string> arguments;

    mutex dispatchMutex;
    condition_variable cvExecuteTask;

    class CommandContextImpl : public cli::CommandContext {
       public:
        CommandContextImpl(const cli::TaskContext& taskContext, const cli::Command& command,
                           bool&(quit))
            : taskContext(taskContext), command(command), quit(quit) {}

        Debugger& GetDebugger() const override { return taskContext.debugger; }

        GdbStub& GetGdbStub() const override { return taskContext.gdbStub; }

        void PrintUsage() const override {
            cout << "usage: " << (command.usage ? command.usage : command.name) << endl << flush;
        }

        void RequestQuit() override { quit = true; }

       private:
        cli::TaskContext taskContext;
        const cli::Command& command;

        bool& quit;
    };

    vector<string> Split(const char* line) {
        istringstream iss(line);

        return vector<string>(istream_iterator<string>(iss), istream_iterator<string>());
    }

    extern "C" char* RlCompletionFunction(const char* word, int state) {
        static vector<string> words;
        if (state == 0) words = Split(rl_line_buffer);

        if ((words.size() > 1 || (words.size() == 1 && strlen(word) == 0)) && words[0] != "help")
            return rl_filename_completion_function(word, state);

        static vector<string> suggestions;

        if (state == 0) {
            suggestions = vector<string>(cli::commands.size());
            suggestions.clear();

            for (auto& command : cli::commands) {
                if (strstr(command.name, word) == command.name) {
                    suggestions.push_back(command.name);
                }
            }
        }

        if (state >= (int)suggestions.size()) return nullptr;

        char* suggestion = (char*)malloc(suggestions[state].size() + 1);
        strcpy(suggestion, suggestions[state].c_str());

        return suggestion;
    }

    extern "C" int ReadlineEventHook() {
        if (stop) {
            rl_done = 1;
        }

        return 0;
    }

    void Dispatch(const cli::Command* _command, vector<string>& _arguments) {
        unique_lock<mutex> lock(dispatchMutex);
        if (stop) return;

        command = _command;
        arguments = _arguments;

        while (command) cvExecuteTask.wait(lock);
    }

    void ThreadMain() {
        rl_event_hook = ReadlineEventHook;
        rl_completion_entry_function = RlCompletionFunction;

        while (!stop) {
            char* lineBuffer = readline("> ");
            if (!lineBuffer) {
                lineBuffer = reinterpret_cast<char*>(malloc(5));
                strcpy(lineBuffer, "quit");
            }

            add_history(lineBuffer);

            vector<string> words = Split(lineBuffer);

            if (!words.empty() && !stop) {
                vector<string> args(words.begin() + 1, words.end());
                const cli::Command* _command = cli::GetCommand(words[0]);

                if (!_command) cout << "invalid command" << endl << flush;
                Dispatch(_command, args);
            }

            free(lineBuffer);
        }
    }
}  // namespace

namespace cli {

    void Start() {
        if (cliThread.joinable()) return;

        stop = false;
        command = nullptr;

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

        command = nullptr;
    }

    bool Execute(const cli::TaskContext& taskContext) {
        bool quit = false;

        if (command) {
            unique_lock<mutex> lock(dispatchMutex);
            CommandContextImpl commandContext(taskContext, *command, quit);

            command->cmd(arguments, commandContext);

            command = nullptr;
        };

        cvExecuteTask.notify_one();

        return quit;
    }

}  // namespace cli
