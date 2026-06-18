#include "uartdev_bcm2035.h"

#include <cstdio>

constexpr uint8_t CMD_ENTER_UPLOAD[] = {0x01, 0x2e, 0xfc, 0x00};
constexpr uint8_t CMD_ENTER_UPLOAD_RESPONSE[] = {0x04, 0x0e, 0x04, 0x01, 0x2e, 0xfc, 0x00};
constexpr uint8_t INVALID_ACK_UPLOAD_MODE_RESPONSE[] = {0x34, 0x31};

constexpr uint64_t CMD_ENTER_UPLOAD_RESPONSE_DELAY_NSEC = 100000000;  // 50 msec
constexpr uint64_t ACK_UPLOAD_MODE_DELAY_NSEC = 100000000;            // 50 msec

enum class State {
    receiveUploadCmd,
    waitForSendCmdEnterUploadResponse,
    sendCmdUploadResponse,
    waitForSendAckUploadMode,
    sendAckUploadMode
};

enum class Mode { send, receive, wait };

struct Bcm2035 {
    GetEmuTime getTime;

    State state;
    Mode mode;

    const uint8_t* buffer;
    size_t bufferSize;
    size_t bufferIndex;

    uint64_t delay;
    uint64_t transitionTimestamp;
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
    }

    bcm2035->state = state;
}

static State transitionNext(State currentState) {
    switch (currentState) {
        case State::receiveUploadCmd:
            printf("upload command received\n");
            return State::waitForSendCmdEnterUploadResponse;

        case State::waitForSendCmdEnterUploadResponse:
            printf("sending EnterUpload response\n");
            return State::sendCmdUploadResponse;

        case State::sendCmdUploadResponse:
            printf("response sent off\n");
            return State::waitForSendAckUploadMode;

        case State::waitForSendAckUploadMode:
            printf("sending ACK\n");
            return State::sendAckUploadMode;

        case State::sendAckUploadMode:
            printf("ACK sent off\n");
            return State::receiveUploadCmd;
    }
}

static uint_fast16_t bcm2035ReadF(void* userData) {
    Bcm2035* bcm2035 = reinterpret_cast<Bcm2035*>(userData);

    if (bcm2035->mode != Mode::send) return UART_CHAR_NONE;

    uint8_t c = bcm2035->buffer[bcm2035->bufferIndex++];
    printf("BCM2035 sent 0x%02x\n", (int)c);

    if (bcm2035->bufferIndex >= bcm2035->bufferSize) {
        bcm2035SetState(bcm2035, transitionNext(bcm2035->state));
    }

    return c;
}

static void bcm2035WriteF(uint_fast16_t chr, void* userData) {
    printf("BCM2035 received 0x%04x\n", chr);
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
