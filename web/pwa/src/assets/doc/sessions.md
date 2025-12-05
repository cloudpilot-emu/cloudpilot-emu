On the "Sessions" tab you can manage your emulation sessions. Each session
corresponds to a virtual device and its state.

Tap a session in order to launch it in the emulator.

# Adding new sessions

Select the "+" icon in the upper right corner in order to import a ROM or a
session image. The file suffix should be either `.rom`, `.img` or `.bin`.

You can also drag and drop the file that you want to import to the sessions tab.

### ROM files

The emulated device is selected automatically based on the ROM. If the ROM supports
multiple devices CloudpilotEmu will allow you to choose between them. Please check
the "About" tab for a list of supported devices.

You can find a collection of suitable ROM files
[on PalmDB](https://palmdb.net/app/palm-roms-complete).

### Session images

Session images are session snapshots that have been exported by CloudpilotEmu. They
contain the ROM image of the exported device together with a snapshot of the
device RAM and state at the moment the snapshot was taken. Importing a session
image will create a clone of the device.

Changes to the image format are backwards compatible, which means that a newer
version of CloudpilotEmu can always import a session image written by an older
version.

# The sessions list

Selecting a session from the list will launch the session in the emulator and
take you to the emulation tab.

Swiping a session to the left or right will reveal options to export, edit, reset
and delete a session. On devices that have a mouse or trackpad you can also do a
right click on a session in order to open a context menu with the same options.

### Session settings

##### Device type

This setting lets you choose between the different device types supported by a ROM
on import. After import this field is read only.

##### Orientation

This allows you to rotate the virtual device.

##### Speed (OS<=4)

This allows you to adjust the speed of your virtual device. This setting will affect
the speed of the emulated CPU but leave the timers intact.

##### Hotsync name (OS<=4)

Usually CloudpilotEmu devices do not hotsync, so CloudpilotEmu can manage the hotsync
user name instead. The hotsync name can be changed at any time.

Older versions of CloudpilotEmu do not include the hotsync name in session images.
In this case, the name will we read from the device as soon as it is launched.

##### Manage hotsync name (OS<=4)

In case you are using network hotsync to sync the virtual device with Palm Desktop
this setting will conflict with the desktop. Turn it off by unchecking
"Manage hotsync name".

##### NAND (OS5)

PalmOS 5 devices contain NAND flash, which contains some of the preloaded apps
on the original Tungsten E2 ROM. You can provide a NAND flash image by
tapping this setting and loading the image. If you leave NAND blank PalmOS
will format it on first boot.

##### Performance (OS5)

For PalmOS 5 you can configure the following options

* Target speed: the emulator will run at this speed, or at the maximum speed that
  can be sustained by the host.
* Minimum speed: CloudpilotEmu will signal a slowdown in the toolbar if performance
  falls under the specified percentage of the target speed.
* Load limit: limit host utilization to the specified percentage of a single core.
  This may help with preventing mobiles from throttling.

Target speed is specified in million instructions per second (MIPS). A real Tungsten E2
runs at about 100 MIPS, depending on how well cache can be utilized.

### Export

Exports a session lets you save a snapshot to disk.

### Reset

While the emulation is running, CloudpilotEmu persists the device RAM and state
every second. Resetting the session will remove the saved state but preserve the
contents of RAM. This forces the device to reboot the next time the session is
launched.

# Mass exporting and importing sessions

Open the menu by tapping the three vertical dots and select "Export" in order to
start selecting sessions from the list for export. All selected sessions will be
exported as a single zip archive.

Similarly, a zip archive with sessions can be imported by adding it in order to
add all of them to CloudpilotEmu. Note that you will not be prompted to change
the session names or settings during import in this case - you'll have to make
any adjustments afterwards.

# Mass deleting sessions

Open the menu by tapping the three vertical dots and select "Export" in order to
select multiple sessions to delete.
