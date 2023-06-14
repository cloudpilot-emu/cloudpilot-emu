#ifndef _SDL_CLI_H_
#define _SDL_CLI_H_

#include "Debugger.h"
#include "GdbStub.h"

namespace Cli {

    struct TaskContext {
        Debugger& debugger;
        GdbStub& gdbStub;
    };

    void Start();

    void Stop();

    bool Execute(const TaskContext& context);
};  // namespace Cli

#endif  // _SDL_CLI_H_
