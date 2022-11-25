/******************************************************************************
 *
 * Copyright (c) 1999 Palm Computing, Inc. or its subsidiaries.
 * All rights reserved.
 *
 * File: OverlayMgr.h
 *
 * Description:
 *		Public header for routines that support overlays & locales.
 *
 * History:
 *			Created by Ken Krugler
 *		06/24/99	kwk	Created by Ken Krugler.
 *		07/06/99	CS		Added omSpecAttrForBase
 *							(and renumbered omSpecAttrStripped).
 *		07/29/99	CS		Added omOverlayKindBase for the entries in the base
 *							DBs 'ovly' resource (they had been set to
 *							omOverlayKindReplace before).
 *		07/29/99	CS		Bumped version to 3, since now we're supposed to
 *							support omOverlayKindAdd.
 *		09/29/99	kwk	Bumped version to 4, since we added the baseChecksum
 *							field to OmOverlaySpecType, as a way of speeding up
 *							overlay validation.
 *		09/29/99	CS		Actually bumped version to 4, which Ken forgot.
 *		10/08/99	kwk	Added OmGetRoutineAddress selector/declaration.
 *							Moved OmDispatch, OmInit, and OmOpenOverlayDatabase
 *							into OverlayPrv.h
 *
 *****************************************************************************/

#ifndef	__OVERLAYMGR_H__
#define	__OVERLAYMGR_H__

// Include elementary types
#include <PalmTypes.h>
#include <DataMgr.h>

#ifdef _WIN32
  #pragma warning(disable: 4200)  // nonstandard extension used : zero-sized array in struct/union
#endif

/***********************************************************************
 * Overlay Manager constants
 **********************************************************************/

#define	omOverlayVersion		0x0004	// Version of OmOverlaySpecType/OmOverlayRscType
#define	omOverlayDBType		'ovly'	// Overlay database type
#define	omOverlayRscType		'ovly'	// Overlay desc resource type
#define	omOverlayRscID			1000		// Overlay desc resource ID

#define	omFtrCreator			'ovly'	// For get/set of Overlay features.
#define	omFtrShowErrorsFlag	0			// Boolean - True => display overlay errors.

// Flags for OmOverlaySpecType.flags field
#define	omSpecAttrForBase		1			//	'ovly' (in base) describes base itself
#define	omSpecAttrStripped	2			// Localized resources stripped (base only)

// OmFindOverlayDatabase called with stripped base, and no appropriate overlay was found.
#define	omErrBaseRequiresOverlay	(omErrorClass | 1)

// OmOverlayDBNameToLocale or OmLocaleToOverlayDBName were passed an unknown locale.
#define	omErrUnknownLocale			(omErrorClass | 2)

// OmOverlayDBNameToLocale was passed a poorly formed string.
#define	omErrBadOverlayDBName		(omErrorClass | 3)

// OmGetIndexedLocale was passed an invalid index.
#define	omErrInvalidLocaleIndex		(omErrorClass | 4)	

// OmSetSystemLocale was passed an invalid locale (doesn't correspond to available
// system overlay).

#define	omErrInvalidLocale			(omErrorClass | 5)

// OmSetSystemLocale was passed a locale that referenced an invalid system overlay
// (missing one or more required resources)

#define	omErrInvalidSystemOverlay	(omErrorClass | 6)

// Values for OmOverlayKind
#define	omOverlayKindHide		0		// Hide base resource (not supported in version <= 3)
#define	omOverlayKindAdd		1		// Add new resource (not support in version <= 2)
#define	omOverlayKindReplace	2		// Replace base resource
#define	omOverlayKindBase		3		// Description of base resource itself (not supported in version <= 2)

/***********************************************************************
 * Selectors & macros used for calling Overlay Manager routines
 **********************************************************************/

#ifdef DIRECT_OVERLAY_CALLS
	#define	OMDISPATCH_TRAP(omSelectorNum)
#else
	#define	OMDISPATCH_TRAP(sel) \
		_SYSTEM_API(_CALL_WITH_SELECTOR)(_SYSTEM_TABLE, sysTrapOmDispatch, sel)
#endif

// Selectors used for getting to the right Overlay Manager routine via
// the OmDispatch trap.
typedef enum {
	omInit = 0,
	omOpenOverlayDatabase,
	omLocaleToOverlayDBName,
	omOverlayDBNameToLocale,
	omGetCurrentLocale,
	omGetIndexedLocale,
	omGetSystemLocale,
	omSetSystemLocale,
	omGetRoutineAddress,
	
	omMaxSelector = omGetRoutineAddress,
	omBigSelector = 0x7FFF	// Force OmSelector to be 16 bits.
} OmSelector;

/***********************************************************************
 * Overlay Manager types
 **********************************************************************/

typedef UInt16 OmOverlayKind;

#ifndef PUBLIC_STUFF_STRIPPED
// DOLATER kwk - figure out exact format for portable struct declaration.
// We might also want to hide this information in a private header, and
// just have OmLocaleType in here.
#endif // PUBLIC_STUFF_STRIPPED

typedef struct {
	OmOverlayKind	overlayType;		// Replace, delete, etc.
	UInt32			rscType;				// Resource type to overlay
	UInt16			rscID;				// Resource ID to overlay
	UInt32			rscLength;			// Length of base resource
	UInt32			rscChecksum;		// Checksum of base resource data
} OmOverlayRscType;

typedef struct {
	UInt16			language;			// Language spoken in locale
	UInt16			country;				// Specifies "dialect" of language
} OmLocaleType;

// Definition of the Overlay Description Resource ('ovly')
typedef struct {
	UInt16				version;				// Version of this structure
	UInt32				flags;				// Flags
	UInt32				baseChecksum;		// Checksum of all overlays[].checksum
	OmLocaleType		targetLocale;		// Language, & country of overlay resources
	UInt32				baseDBType;			// Type of base DB to overlay
	UInt32				baseDBCreator;		// Creator of base DB to overlay
	UInt32				baseDBCreateDate; // Date base DB was created
	UInt32				baseDBModDate;		// Date base DB was last modified
	UInt16				numOverlays;		// Number of resources to overlay
	OmOverlayRscType	overlays[0];		// Descriptions of resources to overlay
} OmOverlaySpecType;


/***********************************************************************
 * Overlay Manager routines
 **********************************************************************/

#ifdef REMOVE_FOR_EMULATOR
#ifdef __cplusplus
	extern "C" {
#endif

// Return in <overlayDBName> an overlay database name that's appropriate
// for the base name <baseDBName> and the locale <targetLocale>. If the
// <targetLocale> param in NULL, use the current locale. The <overlayDBName>
// buffer must be at least dmDBNameLength bytes.

Err OmLocaleToOverlayDBName(const Char *baseDBName, const OmLocaleType *targetLocale,
									Char *overlayDBName)
			OMDISPATCH_TRAP(omLocaleToOverlayDBName);

// Given the name of an overlay database in <overlayDBName>, return back
// the overlay in overlayLocale. If the name isn't an overlay name,
// return omErrBadOverlayDBName.

Err OmOverlayDBNameToLocale(const Char *overlayDBName, OmLocaleType *overlayLocale)
			OMDISPATCH_TRAP(omOverlayDBNameToLocale);

// Return the current locale in <currentLocale>. This may not be the same as
// the system locale, which will take effect after the next reset.

void OmGetCurrentLocale(OmLocaleType *currentLocale)
			OMDISPATCH_TRAP(omGetCurrentLocale);

//	Return the nth available locale in <theLocale>. Indexes are zero-based, and the
// omErrInvalidLocaleIndex result will be returned if <localeIndex> is out of bounds.

Err OmGetIndexedLocale(UInt16 localeIndex, OmLocaleType *theLocale)
			OMDISPATCH_TRAP(omGetIndexedLocale);

// Return the system locale in <systemLocale>. This may not be the same as
// the current locale.

void OmGetSystemLocale(OmLocaleType *systemLocale)
			OMDISPATCH_TRAP(omGetSystemLocale);

//	Set the post-reset system locale to be <systemLocale>. Return omErrInvalidLocale if
// the passed locale doesn’t correspond to a valid System.prc overlay.

Err OmSetSystemLocale(const OmLocaleType *systemLocale)
			OMDISPATCH_TRAP(omSetSystemLocale);

// Return back the address of the routine indicated by <inSelector>. If
// <inSelector> isn't a valid routine selector, return back NULL.

void *OmGetRoutineAddress(OmSelector inSelector)
			OMDISPATCH_TRAP(omGetRoutineAddress);

#ifdef __cplusplus
	}
#endif
#endif

#endif
