# 1.5.4

-   Fix a regression that prevented the overwriting existing RAM DBs during
    PRC installation.
-   Support additional network APIs (NetlibSendPB, NetlibReceivePB).

# 1.5.3

-   Fix an emulator crash on the execution of invalid code.

# 1.5.2

-   Proxy Server: fix nonblocking sockets. This avoids spurious error messages during
    hotsync and potential networking issues.

# 1.5.1

-   Improve dark mode on non-iOS.
-   Limit width of session list.

# 1.5.0

-   Work around worker update idosyncrasies on iOS.
-   Networking support via websocket proxy. Check the
    [online documentation](https://github.com/cloudpilot-emu/cloudpilot/blob/master/doc/networking.md)
    for more details.
-   Improve memory safety of the emulation core.
-   Add an option to keep Cloudpilot running when the tab is hidden.
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

-   Work around an IndexedDB bug on iOS 14.6 that causes spurious hangs if Cloudpilot
    is started from the homescreen. The workaround may delay startup by a second
    or so if the bug is triggered, but at least Cloudpilot starts reliably again.

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
