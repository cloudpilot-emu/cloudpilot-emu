# Virtual Card Manager

**The 'Cards' tab allows you to manage your virtual storage cards.**


## Background
 Many Palm OS devices featured SD or MemoryStick card expansion ports.  CloudPilot allows you to create and connect virtual storage cards to your emulated device, where this was supported by the original device.
 
 Adding an expansion card could be used for many purposes, such as storing addition programs, or storing large files or databases or multimedia. Some software was even distributed on SD cards. These devices generally only supported the FAT16 filesystem, and as a result, **the maximum size of a card is 2 GiB**.

### MemoryStick Card Support
CloudPilot supports the Sony MemoryStick standard, which was used on Palm OS devices by Sony (Clié) and Acer. An important thing to note is that the **MemoryStick standard only supports cards with specific sizes** (e.g. 4, 8, 16, 32, 64, or 128 MiB). If your card is not working on a MemoryStick device, it is likely that the card is not one of these standard sizes. Instead, you may like to create a new card with one of these standard sizes within the virtual card manager, and then import the desired contents into this new virtual card.

## Managing Cards
### Adding a card
 Tap the plus button in the top right corner to add a card. You can either create a blank card, or you can import an existing image file into CloudPilot.
 
 **Importing a card**: Imported cards should be provided as a filesystem image. *Please note: Most images can be used as SD cards, but only very specific sizes are suitable for use as memory sticks (see 'MemoryStick Card Support' above).*
 
 **Creating a blank card**: If you choose to create a card, you will be asked to configure the size of the virtual card. *Please note: the size of the card currently cannot be changed after creation.*

### Exporting a card
If you wish to export an image of a current card in CloudPilot:

* Desktop: right click on the card in the list, and choose 'Save'. 
* Mobile: swipe on the card name from left-to-right, and choose th cyan colour'Share' icon.

An image of the card will then proceed to download.

### Deleting a card
To delete a card:

* Desktop: right click on the card in the list, and choose 'Delete'.
* Mobile: swipe on the card name from right-to-left, and choose the red delete 'Bin' icon.

### Renaming a card
To rename a card:
* Desktop: right click on the card in the list, and then choose 'Edit'.
* Mobile: swipe on the card name from left-to-right, and choose the blue 'Edit' icon.

### Checking card filesystem
To verify a card is not corrupted:

* Desktop: right click on the card in the list, and choose 'Check'.
* Mobile: swipe on the card name from right-to-left, and choose the purple 'Health' icon.

## Mounting & Ejecting Cards
*Note: It is recommended to return to the Palm OS program launcher (home screen) before inserting or removing cards.*

To 'insert' or 'mount' a card in a device, go to the 'Emulator' tab, open the contextual menu (three dots), and tap 'Insert SD Card'. The card will then be mounted in the simulated device. 

To eject a currently mounted card, choose 'Eject SD card'. 

**Please Note: To avoid data corruption, do not eject a card during file operations.**

## Browsing a card
To browse the contents of a virtual card, tap on the card in the list. See the help menu within the file explorer for more detailed information. 

*Note: If the card is currently 'inserted' or 'mounted' in a virtual device, you will be prompted to eject this card before it can be browsed.*

