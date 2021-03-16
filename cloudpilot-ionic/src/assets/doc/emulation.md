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

The device can be reset by selecting one of the reset options from the menu on
the top left. This is equivalent to pushing a pin into the reset hole on a real
device The different options correspond to different key combination held while
the device resets.

# Audio

The speaker button in the upper right corner allows to toggle audio emulation on / off.
Cloudpilot always starts with audio disabled, and the icon must be tapped to enable
it.

The volume can be changed on the settings tab. The speaker button is not shown if the
volume is set to zero.

On iOS, muting the phone also mutes audio from the emulator.

### Reset

No key held. This causes a plain reset.

### Reset (no extensions)

This corresponds to holding "down" while resetting
the deviCe and will disable the loading of system extensions during startup.
This is very useful to recover from a bad hack or system extension.

### Hard reset

Corresponds to "power" being held during reset. This will
trigger a prompt that will allow you to factory reset the device.
