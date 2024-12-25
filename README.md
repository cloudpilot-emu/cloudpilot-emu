# What is this?

CloudpilotEmu is an emulator for Dragonball-based PalmOS devices that runs in a web
browser. In particular, the emulator works on iOS. The emulator is derived from
the original POSE emulator. Please see below for the list of currently supported
devices.

<img src="doc/images/m515.jpeg" width="310" height="552" alt="Palm m515 emulation"></img>
&nbsp;
<img src="doc/images/palmvx.jpeg" width="310" height="552" alt="Palm Vx emulation"></img>
&nbsp;
<img src="doc/images/tungstenw.jpeg" width="310" height="552" alt="Tungsten W emulation"></img>
&nbsp;
<img src="doc/images/palmpilot.jpeg" width="310" height="552" alt="Palm Pilot emulation"></img>

# The emulator

The current version of the emulator can be found at

https://cloudpilot-emu.github.io/app

The emulator can be added as an app to the homescreen of iOS and Android devices.
Please check out the documentation in the app.

## Embedded emulator

The is an embedded version of CloudpilotEmu that allows you to embed the emulator
into your own web pages. Please check the
[documentation](https://cloudpilot-emu.github.io/embedded/)
for more details.

## ROM files

In order to use the emulator you need to provide a ROM file for the device you
want to emulate. You can find a collection of ROMs
[on PalmDB](https://palmdb.net/app/palm-roms-complete).

## Networking support

CloudpilotEmu supports networking by tunneling network connections over a
custom proxy server via a websocket connection. Please check out the
[dedicated networking documentation](doc/networking.md) for setting up
networking with CloudpilotEmu.

## Emulated devices

The following devices are currently emulated:

-   Pilot, Palm Pilot
-   Palm III
-   Palm IIIxe, Palm IIIx, Palm IIIe
-   Palm IIIc
-   Palm Vx, Palm V
-   Palm VII, Palm VII EZ, Palm VIIx
-   Palm m500, Palm m505, Palm m515
-   Palm m520 (an unreleased prototype of a highres 5xx)
-   Palm m100, Palm m105, Palm m125, Palm m130
-   Palm i705
-   Tungsten W (silkscreen version)
-   Handera 330
-   Handera 330c (the lost color version of the 330c)
-   Sony PEG-S300, PEG-S320
-   Sony PEG-S500C series
-   Sony PEG-T400 series
-   Sony PEG-N600C series
-   Sony PEG-T600C series
-   Sony PEG-N700C series
-   Sony PEG-T650C series
-   Sony PEG-NR70 series
-   Acer S1x
-   Legend P168 (no support for SD card)

## Other versions

A preview build of the next version of CloudpilotEmu is available
[here](https://cloudpilot-emu.github.io/app-preview).

## Native app

In addition to the web version, CloudpilotEmu can be built as a native app.
The native version targets developers and does not provide skins or audio
support. However, it adds a GDB stub that allows for remote debugging via
GDB. You can find more documentation on debugging with CloudpilotEmu
[here](doc/debugging.md).

The native app does not come with a GUI, but is controlled with a CLI
instead. The CLI provides commands for loading and saving images, mounting
card images, generating traces of m68k code and much more. The CLI supports
tab completion, and there is a `help` command that provides further
documentation.

Networking is supported in the native app both natively and via the proxy.
For proxy support, the app needs to be built against libcurl 8.0.0 or newer,
and libcurl needs to be configured with websocket support.

The native app can be built and run on MacOS, Linux and on the WSL for
Windows. Please see below for build instructions. Once built, the app can
be started from the command line by supplying a session image or a ROM file
as command line argument. Command line options exist for activating the
GDN stub, mounting a card on launch and more. Run `cloudpilot-emu --help` in
order to get an overview of the supported options.

# Reporting issues

Please report issues on the [Github tracker](https://github.com/cloudpilot-emu/cloudpilot-emu/issues).

# Known issues and limitations

-   iOS: changing device orientation between portrait and landscape may mess
    up the layout of the app. This is an iOS bug that can be worked around by
    rotating the device by 180° in portrait and rotating back.
-   Audio timing is not perfect and processed at the refresh rate of the emulator
    (usually the same as the screen refresh rate of the host device). Sound
    effects that rely on quickly modulating the audio signal may not be
    reproduced correctly.
-   On rare cassinos installing files or changing the hotsync name can catch
    PalmOS on the wrong foot and crash the emulator. In particular, this happens
    if a file is installed on a virtual Palm IIIc with PalmOS 3.5 while the
    launcher is active and the category menu is open.
-   Tungsten W, i705, PEG-T650C and PEG-NR70 cannot enter and exit sleep correctly,
    and the corresponding functionality has been disabled.
-   On Clié devices, audio is not emulated beyond the usual beeps.
-   The MQ-1168 video chip in PEG-NR70 series devices is not emulated beyond
    framebuffer support, 2D acceleration is not available.
-   Formatting an unformatted memory stick in Clié devices that run PalmOS 3.x
    locks up PalmOS.
-   Native CLI app: switching between devices with different display resolution
    does not work properly.

# Source code and relationship to POSE

CloudpilotEmu is derived from the original POSE sources. It is not a
straightforward port, though, as the original code has been adapted and partly
rewritten to fit well with a browser environment. In addition, it contains
bug fixes and many new features. In particular:

-   The codebase has been updated to build and run on both 32bit and 64bit
    little endian systems. Big endian systems should work, but I cannot test
    this (are there even any relevant big endian systems left?).
-   POSE was built as a tool for debugging and profiling PalmOS applications.
    Most of this functionality has been removed in CloudpilotEmu.
-   All UI parts and in particular threading were removed from the source.
-   Dispatch and timing have been partly rewritten, timing should now be pretty
    close to the original device.
-   Adjusting the grayscale in 2bpp mode works.
-   All 16MB of the Palm m515 are usable.
-   A few conditions that can cause interrupt storms have been fixed.
-   Savestate code has been rewritten to work with a fixed buffer without
    allocations.
-   Event injection works without generating null events.
-   Reminders and alarms trigger propely.
-   Clié devices use full MQ11xx video acceleration.
-   SD card and Memory Stick emulation.
-   A GDB stub for debugging PalmOS and apps written with a suitable toolchain.

# Building

The build is controlled by `src/Makefile`. You can tune the compiler and linker
options by copying `Makefile.local.example` to `Makefile.local` and editing it.

The following instructions apply to Linux and MacOS.

Please not the parts of the source code for the native build reside in a submodule
which needs to be checked out and updated together with the main code. This
can be done with

```
    $ git clone --recurse-submodules
    $ git submodule update --recursive
```

## Web app

### Emulator

The emulator is written in C++ and compiled to Web Assembly using emscripten. In
order to build the WASM binary you need to install a recent version of the
emscripten toolchain. CloudpilotEmu currently builds with 2.0.24.

With the SDK properly set up you can build the source via

```
    $ make emscripten
```

### Javascript

The web app is written in Typescript using [Angular](https://angular.io) and
[Ionic](https://ionicframework.com). In order to build
you need [NodeJS](https://nodejs.org/en/) and [yarn](https://yarnpkg.com)
installed. Go to the `web` directory and do

```
    $ yarn install
    $ yarn gen-revision
    $ yarn build
    $ yarn generate-worker
```

The result will be in the `build-pwa` subdir. The last step generates a service
worker for offline operation. You can skip it if you don't require offline
operation, the app will work fine without it.

Note that you need to build the WASM binary (see above) before building the web
app.

If you want to do development on the frontend, you can launch a live build
on `http://localhost:4200` by running

```
    $ yarn start
```

## Native build

Building the native version of CloudpilotEmu requires SDL2, sdl2-image and libcurl.

Note that for network support via proxy to actually work, libcurl must be at
least version 8.0.0 and needs to be built with websocket support, which may or
not be the case for the libcurl shipped with your system. CloudpilotEmu will
build and run against any version of libcurl, but the proxy will be disabled.
On MacOS you can use libcurl from homebrew (which enables websocket support), on
Ubuntu you currently need to build libcurl yourself for websocket support. 

On Ubuntu the following will give you the necessary packages:

```
    $ apt-get install libreadline-dev libcurl4-openssl-dev libsdl2-image-dev libsdl2-dev
```

On Fedora

```
    $ dnf install readline-devel libcurl-devel SDL2_image-devel SDL2-devel 
```

The build is accomplished with

```
    $ make bin
```

and you will up with a `src/cloudpilot/cloudpilot-emu` binary.

# OS5 emulation and uARM

CloudpilotEmu contains a fork of Dmitry Grinberg's brilliant
[uARM](https://github.com/uARM-Palm/uARM). This fork
is heavily optimized and currently restricted to emulating a Tungsten E2. uARM will
eventually be integrated into the CloudpilotEmu web app, but at the moment it
is available as a separate prototype.

You can try a live build of the current code
[here](https://cloudpilot-emu.github.io/uarm-preview/). You'll need Tungsten E2
NAND and NOR images
(for example from [PalmDB](https://palmdb.net/app/palm-roms-complete)), and you can
mount an optional SD card image.

A native build of this version of uARM is built via `make bin` (alongside
`cloudpilot-emu`) and you can find it as `src/uarm/cp-uarm`.

# Credits

Artwork for CloudpilotEmu was done by Paolo Lazatin.

SDCTL emulation to enable the full 16MB of RAM on the m515 is taken from
[Mu](https://github.com/meepingsnesroms/Mu).

Zipfile I/O uses the [zip library](https://github.com/kuba--/zip).

Protobuf (de)serialization in C++ is done using the awesome
[NanoPB](https://github.com/nanopb/nanopb) library.

The native build uses [argparse](https://github.com/p-ranav/argparse) and
[uri-library](https://github.com/ben-zen/uri-library). Stacktraces in the native
build are based on Farooq Melas [gist](https://gist.github.com/fmela/591333).
HTTP and Websocket I/O in the native build use
[libcurl](https://curl.se/libcurl/c/libcurl-ws.html). FAT support
is built on [dosfstools](https://github.com/dosfstools/dosfstools) and
[FatFs](http://elm-chan.org/fsw/ff). Parts of the GDB stub are taken from
[uARM](https://github.com/uARM-Palm/uARM).

# LICENSE

This program is free software: you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later
version.
