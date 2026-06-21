#include "uartdev_bcm2035.h"

#include <cstdio>

#include "savestate/savestateAll.h"
#include "scheduler.h"

#define SAVESTATE_VERSION 0

constexpr uint8_t CMD_ENTER_UPLOAD[] = {0x01, 0x2e, 0xfc, 0x00};
constexpr uint8_t CMD_ENTER_UPLOAD_RESPONSE[] = {0x04, 0x0e, 0x04, 0x01, 0x2e, 0xfc, 0x00};
constexpr uint8_t INVALID_ACK_UPLOAD_MODE_RESPONSE[] = {0xff, 0xff};

constexpr uint64_t CMD_ENTER_UPLOAD_RESPONSE_DELAY_NSEC = 10_msec;  // 50 msec
constexpr uint64_t ACK_UPLOAD_MODE_DELAY_NSEC = 150_msec;           // 50 msec
constexpr uint64_t WAIT_FOR_RECEIVE_ACK_DELAY = 150_msec;           // 50 msec

enum class State : uint8_t {
    receiveUploadCmd,
    waitForSendCmdEnterUploadResponse,
    sendCmdUploadResponse,
    waitForSendAckUploadMode,
    sendAckUploadMode,
    waitForReceiveAckDelay
};

enum class Mode : uint8_t { send, receive, wait };

struct Bcm2035 {
    GetEmuTime getTime;

    State state;
    Mode mode;

    const uint8_t* buffer;
    uint32_t bufferSize;
    uint32_t bufferIndex;

    uint64_t delay;
    uint64_t transitionTimestamp;

    template <typename T>
    void DoSaveLoad(T& chunkHelper) {
        uint8_t state8 = static_cast<uint8_t>(state);
        uint8_t mode8 = static_cast<uint8_t>(mode);

        chunkHelper.Do(typename T::Pack8() << state8 << mode8)
            .Do32(bufferSize)
            .Do32(bufferIndex)
            .Do64(delay)
            .Do64(transitionTimestamp);

        state = static_cast<State>(state8);
        mode = static_cast<Mode>(mode8);
    }
};

static void bcm2035SetModeSend(Bcm2035* bcm2035, const uint8_t* buffer, size_t bufferSize) {
    bcm2035->buffer = buffer;
    bcm2035->bufferSize = bufferSize;
    bcm2035->bufferIndex = 0;

    bcm2035->mode = Mode::send;
}

static void bcm2035SetModeReceive(Bcm2035* bcm2035, const uint8_t* buffer, size_t bufferSize) {
    bcm2035->buffer = buffer;
    bcm2035->bufferSize = bufferSize;
    bcm2035->bufferIndex = 0;

    bcm2035->mode = Mode::receive;
}

static void bcm2035SetModeWait(Bcm2035* bcm2035, uint64_t delay) {
    bcm2035->transitionTimestamp = bcm2035->getTime.getTimeF(bcm2035->getTime.userdata);
    bcm2035->delay = delay;

    bcm2035->mode = Mode::wait;
}

static void bcm2035SetState(Bcm2035* bcm2035, State state) {
    switch (state) {
        case State::receiveUploadCmd:
            bcm2035SetModeReceive(bcm2035, CMD_ENTER_UPLOAD, sizeof(CMD_ENTER_UPLOAD));
            break;

        case State::waitForSendCmdEnterUploadResponse:
            bcm2035SetModeWait(bcm2035, CMD_ENTER_UPLOAD_RESPONSE_DELAY_NSEC);
            break;

        case State::sendCmdUploadResponse:
            bcm2035SetModeSend(bcm2035, CMD_ENTER_UPLOAD_RESPONSE,
                               sizeof(CMD_ENTER_UPLOAD_RESPONSE));
            break;

        case State::waitForSendAckUploadMode:
            bcm2035SetModeWait(bcm2035, ACK_UPLOAD_MODE_DELAY_NSEC);
            break;

        case State::sendAckUploadMode:
            bcm2035SetModeSend(bcm2035, INVALID_ACK_UPLOAD_MODE_RESPONSE,
                               sizeof(INVALID_ACK_UPLOAD_MODE_RESPONSE));
            break;

        case State::waitForReceiveAckDelay:
            bcm2035SetModeWait(bcm2035, WAIT_FOR_RECEIVE_ACK_DELAY);
    }

    bcm2035->state = state;
}

static State transitionNext(State currentState) {
    switch (currentState) {
        case State::receiveUploadCmd:
            fprintf(stderr, "BCM2035: upload command received\n");
            return State::waitForSendCmdEnterUploadResponse;

        case State::waitForSendCmdEnterUploadResponse:
            fprintf(stderr, "BCM2035:sending EnterUpload response\n");
            return State::sendCmdUploadResponse;

        case State::sendCmdUploadResponse:
            fprintf(stderr, "BCM2035:response sent off\n");
            return State::waitForSendAckUploadMode;

        case State::waitForSendAckUploadMode:
            fprintf(stderr, "BCM2035:sending ACK\n");
            return State::sendAckUploadMode;

        case State::sendAckUploadMode:
            fprintf(stderr, "BCM2035: ACK sent off\n");
            return State::waitForReceiveAckDelay;

        case State::waitForReceiveAckDelay:
            fprintf(stderr, "BCM2035: resetting state machine\n");
            return State::receiveUploadCmd;
    }
}

static uint_fast16_t bcm2035ReadF(void* userData) {
    Bcm2035* bcm2035 = reinterpret_cast<Bcm2035*>(userData);

    if (bcm2035->mode != Mode::send) return UART_CHAR_NONE;

    uint8_t c = bcm2035->buffer[bcm2035->bufferIndex++];

    if (bcm2035->bufferIndex >= bcm2035->bufferSize) {
        bcm2035SetState(bcm2035, transitionNext(bcm2035->state));
    }

    return c;
}

static void bcm2035WriteF(uint_fast16_t chr, void* userData) {
    Bcm2035* bcm2035 = reinterpret_cast<Bcm2035*>(userData);

    if (bcm2035->mode != Mode::receive) return;

    if (static_cast<uint_fast16_t>(bcm2035->buffer[bcm2035->bufferIndex++]) != chr) {
        bcm2035SetState(bcm2035, State::receiveUploadCmd);
    }

    if (bcm2035->bufferIndex >= bcm2035->bufferSize) {
        bcm2035SetState(bcm2035, transitionNext(bcm2035->state));
    }
}

static bool bcm2035ClientIsActive(void* userData) {
    Bcm2035* bcm2035 = reinterpret_cast<Bcm2035*>(userData);

    return bcm2035->state != State::receiveUploadCmd;
}

static void bcm2035ClientTick(void* userData) {
    Bcm2035* bcm2035 = reinterpret_cast<Bcm2035*>(userData);

    if (bcm2035->mode != Mode::wait) return;

    const uint64_t timestamp = bcm2035->getTime.getTimeF(bcm2035->getTime.userdata);

    if (timestamp - bcm2035->transitionTimestamp >= bcm2035->delay) {
        bcm2035SetState(bcm2035, transitionNext(bcm2035->state));
    }
}

struct Bcm2035* bcm2035Init(GetEmuTime getTime) {
    Bcm2035* bcm2035 = reinterpret_cast<Bcm2035*>(malloc(sizeof(Bcm2035)));
    memset(bcm2035, 0, sizeof(*bcm2035));

    bcm2035->getTime = getTime;
    bcm2035SetState(bcm2035, State::receiveUploadCmd);

    return bcm2035;
}

void bcm2035RegisterWithUart(struct Bcm2035* bcm2035, SocUart* uart) {
    socUartSetFuncs(uart, bcm2035ReadF, bcm2035WriteF, bcm2035ClientIsActive, bcm2035ClientTick,
                    bcm2035);
}

template <typename T>
void bcm2035Save(struct Bcm2035* bcm2035, T& savestate) {
    auto chunk = savestate.GetChunk(ChunkType::bcm2035, SAVESTATE_VERSION);
    if (!chunk) abort();

    SaveChunkHelper helper(*chunk);
    bcm2035->DoSaveLoad(helper);
}

template <typename T>
void bcm2035Load(struct Bcm2035* bcm2035, T& loader) {
    auto chunk = loader.GetChunk(ChunkType::bcm2035, SAVESTATE_VERSION, "bcm2035");
    if (!chunk) return;

    LoadChunkHelper helper(*chunk);

    switch (bcm2035->state) {
        case State::receiveUploadCmd:
            bcm2035->buffer = CMD_ENTER_UPLOAD;
            break;

        case State::sendCmdUploadResponse:
            bcm2035->buffer = CMD_ENTER_UPLOAD_RESPONSE;
            break;

        case State::sendAckUploadMode:
            bcm2035->buffer = INVALID_ACK_UPLOAD_MODE_RESPONSE;
            break;

        default:
            break;
    }
}

template void bcm2035Save<Savestate<ChunkType>>(Bcm2035* bcm2035, Savestate<ChunkType>& savestate);
template void bcm2035Save<SavestateProbe<ChunkType>>(Bcm2035* bcm2035,
                                                     SavestateProbe<ChunkType>& savestate);
template void bcm2035Load<SavestateLoader<ChunkType>>(Bcm2035* bcm2035,
                                                      SavestateLoader<ChunkType>& loader);