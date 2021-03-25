Cloudpilot is a web-based emulator for PalmOS. It emulates dragonball based devices
and supports PalmOS up to version 4.x. Cloudpilot is based on the original POSE
emulator and currently emulates Palm V and Palm m515 devices.

## Usage

Cloudpilot's interface is divided in tabs.
Each tab has a dedicated help page that can be accessed by tapping the "?" in
the toolbar (or selecting "Help" from the menu on the emulation page).

#### Sessions tab

Navigate to the sessions tab in order to create, manage and launch emulation sessions.
New emulation sessions are created either by loading a ROM or a by loading a
previously saved session snapshot.

#### Emulation tab

The emulator tab hosts the emulated device.

#### Settings tab

The settings tab allows you to adjust the emulator settings.

## Reloads and emulation state

Cloutpilot automatically saves the state of the emulated device every second while
the while the emulation is running. If Cloudpilot terminates and restart the emulation
will resume exaclty where you left off.

## Installing programs and databases

Cloudpilot cannot hotsync, but program (.prc) and database (.pdb)
files can be installed directly into the emulated device.

## Backing up your virtual devices

Saving a session snapshot on the Sessions tab every now and then will protect
your data in case anything goes wrong.

## Requirements and browser support

Cloudpilot runs on all reasonable recent browsers and can be added to the homescreen
of Android and iOS devices as a mobile app. For iPhones and iPads this is highly
recommended as Safari may clear the data of ordinary web sites if they are not used
for ab extended period of time.

On iOS devices iOS 14 or higher is recommended. Earlier versions have bugs that
may make it impossible to save session snapshots. The emulator runs smoothly on all
iPhones and iPads that support iOS 14.

On older Android devices performance may be an issue. Cloudpilot requires decent
single core performance, and this is a domain where Android devices are traditionally
lacking. Cloudpilot will dynamically adjust the speed of the emulated device if
the host device cannot sustain emulation at full speed. A score of 350 in the single
core [geekbench benchmark](https://browser.geekbench.com/android-benchmarks) is
enough to run Cloudpilot smoothly with minor slowdowns, and 500
or more will give smooth emulation without slowdowns.

## Updates

Cloudpilot caches all its files locally and does not require a network connection
to run once the caching is complete.

During startup Cloudpilot checks for new versions. If a new version is available
Cloudpilot will download it and inform you with a notification. At this point
reloading Cloudpilot will apply the update.

In order to force check for an update terminate and restart the app.

## Cloudpilot on the web

 * [Source code](https://github.com/cloudpilot-emu/cloudpilot)
 * [Webpage](https://cloudpilot-emu.github.io)

## License

Cloudpilot program is free software: you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later
version.
