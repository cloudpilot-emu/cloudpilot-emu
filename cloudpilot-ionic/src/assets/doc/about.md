CloudpilotEmu (or just Cloudpilot) is a web-based emulator for PalmOS. It
emulates dragonball based devices and supports PalmOS up to version 4.x. The
emulator is based on the original POSE. Currently, the following devices are
emulated:

-   Pilot, Palm Pilot
-   Palm III
-   Palm IIIxe, Palm IIIx, Palm IIIe
-   Palm Vx, Palm V
-   Palm VII, Palm VII EZ, Palm VIIx
-   Palm m500, Palm m505, Palm m515
-   Palm m100, Palm m105, Palm m125, Palm m130
-   Palm i705
-   Tungsten W (silkscreen version)
-   Handera 330
-   Sony PEG-S300, PEG-S320
-   Sony PEG-T400 series
-   Sony PEG-N600C series
-   Sony PEG-T600C series
-   Sony PEG-N700C series

The coorrect device type is automatically determined from the ROM you load. If
there are multiple possible devices CloudpilotEmu will let you choose when you
create the session.

# Tabs

The interface is divided in tabs. Each tab has a dedicated help page that can be
accessed by tapping the "?" in the toolbar (or selecting "Help" from the menu on
the emulation page).

### Sessions tab

Navigate to the sessions tab in order to create, manage and launch emulation sessions.
New emulation sessions are created either by loading a ROM or a by loading a
previously saved session snapshot.

### Emulation tab

The emulation tab hosts the emulated device.

### Settings tab

The settings tab allows you to adjust the emulator settings.

# Reloads and emulation state

The emulator automatically saves the state of the emulated device every second while
the while the emulation is running. If CloudpilotEmu terminates and restarts the emulation
will resume exactly where you left off.

# Installing programs and databases

Program (.prc) and database (.pdb) files can be installed directly into the emulated
device. `.zip` archives containing such files can be installed as well.

# Network support

CloudpilotEmu can connect the virtual PalmOS device to the network via
a proxy server. This configuration supports network hotsync. Please check the
[online documentation](https://github.com/cloudpilot-emu/cloudpilot-emu/blob/master/doc/networking.md)
for detailed instructions.

# Backing up your virtual devices

Virtual devices can be backed up by creating either session snapshots
or DB backups. The latter feature is not available on PalmOS 2 and earlier.

### Device snapshots

Saving a session snapshot on the Sessions tab every now and then will protect
your data in case anything goes wrong. Re-importing a session snapshot
will create a clone of the device at the moment where it was backed up.

### DB backups

In addition to full device snapshots you can export a zip archive on the
emulation tab. This archive contains all databases and applications in device RAM
as portable .prc and .pdb files.

# Handling fatal errors

The emulator is pretty stable, but you may encounter situations in which
it crashes. This may be due to a bug in the emulator, or due to the way
POSE and CloudpilotEmu interact with PalmOS for installing software,
backing up databases, setting the hotsync name and similar tasks.

If the emulator crashes CloudpilotEmu will stop immediatelly and show you a dialog
that allows you to restart the emulator. Upon restart CloudpilotEmu will not
automatically resume your session but will instead start up with the session
page. You can resume your session from there.

### Escaping a crash loop

In the unlikely event that the emulated device keeps
crashing the emulator you can reset the device directly on the
sessions page (see the help page there for more info). This will remove the
hardware state but preserve the RAM, resulting in a reboot when you resume your
session. This allows you to break the crash loop.

# Requirements and browser support

CloudpilotEmu runs on all reasonable recent browsers and can be added to the homescreen
of Android and iOS devices as a mobile app. For iPhones and iPads this is highly
recommended as Safari may clear the data of ordinary web sites if they are not used
for an extended period of time.

On iOS devices iOS 14 or higher is recommended. Earlier versions have bugs that
may make it impossible to save session snapshots. The emulator runs smoothly on all
iPhones and iPads that support iOS 14.

On older Android devices performance may be an issue. The emulator requires decent
single core performance, and this is a domain where Android devices are traditionally
lacking. CloudpilotEmu will dynamically adjust the speed of the emulated device if
the host device cannot sustain emulation at full speed. A score of 350 in the single
core [geekbench benchmark](https://browser.geekbench.com/android-benchmarks) is
enough to run CloudpilotEmu smoothly with minor slowdowns, and 500
or more will give smooth emulation without slowdowns.

# Updates

CloudpilotEmu caches all its files locally and does not require a network connection
to run once the caching is complete.

During startup CloudpilotEmu checks for new versions. If a new version is available
CloudpilotEmu will download it and inform you with a notification. After this point
reloading will apply the update.

In order to force check for an update terminate and restart the app.

# Known issues and limitations

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
-   The power switch on the Tungsten W andd i705 does not operate correctly and
    is disabled. You'll have to do a reset in order to wake up the device if you
    acidentially power off via software.
-   On Clié devices, audio is not emulated beyond the usual beeps.
-   External memory (CF card, SD card, memory stick) is not supported

# CloudpilotEmu on the web

 * [Source code](https://github.com/cloudpilot-emu/cloudpilot-emu)
 * [Webpage](https://cloudpilot-emu.github.io)

# License

CloudpilotEmu is free software: you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later
version.
