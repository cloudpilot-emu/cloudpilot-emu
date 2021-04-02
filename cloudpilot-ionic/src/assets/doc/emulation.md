The "Emulation" tab displays your emulated device. The device is running as
long as the tab is visible. Once the tab is switched (or Cloudpilot goes into
the background) the emulation pauses. Date and time are read from the host
system, so the clock is not affected by the emulator pausing and resuming.

# Interacting with the emulator

You can interact with the emulator by touching or clicking the screen and the
hardware buttons below. The power button is located in the menu on the top
lefft.

A hardware keyboard can be used to type directly in the emulator.

# Installing program and database files

Select the "Install file" button on the top right in order to install `.prc` and
`.pdb` files into the emulator. You can select and install multiple files at
once.

Please note that the launcher needs to be restarted by switching to another
program and back to the launcher in order for newly installed programs to become
visible. On Palm OS 3.5 and above it is sufficient to switch the launcher
category.

# State saves

Cloudpilot automatically saves the emulator state every second while the
emulator is running. If Cloudpilot is reloaded (or the session is switched) the
state is preserved and restored.

# Device reset

The device can be reset by selecting one of the reset options from the menu.
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

# The power button

The power button can be pressed by selecting the "Power" entry in the menu.

# Audio

The speaker button at the top allows to toggle audio emulation on / off.
Cloudpilot always starts with audio disabled, and the icon must be tapped to enable
it.

The volume can be changed on the settings tab. The speaker button is not shown if the
volume is set to zero.

On iOS, muting the phone also mutes audio from the emulator.

# Keyboard input

If you are using Cloudpilot on a device with a keyboard you can use the keyboard to
control the emulator. There are two different modes of keyboard input, game mode
and normal mode. In normal mode the keyboard can be used to enter text on
the Palm, and in game mode it controls the hardware buttons.

Game mode can be toggled by pressing `shift-ctrl`. A gamepad icon will appear at the top
while game mode is active. In normal mode the game mode mappings are accessible by holding `ctrl`.

In both modes `page up` and `page down` control the up / down buttons of the device.

## Game mode mappings

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
