On the "Sessions" tab you can manage your emulation sessions. Each session
corresponds to a virtual device and its state.

Tap a session in order to launch it in the emulator.

# Adding new sessions

Select the "+" icon in the upper right corner in order to import a ROM or a
session image. The file suffix should be either `.rom`, `.img` or `.bin`.

### ROM files

The emulated device is selected automatically based on the ROM. If the ROM supports
multiple devices Cloudpilot will allow you to choose between them. Please check
the "About" tab for a list of supported devices.

You can find a collection of suitable ROM files
[on PalmDB](https://palmdb.net/app/palm-roms-complete).

### Session images

Session images are session snapshots that have been exported by Cloudpilot. They
contain the ROM image of the exported device together with a snapshot of the
device RAM and state at the moment the snapshot was taken. Importing a session
image will create a clone of the device at the moment in which the snapshot was
taken.

Changes to the image format are backwards compatible, which means that a newer
version of Cloudpilot can always import a session image written by an older
version. However, the opposite is not true, and importing an image created by a
newer version of Cloudpilot may cause the state to be lost during import. The
contents of RAM are still preserved, so loading an incompatible image will
cause a reboot but will preserve all data.

# The sessions list

Selecting a session from the list will launch the session in the emulator and
take you to the emulation tab.

Swiping a session to the left or right will reveal options to save, edit, reset
and delete a session. On devices that have a mouse or trackpad you can also do a
right click on a session in order to open a context menu with the same options.

### Edit

Edit a session in order to change its name and manage hotsync name used by the
session.

##### Hotsync name

Usually Cloudpilot devices do not hotsync, so Cloudpilot can manage the hotsync
user name instead. The hotsync name can be changed at any time.

Older versions of Cloudpilot do not include the hotsync name in session images.
In this case, the name will we read from the device as soon as it is launched.

In case you aer using network hotsync to sync the virtual device with Palm Desktop
this setting will conflict with the desktop. Turn it off by unchecking
"Manage hotsync name".
### Save

Saving a session exports a session snapshot.

### Reset

While the emulation is running, Cloudpilot persists the device RAM and state
every second. Resetting the session will remove the saved state but preserve the
contents of RAM. This forces the device to reboot the next time the session is
launched.
