# Next version

-   Support memory cards in CP embedded.
-   Better 4bit to 8bit color space transform for SED1375 based devices
    (brighter colors for Palm IIIc and Clié PEG-S500C).
-   Support gzip compressin for importing and exporting memory cards.

# Version 1.8.0

This is a major update that adds support for SD card and memory sticks.
Existing devices with a memory stick slot may require a reboot to start
using the emulated memory stick hardware.

-   Support for SD cards and memory sticks on all devices that were
    produced with the corresponding slots.
-   File manager for memory cards.
-   Support dropping files for importing sessions, installing files,
    and managing memory cards.
-   Support for HandEra 330c (sic!).
-   Support for Palm m520 prototype.
-   Support for Acer S11.
-   Bug fixes and UI rework.

# Version 1.7.13

This release changes the way `cloudpilot-server` generates its certificates. The
new setup generates both a root and a leaf certificate and has much better
compatibility with various browser / OS combinations. In particular, Firefox
and recent versions of Chrome reject the self-signed certificate that was
previously generated, but work fine with the new setup. You can continue to use your
current certificates as long as they work, but check out the online
[networking documentation](https://github.com/cloudpilot-emu/cloudpilot-emu/blob/master/doc/networking.md)
in order to generate new certificates.

-   Change proxy server to generate a full certificate chain with a root and a leaf
    certificate.
-   Add a proper index page to the proxy server.
-   Configurable certificate lifetime.

# Version 1.7.12

-   Add documentation to server and network docs to address recent issues
    with Chrome.

# Version 1.7.11

-   Fix display issues in some skins in Safari and on iOS.

# Version 1.7.10

-   Fix a heap overflow.

# Version 1.7.9

-   Fix status bar color in light mode on iOS.
-   Happy 2023.

# Version 1.7.8

-   Minor wording fixes.

# Version 1.7.7

-   Fix a bug that might have caused data corruption if a second instance
    of CP is opened in another window.

# Version 1.7.6

-   Fix a heap overflow.
-   Minor UI tuning.

# Version 1.7.5

-   Fix a UI regression.

# Version 1.7.4

-   More safe area workarounds for iPhone X and later.

# Version 1.7.3

-   UI fixes for iPhones with rounded corners.

# Version 1.7.2

-   Support database backups on PalmOS 1 and 2.
-   Spelling fixes.
-   CP embedded: add a note on styling to the documentation.

# Version 1.7.1

-   Swiping left and right of the emulated device on iOS does not "drag" the
    screen anymore
-   CP embedded: only cancel touch interactions that are actually handled
    by CloudpilotEmu. This enables scrolling by swiping over empty parts of
    the canvas on touch devices

# Version 1.7.0

-   Introduce CloudpilotEmu embedded. You can now embed cloudpilot into your own web pages.
    Check out the [online documentation](https://cloudpilot-emu.github.io/embedded/)
    for more details.
-   Minor bug fixes

# Version 1.6.11

-   Use storage manager API to request persistent storage on browsers that
    support it. This keeps browsers from deleting session data under certain
    conditions.

# Version 1.6.10

-   Support pressing multiple buttons with one finger on touch devices
    by touching the first button and then moving over the other button.
-   Fix RTC interrupts on SZ devices (PEG-T675C and PEG-NR70).

# Version 1.6.9

-   Add "Enable audio on start" option.
-   Save and restore speed and orientation with session images.
-   Compress session images.

# Version 1.6.8

-   Allow to change device orientation (portrait, landscape 90°/270°, upside down).
-   Fix speed change being ignored on import.

# Version 1.6.7

-   Minor fixes + performance improvements.

# Version 1.6.6

-   Fix an ocassional crash on boot introduced in 1.6.5

# Version 1.6.5

-   Support for PEG-NR70 and PEG-T650C series devices.
-   Support statistics display on soft silkscreen devices.
-   Support for PEG-S500C series devices.
-   Improved performance of framebuffer copies.
-   Add session settings to emulation menu.
-   Add a speed slider to session settings.

# Version 1.6.4

-   More wording.

# Version 1.6.3

-   A few more wording changes.

# Version 1.6.2

-   Rename to CloudpilotEmu.

# Version 1.6.1

-   Fix PEG-T4xx skin.

# Version 1.6.0

-   Emulate date rollover interrupts. This means that the virtual Pilot will now
    schedule caledar alarms correctly. Virtual devices may hang for a few seconds
    on the first launch after this update while PalmOS processes all the alarms
    that it missed.
-   Optimizations that give 10% -- 30% performance improvement
-   Support for serveral Clié devices:
    -   Sony PEG-S300, PEG-S320
    -   Sony PEG-T400 series
    -   Sony PEG-N600C series
    -   Sony PEG-T600C series
    -   Sony PEG-N700C series

# Version 1.5.8

-   Link API for importing ROMs or sessions and installing databases via
    hyperlinks. Check the
    [online documentation](https://github.com/cloudpilot-emu/cloudpilot-emu/blob/master/doc/linking.md)
    for more details
-   UI dependecy updates

# Version 1.5.7

-   Catch a rare iOS bug that causes the database to ocassionally come up empty
    on launch.
-   Installation of databases from zip archives.
-   Show reason for failed installations.
-   Improve snapshot performance on Chrome.
-   Fix basic auth handling in poxy to conform with standard. This version of
    CloudpilotEmu requires an updated version of the proxy if basic auth is used
    to secure the connection.

# Version 1.5.6

-   Support Handera 330.
-   Improve snapshot performance for devices with framebuffer (color devices).
    Session images saved with this and later versions of Cloupilot cannot be
    loaded on older versions.
-   Allow backups that include the databases in the device ROM.
-   Reboot device if PalmOS crashes with an invalid stack pointer (instead of dying
    with an assertion or freezing like a real device).

# Version 1.5.5

-   Finalize networking support. This release requires an updated proxy server.

# Version 1.5.4

-   Fix a regression that prevented the overwriting existing RAM DBs during
    PRC installation.
-   Support additional network APIs (NetlibSendPB, NetlibReceivePB).
-   Minor UI fixes.

# Version 1.5.3

-   Fix an emulator crash on the execution of invalid code.

# Version 1.5.2

-   Proxy Server: fix nonblocking sockets. This avoids spurious error messages during
    hotsync and potential networking issues.

# Version 1.5.1

-   Improve dark mode on non-iOS.
-   Limit width of session list.

# Version 1.5.0

-   Work around worker update idosyncrasies on iOS.
-   Networking support via websocket proxy. Check the
    [online documentation](https://github.com/cloudpilot-emu/cloudpilot-emu/blob/master/doc/networking.md)
    for more details.
-   Improve memory safety of the emulation core.
-   Add an option to keep CloudpilotEmu running when the tab is hidden.
-   Add "Hotsync" to emulation menu.
-   Route `DbgMessage` calls to console.
-   Set `cldp` feature on boot.
-   Allow to choose boot mode after a forceful reset.
-   Move audio toggle to the emulation menu
-   Add "Lock UI" for hiding the navigation bar while the Pilot
    is running.

# Version 1.4.6

-   Fix installation of DBs that shadow a ROM DB.

# Version 1.4.5

-   Fix a very rare crash on session restore.

# Version 1.4.4

-   Minor bug fixes.

# Version 1.4.3

-   Work around an IndexedDB bug on iOS 14.6 that causes spurious hangs if CloudpilotEmu
    is started from the homescreen. The workaround may delay startup by a second
    or so if the bug is triggered, but at least CloudpilotEmu starts reliably again.

# Version 1.4.2

-   Fix skin for VII EZ / VIIx

# Version 1.4.1

-   Support for i705, VIIx and VII EZ

# Version 1.4.0

-   Imitate greenish LCD screen for grayscale devices
-   Support for PalmPilot, Palm III, Palm IIIx, Palm IIIxe, Palm IIie, Palm Vx,
    Palm m500, Palm m505, Palm m100, Palm m105, Palm m125
-   Minor UI improvements
-   Minor improvements to timer emulation
-   Improved event injection

# Version 1.3.1

-   Fix separator height at 160x160

# Version 1.3.0

-   Handle pqa files in installation and backup.
-   Palm m130 support.
-   Workaround for dying service worker on Safari desktop.
-   Support for Tungsten W.
-   Clipboard integration on browsers that support it.

# Version 1.2.1

-   Fix alignment error when restoring images with odd size.

# Version 1.2.0

-   Backup: export all applications and DBs on a device as
    .prc and .pdb files in a zip archive.
-   Rework file installation, notify if an installed database
    demands for a reset.

# Version 1.1.2

-   Fix a potential lockup condition in touch handling.

# Version 1.1.1

-   Add a note on handling fatal errors to the "About" tab.

# Version 1.1.0

-   Add statistics display.
-   Palm IIIc support.
-   Improved event injection -> no more timing glitches when injecting
    touch events on PalmOS 3.x

# Version 1.0.2

-   Increase IndexedDB timeout in order to avoid IDB timeouts when
    setting up a new ROM on slow devices.

# Version 1.0.1

-   Support controlling hardware buttons with an attached keyboard, game mode.
    Consult the emulation help page for details.
-   Fix potential issues when installing many files at once on slow host devices.
-   Documentation fixes.

# Version 1.0.0

Initial release.
