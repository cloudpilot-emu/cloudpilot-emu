Cloudpilot is a web-based emulator for PalmOS. It emulates dragonball based devices
and supports PalmOS up to version 4.x. Cloudpilot is based on the original POSE
emulator and currently emulates Palm IIIc, Palm V and Palm m515 devices.

# Usage

Cloudpilot's interface is divided in tabs.
Each tab has a dedicated help page that can be accessed by tapping the "?" in
the toolbar (or selecting "Help" from the menu on the emulation page).

### Sessions tab

Navigate to the sessions tab in order to create, manage and launch emulation sessions.
New emulation sessions are created either by loading a ROM or a by loading a
previously saved session snapshot.

### Emulation tab

The emulator tab hosts the emulated device.

### Settings tab

The settings tab allows you to adjust the emulator settings.

# Reloads and emulation state

Cloutpilot automatically saves the state of the emulated device every second while
the while the emulation is running. If Cloudpilot terminates and restart the emulation
will resume exaclty where you left off.

# Installing programs and databases

Cloudpilot cannot hotsync, but program (.prc) and database (.pdb)
files can be installed directly into the emulated device.

# Backing up your virtual devices

Saving a session snapshot on the Sessions tab every now and then will protect
your data in case anything goes wrong.

# Handling fatal errors

The emulator is pretty stable, but you may encounter situations in which
it crashes. This may be due to a bug in the emulator, or due to the way
POSE and Cloudpilot interact with PalmOS for installing software,
setting the hotsync name and similar tasks.

If the emulator crashes Cloudpilot will stop immediatelly and show you a dialog
that allows you to restart the emulator. Upon restart Cloudpilot will not
automatically resume your session but will instead start up with the session
page. You can resume your session from there.

### Escaping a crash loop

In the unlikely event that the emulated device keeps on
crashing the emulator you can reset the device directly on the
sessions page (see the help page there for more info). This will remove the
hardware state but preserve the RAM, resulting in a reboot when you resume your
session. This allows you to break the crash loop.

# Requirements and browser support

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

# Updates

Cloudpilot caches all its files locally and does not require a network connection
to run once the caching is complete.

During startup Cloudpilot checks for new versions. If a new version is available
Cloudpilot will download it and inform you with a notification. At this point
reloading Cloudpilot will apply the update.

In order to force check for an update terminate and restart the app.

# Known issues

-   iOS: changing device orientation between portrait and landscape may mess
    up the layout of the app. This is an iOS bug that can be worked around by
    rotating the device by 180° in portrait and rotating back.
-   Audio timing is not perfect and processed at the refresh rate of the emulator
    (usually the same as the screen refresh rate of the host device). Sound
    effects that rely on quickly modulating the audio signal may not be
    reproduced correctly.
-   On rare ocassions installing files or changing the hotsync name can catch
    PalmOS on the wrong foot and crash the emulator. In particular, this happens
    if a file is installed on a virtual Palm IIIc with PalmOS 3.5 while the
    launcher is active and the category menu is open.

# Cloudpilot on the web

 * [Source code](https://github.com/cloudpilot-emu/cloudpilot)
 * [Webpage](https://cloudpilot-emu.github.io)

# License

Cloudpilot is free software: you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later
version.
