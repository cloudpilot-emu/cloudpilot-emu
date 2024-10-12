CloudpilotEmu is a web-based emulator for PalmOS. It
emulates dragonball based devices and supports PalmOS up to version 4.x. The
emulator is based on the original POSE. Currently, the following devices are
emulated:

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

The correct device type is automatically determined from the ROM you load. If
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

### Storage cards tab

The cards tab allows you to create and manage virtual storage cards that can be
attached to many PalmOS devices as either SD cards or memory sticks.

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

# SD cards and memory sticks

Many PalmOS devices support removable storage in the for of either SD
cards or memory sticks. CloudpilotEmu allows you to create virtual
memory cards that can be attached as eihter SD cards or memory sticks
to devices that had the corresponding slots. Please see the help on the
"Cards" tab for more details.

# Backing up your virtual devices

Virtual devices can be backed up by creating either session snapshots
or DB backups.

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

If the emulator crashes CloudpilotEmu will stop immediately and show you a dialog
that allows you to restart the emulator. Upon restart CloudpilotEmu will not
automatically resume your session but will instead start up with the session
page. You can resume your session from there.

### Escaping a crash loop

In the unlikely event that the emulated device keeps
crashing the emulator you can reset the device directly on the
sessions page (see the help page there for more info). This will remove the
hardware state but preserve the RAM, resulting in a reboot when you resume your
session. This allows you to break the crash loop.

:::div{.no-feature-native-app}
# Requirements and browser support

CloudpilotEmu runs on all reasonable recent browsers and can be added to the homescreen
of Android and iOS devices as a mobile app. For iPhones and iPads this is highly
recommended as Safari may clear the data of ordinary web sites if they are not used
for more than seven days.

On desktops, Chrome, Edge and Safari allow to install CloudpilotEmu as a standalone
web app. Again, this will keep Safari from deleting data after seven days of inactivity.
:::

# Updates

CloudpilotEmu caches all its files locally and does not require a network connection
to run once the caching is complete.

During startup CloudpilotEmu checks for new versions. If a new version is available
CloudpilotEmu will download it and inform you with a notification. After this point
reloading will apply the update.

In order to force check for an update terminate and restart the app.

If you experience any browser bugs related to the update process such as
repeated update notifications without the "New version..." dialog on reload or
the app not updating to the most recent version, go to the "About" tab, scroll
to the bottom and select the "reset worker" link at the bottom of the page. 

:::div{.feature-native-app}
# Stable and preview builds

CloudpilotEmu is available both as a stable and as a preview version (which eventually
is released as the next stable version). If you are using the native iOS app you can switch
between both versions by opening the "Settings" app, selecting "CloudpilotEmu" and
toggling "Use preview version".

The date of the two different versions is stored separately from each other.
:::

# Known issues and limitations

:::div{.feature-ios-or-safari}
### Issues related to iOS or Safari

-   Safari on iOS: the home indicator (the white-ish bar at the bottom of the
    screen) may display on top of the UI if the toolbar is minimized. This is
    cannot be detected, but there is a setting on the settings page that you can
    change to work around.

-   Safari, iOS: A browser bug can cause lag when loaders or
    modal dialogs are shown in older versions of iOS or Safari. This can be worked around
    with the setting "Reduce animation".

-   Safari: Safari will delete all data stored by the app if the page is not
    opened for more than seven days. The only way to circumvent this is to
    install CloudpilotEmu from the
    [App Store](https://apps.apple.com/de/app/cloudpilotemu/id6478502699)
    or as a homescreen app (on iOS) or to add it to the dock (Safari on MacOS).

-   Safari: On rare occasions there have been issues with Safari handling 
    CloudpilotEmu's service worker. Symptoms of this are either repeated
    (bogus) update notifications without the "New version..." dialog on reload, or
    the app not updating to the most recent version.

    If you suspect this is happening to you, go to the "About" tab, scroll down
    and select the "reset worker" link at the bottom of the page.
:::

:::div{.feature-native-app}
### Issues on iOS

-   Loaders and modal dialogs may be laggy on older versions of iOS. This can be worked around
    with the setting "Reduce animation".
-   The app may refuse to load without internet connectivity. This will be fixed in
    in the next version of the app.
:::

### Emulation issues

-   Audio timing is not perfect and processed at the refresh rate of the emulator
    (usually the same as the screen refresh rate of the host device). Sound
    effects that rely on quickly modulating the audio signal may not be
    reproduced correctly.

-   On rare ocassions installing files or changing the hotsync name can catch
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

# CloudpilotEmu on the web

 * [Source code](https://github.com/cloudpilot-emu/cloudpilot-emu)
 * [Webpage](https://cloudpilot-emu.github.io)

# License

CloudpilotEmu is free software: you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later
version.
