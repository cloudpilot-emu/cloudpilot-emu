#ifndef _CLI_COMMANDS_H_
#define _CLI_COMMANDS_H_

#include <functional>
#include <string>
#include <vector>

#include "Debugger.h"
#include "GdbStub.h"

namespace cli {
    class CommandContext {
       public:
        virtual Debugger& GetDebugger() const = 0;
        virtual GdbStub& GetGdbStub() const = 0;
        virtual void PrintUsage() const = 0;
        virtual void RequestQuit() = 0;
    };

    using CommandCallback = std::function<void(std::vector<std::string>, CommandContext& context)>;

    struct Command {
        const char* name;
        const char* usage;
        const char* description;
        const char* help;
        CommandCallback cmd;
    };

    extern const std::vector<Command> commands;

    const Command* GetCommand(const std::string& name);
}  // namespace cli

#endif  // _CLI_COMMANDS_H_
