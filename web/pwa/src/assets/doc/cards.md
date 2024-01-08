The 'Cards' tab allows you to manage your virtual memory cards.

# Background

Many Palm OS devices featured SD or MemoryStick card expansion ports. CloudPilot
allows you to create and connect virtual memory cards to your emulated device,
where this was supported by the original device.

Adding an expansion card could be used for many purposes, such as storing
addition programs, or storing large files or databases or multimedia. Some
software was even distributed on SD cards. These devices generally only
supported the FAT16 filesystem.

### MemoryStick Card Support
CloudpilotEmu supports the Sony MemoryStick standard, which was used on Palm OS
devices by Sony (Clié) and Acer. An important thing to note is that the
**MemoryStick standard only supports cards with specific sizes** (labeled as  4,
8, 16, 32, 64, or 128 MiB, but the actual usable size is slightly different). If
your card is not working on a MemoryStick device, it is likely that the card is
not one of these standard sizes. Instead, you may like to create a new card with
one of these standard sizes within the virtual card manager, and then import the
desired content into this new virtual card.

# Managing Cards

### Adding a card

Tap the plus button in the top right corner to add a card. You can either create
a blank card, or you can import an existing image file into CloudpilotEmu.

**Importing a card**: Imported cards should be provided as a filesystem image
with optional gzip compression. Files with the ending `.gz` will be treated
as gzip compressed images and decompressed.

Most images can be used as SD cards, but only very specific sizes are suitable
for use as memory sticks (see 'MemoryStick Card Support' above). Imported cards
are stored in the browser's IndexedDB and are loaded into RAM, so you should
avoid importing cards larger than 128MB.

You can download dumps of original cards that were used to distribute PalmOS
software on [PalmDB](https://palmdb.net/search/MMC+image).

**Creating a blank card**: If you choose to create a card, you will be asked to
configure the size of the virtual card. Please note that the size of the card
cannot be changed after creation.

### Exporting a card
If you wish to export an image of a current card in CloudpilotEmu:

* Desktop: right click on the card in the list, and choose 'Save'.
* Mobile: swipe on the card name from left-to-right, and choose th cyan
  colour'Share' icon.

You can choose to either save a gzip compressed or a plain file system image.
An image of the card will then proceed to download.

### Mass exporting and importing cards

Select the "export" icon from the toolbar in order to start selecting caeds
from the list for export. All selected cards will be exported as a single
zip archive.

Similarly, a zip archive with card images can be imported in order to add all of
them to CloudpilotEmu. Note that you will not be prompted to change the
card names or settings during import in this case - you'll have to make any
adjustments afterwards. In addition, no file system check will be performed
on the imported cards - they will be checked when they are used for the first
time.

**WARNING:** Mass export and import can consume a lot of memory and may
crash the browser if the available memory is exceeded.

### Deleting a card
To delete a card:

* Desktop: right click on the card in the list, and choose 'Delete'.
* Mobile: swipe on the card name from right-to-left, and choose the red delete
  'Bin' icon.

### Renaming a card
To rename a card:
* Desktop: right click on the card in the list, and then choose 'Edit'.
* Mobile: swipe on the card name from left-to-right, and choose the blue 'Edit'
  icon.

# Mounting & Ejecting Cards

To 'insert' or 'mount' a card in a device, go to the 'Emulator' tab, open the
contextual menu (three dots), and tap 'Insert SD Card'. The card will then be
mounted in the simulated device.

To eject a currently mounted card, choose 'Eject SD card'.

**Please Note:  It is recommended to return to the Palm OS program launcher
(home screen) before inserting or removing cards. To avoid data corruption, do
not eject a card during file operations.**

# Browsing a card
To browse the contents of a virtual card, tap on the card in the list. See the
help menu within the file explorer for more detailed information.

If the card is currently 'inserted' or 'mounted' in a virtual device, you will
be prompted to eject this card before it can be browsed.

# Filesystem Health and Errors

To maintain the integrity of your virtual cards, CloudpilotEmu will
automatically check the filesystem of any card that is imported or mounted, and
if any errors are found, you will be prompted to attempt a filesystem repair.

**Please note: Formatting cards in PalmOS can produce file system errors in some
cases!**

### Manually Checking card filesystem
To manually verify a card is not corrupted:

* Desktop: right click on the card in the list, and choose 'Check'.
* Mobile: swipe on the card name from right-to-left, and choose the purple
  'Health' icon.

If any errors are detected, you will then be prompted to attempt a file system
repair.

Please Note: this is not usually necessary, as CloudpilotEmu will automatically
check cards when they are inserted into the device or browsed.

### Cards containing filesystem errors
Some original SD cards (and their images) contain filesystem errors, and in some
cases you may not wish to perform a repair. For example, in some cases the
presence of these errors is used as a crude copy protection mechanism, and the
software will stop working if these errors are corrected (e.g. the DRM of
Encyclopeadia Britannica). For this reason, there is an option on the card
properties dialog to enable or disable automatic checking of cards. This option
is enabled by default, but you can disable it if you wish to use cards with
errors present. To access the card properties dialog, see the 'Renaming a card'
section above.
