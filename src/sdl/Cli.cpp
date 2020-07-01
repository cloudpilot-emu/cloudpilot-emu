#include "Cli.h"

// clang-format off
#include <cstdio>
#include <readline/readline.h>
// clang-format on

#include <atomic>
#include <deque>
#include <functional>
#include <iostream>
#include <mutex>
#include <thread>

#include "EmCommon.h"

namespace {
    using Task = function<void()>;

    atomic<bool> stop(false);
    thread cliThread;

    Task task;
    mutex taskMutex;
    condition_variable cvExecuteTask;

    int ReadlineEventHook() {
        if (stop) {
            rl_done = 1;
        }

        return 0;
    }

    void ThreadMain() {
        rl_event_hook = ReadlineEventHook;

        while (!stop) {
            char* lineBuffer = readline("> ");
            if (!lineBuffer) continue;

            string line(lineBuffer);

            if (!line.empty()) {
                unique_lock<mutex> lock(taskMutex);

                task = [=]() { cout << "echo: " << line << endl << flush; };

                while (task) cvExecuteTask.wait(lock);
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

        stop = true;

        cliThread.join();

        task = Task();
    }

    void Execute() {
        {
            unique_lock<mutex> lock(taskMutex);

            if (task) task();

            task = Task();
        }

        cvExecuteTask.notify_one();
    }

}  // namespace Cli
