//(c) uARM project    https://github.com/uARM-Palm/uARM    uARM@dmitry.gr

#include "keys.h"

#include <stdlib.h>
#include <string.h>

#include "util.h"

#define MAX_GPIO_KEYS 64
#define MAX_KP_ROWS 12
#define MAX_KP_COLS 8

struct KeyGpio {
    uint32_t sdlKey;  // 0 for inval
    int8_t gpioNum;
    bool activeHigh;
};

struct KeyMatrix {
    uint32_t sdlKey;  // 0 for inval
    bool isDown;
};

struct Keypad {
    struct SocGpio *gpio;

    struct KeyGpio gpios[MAX_GPIO_KEYS];
    int8_t kpGpioRow[MAX_KP_ROWS];  //<0 for inval
    int8_t kpGpioCol[MAX_KP_COLS];  //<0 for inval
    struct KeyMatrix km[MAX_KP_ROWS][MAX_KP_COLS];

    bool recalcing, matrixHasPullUps;
};

static void keypadPrvMatrixRecalc(struct Keypad *kp) {
    int i, j;

    kp->recalcing = true;

    // calc input rows
    for (i = 0; i < MAX_KP_ROWS; i++) {
        bool rowState = kp->matrixHasPullUps;
        bool haveStrong = false;

        if (kp->kpGpioRow[i] < 0) continue;

        if (socGpioGetState(kp->gpio, kp->kpGpioRow[i]) != SocGpioStateHiZ) continue;

        for (j = 0; j < MAX_KP_COLS; j++) {
            uint8_t colSta;

            if (kp->kpGpioCol[j] < 0) continue;

            if (!kp->km[i][j].isDown) continue;

            colSta = socGpioGetState(kp->gpio, kp->kpGpioCol[j]);

            if (colSta == SocGpioStateLow || colSta == SocGpioStateHigh) {
                bool colHi = colSta == SocGpioStateHigh;

                if (haveStrong && !rowState != !colHi)
                    fprintf(stderr, "row %u (%u) being pulled in different directions\n", i,
                            kp->kpGpioRow[i]);
                haveStrong = true;

                rowState = rowState && colHi;
            }
        }
        socGpioSetState(kp->gpio, kp->kpGpioRow[i], rowState);
    }

    // calc input cols
    for (j = 0; j < MAX_KP_COLS; j++) {
        bool colState = kp->matrixHasPullUps;
        bool haveStrong = false;

        if (kp->kpGpioCol[j] < 0) continue;

        if (socGpioGetState(kp->gpio, kp->kpGpioCol[j]) != SocGpioStateHiZ) continue;

        for (i = 0; i < MAX_KP_ROWS; i++) {
            uint8_t rowSta;

            if (kp->kpGpioRow[i] < 0) continue;

            if (!kp->km[i][j].isDown) continue;

            rowSta = socGpioGetState(kp->gpio, kp->kpGpioRow[i]);

            if (rowSta == SocGpioStateLow || rowSta == SocGpioStateHigh) {
                bool rowHi = rowSta == SocGpioStateHigh;

                if (haveStrong && !rowHi != !colState)
                    fprintf(stderr, "col %u (%u) being pulled in different directions\n", j,
                            kp->kpGpioCol[j]);
                haveStrong = true;

                colState = rowHi && colState;
            }
        }
        socGpioSetState(kp->gpio, kp->kpGpioCol[j], colState);
    }

    kp->recalcing = false;
}

static void keypadPrvGpioDirsChanged(void *userData) {
    struct Keypad *kp = (struct Keypad *)userData;

    if (!kp->recalcing) keypadPrvMatrixRecalc(kp);
}

struct Keypad *keypadInit(struct SocGpio *gpio, bool matrixHasPullUps) {
    struct Keypad *kp = (struct Keypad *)malloc(sizeof(*kp));
    unsigned i;

    if (!kp) ERR("cannot alloc Keypad");

    memset(kp, 0, sizeof(*kp));

    kp->gpio = gpio;
    kp->matrixHasPullUps = matrixHasPullUps;

    for (i = 0; i < MAX_KP_ROWS; i++) kp->kpGpioRow[i] = -1;

    for (i = 0; i < MAX_KP_COLS; i++) kp->kpGpioCol[i] = -1;

    socGpioSetDirsChangedNotif(kp->gpio, keypadPrvGpioDirsChanged, kp);

    return kp;
}

void keypadSdlKeyEvt(struct Keypad *kp, uint32_t sdlKey, bool wentDown) {
    int i, j;

    for (i = 0; i < MAX_GPIO_KEYS; i++) {
        if (kp->gpios[i].sdlKey == sdlKey && kp->gpios[i].gpioNum >= 0) {
            socGpioSetState(
                kp->gpio, kp->gpios[i].gpioNum,
                (wentDown && kp->gpios[i].activeHigh) || (!wentDown && !kp->gpios[i].activeHigh));
        }
    }

    for (i = 0; i < MAX_KP_ROWS; i++) {
        if (kp->kpGpioRow[i] < 0) continue;

        for (j = 0; j < MAX_KP_COLS; j++) {
            if (kp->kpGpioCol[j] < 0) continue;

            if (kp->km[i][j].sdlKey != sdlKey) continue;

            kp->km[i][j].isDown = wentDown;
        }
    }

    keypadPrvMatrixRecalc(kp);
}

static void keypadPrvGpioChanged(void *userData, uint32_t gpio, bool oldState, bool newState) {
    struct Keypad *kp = (struct Keypad *)userData;

    (void)gpio;
    (void)oldState;
    (void)newState;

    if (!kp->recalcing) keypadPrvMatrixRecalc(kp);
}

static bool keypadDefineRowOrCol(struct Keypad *kp, unsigned idx, int8_t *arr, unsigned max,
                                 int8_t gpioNum) {
    if (idx >= max) return false;

    if (arr[idx] >= 0) return false;

    arr[idx] = gpioNum;

    socGpioSetNotif(kp->gpio, gpioNum, keypadPrvGpioChanged, kp);
    socGpioSetState(kp->gpio, gpioNum, kp->matrixHasPullUps);
    keypadPrvMatrixRecalc(kp);

    return true;
}

bool keypadDefineRow(struct Keypad *kp, unsigned rowIdx, int8_t gpio) {
    return keypadDefineRowOrCol(kp, rowIdx, kp->kpGpioRow, MAX_KP_ROWS, gpio);
}

bool keypadDefineCol(struct Keypad *kp, unsigned colIdx, int8_t gpio) {
    return keypadDefineRowOrCol(kp, colIdx, kp->kpGpioCol, MAX_KP_COLS, gpio);
}

bool keypadAddGpioKey(struct Keypad *kp, uint32_t sdlKey, int8_t gpioNum, bool activeHigh) {
    unsigned i;

    for (i = 0; i < MAX_GPIO_KEYS; i++) {
        if (!kp->gpios[i].sdlKey) {
            kp->gpios[i].sdlKey = sdlKey;
            kp->gpios[i].gpioNum = gpioNum;
            kp->gpios[i].activeHigh = activeHigh;

            socGpioSetState(kp->gpio, gpioNum, !activeHigh);
            return true;
        }
    }

    return false;
}

bool keypadAddMatrixKey(struct Keypad *kp, uint32_t sdlKey, unsigned row, unsigned col) {
    // coords must be valid
    if (row >= MAX_KP_ROWS || col >= MAX_KP_COLS) return false;

    // and rows and cols must be hooked up
    if (kp->kpGpioRow[row] < 0 || kp->kpGpioCol[col] < 0) return false;

    // must be unused
    if (kp->km[row][col].sdlKey) return false;

    kp->km[row][col].sdlKey = sdlKey;
    kp->km[row][col].isDown = false;

    return true;
}
