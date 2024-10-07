#ifndef _CLI_H_
#define _CLI_H_

#include <functional>
#include <optional>
#include <string>
#include <vector>

namespace cli {
    class CommandEnvironment {
       public:
        virtual void PrintUsage() const = 0;
        virtual void RequestQuit() = 0;
    };

    using CommandCallback =
        std::function<void(std::vector<std::string>, CommandEnvironment&, void*)>;

    struct Command {
        const char* name;
        const char* usage;
        const char* description;
        const char* help;

        CommandCallback cmd;
    };

    void AddCommands(const std::vector<Command> commands);
    const Command* GetCommand(const std::string& name);
    std::vector<const Command> GetCommands();

    void Start(std::optional<std::string> scriptFile);

    void Stop();

    bool Execute(void* context);
};  // namespace cli

#endif  // _CLI_H_
