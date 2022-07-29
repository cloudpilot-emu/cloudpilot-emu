This is the documentation for the embedded version of CloudpilotEmu
which allows you to embed emulated PalmOS devices into your own web pages.

# Installation

## Global script

If you are developing a "plain" website (without advanced JavaScript tooling),
this is for you. Download `cloudpilot-embedded.zip` from [github
releases](https://github.com/cloudpilot-emu/cloudpilot-emu/releases). Inside the
archive you will find the following files:

* `cloudpilot-emu.js`: The JavaScript bundle
* `cloudpilot-emu.js.map`: Source map for debugging
* `cloudpilot-emu.d.ts`: Type declarations (in case you are using TypeScript)
* `cloudpilot_web.wasm`: The compiled web assembly bytecode

Drop `cloudpilot-emu.js` and `cloudpilot-web.wasm` next to your web page and
include the JavaScript bundle with a script tag in your HTML code

```html
    <script src="cloudpilot-emu.js"></script>
```

CloudpilotEmu is now available from JavaScript via the global `cloudpilot`
object.

## NPM

This is the preferred method of installation if you are developing a JavaScript
application with the corresponding package management and build tooling (NPM /
Yarn / Webpack / etc.). In this case you can just install
`cloudpilot-emu-embedded` from NPM with your preferred package manager. The
package comes with TypeScript typings.

Instead of referring the global `cloudpilot` namespace used in the example code
below you can now import all symbols from this module.

Please note that you still need to serve `cloudpilot-web.wasm` next to your
application in order to actually run the emulator. You can either download it
separately (see above) or find it in the `dist` folder of said package and copy
it from there.

# Usage

## Examples

The [examples page](./examples) has a comprehesive list of examples that show
how to use and interact with the emulator in different ways.

## Reference documentation

Navigate to the [reference documentation](./reference) in order to get a
in-depth overview over the available API methods and their usage, including
the parts of the API not covered by this short introduction.

## Basic setup

In order to run the emulator you need a HTML `canvas` element on your page and
either a ROM file or a file with a session that was previously saved with
CloudpilotEmu. All device types supported by CloudpilotEmu are supported in the
embedded version.

Provided that you are using a session file that is stored as an `Uint8Array` in
the variable `sessionFile` and have a canvas element with ID `canvas` the
emulator can be launched with

```javascript
async function main() {
    const canvas = document.getElementById('canvas');
    const emulator = await cloudpilot.createEmulator();

    emulator.
        loadSession(sessionFile)
        .setCanvas(canvas)
        .bindInput(canvas, canvas)
        .resume();
};

main();
```

### `createEmulator`

[cloudpilot.createEmulator](./reference/functions/createEmulator.html) loads
the web assembly module, compiles it and returns a promise for an instance of the
emulator. This process is async, hence the need to return a promise. On modern
browsers you can use `async/await` to handle the promise. As an alternative (or
to support legacy browsers) you can use the usual promise API

```javascript
cloudpilot.createEmulator().then(emulator => {
    // ...
});

```

Without any arguments, `createEmulator` assumes that the web assembly binary is
hosted next to your HTML file. You can supply a different URL as an argument.

All interactions with the emulator go through methods on the [emulator
instance](./reference/interfaces/Emulator.html). Most of those methods return
the same instance of the emulator, so calls can be chained. Errors are
communicated via exceptions.

### `loadSession`, `loadRom`

Session and rom files need to be provided as `Uint8Array` typed arrays.
[loadSession](./reference/interfaces/Emulator.html#loadSession) loads a
session image, [loadRom](./reference/interfaces/Emulator.html#loadRom) loads a
rom file. On success, the emulator is initialized but stopped.

`loadRom` will try to autodetect the device type from the ROM. If you want to
force a specific choice, you can do so with a second argument.

```
emulator.loadRom(romFile, cloudpilot.DeviceId.m515)
```

Please see the [reference on DeviceId](./reference/enums/DeviceId.html) for
all supported enum values.

### `setCanvas`

[setCanvas](./reference/interfaces/Emulator.html#setCanvas) sets the canvas
element used for displaying the emulator.

### `bindInput`

[bindInput](./reference/interfaces/Emulator.html#bindInput) tells the emulator
to start listening for input events. You can pass the target for keyboard events
as an argument; the default is `window`. You'll usually want to use the canvas
element here, but:

**WARNING:** You **must** set the `tabindex` attribute on the canvas node if you
want to process keyboard events from it.

You can stop listening for events by calling
[releaseInput](./reference/interfaces/Emulator.html#releaseInput).

#### `resume`

Calling [resume](./reference/interfaces/Emulator.html#resume) will resume
emulation. You can call [pause](./reference/interfaces/Emulator.html#resume)
in order to pause it again.

The default for CloudpilotEmu is to use `requestAnimationFrame` for running the
emulator, which will automatically stop it if the window with the emulator is
not visible. You can call
[setRunHidden](./reference/interfaces/Emulator.html#setRunHidden) in order to
run the emulator even if the window is not currently visible.

## ROM or session file?

Starting CloudpilotEmu with a ROM file will boot the device and go through the
initial setup process (digitizer calibration, locale, etc.).

If your goal in embedding the emulatur is running a particular application, then
you are better off preparing a pre-booted session in CloudpilotEmu and using
that with the embedded emulator. Note that you don't have to include all
possible application that you might want to run in the session --- you can
install the relevant applications after initializing the emulator (see below.)

## Installing and launching applications

You can install and launch applications programatically, either individual
`.prc` / `.pdb` databases or zipfiles that contain databases. All files are
passed to the emulator as `Uint8Array` typed arrays.

* [installDatabase](./reference/interfaces/Emulator.html#installDatabase)
  installs a database
* [installAndLaunchDatabase](./reference/interfaces/Emulator.html#installAndLaunchDatabase)
  installs a database and attempts to launch it
* [installFromZipfile](./reference/interfaces/Emulator.html#installFromZipfile)
  searches a `.zip` archive for databases and installs them
* [installFromZipfileAndLaunch](./reference/interfaces/Emulator.html#installFromZipfileAndLaunch)
  installs databases from a `.zip` archive and then attempts to launch a
  particular database
* [installDatabase](./reference/interfaces/Emulator.html#installDatabase)
  installs a database
* [launchDatabase](./reference/interfaces/Emulator.html#launchDatabase)
  extracts the name from a database and attempts to locate it on the device and
  launch it.
* [launchByName](./reference/interfaces/Emulator.html#launchByName) searches
  for a database with a particular name on the device and attempts to launch it.

Installing and launching applications is only possible if the device is powered
on and has finished booting. You can ensure that this is the case by using a
session file of a booted device instead of a plain ROM file for the emulator.

**WARNING:** Installing and launching programs calls into PalmOS. This has the
potential of crashing the device under rare circumstances. Calling those methods
while the launcher is running should be fine. In order to be 100% safe from bad
surprises, always call those methods before the emulator is resumed --- this way
the call always finds the emulator in the same state.

## Modifying the hotsync user name

The hotsync user name was commonly used by software for its registration
process. On real devices the hotsync name is set during the first hotsync.
CloudpilotEmu allows you to change the user name by calling
[setHotsyncName](./reference/interfaces/Emulator.html#setHotsyncName).

**WARNING:** CloudpilotEmu calls into PalmOS in order to change the hotsync
name. This has the potential of crashing the device under rare circumstances.
Doing so while the launcher is running should be fine. In order to be 100% safe
from bad surprises, set up the hotsync name before the emulator is resumed ---
this way the call always finds the emulator in the same state.


## Audio

Audio needs to be enabled by calling
[initializeAudio](./reference/interfaces/Emulator.html#initializeAudio). The
result is a promise that resolves to a boolean that tells you whether audio was
successfully initialized.

Browser policies require this method to be called from an event handler
triggered by a user interaction (i.e. a keyboard or a click event), otherwise
the initialization will fail.

## Game input mode

Just like with the CloudpilotEmu web app, keyboard input is usually converted
into keyboard events for PalmOS. By pressing shift-ctrl the user can enter "game
mode". In game mode, various buttons are mapped to the hardware buttons instead:

* **wasd/qe:** The w/a/s/d buttons control up/cal/down/notes and the q/e buttons
  control contacs/todo.
* **uhjk/uo:** The same as wasd/qe, but shifted to the right of the keyboard.
* **up/down/left/right**: These buttons control up/down/cal/notes.
* **zxcv/yxcv**: z/x/c/v (or y/x/c/v) control cal/contacs/todo/notes.

While game mode is active, a small overlay is shown in the bottom right corner
of the emulator. Game mode and the overlay can be controlled (and permanently
disabled) by various methods on the emulator object; check the [reference
documentation](./reference/interfaces/Emulator.html#setGameMode) for more details.
