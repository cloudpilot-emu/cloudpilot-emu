#pragma GCC diagnostic ignored "-Wmissing-field-initializers"

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
#include <functional>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <memory>
#include <mutex>
#include <sstream>
#include <thread>
#include <unordered_map>
#include <vector>

#include "FileUtil.h"

using namespace std;

namespace {
    atomic<bool> stop(false);
    thread cliThread;

    vector<cli::Command> commands;
    unordered_map<string, uint32_t> commandMap;
    const cli::Command* command = nullptr;
    vector<string> arguments;

    mutex dispatchMutex;
    condition_variable cvExecuteTask;

    deque<string> script;

    class CommandEnvironmentImpl : public cli::CommandEnvironment {
       public:
        CommandEnvironmentImpl(const cli::Command& command, bool&(quit))
            : command(command), quit(quit) {}

        void PrintUsage() const override {
            cout << "usage: " << (command.usage ? command.usage : command.name) << endl << flush;
        }

        void RequestQuit() override { quit = true; }

       private:
        const cli::Command& command;

        bool& quit;
    };

    inline bool IsBlank(char token) { return token == ' ' || token == '\t'; }

    void InitScript(optional<string> scriptFile) {
        script.clear();

        if (!scriptFile) return;

        unique_ptr<uint8_t[]> fileContent;
        size_t len;
        if (!util::ReadFile(*scriptFile, fileContent, len)) {
            cout << "unable to read script " << *scriptFile << endl << flush;
        }

        enum class state { search, command, comment };
        state currentState{state::search};
        string currentCommand;

        for (size_t i = 0; i < len; i++) {
            char token = fileContent.get()[i];
            if (token == '\r') continue;

            switch (currentState) {
                case state::search:
                    if (token == '#')
                        currentState = state::comment;
                    else if (!IsBlank(token) && token != '\n') {
                        currentCommand += token;
                        currentState = state::command;
                    }

                    break;

                case state::command:
                    if (token == '\n' || token == '#') {
                        script.push_back(currentCommand);
                        currentCommand.clear();
                        currentState = token == '#' ? state::comment : state::search;
                    } else
                        currentCommand += token;

                    break;

                case state::comment:
                    if (token == '\n') currentState = state::search;
                    break;
            }
        }

        if (currentState == state::command) script.push_back(currentCommand);
    }

    string Escape(const char* text) {
        string quotedText;

        while (*text != '\0') {
            const char token = *(text++);

            if (IsBlank(token) || token == '\\') quotedText += '\\';
            quotedText += token;
        }

        return quotedText;
    }

    string Unescape(const char* text) {
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
                    if (IsBlank(token)) continue;

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
                    if ((IsBlank(token) && !isQuote) || (token == '"' && isQuote)) {
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
                    if (IsBlank(token)) continue;

                    if (token == '\\') {
                        currentMode = mode::escape;
                        isQuote = false;
                    } else {
                        currentMode = mode::word;
                        isQuote = token == '"';
                    }

                    break;

                case mode::word:
                    if ((IsBlank(token) && !isQuote) || (token == '"' && isQuote)) {
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

            string escapedCompletionString = Escape(completion);
            free(completion);

            return strdup(escapedCompletionString.c_str());
        }

        static vector<string> suggestions;

        if (state == 0) {
            suggestions = vector<string>(commands.size());
            suggestions.clear();

            for (auto& command : commands) {
                if (strstr(command.name, word) == command.name) {
                    suggestions.push_back(command.name);
                }
            }
        }

        if (state >= (int)suggestions.size()) return nullptr;

        return strdup(suggestions[state].c_str());
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

        const string& unescapedFilename = Unescape(*filename);

        free(*filename);

        *filename = strdup(unescapedFilename.c_str());

        return 1;
    }

    void Dispatch(const cli::Command* _command, vector<string>& _arguments) {
        unique_lock<mutex> lock(dispatchMutex);
        if (stop) return;

        command = _command;
        arguments = _arguments;

        while (command) cvExecuteTask.wait(lock);
    }

    void CmdHelp(vector<string> args, cli::CommandEnvironment& env, void* context) {
        if (args.size() > 1) return env.PrintUsage();

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

            for (auto& command : cli::GetCommands()) {
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

    void ThreadMain() {
        char* breakCharacters = strdup(" \t");

        rl_event_hook = ReadlineEventHook;
        rl_completion_entry_function = RlCompletionFunction;
        rl_char_is_quoted_p = CharIsQuoted;
        rl_filename_stat_hook = FilenameStatHook;

        rl_completer_word_break_characters = breakCharacters;
        rl_completer_quote_characters = "\"\\";

        while (!stop) {
            char* lineBuffer;

            if (script.size() > 0) {
                lineBuffer = strdup(script.begin()->c_str());
                script.pop_front();
            } else {
                lineBuffer = readline("> ");
            }

            if (!lineBuffer) lineBuffer = strdup("quit");

            add_history(lineBuffer);

            vector<string> words = ParseLine(lineBuffer);

            if (!words.empty() && !stop) {
                vector<string> args(words.begin() + 1, words.end());
                const cli::Command* _command = cli::GetCommand(words[0]);

                if (!_command) cout << "invalid command " << words[0] << endl << flush;
                Dispatch(_command, args);
            }

            free(lineBuffer);
        }

        free(breakCharacters);
    }
}  // namespace

namespace cli {

    void Start(optional<string> scriptFile) {
        if (cliThread.joinable()) return;

        AddCommands({{.name = "help", .description = "Show help.", .cmd = CmdHelp}});

        stop = false;
        command = nullptr;

        InitScript(scriptFile);

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

    void AddCommands(const std::vector<Command>& newCommands) {
        for (const auto& command : newCommands) {
            commands.push_back(command);
            commandMap.insert({string(command.name), commands.size() - 1});
        }
    }

    const Command* GetCommand(const string& name) {
        auto it = commandMap.find(name);

        return (it == commandMap.end()) ? nullptr : &commands[it->second];
    }

    const std::vector<Command>& GetCommands() { return commands; }

    bool Execute(void* context) {
        bool quit = false;

        if (command) {
            unique_lock<mutex> lock(dispatchMutex);
            CommandEnvironmentImpl commandEnvironment(*command, quit);

            command->cmd(arguments, commandEnvironment, context);
            command = nullptr;
        };

        cvExecuteTask.notify_one();

        return quit;
    }

}  // namespace cli
