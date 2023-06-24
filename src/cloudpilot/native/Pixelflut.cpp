#include "Pixelflut.h"

#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>

#include <atomic>
#include <chrono>
#include <iostream>
#include <mutex>
#include <thread>

#include "Platform.h"

using namespace std;

namespace {
    unique_ptr<uint32_t[]> pixelData;
    mutex pixelDataMutex;

    size_t width;
    size_t height;

    int sock;

    atomic<bool> stopRequested;
    bool isRunning{false};
    thread pixelflutThread;

    pixelflut::Config cfg;

    void connect() {
        sock = -1;

        addrinfo addr_hint = {.ai_family = PF_INET};
        addrinfo* addr;
        if (getaddrinfo(cfg.host.c_str(), nullptr, &addr_hint, &addr) != 0) {
            cerr << "failed to resolve " << cfg.host << endl << flush;
            return;
        }

        struct sockaddr_in sa = {
            .sin_family = AF_INET,
            .sin_addr.s_addr = ((sockaddr_in*)addr->ai_addr)->sin_addr.s_addr,
            .sin_port = htons(cfg.port),
        };

        int _sock = socket(PF_INET, SOCK_STREAM, 0);
        if (_sock < 0) {
            cerr << "failed to create socket: " << errno << endl << flush;
            return;
        }

        if (connect(_sock, reinterpret_cast<sockaddr*>(&sa), sizeof(sa)) < 0) {
            cerr << "failed to connect: " << errno << endl << flush;
            return;
        }

        freeaddrinfo(addr);

        sock = _sock;
    }

    inline char* write_coord(unsigned int x, char* dest) {
        if (x == 0) {
            *(dest++) = '0';
            return dest;
        }

        bool go = false;

        for (unsigned int pow = 1000000; pow != 0; pow /= 10) {
            unsigned char digit = (x / pow) % 10;
            go = go || digit != 0;
            x %= pow;

            if (go) *(dest++) = '0' + digit;
        }

        return dest;
    }

    inline char* write_color(unsigned char color, char* dest) {
        static char digits[] = {'0', '1', '2', '3', '4', '5', '6', '7',
                                '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};

        *(dest++) = digits[color >> 4];
        *(dest++) = digits[color & 0x0f];

        return dest;
    }

    const char* build_command(unsigned int x, unsigned int y, uint32_t abgr, size_t& len) {
        static char command[64] = "PX ";

        char* next = command + 3;

        next = write_coord(x, next);
        *(next++) = ' ';

        next = write_coord(y, next);
        *(next++) = ' ';

        next = write_color(abgr, next);
        next = write_color(abgr >> 8, next);
        next = write_color(abgr >> 16, next);
        *(next++) = '\n';

        len = next - command;

        return command;
    }

    void threadMain() {
        unique_ptr<uint32_t[]> workingCopy = make_unique<uint32_t[]>(width * height);

        while (true) {
            if (stopRequested) return;

            {
                lock_guard lock(pixelDataMutex);
                memcpy(workingCopy.get(), pixelData.get(), width * height * 4);
            }

            if (sock < 0) connect();

            if (sock < 0) {
                this_thread::sleep_for(500ms);
                continue;
            }

            uint32_t* abgr = workingCopy.get();
            for (unsigned int y = 0; y < height; y++) {
                for (unsigned int x = 0; x < width; x++) {
                    size_t len;
                    const char* command = build_command(x, y, *(abgr++), len);

                    if (send(sock, command, len, 0) < 0) {
                        cerr << "failed to send: " << errno << endl << flush;

                        close(sock);
                        sock = -1;

                        goto loop;
                    }
                }
            }

        loop:;
        }

        if (sock > 0) {
            shutdown(sock, SHUT_RDWR);
            close(sock);
        }
    }
}  // namespace

bool pixelflut::IsActive() { return isRunning; }

void pixelflut::Start(Config config, size_t width, size_t height) {
    if (isRunning) return;

    cfg = config;
    ::width = width;
    ::height = height;

    pixelData = make_unique<uint32_t[]>(width * height);
    memset(pixelData.get(), 0, width * height * 4);

    stopRequested = false;
    isRunning = true;
    sock = -1;

    pixelflutThread = thread(threadMain);
}

void pixelflut::Stop() {
    if (!isRunning) return;

    stopRequested = true;
    pixelflutThread.join();

    isRunning = false;
}

void pixelflut::Update(SDL_Renderer* renderer) {
    if (!isRunning) return;

    lock_guard lock(pixelDataMutex);

    SDL_Rect rect =
        SDL_Rect{.h = static_cast<int>(height), .w = static_cast<int>(width), .x = 0, .y = 0};

    SDL_RenderReadPixels(renderer, &rect, SDL_PIXELFORMAT_ABGR8888, pixelData.get(), 4 * width);
}
