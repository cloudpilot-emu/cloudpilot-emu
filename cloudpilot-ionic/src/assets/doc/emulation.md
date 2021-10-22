The "Emulation" tab displays your emulated device. The device is running as
long as the tab is visible. Once the tab is switched (or Cloudpilot goes into
the background) the emulation pauses. Date and time are synced to the host
system, so the clock is not affected by the emulator pausing and resuming.

# Interacting with the emulator

You can interact with the emulator by touching or clicking the screen and the
hardware buttons below. The power button is located in the menu on the top
left.

A hardware keyboard can be used to type directly in the emulator (see "Keyboard
mappings" at the end of this page).

On browsers that support the necessary APIs the clipboard can be shared with
the host. Check "Clipboard integration" below for more details.

# Locking the UI

By default the navigation at the bottom remains visible when the emulator is
running. This may cause your fingers to accidentially touch the navigation
if you are interacting with your pilot.

You can touch the lock item at the top in order to hide the navigation
and make room for your fingers. You can toggle the "Auto-lock UI" setting
on the settings page in order to do that automatically on launch.

# Installing program and database files

Select the "Install file" button on the top right in order to install `.prc` and
`.pdb` files into the emulator. You can select and install multiple files at
once. `.zip` archives can be selected as well, and Cloudpilot will install
all installable files it finds in the archive.

Please note that the launcher needs to be restarted by switching to another
program and back to the launcher in order for newly installed programs to become
visible. On Palm OS 3.5 and above it is sufficient to switch the launcher
category.

On rare ocassions the installation catches PalmOS on the wrong foot and
crashes the emulator. In this case you can simply resume the session and
retry the installation.

# State saves

Cloudpilot automatically saves the emulator state every second while the
emulator is running. If Cloudpilot is reloaded (or the session is switched) the
state is preserved and restored.

# Saving a backup

You can save a backup of all databases in the device RAM by selecting "Save Backup"
from the menu. This will create and save a zip archive with .prc and .pdb
files that can be installed again on any PalmOS device at a later point.

There are two types of backup, and Cloudpilot will prompt you to choose between them:

* Backup only RAM databases. This is an ordinary backup, and you should choose it use unless
  you specifically want to retrieve the databases in the device ROM.
* Include ROM databases. This will include the contents of the device ROM with the
  backup. This is not required for a normal backup.

Databases must be closed in order to back up, so if you are backing up while
an application is opened, the corresponding DB files will not be included
in the backup. Cloudpilot will notify you if this happens.

Backing up databases interacts with PalmOS, and there is a (very) slim chance of crashing
the emulator, depending on the state of the virtual device when the backup is
run. In general it is best to run the backup while the launcher is open.

This feature only is available on PalmOS 3 and later.

# Device reset

The device can be reset by selecting "reset" from the menu.
This is equivalent to pushing a pin into the reset hole on a real
device. The different options correspond to different key combinations held while
the device resets.

### Reset

No key held. This causes a plain reset.

### Reset (no extensions)

This corresponds to holding "down" while resetting
the deviCe and will disable the loading of system extensions during startup.
This is very useful to recover from a bad hack or system extension.

### Hard reset

Corresponds to "power" being held during reset. This will
trigger a prompt that will allow you to factory reset the device.

# Power button

The power button can be pressed by selecting "Power" from the menu.

# Hotsync button

The hotsync button on the cradle can be pressed by selecting "Hotsync" from the menu.

# Audio

The speaker setting in the menu allows to toggle audio emulation on / off.
Cloudpilot always starts with audio disabled, and the icon must be tapped to enable
it.

The volume can be changed on the settings tab. The speaker button is not shown if the
volume is set to zero.

On iOS, muting the phone also mutes audio from the emulator. Ocassionally the audio
may glitch and stay off after resuming Cloudpilot from the background due to browser
bugs. You can reenable audio by turning it off and on again.

# Keyboard input

If you are using Cloudpilot on a device with a keyboard you can use the keyboard to
control the emulator. There are two different modes of keyboard input, game mode
and normal mode. In normal mode the keyboard can be used to enter text on
the Palm, and in game mode it controls the hardware buttons.

Game mode can be toggled by pressing `shift-ctrl`. A gamepad icon will appear at the top
while game mode is active. In normal mode the game mode mappings are accessible by holding `ctrl`.

In both modes `page up` and `page down` control the up / down buttons of the device.

### Game mode mappings

In game mode there are several blocks on the keyboard that correspond to the
hardware buttons

* **wasd/qe:**
  The w/a/s/d buttons control up/cal/down/notes and the q/e buttons control
  contacs/todo.
* **uhjk/uo:**
  The same as wasd/qe, but shifted to the right of the keyboard.
* **up/down/left/right**:
  These buttons control up/down/cal/notes.
* **zxcv/yxcv**:
  z/x/c/v (or y/x/c/v) control cal/contacs/todo/notes.

If unsure take a look at the buttons on the silkscreen --- their background will change
while the corresponding button is pressed.

# Clipboard integration

On browsers that support it the clipboard can be shared with the host. This is disabled
by default and needs to be turned on the settings page. Note that the option is not
available on browsers that do not support it (i.e. Firefox).

Once the setting is turned on copy and paste operations will access the host clipboard.
Accessing the clipboard is a sensitive operation, and browsers implement security
mechanisms around it.

On most webkit-based browsers (including Chrome) a popup asking for permission must be
confirmed the first time Cloudpilot pastes from the host clipboard. On other browsers
(notably Safari) Cloudpilot shows a dialog asking to confirm the operation on every
clipboard access. This is necessary because those browsers require clipboard access to
be the direct consequence of an user interaction. When pasting Safari will show
an additionl "Paste" widget that needs to be tapped in order to paste.

# Statistics

**WARNING** Technical details ahead. This is a technical diagnostics feature and not
required for normal operation. You can skip the remainder of this documentation page unless
you want to interpret the statistics overlay.

If "Show Statistics" is enabled (on the settings tab) Cloudpilot displays an overlay with
performance statistics above the grafitti silkscreen. The statistics are updated
whenever a new state snapshot is taken.

### Snapshot statistics

State snapshots are taken every second while the emulator is running and preserve
the current state of the emulator in the browser's IndexedDB. In order to
reduce the amount of data saved memory is divided into pages of 1k, and only the
pages that have been modfied are saved.

 * **last snapshot**: This is the time at wbich the last snapshot was taken.
 * **snapshot pages**: The number of pages updated in this snapshot.
 * **snapshot time total**: The total amount of time the snapshot took to complete.
   Most of this time is spent by the browser on a separate thread and does not block
   emulation, so this number does not have a direct impact on performance.
 * **snapshot time blocking**: The amount of time that was spent blocking on the main
   thread. During this time the emulator cannot execute. If this value is higher than
   the time for a single frame (typically 1/60th second) the snapshot will cause a
   slight frame drop when it executes.

### Emulation statistics

The CPU load of the emulator can easily vary by orders of magnitude depending on
the code that is currently executing. If the device sleeps the emulator will
consume very little CPU as well. If the emulator is busy executing code and shoveling
data around the CPU load on the host device increases as well.

While all supported iOS devices are fast enough to execute the emulator at full
speed at all times, some (in particular older) Android devices are not able to
keep up all the time. Cloudpilot compensates this by monitoring the "host speed"
(the ratio between the real time required to emulate a chunk of m68k code relative
to the corresponding time in the frame of the virtual device). If this drops below one
Cloudpilot will reduce the clock of the emulated device accordingly while keeping the
timers in sync with real time.

 * **host speed**: See above. Note that this is calculated using the true clock speed
   of the virtual device (ignoring the correction that is applied if host speed
   drops below one).
 * **emulation speed**: The ratio between the actual clock of the emulated device and
   the clock at which it is supposed to run. This drops below 1 if Cloudpilot reduces
   speed in order to compensate for a slow host.
 * **average FPS**: The average frames per second at which the emulator runs.
   This is usually identical to the refresh rate of the display (usually 60Hz), but it
   will drop if the host device cannot keep up. Note that the screen is not actually
   updated during every frame; Cloudpilot redraws only if the display content has
   changed.
