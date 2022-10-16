# Driver globals

```
    struct DspExecState {
        uint16 expectedResult;
        void* Handler;
        void* context;
        uint32 taskId;
    }

    struct DspGlobals {
        ...                                 // 0 - 19
        void* Handler20                     // 20
        void* Handler24                     // 24
        void* Handler28                     // 28
        void* ...                           // 32
        void* Handler36                     // 36
        ...                                 // 37 - 47
        uint16 initCmd;                     // 48       IPC argument for initialization
        DspExecState dspExecState;          // 62 - 75
        DspExecState dspExecState_1;        // 76 - 89
        ...                                 // 90 - 145
        void* Hwr1859DspLoad;               // 146
        void* Hwr1859DspGet;                // 150
        void* Hwr1859DspExec;               // 154
        void* Hwr1859DspLock;               // 158
    };

    struct HalGlobals {
        ...                                 // 0-23
        DspGlobals* dspGlobals              // 24
    };

    HalGlobals* halGlobals = 0x0304;
```

# Registers

| address  | width  | register              | notes                                                                               |
| -------- | ------ | --------------------- | ----------------------------------------------------------------------------------- |
| `0x0202` | 2      | ???                   | bit 12 is set before dispatching a command and after a command interrupt is handled |
| `0x0220` | 2      | Interrupt status      |                                                                                     |
| `0x0222` | 2      | Interrupt enable      |                                                                                     |
| `0x0c04` | 2      | IPC command word      |                                                                                     |
| `0x0c06` | 2      | IPC status word       | `0xfc00` on reset                                                                   |
| `0x0c08` | 2      | IPC arg 1             |                                                                                     |
| `0x0c0a` | 2      | IPC arg 2             |                                                                                     |
| `0x0c0c` | 2      | IPC arg 3             |                                                                                     |
| `0x0c0e` | 2      | IPC arg 4             |                                                                                     |
| `0x0c10` | 2      | IPC arg 5             |                                                                                     |
| `0x0c12` | 2      | IPC arg 6             |                                                                                     |
| `0x0c14` | 2      | IPC result 1          |                                                                                     |
| `0x0c16` | 2      | IPC result 2          |                                                                                     |
| `0x0c18` | 2      | IPC result 3          |                                                                                     |
| `0x0c1a` | 2      | IPC result 4          |                                                                                     |
| `0x0c1c` | 2      | IPC result 5          |                                                                                     |
| `0x0c1e` | 2      | IPC result 6          |                                                                                     |
| `0x0e60` | 2      | ???                   | bit 4 is set before dispatching a command and after a command interrupt is handled  |
| `0x8000` | 0x8000 | shared memory for IPC |                                                                                     |

## IPC status word

-   bits 15 -- 10: mirrors the command once execution has completed
-   bits 9 -- 0: return status, 0 = OK
-   Clearing seems to reset the interrup line

# IPC commands

## Read oob

 * Command: `0x3a01`
 * Arg 1: ??? (set to zero)
 * Arg 2: number of bad blocks
 * Arg 3: SHM base
 * No results

Reads the first 4 OOB bytes of all blocks into SHM

## Erase block

 * Command: `0x3201`
 * Arg 1: block to erase
 * No results

## Probe block

 * Command: `0x3601`
 * Arg 1: block
 * Arg 2: page
 * Result 1: `0x0030`

Dunno what this really does, but it must return `0x0030` in order to signal
a good block ¯\_(ツ)_/¯

# Init

* Poll `0x0c06` until it reads `0xfc00`
* Clear `0x0c06`
* Upload DSP init program
* Clear `0x0c06`
* Move IPC argument from loaded program to IPC arg 1 (`0x0c08`)
* Write `0x0037` to `0x0c04`
* Wait until `0x0c06` is nonzero, validate that status is zero
* Clear `0x0c06`
* Write `0x0001` to `0x0200`
* Write `0x0001` to `0x0222`
* Toggle some flags in `0x0c00`

# Uploading a DSP program

-   Copy program to SHM starting at base
-   Copy some flag to IPC arg 1
-   Copy program size to IPC arg 2
-   Copy `(base + 0xef000000) >> 1`to IPC arg 3
-   Copy some flag to IPC arg 4
-   Copy 54 or 3205 to IPC command word
-   Wait until IPC status word becomes nonzero
