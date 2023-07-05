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

    inline bool IsBreakChar(char token) { return token == ' ' || token == '\t'; }

    string Quote(const char* text) {
        string quotedText;

        while (*text != '\0') {
            const char token = *(text++);

            if (IsBreakChar(token) || token == '\\') quotedText += '\\';
            quotedText += token;
        }

        return quotedText;
    }

    string Dequote(const char* text) {
        string dequotedText;
        bool isQuote = false;

        while (*text != '\0') {
            const char token = *(text++);

            if (!isQuote && token == '\\') {
                isQuote = true;
                continue;
            }

            isQuote = false;
            dequotedText += token;
        }

        return dequotedText;
    }

    const vector<string> ParseLine(const char* line, bool& quote) {
        enum class mode { blank, word, escape };

        mode currentMode{mode::blank};
        bool isQuote{false};
        vector<string> words;
        string currentWord;

        while (*line != '\0') {
            const char token = *(line++);

            switch (currentMode) {
                case mode::blank:
                    if (IsBreakChar(token)) continue;

                    currentWord = "";

                    if (token == '\\') {
                        currentMode = mode::escape;
                        isQuote = false;
                    } else {
                        currentMode = mode::word;
                        isQuote = token == '"';

                        if (!isQuote) currentWord += token;
                    }

                    break;

                case mode::word:
                    if ((IsBreakChar(token) && !isQuote) || (token == '"' && isQuote)) {
                        currentMode = mode::blank;
                        words.push_back(currentWord);
                        continue;
                    }

                    if (token == '\\') {
                        currentMode = mode::escape;
                        continue;
                    }

                    currentWord += token;

                    break;

                case mode::escape:
                    currentMode = mode::word;
                    currentWord += token;

                    break;
            }
        }

        quote = false;
        if (currentMode != mode::blank) {
            words.push_back(currentWord);
            quote = isQuote;
        }

        return words;
    }

    const vector<string> ParseLine(const char* line) {
        bool quote;
        return ParseLine(line, quote);
    }

    extern "C" int CharIsQuoted(char* line, int index) {
        enum class mode { blank, word, escape };

        mode currentMode{mode::blank};
        bool isQuote{false};

        for (int i = 0; i <= index && *line != '\n'; i++) {
            const char token = *(line++);

            switch (currentMode) {
                case mode::blank:
                    if (IsBreakChar(token)) continue;

                    if (token == '\\') {
                        currentMode = mode::escape;
                        isQuote = false;
                    } else {
                        currentMode = mode::word;
                        isQuote = token == '"';
                    }

                    break;

                case mode::word:
                    if ((IsBreakChar(token) && !isQuote) || (token == '"' && isQuote)) {
                        currentMode = mode::blank;
                        continue;
                    }

                    if (token == '\\') {
                        currentMode = mode::escape;
                        continue;
                    }

                    break;

                case mode::escape:
                    currentMode = mode::word;

                    break;
            }
        }

        return currentMode == mode::word;
    }

    extern "C" char* RlCompletionFunction(const char* word, int state) {
        static vector<string> words;
        static bool isQuoted = true;

        if (state == 0) {
            words = ParseLine(rl_line_buffer, isQuoted);
            if (strlen(word) == 0) words.push_back("");
        }

        if (words.size() > 1 && words[0] != "help") {
            char* completion =
                rl_filename_completion_function(words[words.size() - 1].c_str(), state);

            if (completion == nullptr || isQuoted) return completion;

            string quotedCompletionString = Quote(completion);
            free(completion);

            char* quotedCompletion =
                reinterpret_cast<char*>(malloc(quotedCompletionString.size() + 1));
            strcpy(quotedCompletion, quotedCompletionString.c_str());

            return quotedCompletion;
        }

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

        char* suggestion = reinterpret_cast<char*>(malloc(suggestions[state].size() + 1));
        strcpy(suggestion, suggestions[state].c_str());

        return suggestion;
    }

    extern "C" int ReadlineEventHook() {
        if (stop) {
            rl_done = 1;
        }

        return 0;
    }

    extern "C" int FilenameStatHook(char** filename) {
        bool isQuoted = true;
        ParseLine(rl_line_buffer, isQuoted);

        if (strcmp(*filename, ".") == 0 || isQuoted) return 0;

        const string& dequotedFilename = Dequote(*filename);

        free(*filename);
        *filename = reinterpret_cast<char*>(malloc(dequotedFilename.size() + 1));

        strcpy(*filename, dequotedFilename.c_str());

        return 1;
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
        rl_char_is_quoted_p = CharIsQuoted;
        rl_filename_stat_hook = FilenameStatHook;

        rl_completer_word_break_characters = " \t";
        rl_completer_quote_characters = "\"\\";

        while (!stop) {
            char* lineBuffer = readline("> ");
            if (!lineBuffer) {
                lineBuffer = reinterpret_cast<char*>(malloc(5));
                strcpy(lineBuffer, "quit");
            }

            add_history(lineBuffer);

            vector<string> words = ParseLine(lineBuffer);

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
