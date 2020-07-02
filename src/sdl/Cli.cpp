#include "Cli.h"

// clang-format off
#include <cstdio>
#include <readline/readline.h>
#include <readline/history.h>
// clang-format on

#include <atomic>
#include <deque>
#include <functional>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>
#include <sstream>
#include <algorithm>
#include <cstring>

#include "EmCommon.h"

namespace {
    using Task = function<bool()>;
    using Cmd = function<bool(vector<string>)>;

    atomic<bool> stop(false);
    thread cliThread;

    Task task;
    mutex dispatchMutex;
    condition_variable cvExecuteTask;

    bool CmdQuit(vector<string> args) { return true; }

    bool CmdInstallFile(vector<string> args) {
        if (args.empty()) {
            cout << "usage: install <file> [file...]" << endl << flush;
            return false;
        }

        for (auto file : args) cout << "installing '" << file << "'..." << endl << flush;

        return false;
    }

    bool CmdInvalidCommand(vector<string> args) {
        cout << "invalid command" << endl << flush;
        return false;
    }

    struct Command {
        string name;
        Cmd cmd;
    };

    Command commands[] = {{.name = "quit", .cmd = CmdQuit},
                          {.name = "exit", .cmd = CmdQuit},
                          {.name = "install", .cmd = CmdInstallFile}};

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

                Dispatch(bind(CmdQuit, vector<string>()));

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

                Dispatch(bind(cmd, args));
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
            task = Task();
        }

        cvExecuteTask.notify_one();

        cliThread.join();

        task = Task();
    }

    bool Execute() {
        bool result = false;

        {
            unique_lock<mutex> lock(dispatchMutex);

            if (task) result = task();

            task = Task();
        }

        cvExecuteTask.notify_one();

        return result;
    }

}  // namespace Cli
