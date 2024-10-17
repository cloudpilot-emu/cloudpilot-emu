#ifndef _CLI_UARM_COMMANDS_H_
#define _CLI_UARM_COMMANDS_H_

#include <functional>
#include <string>
#include <vector>

#include "MainLoop.h"
#include "SdlAudioDriver.h"
#include "SoC.h"

namespace commands {
    struct Context {
        SoC* soc;
        MainLoop& mainLoop;
        SdlAudioDriver& audioDriver;
    };

    void Register();
}  // namespace commands

#endif  // _CLI_UARM_COMMANDS_H_
