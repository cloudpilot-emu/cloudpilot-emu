# What is this?

This is an emulator for Dragonball-based PalmOS devices that runs in a web
browser. In particular, the emulator works on iOS. The emulator is derived from
the original POSE emulator. At the moment, Palm V amd Palm m515 devices are
emulated. Support for more devices may follow in the future.

<img src="doc/m515.jpeg" width="310" height="552" alt="Palm m515 emulation"></img>
&nbsp;
<img src="doc/palmv.jpeg" width="310" height="552" alt="Palm V emulation"></img>

# The emulator

The current version of the emulator can be found at

https://cloudpilot-emu.github.io/app

The emulator can be added as an app to the homescreen of iOS and Android devices.
Please check out the documentation in the app.

## Other versions

A preview build of the next version of Cloudpilot is available
[here](https://cloudpilot-emu.github.io/app).

There also is a web build of the native SDL version of cloudpilot
[here](https://cloudpilot-emu.github.io/sdl)
([preview build here](https://cloudpilot-emu.github.io/sdl-preview)).
This has only a very basic interface and offers less features than the full
web app --- it is mainly a debugging tool.

# Reporting issues

Please report issues on the [Github tracker](https://github.com/DirtyHairy/cloudpilot/issues).

# Known issues

-   The method that POSE uses to inject events into the virtual device can mess
    up the timing of games on PalmOS 3 and earlier. In particular, Lemmings is
    affected by this and runs too fast while the screen is touched. On PalmOS 4
    the emulator uses a slightly different method that does not cause this
    issue.
-   The web UI does not currently allow pressing multiple hardware buttons at once.

# Source code and relationship to POSE

Cloudpilot is derived from the original POSE sources. It is not a
straightforward port, though, as the original code has been adapted and partly
rewritten to fit well with a browser environment. In addition, it contains a few
bug fixes new features. In particular:

-   The codebase has been updated to build and run on both 32bit and 64bit
    little endian systems. Big endian systems should work, but I have not tested
    this (are there even any relevant big endian systems left?).
-   POSE was built as a tool for debugging and profiling PalmOS applications.
    Most of this functionality has been removed in Cloudpilot.
-   All UI parts and in particular threading were removed from the source.
-   Dispatch and timing have been partly rewritten, timing should now be pretty
    close to the original device.
-   Adjusting the grayscale in 2bpp mode works.
-   All 16MB of the Palm m515 are usable.
-   A few conditions that can cause interrupt storms have been fixed.
-   Savestate code has been rewritten to work with a fixed buffer without
    allocations.

# Building

The build is controlled by `src/Makefile`. You can tune the compiler and linker
options by copying `Makefile.local.example` to `Makefile.local` and editing it.

The following instructions apply to Linux and MacOS.

## Web app

### Emulator

The emulator is written in C++ and compiled to Web Assembly using emscripten. In
order to build the WASM binary you need to install the latest version of the
emscripten toolchain. With that in place you can build the source via

```
    $ make -Csrc emscripten
```

### Javascript

The web app is written in Typescript using [Angular](https://angular.io) and
[Ionic].(https://ionicframework.com). In order to build
you need [NodeJS](https://nodejs.org/en/) and [yarn](https://yarnpkg.com)
installed. Go to the `cloudpilot-ionic" directory and do

```
    $ yarn install
    $ yarn build
```

The result will be in the `www` subdir.
Note that you need to build the WASM binary (see above) before building the web
app.

## Native build

There is a simple native build for debugging Cloudpilot outside the browser. You
need SDL2 and its headers available in order to build. The build is accomplished
with

```
    $ make -Csrc bin
```

and you will up with a `src/cloudpilot` binary. The binary takes a ROM file or
session image as an argument. There is a rudemtary readline CLI with tab
completion that allows you to save session images and reset the virtual device.

## Web SDL build

The web frontend for the SDL build currently is a single `index.html`. You can serve
it directly from the git repository and the compiled WASM code is loaded from `src`.
Unsurprisingly, the WASM code needs to compiled first (see above.)

# Credits

Artwork for Cloudpilot was done by Paolo Lazatin.

# LICENSE

This program is free software: you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later
version.
