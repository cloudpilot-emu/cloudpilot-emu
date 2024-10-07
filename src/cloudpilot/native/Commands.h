#ifndef _CLI_COMMANDS_H_
#define _CLI_COMMANDS_H_

#include <functional>
#include <string>
#include <vector>

#include "Debugger.h"
#include "GdbStub.h"

namespace commands {
    struct Context {
        Debugger& debugger;
        GdbStub& gdbStub;
    };

    void Register();
}  // namespace commands

#endif  // _CLI_COMMANDS_H_
