/******************************************************************************
 *
 * Copyright (c) 1994-1999 Palm Computing, Inc. or its subsidiaries.
 * All rights reserved.
 *
 * File: DataPrv.h
 *
 * Description:
 *		Private Header for the Data Manager
 *
 * History:
 *		08/10/95	RM		Created by Ron Marianetti
 *		10/26/99	kwk	Backed out overlay cache modifications, and added
 *							constants for RAM vs. ROM store ids.
 *
 *****************************************************************************/

#ifdef	NON_PORTABLE
#ifndef __DATAPRV_H__
#define __DATAPRV_H__

// DOLATER kwk - find out if the decision was made to use nested includes,
// or do I need to go through and hit all of the source files that include
// DataPrv.h to also include OverlayMgr.h (or add OverlayMgr.h to SysAll.h)

#include	<OverlayMgr.h>

/************************************************************
 * Ways that a database can be open.
 *************************************************************/
#define	openTypeRegular   0							// Nothing special.
#define	openTypeBase      (openTypeRegular+1)	// Rsc db opened as base.
#define	openTypeOverlay	(openTypeRegular+2)	// Rsc db opened as overlay.


/************************************************************
 * Structure of a Record entry
 *************************************************************/
typedef struct {
	LocalID		localChunkID;				// local chunkID of a record
	UInt8			attributes;					// record attributes;
	UInt8			uniqueID[3];				// unique ID of record; should
													//	not be 0 for a legal record.
	} RecordEntryType;
typedef RecordEntryType*	RecordEntryPtr;



/************************************************************
 * Structure of a Resource entry
 *************************************************************/
typedef struct {
	UInt32		type;							// resource type
	UInt16		id;							// resource id
	LocalID		localChunkID;				// resource data chunk ID
	} RsrcEntryType;
typedef RsrcEntryType*	RsrcEntryPtr;

// Attributes field
#define	dmRsrcAttrUnused		0x0000	// to be defined...



/************************************************************
 * Structure of a record list extension. This is used if all
 *  the database record/resource entries of a database can't fit into
 *  the database header.
 *************************************************************/
typedef struct {
	LocalID				nextRecordListID;			// local chunkID of next list
	UInt16				numRecords;					// number of records in this list
	UInt16				firstEntry;					// array of Record/Rsrc entries 
															// starts here
	} RecordListType;
typedef RecordListType*	RecordListPtr;




/************************************************************
 * Structure of a Database Header
 *************************************************************/
typedef struct {
	UInt8			name[dmDBNameLength];	// name of database
	UInt16		attributes;					// database attributes
	UInt16		version;						// version of database

	UInt32		creationDate;				// creation date of database
	UInt32		modificationDate;			// latest modification date
	UInt32		lastBackupDate;			// latest backup date
	UInt32		modificationNumber;		// modification number of database

	LocalID		appInfoID;					// application specific info
	LocalID		sortInfoID;					// app specific sorting info

	UInt32		type;							// database type
	UInt32		creator;						// database creator 
	
	UInt32		uniqueIDSeed;				// used to generate unique IDs.
													//	Note that only the low order
													//	3 bytes of this is used (in
													//	RecordEntryType.uniqueID).
													//	We are keeping 4 bytes for 
													//	alignment purposes.

	RecordListType	recordList;				// first record list
	} DatabaseHdrType;		

typedef DatabaseHdrType*	DatabaseHdrPtr;





/************************************************************
 * Directory of all available databases - maintained by the
 *  Database Manager. This directory also includes resource
 *  databases. There is one of these directories for each store
 *  (either RAM or ROM store) in a memory card. The ROM store
 *	 list gets copied into the RAM store list at DmInit time.
 *************************************************************/

typedef struct {
	LocalID	baseID;
	} DatabaseDirEntryType;

typedef struct {
	LocalID						nextDatabaseListID;		// continuation list, if any
	UInt16						numDatabases;				// # of database ID's in this list
	DatabaseDirEntryType		databaseID[1];				// one entry per database
	} DatabaseDirType;

typedef DatabaseDirType*	DatabaseDirPtr;




/************************************************************
 * This structure is used to hold information on an open database
 * It gets allocated as a non-movable chunk in the dynamic heap
 *  when a database is opened and caches database information that
 *  helps speed up performance.
 *************************************************************/
typedef struct  {
	void *				next;							// MemPtr to next access structure
	UInt16				openCount;					// incremented every time it's opened
	UInt32				ownerTaskID;				// taskID of owner
	UInt16				exclusive 	: 1;			// true if opened with exclusive access
	UInt16				writeAccess : 1;			// true if opened with write access
	UInt16				resDB		 	: 1;			// true if resource database
	LocalID				hdrID;						// local ID of database header
	MemHandle			hdrH;							// Database header handle
	DatabaseHdrPtr		hdrP;							// Database header MemPtr for faster access
	UInt16				cardNo;						// card number of database
	UInt16				numRecords; 				// cached here for better performance
	MemPtr				hdrMasterP;					// master MemPtr to header if ROM based
	MemPtr*				handleTableP;				// pointer to block of ROM handles
	} DmOpenInfoType;

typedef DmOpenInfoType*		DmOpenInfoPtr;


/************************************************************
 * This structure is used to hold client specific information for
 *  an open database. There is one of these allocated every time 
 *  a database is opened, even if it's already open. This structure
 *  holds the open mode of the client and a pointer to the DmOpenInfoType
 *  of the database which contains the common database info for all
 *  clients. The SysAppInfoPtr, referenced off of the current A5, 
 *  has a pointer to the first DmAccessType in the linked list.
 *************************************************************/
typedef struct DmAccessType {
	struct DmAccessType*	next;						// MemPtr to next access structure
	UInt16					mode;						// mode it was opened in
	DmOpenInfoPtr			openP;					// MemPtr to DmOpenInfoType for database
	
	// NEW FOR PalmOS v3.0
	UInt32					savedModNum;			// saved modification number of database
															// at the time of opening
	
	// NEW FOR PalmOS 3.5
	UInt8						openType;				// Regular, base, or overlay
	UInt8						reserved;			
	} DmAccessType;

typedef DmAccessType*		DmAccessPtr;



/************************************************************
 * This structure is used to hold the results of a search
 *  in DmGetNextDatabaseByTypeCreator. It is the "real" structure
 *  of the generic type DmSearchStateType used by applications.
 *************************************************************/
#if EMULATION_LEVEL == EMULATION_NONE
#define MAX_CARDS					1
#else
#define MAX_CARDS					2
#endif
#define MAX_STORES_PER_CARD	2
#define MAX_STORES				(MAX_CARDS * MAX_STORES_PER_CARD)
 
typedef struct {
		UInt16	indexes[MAX_STORES];
		UInt16	cardNos[MAX_STORES];						// cardNo for each store
		UInt32	latestVersionCandidateType;			// saved type of latest version candidate
		UInt32	latestVersionCandidateCreator;		// saved creator of latest version candidate
		Int16		latestVersionCandidateStore;			// saved store for latest versions
		} DmPrvSearchStateType;
typedef DmPrvSearchStateType*	DmPrvSearchStatePtr;	
	

/************************************************************
 * This structure is used to keep track of protected databases. These
 *  are databases which have been protected through the DmDatabaseProtect()
 *  call. A handle to an array of these structures is stored in the
 *  low memory global GDmProtectListH. The number of entries in the array
 *  must be calculated by taking the size of the handle and dividing
 *  by sizeof (DmProtectEntryType)
 *************************************************************/
typedef struct {
		UInt8			protectCount;
		UInt8			cardNo;
		LocalID		dbID;
		} DmProtectEntryType, *DmProtectEntryPtr;
	

/************************************************************
 * This structure is used to cache data about applications and
 * launchable data databases in the system Launch Database.
 * This is the record structure for the database.
  *************************************************************/
#define dmMaxVersionStrLength	16

typedef struct {
	// Cached database info:
	UInt32	type;							// type of database
	UInt32	creator;						// creator of database
	LocalID	dbID;						// LocalID of database
	UInt16 	cardNo;						// card number of database
	UInt16	dbAttrs;						// database attributes
	UInt16	version;						// version of database
	UInt8		dbName[dmDBNameLength];	// database name
	
	// Launcher info:
	UInt16	lgIconOffset;				// offset in bytes from beginning of record to bitmap for large icon
	UInt16	smIconOffset;				// offset in bytes from beginning of record to bitmap for small icon
	UInt8		iconName[dmDBNameLength];		// application icon name (from tain resource)
	Char		versionStr[dmMaxVersionStrLength];	// version string to display in Launcher
	UInt8		flags;								// launch flags, defined below
	UInt8		reserved;
	
	// Small & Large bitmaps for the database are stored
	// at the end of the record, and are 4-byte aligned.
	
	} DmLaunchDBRecordType, *DmLaunchDBRecordPtr;

// Constants for the 'flags' field in DmLaunchDBRecordType
#define dmLaunchFlagDirty				0x01	// Used to tell the launcher to update 
														// the category & launcher info, 
#define dmLaunchFlagOrphaned			0x02	// Used during boot in PrvSyncLaunchDatabase() 
														// to remove superfluous entries.
#define dmLaunchFlagLaunchable		0x04	// Used during boot in PrvSyncLaunchDatabase() 
														// to remove superfluous entries.
#define dmLaunchFlagResetCategory	0x08	// Used to force the launcher to move the
														// entity into its default category.


// Type & creator for the launch database:
#define dmLaunchDatabaseType		'lnch'
#define dmLaunchDatabaseCreator	sysFileCSystem
#define dmLaunchDatabaseVersion	1
#define dmLaunchDatabaseName		"psysLaunchDB"

// For private database searching routine
typedef Int16 (*DmSearchFuncPtr) (void const * searchData, void const * dbData, Int32 other);

/************************************************************
 * The owner ID of chunks allocated by the Database Manager
 *************************************************************/
#define	dmDynOwnerID					0x00		// Dynamic heap chunks
#define	dmMgrOwnerID					0x01		// Management chunks
#define	dmRecOwnerID					0x02		// Record chunks
#define	dmOrphanOwnerID				0x03		// Orphaned record chunks


/************************************************************
 * Private Data Manager procedures
 *************************************************************/
#ifdef REMOVE_FOR_EMULATOR
#ifdef __cplusplus
extern "C" {
#endif

Err			DmMoveOpenDBContext(DmAccessPtr* listHeadP, DmAccessPtr dbP)
							SYS_TRAP(sysTrapDmMoveOpenDBContext);

DmOpenRef	DmOpenDBWithLocale(UInt16 cardNo, LocalID dbID, UInt16 mode, const OmLocaleType* localeP)
							SYS_TRAP(sysTrapDmOpenDBWithLocale);

#ifdef __cplusplus
}
#endif
#endif


#endif // __DATAPRV_H__
#endif //NON_PORTABLE
