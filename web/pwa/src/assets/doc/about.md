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
:::

# Updates

CloudpilotEmu caches all its files locally and does not require a network connection
to run once the caching is complete.

During startup CloudpilotEmu checks for new versions. If a new version is available
CloudpilotEmu will download it and inform you with a notification. After this point
reloading will apply the update.

In order to force check for an update terminate and restart the app.

# Known issues and limitations

:::div{.feature-ios-not-native}
### Homescreen app support on iPhones in the European Union

Starting with iOS 17.4 Apple has removed support
for homescreen apps on iPhones for all users in the European Union. After the update,
homescreen apps will open as tabs in Safari. All data stored by the native app
will be lost. Please back up you sessions
and cards by selecting "Export" from the menu on the respective tabs before
installing the update to iOS 17.4.

After the update, CloudpilotEmu will continue to work fine as a
app in Safari, and you can reimport your previously backed up data. However,
there are several quirks that you should be aware of:

-   **Potential loss of data**: All pages
    in Safari loose their stored data if they are not opened for more than seven
    days. The only way to prevent this reliably is to disable cross site
    tracking protection in Settings, which may be harmful to your
    privacy. Please make sure that you open CloudpilotEmu on a regular basis
    (for example by setting a calendar reminder with a link) in order prevent
    data loss, and save regular backups of your data by exporting sessions and
    cards.

    It is possible to decrease the risk of data loss by going to Settings
    and enabling
    "Safari / Advanced / Feature Flags / Disable removal of non-cookie data
    after 7 days...". However, this setting resets
    and needs to be reenabled if Safari is updated, and Safari may still remove
    data if the app is not opened for more than 30 days.

-   **Browser UI**: Opening as an ordinary web page instead of a homescreen app
    will display the browser tab and navigation bars. This cannot be disabled
    by CloudpilotEmu, but you can tap the "aA" icon left of the URL and select
    "Hide toolbar".

    To complicate things, Safari has a glitch that may display the home
    indicator (the white-ish bar at the bottom of the screen) on top of the UI
    instead below it. Whether you are affected depends on how the toolbar is
    configured in Settings. This is cannot be detected, but there is
    a setting on the settings page that you can change to work around.

I am sorry for the inconvenience, but Apple enforced this change (supposedly to
accommodate the European Digital Markets Act), and there is nothing that I can
do about it. It is possible that the introduction of alternate app stores in the
EU (which has been forced by the DMA, too) will open the possibility of
distributing CloudpilotEmu as an app in the future.
:::

:::div{.feature-ios-or-safari}
### Issues related to iOS or Safari

-   iOS: changing device orientation between portrait and landscape may mess
    up the layout of the app. This is an iOS bug that can be worked around by
    rotating the device by 180° in portrait and rotating back.

-   iOS native app: The app may blink occasionally when it is resumed from the
    background if "enable audio on start" is turned on.

-   Safari, iOS: A browser bug can cause lag when loaders or
    modal dialogs are shown in older versions of iOS or Safari. This can be worked around
    with the setting "Reduce animation".
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
