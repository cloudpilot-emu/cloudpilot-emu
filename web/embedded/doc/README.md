This is the documentation for the embedded version of CloudpilotEmu
which allows you to embed emulated PalmOS devices into your own web pages.

# Installation

## Global script

If you are developing a "plain" website (without advanced JavaScript tooling),
this is for you. Download `cloudpilot-embedded.zip` from [github
releases](https://github.com/cloudpilot-emu/cloudpilot-emu/releases). Inside the
archive you will find the following files:

* `cloudpilot-emu.js`: The main JavaScript bundle
* `cloudpilot-emu.d.ts`: Type declarations (in case you are using TypeScript)
* `cloudpilot_web.wasm`: The compiled web assembly bytecode for cloudpilot
* `uarm_web.wasm`: The compiled web assembly bytecode for uARM (for OS5 support)
* `uarm-worker.js`: The web worker for OS5 support.
* `pcm-worklet.js`: The PCM worklet for OS5 PCM audio support. 

Drop the Javascript and web assembly files next to your web page and
include the main JavaScript bundle with a script tag in your HTML code.

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

Please note that you still need to serve the web assembly binaries and worker
files next to your application in order to actually run the emulator and get OS5
support. You can either download these separately (see above) or copy them from
the `dist` folder of the NPM package.

# Usage

## Examples

The [examples page](./examples) has a comprehensive list of examples that show
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

    await emulator.loadSession(sessionFile);
    
    emulator.setCanvas(canvas);
    emulator.bindInput(canvas, canvas);
        
    await emulator.resume();
};

main().catch(e => console.error(e));
```

### `createEmulator`

[cloudpilot.createEmulator](./reference/functions/createEmulator.html) loads the
web assembly module, compiles it and returns a promise for an instance of the
emulator. On modern browsers you can use `async/await` to handle the promise. As
an alternative (or to support legacy browsers) you can use the promise API

```javascript
cloudpilot.createEmulator().then(emulator => {
    // ...
});

```

Without any arguments, `createEmulator` assumes that the web assembly binaries are
hosted next to your HTML file. Please check the docs if you need to specify a
separate URL. For OS5 support, the compiled worker and worklet binaries (see
above) **have** to be hosted next to the HTML or script that loads the emulator.

If you want to create multiple instances of the emulator, you can use
`createEmulatorFactory` to create a factory function. Each time you call the
factory function you'll get a promise for a new emulator instance. But the web
assembly binary will only be downloaded and compiled once.

Support for PalmOS 5 emulator requires the uARM WebAssembly binary. By default,
this is loaded lazily when the emulator is launched, but you can force
CloudpilotEmu to load it in advance by specifying the `{preloadUarm: true}`
option.

All interactions with the emulator go through methods on the [emulator
instance](./reference/interfaces/Emulator.html). Most of those methods return
the same instance of the emulator, so calls can be chained. Errors are
communicated via exceptions.

### Asynchronous methods

Since CloudpilotEmu 2.0, most methods that interact with the emulator are async
and return promises. The library serializes all calls to those methods, so there
is no danger of race conditions if the promise is not awaited. However, you
should always await the result for proper error handling and to be sure that the
operation has completed before continuing.

### `loadSession`, `loadRom`

Session and rom files need to be provided as `Uint8Array` typed arrays.
[loadSession](./reference/interfaces/Emulator.html#loadSession) loads a
session image, [loadRom](./reference/interfaces/Emulator.html#loadRom) loads a
rom file. On success, the emulator is initialized but stopped.

`loadRom` will try to autodetect the device type from the ROM. If you want to
force a specific choice, you can do so with a second argument.

```javascript
await emulator.loadRom(romFile, cloudpilot.DeviceId.m515)
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
want to process keyboard events on it.

You can stop listening for events by calling
[releaseInput](./reference/interfaces/Emulator.html#releaseInput).

#### `resume`

Calling [resume](./reference/interfaces/Emulator.html#resume) will resume
emulation. You can call [pause](./reference/interfaces/Emulator.html#resume)
in order to pause it again. The result of those calls should be awaited.

The default for CloudpilotEmu is to stop the emulator when the tab or window is
backgrounded. You can call
[setRunHidden](./reference/interfaces/Emulator.html#setRunHidden) in order to
run the emulator even if the window is not currently visible.

## ROM or session file?

Starting CloudpilotEmu with a ROM file will boot the device and go through the
initial setup process (digitizer calibration, locale, etc.).

If you are embedding the emulator to run a particular application, then
you are better off preparing a pre-booted session in CloudpilotEmu and using
that with the embedded emulator. Note that you don't have to include all
possible application that you might want to run in the session --- you can
install the relevant applications after initializing the emulator (see below).

## Installing and launching applications

You can install and launch applications programmatically, either individual
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
* [launchDatabase](./reference/interfaces/Emulator.html#launchDatabase)
  extracts the name from a database and attempts to locate it on the device and
  launch it.
* [launchByName](./reference/interfaces/Emulator.html#launchByName) searches
  for a database with a particular name on the device and attempts to launch it.

Again, all those methods return promises that should be awaited.

Installing and launching applications is only possible if the device is powered
on and has finished booting. You can ensure that this is the case by using a
session file of a booted device instead of a plain ROM file for the emulator.

**WARNING:** Installing and launching programs calls into PalmOS. This has the
potential of crashing the device under rare circumstances. Calling those methods
while the launcher is running should be fine. In order to be 100% safe from bad
surprises, always call those methods before the emulator is resumed --- this way
the call always finds the emulator in the same state.

## Modifying the hotsync user name (OS4 and earlier only)

The hotsync user name was commonly used by software for its registration
process. On real devices the hotsync name is set during the first hotsync.
CloudpilotEmu allows you to change the user name by calling
[setHotsyncName](./reference/interfaces/Emulator.html#setHotsyncName).
This method returns a promise that should be awaited.

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

Note that PCM audio on OS5 requires that the emulator is served via HTTPS due to
DOM API restrictions.

## Game input mode (OS4 and earlier only)

Just like with the CloudpilotEmu web app, keyboard input is usually converted
into keyboard events for PalmOS. By pressing shift-ctrl the user can enter "game
mode". In game mode, various buttons are mapped to the hardware buttons instead:

* **wasd/qe:** The w/a/s/d buttons control up/cal/down/notes and the q/e buttons
  control contacts/todo.
* **uhjk/uo:** The same as wasd/qe, but shifted to the right of the keyboard.
* **up/down/left/right**: These buttons control up/down/cal/notes.
* **zxcv/yxcv**: z/x/c/v (or y/x/c/v) control cal/contacts/todo/notes.

While game mode is active, a small overlay is shown in the bottom right corner
of the emulator. Game mode and the overlay can be controlled (and permanently
disabled) by various methods on the emulator object; check the [reference
documentation](./reference/interfaces/Emulator.html#setGameMode) for more details.

Keyboard input is not supported for OS5 devices, and for those game mappings are
always active.

## Creating a session file

Session files can be created in CloudpilotEmu on the sessions tab. Either right
click on a session and select "save" or swipe the session to the right and
select the icon with the box and the arrow. A dialog will open that allows you
save a session snapshot.

## Using SD cards and memory sticks

The emulator supports attaching SD card and memory stick images into devices
that support them. Both plain and gzipped images are supported. SD card images
can be of pretty much any size (as long as it is a multiple of 512 bytes),
but memory stick images can only come in very specific sizes.

Images can be created and exported in CloudpilotEmu and exported from there,
but you can also use images taken from from actual, physical media. The relevant
API is:

 * [insertCompressedCardImage](./reference/interfaces/Emulator.html#insertCompressedCardImage)
   insert and mount a gzipped memory card image
 * [insertCardImage](./reference/interfaces/Emulator.html#insertCardImage)
   insert and mount a plain memory card image
 * [ejectCard](./reference/interfaces/Emulator.html#ejectCard)
   eject a previously mounted card
*  [isCardMounted](./reference/interfaces/Emulator.html#isCardMounted)
   check whether a card is currently mounted

All of these return promises that should be awaited.

## Serial port and IrDA (OS4 and earlier only)

CloudpilotEmu embedded exposes the serial port of the emulated device. This
can be used to communicate with an application running on the device via the
serial port, or to link two emulator instances via IrDA (which enables beaming
between those instances).

Note that IrDA requires switching the serial ports
into a special 'sync' mode which synchronizes execution between the two
instances while the data transfer is in progress. This is done by stalling
the emulator while it is waiting for data and allows for reliable IrDA
communication over network connections.

The [serial port example](./examples/serial.html) shows how to connect the
serial port to [xterm.js](http://xtermjs.org) in order to communicate with
uClinux running on the virtual device.
The [two instance example](./examples/two-instances.html) shows how to connect
two instances of the emulator via IrDA. Please refer to the API documentation
for more details.

Serial communication is not supported for OS5.

## Sizing and styling the canvas

When applying CSS to the canvas element you must make sure not to touch
`border`, `padding`, `box-sizing` and `object-fit`. Messing with those breaks
CloudpilotEmu's handling of pointer events. If you need border or padding, wrap the
canvas in a containing element and apply those properties on the container
instead.

Apart from this restriction you are free to style CloudpilotEmu as you like.
Regardless of the layout dimensions and aspect of the canvas, the virtual Palm
will always be rendered at the maximum size possible and centered, and the
surrounding area will be transparent and invisible for pointer events.

CloudpilotEmu renders at a high resolution internally and lets the browser do the
(down)scaling. Non-integer scaling adds a slight blur to the pixelated pilot
screen. If you want a fully crisp image instead you can set
`image-rendering` to either `pixelated` or `crisp`. Be aware, however, that this
will cause aliasing artifacts (non-uniformly sized pixels) and will degrade the
visual quality of the graffiti area and button controls.
