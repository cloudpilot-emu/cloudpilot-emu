#ifndef _SDL_CLI_H_
#define _SDL_CLI_H_

#include <optional>
#include <string>

#include "Debugger.h"
#include "GdbStub.h"

namespace cli {

    struct TaskContext {
        Debugger& debugger;
        GdbStub& gdbStub;
    };

    void Start(std::optional<std::string> scriptFile);

    void Stop();

    bool Execute(const TaskContext& context);
};  // namespace cli

#endif  // _SDL_CLI_H_
