#ifndef _PALMOS_ERRORS_H_
#define _PALMOS_ERRORS_H_

#define errNone 0x0000  // No error

#define memErrorClass 0x0100    // Memory Manager
#define dmErrorClass 0x0200     // Data Manager
#define serErrorClass 0x0300    // Serial Manager
#define slkErrorClass 0x0400    // Serial Link Manager
#define sysErrorClass 0x0500    // System Manager
#define fplErrorClass 0x0600    // Floating Point Library
#define flpErrorClass 0x0680    // New Floating Point Library
#define evtErrorClass 0x0700    // System Event Manager
#define sndErrorClass 0x0800    // Sound Manager
#define almErrorClass 0x0900    // Alarm Manager
#define timErrorClass 0x0A00    // Time Manager
#define penErrorClass 0x0B00    // Pen Manager
#define ftrErrorClass 0x0C00    // Feature Manager
#define cmpErrorClass 0x0D00    // Connection Manager (HotSync)
#define dlkErrorClass 0x0E00    // Desktop Link Manager
#define padErrorClass 0x0F00    // PAD Manager
#define grfErrorClass 0x1000    // Graffiti Manager
#define mdmErrorClass 0x1100    // Modem Manager
#define netErrorClass 0x1200    // Net Library
#define htalErrorClass 0x1300   // HTAL Library
#define inetErrorClass 0x1400   // INet Library
#define exgErrorClass 0x1500    // Exg Manager
#define fileErrorClass 0x1600   // File Stream Manager
#define rfutErrorClass 0x1700   // RFUT Library
#define txtErrorClass 0x1800    // Text Manager
#define tsmErrorClass 0x1900    // Text Services Library
#define webErrorClass 0x1A00    // Web Library
#define secErrorClass 0x1B00    // Security Library
#define emuErrorClass 0x1C00    // Emulator Control Manager
#define flshErrorClass 0x1D00   // Flash Manager
#define pwrErrorClass 0x1E00    // Power Manager
#define cncErrorClass 0x1F00    // Connection Manager (Serial Communication)
#define actvErrorClass 0x2000   // Activation application
#define radioErrorClass 0x2100  // Radio Manager (Library)
#define dispErrorClass 0x2200   // Display Driver Errors.
#define bltErrorClass 0x2300    // Blitter Driver Errors.
#define winErrorClass 0x2400    // Window manager.
#define omErrorClass 0x2500     // Overlay Manager
#define menuErrorClass 0x2600   // Menu Manager
#define lz77ErrorClass 0x2700   // Lz77 Library
#define smsErrorClass 0x2800    // Sms Library
#define expErrorClass 0x2900    // Expansion Manager and Slot Driver Library
#define vfsErrorClass 0x2A00    // Virtual Filesystem Manager and Filesystem library
#define lmErrorClass 0x2B00     // Locale Manager
#define intlErrorClass 0x2C00   // International Manager
#define pdiErrorClass 0x2D00    // PDI Library
#define attnErrorClass 0x2E00   // Attention Manager
#define telErrorClass 0x2F00    // Telephony Manager
#define hwrErrorClass 0x3000    // Hardware Manager (HAL)
#define blthErrorClass 0x3100   // Bluetooth Library Error Class
#define udaErrorClass 0x3200    // UDA Manager Error Class

#define oemErrorClass 0x7000  // OEM/Licensee errors (0x7000-0x7EFF shared among ALL partners)
#define errInfoClass 0x7F00   // special class shows information w/o error code
#define appErrorClass 0x8000  // Application-defined errors

#define dmErrMemError (dmErrorClass | 1)
#define dmErrIndexOutOfRange (dmErrorClass | 2)
#define dmErrInvalidParam (dmErrorClass | 3)
#define dmErrReadOnly (dmErrorClass | 4)
#define dmErrDatabaseOpen (dmErrorClass | 5)
#define dmErrCantOpen (dmErrorClass | 6)
#define dmErrCantFind (dmErrorClass | 7)
#define dmErrRecordInWrongCard (dmErrorClass | 8)
#define dmErrCorruptDatabase (dmErrorClass | 9)
#define dmErrRecordDeleted (dmErrorClass | 10)
#define dmErrRecordArchived (dmErrorClass | 11)
#define dmErrNotRecordDB (dmErrorClass | 12)
#define dmErrNotResourceDB (dmErrorClass | 13)
#define dmErrROMBased (dmErrorClass | 14)
#define dmErrRecordBusy (dmErrorClass | 15)
#define dmErrResourceNotFound (dmErrorClass | 16)
#define dmErrNoOpenDatabase (dmErrorClass | 17)
#define dmErrInvalidCategory (dmErrorClass | 18)
#define dmErrNotValidRecord (dmErrorClass | 19)
#define dmErrWriteOutOfBounds (dmErrorClass | 20)
#define dmErrSeekFailed (dmErrorClass | 21)
#define dmErrAlreadyOpenForWrites (dmErrorClass | 22)
#define dmErrOpenedByAnotherTask (dmErrorClass | 23)
#define dmErrUniqueIDNotFound (dmErrorClass | 24)
#define dmErrAlreadyExists (dmErrorClass | 25)
#define dmErrInvalidDatabaseName (dmErrorClass | 26)
#define dmErrDatabaseProtected (dmErrorClass | 27)
#define dmErrDatabaseNotProtected (dmErrorClass | 28)

#define memErrChunkLocked (memErrorClass | 1)
#define memErrNotEnoughSpace (memErrorClass | 2)
#define memErrInvalidParam (memErrorClass | 3) /* invalid param or requested size is too big */
#define memErrChunkNotLocked (memErrorClass | 4)
#define memErrCardNotPresent (memErrorClass | 5)
#define memErrNoCardHeader (memErrorClass | 6)
#define memErrInvalidStoreHeader (memErrorClass | 7)
#define memErrRAMOnlyCard (memErrorClass | 8)
#define memErrWriteProtect (memErrorClass | 9)
#define memErrNoRAMOnCard (memErrorClass | 10)
#define memErrNoStore (memErrorClass | 11)
#define memErrROMOnlyCard (memErrorClass | 12)

#endif  // _PALMOS_ERRORS_H_